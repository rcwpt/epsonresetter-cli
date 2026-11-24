# ==============================================================================
#  RCWPT EPSON RESETTER CLI - BUILD SCRIPT
#  Compiles both Native C++ (via CMake) and Standalone Windows Executable
# ==============================================================================
#Requires -Version 5.1

$ScriptDir = if ($PSScriptRoot) { $PSScriptRoot } else { Split-Path -Parent $MyInvocation.MyCommand.Path }
$ProjectRoot = Split-Path -Parent $ScriptDir
Set-Location $ProjectRoot

Write-Host "================================================================================" -ForegroundColor Cyan
Write-Host "             RCWPT EPSON RESETTER CLI - AUTOMATED BUILD PIPELINE                " -ForegroundColor Yellow
Write-Host "================================================================================" -ForegroundColor Cyan

# 1. Standalone Native Win32 Build (via csc.exe)
Write-Host "`n[1/3] Building Standalone Portable Executable (rcwpt.exe)..." -ForegroundColor Cyan
$cscPath = "C:\Windows\Microsoft.NET\Framework64\v4.0.30319\csc.exe"
if (-not (Test-Path $cscPath)) {
    $cscPath = "C:\Windows\Microsoft.NET\Framework\v4.0.30319\csc.exe"
}

$targetExe = Join-Path $ProjectRoot "rcwpt.exe"
$srcFile = Join-Path $ProjectRoot "src\NativeCli.cs"

if (Test-Path $cscPath) {
    & $cscPath /target:exe /platform:x64 /optimize+ /out:$targetExe $srcFile
    if (Test-Path $targetExe) {
        $hash = (Get-FileHash -Path $targetExe -Algorithm SHA256).Hash
        $sizeKb = [math]::Round((Get-Item $targetExe).Length / 1KB, 2)
        Write-Host "    [OK] Compiled rcwpt.exe successfully ($sizeKb KB)" -ForegroundColor Green
        Write-Host "    SHA-256: $hash" -ForegroundColor DarkGray
    } else {
        Write-Host "    [x] Failed to compile standalone binary." -ForegroundColor Red
    }
} else {
    Write-Host "    [!] csc.exe not found on system." -ForegroundColor Yellow
}

# 2. CMake / C++20 Build Check
Write-Host "`n[2/3] Checking C++20 CMake Environment..." -ForegroundColor Cyan
if (Get-Command cmake -ErrorAction SilentlyContinue) {
    Write-Host "    [+] CMake detected. Configuring build directory..." -ForegroundColor Green
    cmake -B build -S .
    cmake --build build --config Release
} else {
    Write-Host "    [i] CMake not in active PATH. Standalone rcwpt.exe is ready." -ForegroundColor DarkGray
    Write-Host "    (C++20 source tree is prepared for GitHub Actions CI)." -ForegroundColor DarkGray
}

# 3. Security & Hash Manifest Generation
Write-Host "`n[3/3] Generating Release Hashes & Manifest..." -ForegroundColor Cyan
$manifest = @()
Get-ChildItem -Path $ProjectRoot -Recurse | Where-Object { $_.Extension -match "\.(exe|json|md|ps1)$" -and $_.FullName -notmatch "\\build|\\.git" } | ForEach-Object {
    $h = Get-FileHash -Path $_.FullName -Algorithm SHA256
    $rel = $_.FullName.Substring($ProjectRoot.Length).TrimStart('\', '/')
    $manifest += "$($h.Hash)  $rel"
}
$manifestFile = Join-Path $ProjectRoot "SHA256SUMS.txt"
$manifest | Set-Content -Path $manifestFile -Encoding UTF8
Write-Host "    [OK] Generated SHA256SUMS.txt with $($manifest.Count) entries." -ForegroundColor Green

Write-Host "`n================================================================================" -ForegroundColor Cyan
Write-Host "  BUILD COMPLETE: RCWPT Epson Resetter CLI is ready." -ForegroundColor Green
Write-Host "================================================================================`n" -ForegroundColor Cyan
