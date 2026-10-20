param([Parameter(Mandatory=$true)][string]$OutputFile)
$ErrorActionPreference = 'Stop'
$source = [IO.File]::ReadAllText((Join-Path $PSScriptRoot '..\pl.cpp'))
$methods = @()
foreach ($name in @('getFreeToRecievePos','getFreeToMoveBasketPosition')) {
    $match = [regex]::Match($source, '(?ms)^basketPositionClass\* positionListClass::' + $name + '\([^\r\n]*\)\r?\n\{.*?^\}')
    if (!$match.Success) { throw ('Cannot extract availability method ' + $name) }
    $methods += $match.Value
}
[IO.Directory]::CreateDirectory([IO.Path]::GetDirectoryName($OutputFile)) | Out-Null
[IO.File]::WriteAllText($OutputFile, ($methods -join "`r`n"))
