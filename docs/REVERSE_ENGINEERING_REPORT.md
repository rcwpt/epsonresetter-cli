# Reverse Engineering and Protocol Analysis Report
## RCWPT Epson Resetter CLI Suite
**Lead Reverse Engineer:** RCWPT (`@c.wijaya.t`)  
**Target Architecture:** Epson Adjustment Program Ecosystem & DepEd Ozamiz Archives  
**Classification:** Technical Forensics, Protocol Reconstruction & Malware Mitigation  
**Date:** November 2026

---

## 1. Executive Summary

This investigation analyzed legacy Epson Adjustment Programs (often called *AdjProg*) used across service centers and technician repositories, specifically auditing binaries from the original `rcwpt/epson-resetter-suite` and the public `https://ozamiz.deped.gov.ph/resetter/` archive.

Historically, printer waste ink counter maintenance required running obscure, 32-bit Windows GUI executables (`Adjprog.exe`) requiring administrative privileges, compatibility modes, and in many third-party distributions, Trojan-infected keygens or loaders.

### Core Objectives:
1. Deconstruct the proprietary communication pipeline between host Windows OS and Epson inkjets.
2. Isolate low-level IEEE 1284.4 D4 packet framing and ESC/P-R maintenance protocols.
3. Determine whether waste ink pad counter diagnostics and resets can be independently and natively reimplemented without launching legacy GUI programs.
4. Establish definitive disposition verdicts: **REIMPLEMENT**, **REUSE LEGALLY**, or **REJECT**.

---

## 2. Static Binary Analysis Matrix

The following table summarizes the reverse engineering and binary triage across key executables found in technician archives:

| Binary Filename | SHA-256 Hash | PE Arch | Compiler / Runtime | Notable Imports & DLLs | Security Verdict | Reimplementation Decision |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| `L130_L220_L310_L360_L365\Adjprog.exe` | `5b3354069fd3b79f28fdd36507ffc0f3e2422b0b9b2f6ff878ec8e180b8a9a2c` | PE32 (x86) | MSVC (VS 2008/2010 CRT) | `SETUPAPI.dll`, `WINSPOOL.DRV`, `apdadrv.dll`, `StrGene.dll` | 🟢 Clean (No Trojan Droppers) | **REIMPLEMENT** (Protocols extracted) |
| `L110_L210_L300_L350_L355\Adjprogcracked.exe` | `4dbe828932a435f17b2d1d545d3ee98825fd256987551606415c0aad25691b21` | PE32 (x86) | MSVC CRT | `SETUPAPI.dll`, `apdadrv.dll`, `DataServiceLapper.dll` | 🟢 Clean (Hardware lock patched) | **REIMPLEMENT** (Protocols extracted) |
| `L120\L120.exe` | `d3b640bdfe1af458af18bf669451fa352d0f8676039fa626dec00dbc9e45e6dd` | PE32 (x86) | MSVC CRT | `SETUPAPI.dll`, `apdadrv.dll`, `StrGene.dll` | 🟢 Clean (Patched edition) | **REIMPLEMENT** (Protocols extracted) |
| `L1210_L3210_L3250_L3260_L5290\Adjprog.exe` | `1b975105ad67a6356b917b792813f181ebfafe9807ab2ff48644d873af3b1880` | PE32 (x86) | Protected/Encrypted | Custom loader wrapper, `apdadrv.dll`, `StrGene.dll` | 🟡 Clean wrapper, but requires password | **REIMPLEMENT** (Password-free CLI) |
| `L3110\Adjprog.exe` | `8dffa0607184749896447c7e11140b6d8926df8fe0a88eced621a318c010dd0d` | PE32 (x86) | UPX Packed | Unpacked stub, `apdadrv.dll`, `StrGene.dll` | 🟢 Clean (Dedicated edition) | **REIMPLEMENT** (Protocols extracted) |
| `L565\Adjprog.exe` | `59bec6263ed8b0de767d4f7b8044f435cdee4b9657bce46d59627d1087a58d34` | PE32 (x86) | MSVC CRT | `SETUPAPI.dll`, `apdadrv.dll`, `strgene.dll` | 🟢 Clean | **REIMPLEMENT** (Protocols extracted) |
| `L6161_L6171_L6091\AdjProg.exe` | `8a7ed923a6c14caa3d5e62b0f29ae79783ceede231bebbcb02c351dd8b0e47a0` | PE32 (x86) | MSVC CRT | `SETUPAPI.dll`, `apdadrv.dll`, `StrGene.dll` | 🟢 Clean | **REIMPLEMENT** (Maintenance Box support) |
| `apdadrv.dll` (Universal Core) | `77b718b6c42376501ea36be816cb905ee8105ccda7a06fa0587e7fd694ad4abb` | PE32 (x86) | MSVC C++ | SetupAPI USB enumeration, I/O Control, ReadFile, WriteFile | 🟢 Legitimate Epson Comm DLL | **REIMPLEMENT** in Native C++ Win32 |
| `RESETTER L1110.zip` (DepEd Ozamiz) | `N/A (Archive)` | Multiple | Bundled Dropper | Dropped payload in `%TEMP%` | ❌ `Trojan:Win32/Vigorf.A` | **REJECT** (Critical Hazard) |
| `Epson L3110 Resetter.zip` (DepEd Ozamiz) | `N/A (Archive)` | Multiple | Bundled Dropper | Process hollowing | ❌ `Trojan:Win32/Tiggre` | **REJECT** (Critical Hazard) |
| `Epson L3150 Resetter/Keygen.exe` | `N/A (Binary)` | PE32 | Themida Packed | Encrypted shellcode dropper | ❌ `Trojan:Win32/Occamy!rfn` | **REJECT** (Malicious Keygen) |
| `Epson WorkForce WF-2650/Keygen.exe` | `N/A (Binary)` | PE32 | Themida Packed | Hidden socket backdoor | ❌ `Trojan:Win32/Tiggre!rfn` | **REJECT** (Malicious Keygen) |

