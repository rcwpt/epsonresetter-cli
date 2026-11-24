# Security Policy
## RCWPT Epson Resetter CLI Suite

The RCWPT Epson Resetter CLI Suite is designed and maintained according to strict defensive engineering standards. It eliminates risks associated with legacy printer resetters, including hidden trojans, keygens, clipboard hijacking, and persistent backdoors.

---

## 1. Supported Versions

| Version | Supported | Security Updates |
| :--- | :--- | :--- |
| **v1.0.x** | ✅ Yes | Active Maintenance |
| Legacy PowerShell Suite (`epson-resetter-suite`) | ⚠️ Deprecated | Preserved for historical reference |

---

## 2. Core Security Principles

1. **Zero Trojan Droppers & Keygen Ban:**  
   Under no circumstances does this suite include, distribute, or execute third-party keygens, loaders, or cracked binaries flagged with remote access payloads (`Vigorf`, `Tiggre`, `Occamy`, `Ymacco`).
2. **Never Disable Antivirus / Defender:**  
   This software will **NEVER** instruct the user to disable Microsoft Defender, disable Windows Firewall, disable SmartScreen, or exclude any directory from antivirus scanning.
3. **No Credential Harvesting or Automatic Clipboard Modification:**  
   The application does not copy plain-text passwords or keys to the Windows clipboard. Any required operational secrets must be managed through secure Windows APIs (Windows Credential Manager or DPAPI).
4. **Mandatory Dry-Run & Safety Verifications:**  
   Any routine that modifies physical printer EEPROM memory defaults to `--dry-run` mode, creates an automatic backup file before writing, requires explicit user confirmation, and performs a readback verification.
5. **Supply Chain Integrity:**  
   Every distributed release includes a cryptographic SHA-256 manifest and Software Bill of Materials (SBOM).

---

## 3. Reporting a Vulnerability

If you discover a potential security flaw, unintended privilege escalation, or protocol bug:

1. **Do not create a public issue.**
2. Send an advisory via email to: `security@rcwpt.com` or contact the maintainer directly via GitHub Security Advisories.
3. Include:
   - Specific model and firmware revision tested.
   - Operating system and environment details.
   - Exact CLI parameters and output log.
   - Impact assessment.

We adhere to a 48-hour response acknowledgment policy.

---

## 4. Security Disclaimer

> **Important:** Security scanning and static analysis reduce risk but cannot guarantee that a program is completely malware-free on every computer. Users should obtain releases strictly from the official repository (`https://github.com/rcwpt/epsonresetter-cli`) and independently verify published hashes and signatures.
> 
> Resetting a waste ink counter does not physically clean or replace saturated waste ink pads. A physically overflowing pad can leak conductive ink into printer logic boards and power supplies. Always inspect and replace or clean the physical waste sponge before clearing software counters.
