import subprocess, os, sys

CLI_PATH = os.path.join(os.path.dirname(__file__), "..", "rcwpt.exe")

def run_cmd(arg):
    res = subprocess.run([CLI_PATH, arg], capture_output=True, text=True, encoding='utf-8')
    return res.returncode, res.stdout, res.stderr

def test_version():
    code, out, _ = run_cmd("--version")
    assert code == 0, f"Expected 0, got {code}"
    assert "RCWPT Epson Resetter CLI" in out
    print("[PASS] test_version")

def test_help():
    code, out, _ = run_cmd("--help")
    assert code == 0
    assert "General Commands" in out
    assert "--maintenance" in out
    print("[PASS] test_help")

def test_models():
    code, out, _ = run_cmd("--models")
    assert code == 0
    assert "L3250" in out
    assert "L3110" in out
    assert "Total Models Registered: 37" in out
    print("[PASS] test_models")

def test_security_check():
    code, out, _ = run_cmd("--security-check")
    assert code == 0
    assert "0 Threats Found" in out
    assert "ASLR + DEP/NX Enabled" in out
    print("[PASS] test_security_check")

def test_detect():
    code, out, _ = run_cmd("--detect")
    assert code == 0
    assert "Epson Printer Auto-Detection" in out
    print("[PASS] test_detect")

def test_dry_run_maintenance():
    code, out, _ = run_cmd("--maintenance")
    assert code == 0
    print("[PASS] test_dry_run_maintenance")

if __name__ == '__main__':
    print("=== Executing RCWPT Test Suite ===")
    test_version()
    test_help()
    test_models()
    test_security_check()
    test_detect()
    test_dry_run_maintenance()
    print("=== All 6 Test Scenarios PASSED ===")
