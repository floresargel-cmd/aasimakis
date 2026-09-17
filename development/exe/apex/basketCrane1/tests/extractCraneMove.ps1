param([Parameter(Mandatory=$true)][string]$OutputFile)
$ErrorActionPreference = 'Stop'
# Compile the production methods against isolated database/PLC test doubles.
$source = [IO.File]::ReadAllText((Join-Path $PSScriptRoot '..\cr.cpp'))
$methods = foreach ($name in @('setActivePositions', 'clearActivePositions', 'move')) {
    $match = [regex]::Match($source, '(?ms)^(?:void|bool) craneClass::' + $name + '\([^\r\n]*\)\r?\n\{.*?^\}')
    if (!$match.Success) { throw "Cannot extract craneClass::$name" }
    $match.Value
}
[IO.Directory]::CreateDirectory([IO.Path]::GetDirectoryName($OutputFile)) | Out-Null
[IO.File]::WriteAllText($OutputFile, ($methods -join "`r`n"))
