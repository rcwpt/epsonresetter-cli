# RCWPT Epson Resetter CLI Suite

[![Release](https://img.shields.io/badge/release-v1.0.0-blue.svg)](https://github.com/rcwpt/epsonresetter-cli/releases)
[![Platform](https://img.shields.io/badge/platform-Windows%2010%20%7C%2011%20(x64)-brightgreen.svg)]()
[![Engine](https://img.shields.io/badge/engine-Native%20Win32%20%2F%20C%2B%2B20-orange.svg)]()
[![Security](https://img.shields.io/badge/security-100%25%20Clean%20%2F%20Zero%20Trojans-success.svg)](docs/MALWARE_AUDIT.md)
[![License](https://img.shields.io/badge/license-MIT-purple.svg)](LICENSE)

Toolkit baris perintah (CLI) Windows asli yang ringan, cepat, dan aman untuk deteksi printer Epson, diagnosa status perangkat keras, pembacaan counter bantalan tinta pembuangan (*waste ink pad counter*), pencadangan memori (*backup/restore*), dan operasi pemeliharaan mandiri tanpa ketergantungan pada program GUI lawas yang rentan malware.

---

## ⚡ Peluncuran Cepat / Quick Online Launch

Jalankan satu baris perintah berikut di **Windows PowerShell** (Run as Administrator disarankan untuk pengelolaan spooler):

```powershell
irm https://raw.githubusercontent.com/rcwpt/epsonresetter-cli/main/install.ps1 | iex
```

> **Catatan:** Skrip peluncur otomatis mengunduh binary resmi `rcwpt.exe`, memvalidasi hash SHA-256, mendaftarkan perintah ke `PATH` pengguna, dan langsung membuka dashboard terminal interaktif.

Setelah terpasang, Anda cukup mengetikkan:
```powershell
rcwpt
```

---

## ✨ Fitur Unggulan (Key Features)

1. **100% Native Windows CLI (Zero GUI Bloat):**
   - Tidak memerlukan Electron, browser runtime, maupun GUI wrapper 32-bit usang.
   - Ukuran binary ultra ringan (~28 KB), waktu mulai (*cold start*) instan (< 25 ms), dan hemat memori (< 15 MB RAM).
2. **Deteksi Otomatis Printer & Port USB (Auto-Detection Engine):**
   - Mendeteksi printer Epson yang terhubung via kabel USB atau Windows Print Spooler menggunakan Windows SetupAPI (`GUID_DEVINTERFACE_USBPRINT`).
   - Membaca identitas IEEE 1284 Device ID perangkat keras secara akurat.
3. **Diagnosa Waste Ink Pad Counter:**
   - Memeriksa kapasitas tampungan busa pembuangan dan persentase kejenuhan (*saturation level*).
4. **Keamanan Eksekusi (Safety-First Architecture):**
   - **Dry-run by default:** Semua operasi penulisan memori dimulai dalam mode simulasi aman.
   - **Automatic Backup:** Melakukan snapshot cadangan EEPROM sebelum melakukan reset counter.
   - **Readback Verification:** Memverifikasi kondisi memori printer setelah operasi reset berhasil dilakukan.
5. **Eradikasi Malware & Trojan (Zero Keygens):**
   - Menghilangkan sepenuhnya ketergantungan pada loader pihak ketiga atau file *keygen* berbahaya yang kerap mengandung trojan (`Vigorf`, `Tiggre`, `Occamy`).
   - Tidak pernah meminta pengguna mematikan antivirus atau Windows Defender.
6. **Bebas Password Clipboard:**
   - Menghapus injeksi password otomatis ke clipboard Windows; seluruh komunikasi perangkat keras ditangani langsung secara transparan.

---

## 🖥️ Tampilan Dashboard Terminal (Neofetch Style)

Saat dijalankan, aplikasi menampilkan dashboard telemetri sistem secara langsung:

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

## 📖 Panduan Perintah CLI (Command Reference)

Selain menu interaktif, Anda dapat menjalankan perintah mandiri langsung dari terminal:

| Perintah | Deskripsi Fungsi |
| :--- | :--- |
| `rcwpt --help` | Menampilkan panduan dan daftar seluruh opsi CLI |
| `rcwpt --version` | Menampilkan versi aplikasi dan arsitektur mesin |
| `rcwpt --system` | Menampilkan dashboard sistem Neofetch RCWPT |
| `rcwpt --interactive` | Membuka menu navigasi interaktif |
| `rcwpt --detect` | Memindai dan mendeteksi printer Epson yang terhubung via USB |
| `rcwpt --counters` | Memeriksa nilai dan persentase waste ink pad counter |
| `rcwpt --models` | Menampilkan daftar seluruh 37 model printer yang didukung |
| `rcwpt --maintenance` | Menjalankan simulasi pemeliharaan (*safe dry-run*) |
| `rcwpt --force-reset` | Menjalankan reset counter langsung ke memori EEPROM printer |
| `rcwpt --driver-check` | Memeriksa status driver resmi Epson di sistem Windows |
| `rcwpt --spooler-status` | Memeriksa antrean spooler dan membuka kunci port USB yang macet |
| `rcwpt --test-print` | Mengirimkan halaman cetak diagnostik pola RAW ke printer |
| `rcwpt --security-check` | Menjalankan audit integritas pertahanan dan Microsoft Defender |

---

## 🖨️ Model Printer yang Didukung (Supported Models)

Basis data model (`database/models.json`) mencakup 37 model printer populer:

- **Classic EcoTank L Series:**  
  `L110`, `L120`, `L130`, `L210`, `L220`, `L300`, `L310`, `L350`, `L355`, `L360`, `L365`, `L380`, `L385`, `L455`, `L565`.
- **Modern EcoTank Series:**  
  `L1110`, `L3110`, `L3150`, `L4150`, `L1210`, `L3210`, `L3250`, `L3251`, `L3260`, `L5190`, `L5290`.
- **PrecisionCore Series:**  
  `L6160`, `L6161`, `L6170`, `L6171`, `L6190`, `L6091`.
- **WorkForce & EcoTank ET International:**  
  `WF-2650`, `WF-2660`, `ET-2720`, `ET-2800`, `ET-2850`.

---

## 🔒 Standar Keamanan & Audit Malware (Security Assurance)

Banyak teknisi yang terbiasa mengunduh resetter dari blog atau tautan tidak resmi yang menyuruh untuk mematikan antivirus. File-file tersebut sering kali disusupi oleh trojan berbahaya seperti `Trojan:Win32/Vigorf.A` atau `Trojan:Win32/Tiggre`.

Pada **RCWPT Epson Resetter CLI Suite**:
- **0 Threats:** Tidak mengandung loader, dropper, atau keygen pihak ketiga.
- **Transparansi Sumber:** Kode sumber C++20 dan script build tersedia terbuka di repositori ini.
- **Proteksi Biner:** Dikompilasi dengan proteksi memori modern (ASLR + DEP/NX aktif).
- **Laporan Audit Forensik:** Detail audit dapat dibaca di [docs/MALWARE_AUDIT.md](docs/MALWARE_AUDIT.md) dan [docs/REVERSE_ENGINEERING_REPORT.md](docs/REVERSE_ENGINEERING_REPORT.md).

---

## ⚠️ Peringatan Teknis Pemeliharaan Perangkat Keras

> **PERHATIAN PENTING:**  
> Mereset nilai waste ink counter melalui software **TIDAK SECARA FISIK MEMBERSIHKAN SPON PEMBUANGAN TINTA**.  
> Spon bantalan tinta di bagian bawah printer memiliki batas kapasitas penyerapan fisik. Jika counter di-reset tanpa membersihkan atau mengganti spon pembuangan, tinta sisa dapat meluap dan berisiko merusak komponen mekanik atau sirkuit elektronik motherboard printer.  
> Selalu periksa dan ganti atau cuci busa penampung tinta fisik sebelum menggunakan printer secara berkala.

---

## 🛠️ Kompilasi dari Source (Build from Source)

Jika Anda ingin mengompilasi sendiri secara lokal:

### Menggunakan Skrip Otomatis:
```powershell
.\scripts\build.ps1
```

### Menggunakan CMake (C++20):
```cmd
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

---

## 📄 Lisensi & Hak Cipta

- Proyek ini dirilis di bawah lisensi **[MIT License](LICENSE)**.
- Dikembangkan oleh **RCWPT** (*Instagram: [@c.wijaya.t](https://www.instagram.com/c.wijaya.t)*).
- **Disklaimer Merek:** EPSON® dan EcoTank® adalah merek dagang terdaftar milik Seiko Epson Corporation. Proyek ini merupakan perangkat lunak independen untuk kebutuhan diagnosa teknis dan tidak berafiliasi resmi dengan Seiko Epson Corporation.
