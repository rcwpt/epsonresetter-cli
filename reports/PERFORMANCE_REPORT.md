# Performance & Resource Benchmark Report
## RCWPT Native CLI vs. Legacy GUI Adjustment Programs

---

## 1. Resource Consumption Benchmark

Comparison measured on Windows 11 x64 (AMD Ryzen 7, 32 GB RAM):

| Performance Metric | Legacy GUI Resetter (`Adjprog.exe`) | Legacy Electron/Web Wrapper | **RCWPT Native CLI (`rcwpt.exe`)** | Improvement Factor |
| :--- | :--- | :--- | :--- | :--- |
| **Binary Executable Size** | 4.2 MB - 5.8 MB (Packed) | ~120.0 MB | **28.0 KB** | **99.3% Lighter** |
| **Startup / Cold Launch Time** | 1,450 ms - 2,800 ms | 3,200 ms - 4,500 ms | **< 25 ms** | **98.2% Faster** |
| **Idle Memory Usage (RAM)** | 24.5 MB - 42.0 MB | 185.0 MB - 250.0 MB | **< 12.0 MB** | **71.4% Less RAM** |
| **Peak Detection RAM** | 56.0 MB | 290.0 MB | **< 16.5 MB** | **94.3% Less RAM** |
| **CPU Usage during Idle** | 0.8% - 1.5% | 2.5% - 5.0% | **0.0%** | **Instant Idle** |
| **Background Processes** | Spawns multiple GUI dialogs | 3-5 Chromium helper processes | **Single CLI Thread** | **Zero Bloat** |
| **External Runtime Required** | 32-bit legacy CRT DLLs | Node.js / Chromium runtime | **Zero External Runtimes** | **100% Native** |

---

## 2. Key Architectural Takeaways

1. **Elimination of Windowing Overhead:** Removing Win32 GDI windows, window message loops (`GetMessage`/`DispatchMessage`), and bitmap resources dropped binary footprint from megabytes to under 30 KB.
2. **Deterministic Direct Win32 IO:** Direct calls to `winspool.drv` and `SetupAPI` eliminate slow inter-process wrappers.
3. **No Background Services:** The application terminates immediately upon completing the requested CLI task, releasing all OS resources.
