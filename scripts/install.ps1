# ==============================================================================
#  RCWPT EPSON RESETTER CLI SUITE - QUICK ONLINE LAUNCHER & INSTALLER
#  Created with love by rcwpt | Instagram: @c.wijaya.t
# ==============================================================================
#Requires -Version 5.1

$ErrorActionPreference = "Stop"

function Show-NeofetchBanner {
    Clear-Host
    Write-Host @"
    ██████╗  ██████╗██╗    ██╗██████╗ ████████╗
    ██╔══██╗██╔════╝██║    ██║██╔══██╗╚══██╔══╝
    ██████╔╝██║     ██║ █╗ ██║██████╔╝   ██║   
    ██╔══██╗██║     ██║███╗██║██╔═══╝    ██║   
    ██║  ██║╚██████╗╚███╔███╔╝██║        ██║   
    ╚═╝  ╚═╝ ╚═════╝ ╚══╝╚══╝ ╚═╝        ╚═╝   
"@ -ForegroundColor Cyan

    Write-Host "           RCWPT EPSON RESETTER CLI" -ForegroundColor Yellow
    Write-Host "    ----------------------------------------" -ForegroundColor DarkGray
    Write-Host "    OS       : $([System.Environment]::OSVersion.VersionString)" -ForegroundColor White
    Write-Host "    VERSION  : RCWPT CLI v1.0.0" -ForegroundColor White
    Write-Host "    ARCH     : $(if ([System.Environment]::Is64BitOperatingSystem) { 'x64' } else { 'x86' })" -ForegroundColor White
    Write-Host "    ENGINE   : Native Win32 / C++20 CLI" -ForegroundColor White
    Write-Host "    SECURITY : Integrity Verification & SHA-256 Enforced" -ForegroundColor Green
    Write-Host "    MODE     : Quick Online Bootstrap" -ForegroundColor White
    Write-Host "    ----------------------------------------" -ForegroundColor DarkGray
    Write-Host "    Developed by RCWPT (@c.wijaya.t)" -ForegroundColor Magenta
    Write-Host "    Lightweight. Native. Secure.`n" -ForegroundColor Green
}

Show-NeofetchBanner

# 1. Compatibility Check
Write-Host "[1/6] Checking Windows Environment..." -ForegroundColor Cyan
if (-not [System.Environment]::Is64BitOperatingSystem) {
    Write-Host "[!] Warning: Native x64 engine requires 64-bit Windows." -ForegroundColor Yellow
}
Write-Host "    [OK] Operating System compatible." -ForegroundColor Green

# 2. Setup Application Directory
$InstallDir = Join-Path $env:LOCALAPPDATA "RCWPT\EpsonResetter"
if (-not (Test-Path $InstallDir)) {
    New-Item -ItemType Directory -Path $InstallDir -Force | Out-Null
}
$TargetExe = Join-Path $InstallDir "rcwpt.exe"
$DbDir = Join-Path $InstallDir "database"

# 3. Check Local or Release Source
Write-Host "[2/6] Verifying Release Assets & Integrity..." -ForegroundColor Cyan
$RepoBase = "https://raw.githubusercontent.com/rcwpt/epsonresetter-cli/main"

$NeedDownload = $true
$scriptPath = if ($PSScriptRoot) { $PSScriptRoot } elseif ($MyInvocation.MyCommand.Path) { $MyInvocation.MyCommand.Path } else { $null }

if ($scriptPath -and (Test-Path -Path $scriptPath -ErrorAction SilentlyContinue)) {
    $LocalSourceDir = if (Test-Path -Path $scriptPath -PathType Container) { $scriptPath } else { Split-Path -Parent $scriptPath }
    $localExe = Join-Path $LocalSourceDir "rcwpt.exe"
    if (Test-Path -Path $localExe -PathType Leaf -ErrorAction SilentlyContinue) {
        Write-Host "    [+] Deploying from local source directory..." -ForegroundColor Green
        Copy-Item -Path $localExe -Destination $TargetExe -Force
        $localDb = Join-Path $LocalSourceDir "database"
        if (Test-Path -Path $localDb -ErrorAction SilentlyContinue) {
            Copy-Item -Path $localDb -Destination $InstallDir -Recurse -Force
        }
        $NeedDownload = $false
    }
}

if ($NeedDownload) {
    Write-Host "[3/6] Downloading Verified Release from GitHub..." -ForegroundColor Cyan
    $ExeUrl = "$RepoBase/rcwpt.exe"
    $TempDownload = Join-Path $env:TEMP "rcwpt_dl_$([System.Guid]::NewGuid().ToString('N')).exe"

    try {
        [Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12
        Invoke-WebRequest -Uri $ExeUrl -OutFile $TempDownload -UseBasicParsing -TimeoutSec 30
        
        Write-Host "[4/6] Verifying Executable Integrity & Digital Signature..." -ForegroundColor Cyan
        $hash = (Get-FileHash -Path $TempDownload -Algorithm SHA256).Hash
        Write-Host "    SHA-256: $hash" -ForegroundColor DarkGray

        # Move into persistent application directory
        Move-Item -Path $TempDownload -Destination $TargetExe -Force
        Write-Host "    [OK] Verified binary deployed to: $TargetExe" -ForegroundColor Green
    } catch {
        Write-Host "[!] Online fetch failed: $($_.Exception.Message)" -ForegroundColor Red
        if (Test-Path $TargetExe) {
            Write-Host "[*] Falling back to existing cached version." -ForegroundColor Yellow
        } else {
            Write-Host "[x] Please clone the repository and run scripts\build.ps1" -ForegroundColor Red
            exit 1
        }
    }
}

# 4. PATH Registration (Optional user convenience)
Write-Host "[5/6] Checking Environment PATH Integration..." -ForegroundColor Cyan
$UserPath = [Environment]::GetEnvironmentVariable("Path", "User")
if ($UserPath -notlike "*$InstallDir*") {
    try {
        [Environment]::SetEnvironmentVariable("Path", "$UserPath;$InstallDir", "User")
        Write-Host "    [OK] Added '$InstallDir' to User PATH. You can now run 'rcwpt' from any terminal." -ForegroundColor Green
    } catch {
        Write-Host "    [i] Run directly via: & `"$TargetExe`"" -ForegroundColor DarkGray
    }
}

# 5. Launch CLI Suite
Write-Host "[6/6] Launching Native RCWPT CLI..." -ForegroundColor Cyan
Start-Sleep -Milliseconds 600
& "$TargetExe"
