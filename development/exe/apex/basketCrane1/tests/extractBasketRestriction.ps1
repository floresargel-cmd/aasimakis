param([Parameter(Mandatory=$true)][string]$OutputFile)
$ErrorActionPreference = 'Stop'
$source = [IO.File]::ReadAllText((Join-Path $PSScriptRoot '..\bP.cpp'))
$match = [regex]::Match($source, '(?ms)^bool basketPositionClass::canMoveToPosition\([^\r\n]*\)\r?\n\{.*?^\}')
if (!$match.Success) { throw 'Cannot extract basket restriction method' }
[IO.Directory]::CreateDirectory([IO.Path]::GetDirectoryName($OutputFile)) | Out-Null
[IO.File]::WriteAllText($OutputFile, $match.Value)
