param(
    [Parameter(Mandatory=$true)][string]$StatementsFile,
    [string]$ConfigurationFile = (Join-Path $PSScriptRoot '../basketCrane.ini'),
    [string]$Profile = 'MainDatabaseLive'
)
$ErrorActionPreference = 'Stop'
# Only session-local temporary tables in tempdb are mutated. No production
# positions, basketHistory, EPICS data, or PLC commands are touched.
$statements = @(Get-Content -LiteralPath $StatementsFile -Raw | ConvertFrom-Json)
$statements = @($statements | ForEach-Object {
    [regex]::Replace([regex]::Replace($_, '\bpositions\b', '#positions'), '\bbasketHistory\b', '#basketHistory')
})
foreach ($statement in $statements) {
    foreach ($table in [regex]::Matches($statement, '(?i)\b(?:from|update|into)\s+([^\s(]+)')) {
        if ($table.Groups[1].Value -notin @('#positions','#basketHistory')) { throw 'Fixture contains a non-temporary table.' }
    }
}
$profiles=@{}; $section=''
Get-Content -LiteralPath $ConfigurationFile | ForEach-Object {
    if ($_ -match '^\[(.+)\]$') { $section=$matches[1]; $profiles[$section]=@{} }
    elseif ($_ -match '^([^;=]+)=(.*)$' -and $section) { $profiles[$section][$matches[1].Trim()]=$matches[2].Trim() }
}
$builder = New-Object System.Data.Odbc.OdbcConnectionStringBuilder
$builder['DSN']=$profiles[$Profile]['dsn']; $builder['UID']=$profiles[$Profile]['user']; $builder['PWD']=$profiles[$Profile]['password']; $builder['Database']='tempdb'
$connection = New-Object System.Data.Odbc.OdbcConnection($builder.ConnectionString)
$connection.ConnectionTimeout=8
function Execute([string]$Sql, $Transaction=$null) {
    $command=$connection.CreateCommand()
    try { $command.CommandText=$Sql; $command.CommandTimeout=20; $command.Transaction=$Transaction; $null=$command.ExecuteNonQuery() }
    finally { $command.Dispose() }
}
function Snapshot {
    $command=$connection.CreateCommand()
    try {
        $command.CommandText='select posNumber,posIndex,basketTableId,locked,targetNumber from #positions order by posNumber,posIndex,basketTableId'
        $reader=$command.ExecuteReader(); $rows=@()
        try { while ($reader.Read()) { $row=@(); for ($i=0;$i -lt 5;$i++) { $row+=@($reader.GetValue($i).ToString()) }; $rows+=($row -join '|') } }
        finally { $reader.Dispose() }
        return ($rows -join ';')
    } finally { $command.Dispose() }
}
function Scalar([string]$Sql) {
    $command=$connection.CreateCommand()
    try { $command.CommandText=$Sql; return $command.ExecuteScalar() }
    finally { $command.Dispose() }
}
$cases=@(
    @{Name='successful stack transfer'; Setup=''; Reject=$false},
    @{Name='occupied destination after memory refresh'; Setup='update #positions set basketTableId=1100 where posNumber=710 and posIndex=1'; Reject=$true},
    @{Name='source corrected after memory refresh'; Setup='update #positions set basketTableId=99 where posNumber=700 and posIndex=1'; Reject=$true},
    @{Name='duplicate destination row'; Setup='insert into #positions values(710,1,0,2,0)'; Reject=$true},
    @{Name='missing source row'; Setup='delete from #positions where posNumber=700 and posIndex=1'; Reject=$true},
    @{Name='NULL destination assignment'; Setup='update #positions set basketTableId=NULL where posNumber=710 and posIndex=1'; Reject=$true},
    @{Name='duplicate basket elsewhere'; Setup='insert into #positions values(800,1,110,2,0)'; Reject=$true},
    @{Name='outgoing crane mission'; Setup='update #positions set targetNumber=400 where posNumber=700 and posIndex=1'; Reject=$true},
    @{Name='late history failure rolls back earlier writes'; Setup=''; Reject=$true; HistoryFailure=$true}
)
try {
    $connection.Open()
    foreach ($case in $cases) {
        Execute 'create table #positions(posNumber int,posIndex int,basketTableId int NULL,locked int,targetNumber int)'
        $constraint = if ($case.HistoryFailure) { 'check(basketTableId<>95)' } else { '' }
        Execute ("create table #basketHistory(type varchar(10),position varchar(100),destination varchar(100),basketTableId int $constraint,date varchar(30),sec int)")
        Execute 'insert into #positions values(700,1,110,2,0),(700,2,38,2,0),(700,3,95,2,0),(700,4,0,2,0),(710,1,0,2,0),(710,2,0,2,0),(710,3,0,2,0),(710,4,0,2,0)'
        if ($case.Setup) { Execute $case.Setup }
        $before=Snapshot
        $transaction=$connection.BeginTransaction(); $rejected=$false
        try {
            foreach ($statement in $statements) { Execute $statement $transaction }
            $transaction.Commit()
        } catch {
            $rejected=$true; $transaction.Rollback()
        } finally { $transaction.Dispose() }
        if ($rejected -ne $case.Reject) { throw "Unexpected transaction result: $($case.Name)" }
        if ($rejected) {
            if ((Snapshot) -ne $before -or (Scalar 'select count(*) from #basketHistory') -ne 0) { throw "Partial transfer persisted: $($case.Name)" }
        } else {
            if ((Scalar 'select count(*) from #positions where posNumber=700 and basketTableId<>0') -ne 0) { throw 'Source not cleared.' }
            if ((Scalar 'select basketTableId from #positions where posNumber=710 and posIndex=1') -ne 110) { throw 'Destination identity changed.' }
            if ((Scalar 'select basketTableId from #positions where posNumber=710 and posIndex=2') -ne 38 -or
                (Scalar 'select basketTableId from #positions where posNumber=710 and posIndex=3') -ne 95 -or
                (Scalar 'select basketTableId from #positions where posNumber=710 and posIndex=4') -ne 0) { throw 'Incomplete destination stack.' }
            if ((Scalar 'select count(*) from #basketHistory') -ne 6) { throw 'Expected assignment history for all three baskets.' }
            if ((Scalar 'select count(*) from #positions where locked<>2') -ne 0) { throw 'Transfer overwrote operator lock settings.' }
        }
        Write-Output "PASS: $($case.Name)"
        Execute 'drop table #basketHistory'; Execute 'drop table #positions'
    }
} finally { $connection.Dispose() }
