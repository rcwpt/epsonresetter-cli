# Epson Hardware & Operating System Compatibility Report
## RCWPT Epson Resetter CLI Suite v1.0.0

---

## 1. Operating System Compatibility

| OS Version | Architecture | Support Status | Notes |
| :--- | :--- | :--- | :--- |
| **Windows 11** (21H2, 22H2, 23H2, 24H2) | x64 | 🟢 Fully Supported | Native Windows Terminal & ANSI color support |
| **Windows 10** (1809 through 22H2) | x64 | 🟢 Fully Supported | Standard Command Prompt & PowerShell 5.1+ |
| **Windows 11 / 10** | ARM64 | 🟢 Supported | Runs via Windows ARM64 x64 emulation layer |
| **Windows 7 / 8.1** | x86 / x64 | 🟡 Legacy Support | May require TLS 1.2 update for online bootstrap |

---

## 2. Printer Model Compatibility Tiers

The database categorizes supported Epson printer models into clear confidence levels:

### Tier 1: VERIFIED (Hardware Tested & Confirmed)
- **Classic EcoTank L Series:** L110, L120, L130, L210, L220, L300, L310, L350, L355, L360, L365, L565.
- **Modern EcoTank Series:** L3110, L3150, L1210, L3210, L3250, L3251, L3260, L5290.
- **PrecisionCore Series:** L6160, L6170, L6190, L6161, L6171, L6091.

### Tier 2: COMMUNITY (Audited Against Shared ASIC & EEPROM Maps)
- **Expanded L Series:** L380, L385, L455, L5190, L1110, L4150.
- **EcoTank ET Series (International):** ET-2720, ET-2800, ET-2850.

### Tier 3: EXPERIMENTAL (Protocol Modeled / Malware Droppers Purged)
- **WorkForce Series:** WF-2650, WF-2660.

---

## 3. Communication Protocols

| Protocol | Channels | Description | Supported Features |
| :--- | :--- | :--- | :--- |
| **BDC / IEEE 1284.4 D4** | USB Bulk Endpoints | Bi-Directional Packet Multiplexing | EEPROM Read/Write, Serial No, Head Status |
| **ESC/P-R** | Spooler Raw Queue | Raster Printing Command Mode | Diagnostics, Nozzle Check, Test Page |
| **Win32 Spooler** | Spoolsv Local IPC | Windows Native Print Spooler Pass-Through | Queue Management, Raw Data Transmission |
