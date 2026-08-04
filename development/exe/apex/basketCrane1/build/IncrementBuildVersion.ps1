param(
    [Parameter(Mandatory = $true)]
    [string]$VersionFile,

    [Parameter(Mandatory = $true)]
    [string]$HeaderFile
)

$ErrorActionPreference = 'Stop'
$utf8NoBom = New-Object System.Text.UTF8Encoding($false)
$versionPath = [System.IO.Path]::GetFullPath($VersionFile)
$headerPath = [System.IO.Path]::GetFullPath($HeaderFile)
$lockPath = "$versionPath.lock"
$lockStream = $null

for ($attempt = 0; $attempt -lt 100 -and $null -eq $lockStream; $attempt++) {
    try {
        $lockStream = [System.IO.File]::Open(
            $lockPath,
            [System.IO.FileMode]::OpenOrCreate,
            [System.IO.FileAccess]::ReadWrite,
            [System.IO.FileShare]::None)
    }
    catch [System.IO.IOException] {
        Start-Sleep -Milliseconds 100
    }
}

if ($null -eq $lockStream) {
    throw "Unable to acquire build-version lock: $lockPath"
}

try {
    $current = if ([System.IO.File]::Exists($versionPath)) {
        [System.IO.File]::ReadAllText($versionPath).Trim()
    }
    else {
        '5.5.0'
    }

    if ($current -notmatch '^(\d+)\.(\d+)\.(\d+)$') {
        throw "Invalid build version '$current'. Expected major.minor.build."
    }

    $major = [int]$Matches[1]
    $minor = [int]$Matches[2]
    $build = [int]$Matches[3] + 1
    if ($build -gt 65535) {
        $minor++
        $build = 0
    }

    $nextVersion = '{0}.{1}.{2}' -f $major, $minor, $build
    $headerDirectory = [System.IO.Path]::GetDirectoryName($headerPath)
    if (-not [System.IO.Directory]::Exists($headerDirectory)) {
        [System.IO.Directory]::CreateDirectory($headerDirectory) | Out-Null
    }

    [System.IO.File]::WriteAllText($versionPath, "$nextVersion`r`n", $utf8NoBom)
    $header = "#pragma once`r`n`r`n#define APEX_BASKET_CRANE_VERSION `"$nextVersion`"`r`n"
    [System.IO.File]::WriteAllText($headerPath, $header, $utf8NoBom)
    Write-Host "Apex Basket Crane build version: $nextVersion"
}
finally {
    $lockStream.Dispose()
    Remove-Item -LiteralPath $lockPath -Force -ErrorAction SilentlyContinue
}
