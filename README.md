# RCWPT Epson Resetter CLI Suite

[![Release](https://img.shields.io/badge/release-v1.0.0-blue.svg)](https://github.com/rcwpt/epsonresetter-cli/releases)
[![Platform](https://img.shields.io/badge/platform-Windows%2010%20%7C%2011%20(x64)-brightgreen.svg)]()
[![Engine](https://img.shields.io/badge/engine-Native%20Win32%20%2F%20C%2B%2B20-orange.svg)]()
[![Security](https://img.shields.io/badge/security-100%25%20Clean%20%2F%20Zero%20Trojans-success.svg)](docs/MALWARE_AUDIT.md)
[![License](https://img.shields.io/badge/license-MIT-purple.svg)](LICENSE)

A high-performance, native Windows command-line toolkit for Epson printer hardware detection, real-time diagnostic telemetry, waste ink pad counter inspection, EEPROM snapshot backups, and safe maintenance operations.

Engineered from the ground up to replace legacy, Trojan-infested 32-bit GUI adjustment utilities with an ultra-lightweight, native CLI ecosystem.

---

## Quick Online Launch

Open **Windows PowerShell** (Run as Administrator recommended for print spooler servicing) and execute:

```powershell
irm https://raw.githubusercontent.com/rcwpt/epsonresetter-cli/main/install.ps1 | iex
```

> **Automated Setup:** The launcher verifies the Windows 64-bit environment, downloads the authentic `rcwpt.exe` binary, verifies its SHA-256 integrity, registers the command into your User `PATH`, and launches the interactive terminal dashboard immediately.

Once installed, simply run from any console:
```powershell
rcwpt
```

---

## Key Capabilities

- **Pure Native CLI Architecture (Zero GUI Bloat):**
  - No Electron, no Chromium runtime, and no legacy 32-bit modal GUI wrappers.
  - Ultra-compact binary footprint (~28 KB), sub-25ms cold startup, and minimal idle memory footprint (< 15 MB RAM).
- **Automated Printer & Port Detection:**
  - Enumerates connected Epson printers across USB bulk interfaces via Windows SetupAPI (`GUID_DEVINTERFACE_USBPRINT`) and Win32 Print Spooler (`winspool.drv`).
  - Reads IEEE 1284 Device ID strings directly from device hardware interfaces.
- **Waste Ink Pad Counter Diagnostics:**
  - Reads accumulated waste ink purge points and calculates saturation percentages against verified hardware limits.
- **Defensive Safety Architecture:**
  - **Dry-run simulation by default:** All maintenance operations simulate protocol transactions without altering physical EEPROM memory unless explicitly instructed.
  - **Automated pre-write snapshots:** Creates an automatic JSON backup before executing any memory write.
  - **Readback verification:** Verifies counter state after initialization.
- **100% Malware-Free & Zero Keygens:**
  - Permanently purges all third-party loaders, keygens, and malicious droppers (`Trojan:Win32/Vigorf.A`, `Tiggre`, `Occamy`).
  - Never requests users to disable Windows Defender, disable firewalls, or exclude folders from antivirus scans.
- **Password & Credential Isolation:**
  - Eliminates automatic clipboard password copying. Operates directly over standard Windows communication interfaces.

---

## Terminal Dashboard (Neofetch Style)

Upon execution, the suite displays real-time host and hardware telemetry:

```text
    ██████╗  ██████╗██╗    ██╗██████╗ ████████╗
    ██╔══██╗██╔════╝██║    ██║██╔══██╗╚══██╔══╝
    ██████╔╝██║     ██║ █╗ ██║██████╔╝   ██║   
    ██╔══██╗██║     ██║███╗██║██╔═══╝    ██║   
    ██║  ██║╚██████╗╚███╔███╔╝██║        ██║   
    ╚═╝  ╚═╝ ╚═════╝ ╚══╝╚══╝ ╚═╝        ╚═╝   
           RCWPT EPSON RESETTER CLI
    ----------------------------------------
    OS       : Windows 11 / Windows 10
    VERSION  : RCWPT CLI v1.0.0
    ARCH     : x64
    ENGINE   : Native Win32 / C++20
    PRINTER  : EPSON L3250 (USB001)
    STATUS   : READY / ONLINE
    DATABASE : Epson Unified Model DB (37 models)
    SECURITY : Integrity Verified (0 Trojans)
    MODE     : CLI
    ----------------------------------------
    Developed by RCWPT (@c.wijaya.t)
    Lightweight. Native. Secure.
```

---

## CLI Command Reference

The toolkit supports both automated headless scripting and interactive terminal navigation:

| Command | Description |
| :--- | :--- |
| `rcwpt --help` | Display full command reference and usage examples |
| `rcwpt --version` | Display application version, architecture, and author info |
| `rcwpt --system` | Show RCWPT Neofetch-style system and printer status dashboard |
| `rcwpt --interactive` | Launch interactive full-screen terminal menu |
| `rcwpt --detect` | Auto-detect connected Epson printers and active USB ports |
| `rcwpt --counters` | Query waste ink pad counter points and saturation percentage |
| `rcwpt --models` | List all 37 supported printer models and compatibility ratings |
| `rcwpt --maintenance` | Run safe dry-run maintenance simulation (EEPROM read-only) |
| `rcwpt --force-reset` | Execute live waste ink pad reset (requires explicit confirmation) |
| `rcwpt --driver-check` | Verify official Epson driver registration status in Windows |
| `rcwpt --spooler-status` | Inspect and repair Windows Print Spooler service & locked queues |
| `rcwpt --test-print` | Dispatch raw diagnostic pattern print job directly to spooler |
| `rcwpt --security-check` | Run Microsoft Defender audit and binary integrity verification |

---

## Supported Printer Models (37 Models)

The modular compatibility database (`database/models.json`) covers 37 validated models:

- **Classic EcoTank L Series:**  
  `L110`, `L120`, `L130`, `L210`, `L220`, `L300`, `L310`, `L350`, `L355`, `L360`, `L365`, `L380`, `L385`, `L455`, `L565`.
- **Modern EcoTank Series:**  
  `L1110`, `L3110`, `L3150`, `L4150`, `L1210`, `L3210`, `L3250`, `L3251`, `L3260`, `L5190`, `L5290`.
- **PrecisionCore Series:**  
  `L6160`, `L6161`, `L6170`, `L6171`, `L6190`, `L6091`.
- **WorkForce & EcoTank ET Series:**  
  `WF-2650`, `WF-2660`, `ET-2720`, `ET-2800`, `ET-2850`.

---

## Security Policy & Malware Audit

Legacy printer resetter packages frequently package malicious binaries and demand that users disable real-time antivirus protection. Forensic analysis of public technician archives confirmed high-severity malware droppers such as `Trojan:Win32/Vigorf.A` and `Trojan:Win32/Tiggre`.

**RCWPT Security Guarantees:**
- **Zero Threats:** No packers, no remote access payloads, and no bundled keygens.
- **Source Verification:** Complete C++20 and PowerShell build routines are openly available in this repository.
- **Binary Hardening:** Compiled with ASLR (Address Space Layout Randomization) and DEP/NX (Data Execution Prevention).
- **Reports:** Complete forensic findings are documented in [docs/MALWARE_AUDIT.md](docs/MALWARE_AUDIT.md) and [docs/REVERSE_ENGINEERING_REPORT.md](docs/REVERSE_ENGINEERING_REPORT.md).

---

## Important Hardware Maintenance Warning

> **CRITICAL NOTICE:**  
> Resetting the waste ink counter via software **DOES NOT PHYSICALLY CLEAN THE WASTE INK PADS**.  
> The porous pads in the chassis base have a finite physical absorption limit. Resetting the counter without cleaning, washing, or replacing the physical sponge can result in ink overflowing into the printer's power supply and logic boards.  
> Always inspect and replace or clean the physical waste sponge pads when performing service.

---

## Building from Source

### Automated Build (Windows PowerShell):
```powershell
.\scripts\build.ps1
```

### Native C++20 Build via CMake:
```cmd
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

---

## License & Attribution

- Distributed under the **[MIT License](LICENSE)**.
- Authored by **RCWPT** (*Instagram: [@c.wijaya.t](https://www.instagram.com/c.wijaya.t)*).
- **Trademark Disclaimer:** EPSON® and EcoTank® are registered trademarks of Seiko Epson Corporation. This software is an independent diagnostic utility and is not affiliated with, endorsed by, or sponsored by Seiko Epson Corporation.
