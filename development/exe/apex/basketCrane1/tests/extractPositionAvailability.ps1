param([Parameter(Mandatory=$true)][string]$OutputFile)
$ErrorActionPreference = 'Stop'
$source = [IO.File]::ReadAllText((Join-Path $PSScriptRoot '..\pl.cpp'))
$match = [regex]::Match($source, '(?ms)^basketPositionClass\* positionListClass::getFreeToRecievePos\([^\r\n]*\)\r?\n\{.*?^\}')
if (!$match.Success) { throw 'Cannot extract destination availability method' }
[IO.Directory]::CreateDirectory([IO.Path]::GetDirectoryName($OutputFile)) | Out-Null
[IO.File]::WriteAllText($OutputFile, $match.Value)