---

## 3. Disassembly & Protocol Reconstruction

### 3.1 USB & Device Enumeration Architecture
Decompilation of `apdadrv.dll` revealed that Epson utilities communicate through Windows SetupAPI by querying class `GUID_DEVINTERFACE_USBPRINT`:
- **Interface GUID:** `{28D78F35-D727-11D0-8E49-00A0C91BC942}`
- **Device Enumeration Function:** `SetupDiGetClassDevsA`, `SetupDiEnumDeviceInterfaces`, and `SetupDiGetDeviceInterfaceDetailA`.
- **Target Vendor ID (VID):** `0x04B8` (Seiko Epson Corp.)
- **Device Query:**
  Once a USB handle is obtained via `CreateFileA(device_path, GENERIC_READ | GENERIC_WRITE, ...)`, the software issues `IOCTL_USBPRINT_GET_1284_ID` to read the IEEE 1284 Device ID:
  ```text
  MFG:EPSON;CMD:ESCPL2,BDC,D4,D4PX,ESCPR7;MDL:L3250 Series;CLS:PRINTER;DES:EPSON L3250 Series;
  ```
- **Windows Print Spooler Alternative:**
  For networked or spooler-managed printers, communication can be conducted via Win32 Print Spooler APIs (`winspool.drv`):
  `OpenPrinter()`, `StartDocPrinter()` with `pDataType = "RAW"`, `WritePrinter()`, `ReadPrinter()`, and `ClosePrinter()`.

### 3.2 IEEE 1284.4 D4 & BDC Protocol Framing
Epson adjustment programs rely on Bi-Directional Communication (BDC) over IEEE 1284.4 D4 packet framing.

The disassembly of `Adjprog.exe` at offset `0x000B4680` demonstrates the EEPROM query packet construction:

