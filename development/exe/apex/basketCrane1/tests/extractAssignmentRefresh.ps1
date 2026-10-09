param([Parameter(Mandatory=$true)][string]$OutputFile)
$ErrorActionPreference = 'Stop'
$methods = @()
foreach ($entry in @(@('ap.cpp', 'void allPositionsClass::reloadBasketAssignmentsFromDatabase'), @('cr.cpp', 'bool craneClass::isAssignmentReservedForMission'))) {
    $source = [IO.File]::ReadAllText((Join-Path $PSScriptRoot ('../' + $entry[0])))
    $match = [regex]::Match($source, '(?ms)^' + [regex]::Escape($entry[1]) + '\([^\r\n]*\)\r?\n\{.*?^\}')
    if (!$match.Success) { throw ('Cannot extract ' + $entry[1]) }
    $methods += $match.Value
}
[IO.Directory]::CreateDirectory([IO.Path]::GetDirectoryName($OutputFile)) | Out-Null
[IO.File]::WriteAllText($OutputFile, ($methods -join "`r`n"))

