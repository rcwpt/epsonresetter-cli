# ==============================================================================
#  RCWPT EPSON RESETTER CLI - UNINSTALLER
# ==============================================================================
#Requires -Version 5.1

$InstallDir = Join-Path $env:LOCALAPPDATA "RCWPT\EpsonResetter"
Write-Host "[*] Removing RCWPT Epson Resetter CLI..." -ForegroundColor Cyan

if (Test-Path $InstallDir) {
    Remove-Item -Path $InstallDir -Recurse -Force
    Write-Host "[+] Removed application directory: $InstallDir" -ForegroundColor Green
}

# Remove from User PATH
$UserPath = [Environment]::GetEnvironmentVariable("Path", "User")
if ($UserPath -like "*$InstallDir*") {
    $newPath = ($UserPath -split ';' | Where-Object { $_ -ne $InstallDir -and $_ -ne "" }) -join ';'
    [Environment]::SetEnvironmentVariable("Path", $newPath, "User")
    Write-Host "[+] Cleaned up User PATH." -ForegroundColor Green
}

Write-Host "[+] Uninstallation complete." -ForegroundColor Green