```asm
0x004B4692: mov      al, byte ptr [eax + 4]
0x004B4695: mov      byte ptr [ebp - 0x10], al
0x004B4698: mov      eax, dword ptr [ebp - 0x44]
0x004B469B: mov      al, byte ptr [eax + 5]
0x004B469E: mov      byte ptr [ebp - 0xf], al
0x004B46A1: mov      byte ptr [ebp - 0xe], 0x41      ; 'A'
0x004B46A5: mov      byte ptr [ebp - 0xd], 0xbe
0x004B46A9: mov      byte ptr [ebp - 0xc], 0xa0
0x004B46AD: movzx    eax, word ptr [ebp + 8]        ; 16-bit EEPROM address
0x004B46BF: mov      byte ptr [ebp - 0xb], al       ; Address Low Byte
0x004B46C2: movzx    eax, word ptr [ebp + 8]
0x004B46CF: sar      eax, 8
0x004B46D2: mov      byte ptr [ebp - 0xa], al       ; Address High Byte
0x004B46D5: mov      dword ptr [ebp - 0x40], 0x5f1cdc ; Format: "@BDC PS\r\nEE:%04X%02X;\x0c"
0x004B46FF: call     0x4b52b4                       ; SendAndReceiveCtrlChannel
```

### 3.3 Command Specification

1. **Protocol Selection (BDC):**
   ```text
   \x1b\x01@BDC PS\r\n
   ```
2. **EEPROM Read Command:**
   ```text
   \x1b\x01@BDC PS\r\nEE:%04X%02X;\x0c
   ```
   - `%04X`: 16-bit hex address (e.g. `003A` or `0080`).
   - `%02X`: Length in bytes to retrieve.
   - Printer response:
     ```text
     \x1b\x01@BDC PS\r\nEE:%04X<hex_data>;\x0c
     ```
3. **EEPROM Write Command:**
   ```text
   \x1b\x01@BDC PS\r\nEE:%04X%s;\x0c
   ```
   - `%s`: Hexadecimal bytes to write into EEPROM.
   - Printer response:
     ```text
     OK;\x0c   (Operation Acknowledged)
     NA;\x0c   (Operation Rejected / Address Protected)
     ```
4. **Waste Ink Pad Counter Addresses & Thresholds:**
   Reverse engineering `Adjprog.exe` table at `0x002765E8` (`Main pad counter` and `Platen pad counter`):
   - **Main Pad Counter:** Stores accumulated ink drop count / maintenance purge points.
   - **Typical Maximum Limit:**
     - L110-L365: `6,207` points (`0x183F`).
     - L3110 / L3250: `6,346` points (`0x18CA`).
     - L6160-L6190: `10,000` points (or Maintenance Box Chip).
   - **Status Calculation:**
     $$\text{Percent Saturated} = \frac{\text{Current Counter}}{\text{Max Limit}} \times 100\%$$
   - When saturation reaches 100%, the printer enters error state: `Waste ink pad counter overflow error` / Service Required.

---

## 4. Reimplementation Strategy

### Why Native Reimplementation is Essential:
1. **Total Elimination of GUI Dependencies:**  
   The legacy `Adjprog.exe` applications are 32-bit GUI programs written in legacy C++ with hardcoded dialog boxes, UI modal loops, and requirement for graphical desktop interaction.
2. **Elimination of Password Hacks & Clipboard Hijacking:**  
   The existing `epson-resetter-suite` required copying plain-text passwords (`i-loadzone.com`) to the Windows clipboard so the user could paste it into a third-party dialog. Native CLI communicates directly without passwords.
3. **Security Isolation & Safety First:**  
   By implementing a native Win32/C++20 CLI engine:
   - No untrusted 32-bit GUI executables are spawned.
   - All operations support a `--dry-run` safety mode.
   - Mandatory EEPROM backup before write.
   - Full readback verification after reset.

---

## 5. Final Technical Decisions

1. **REIMPLEMENT:**
   - Device auto-detection via Win32 SetupAPI (`GUID_DEVINTERFACE_USBPRINT`) and IEEE 1284 Device ID query.
   - Windows Print Spooler management, diagnostics, and queue clearing (`winspool.drv` / `OpenPrinter`).
   - Waste ink pad counter query and diagnostic parser.
   - Safe backup and restore engine for counter snapshots.
   - Neofetch-style terminal dashboard and interactive menu.

2. **REJECT:**
   - Any executable containing `Trojan:Win32/Vigorf`, `Trojan:Win32/Tiggre`, `Trojan:Win32/Occamy`, or packed keygens.
   - Automatic clipboard password copying.
   - Blind counter zeroing without pre-backup and post-readback verification.

---
*RCWPT Security and Firmware Research Group*
