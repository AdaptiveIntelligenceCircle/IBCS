#!/usr/bin/env python3
import subprocess, os, sys, json, platform

ROOT_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
SCRIPTS_DIR = os.path.join(ROOT_DIR, "scripts")
LOGS_DIR = os.path.join(ROOT_DIR, "tests", "logs")
os.makedirs(LOGS_DIR, exist_ok=True)

def run_cmd(cmd):
    print(f"[RUN] {' '.join(cmd)}")
    result = subprocess.run(cmd, capture_output=True, text=True)
    print(result.stdout)
    if result.stderr:
        print("[STDERR]", result.stderr)
    result.check_returncode()

# -----------------------------------------------------------------
# 1. Test export_behavior.py
# -----------------------------------------------------------------
def test_export_behavior():
    print("\n=== TEST: export_behavior.py ===")
    ir_file = os.path.join(LOGS_DIR, "sample_ir.json")
    out_json = os.path.join(LOGS_DIR, "out.json")
    out_dot = os.path.join(LOGS_DIR, "out.dot")

    # Sample IR
    with open(ir_file, "w") as f:
        json.dump({
            "nodes": [
                {"id": "n1", "label": "Start"},
                {"id": "n2", "label": "Action"}
            ],
            "edge": [
                {"source": "n1", "target": "n2", "relation": "next"}
            ]
        }, f)

    run_cmd([sys.executable, os.path.join(SCRIPTS_DIR, "export_behavior.py"),
             ir_file, "--to_json", out_json, "--to_dot", out_dot])

    assert os.path.exists(out_json), "out.json not created"
    assert os.path.exists(out_dot), "out.dot not created"
    print("[PASS] export_behavior.py")

# -----------------------------------------------------------------
# 2. Test profile_memory.py
# -----------------------------------------------------------------
def test_profile_memory():
    print("\n=== TEST: profile_memory.py ===")
    log_file = os.path.join(LOGS_DIR, "mem_profile.csv")

    run_cmd([sys.executable, os.path.join(SCRIPTS_DIR, "profile_memory.py"),
             "--process", str(os.getpid()),
             "--duration", "2", "--interval", "1",
             "--log", log_file])

    assert os.path.exists(log_file), "mem_profile.csv not created"
    print("[PASS] profile_memory.py")

# -----------------------------------------------------------------
# 3. Test run_simulator (sh/ps1)
# -----------------------------------------------------------------
def test_run_simulator():
    print("\n=== TEST: run_simulator ===")
    build_dir = os.path.join(ROOT_DIR, "build")
    os.makedirs(build_dir, exist_ok=True)
    sim_exec = os.path.join(build_dir, "ibcs_simulator")

    # Mock executable
    with open(sim_exec, "w") as f:
        if platform.system() == "Windows":
            f.write("@echo off\n")
            f.write("echo Mock simulator running with config: %*\n")
        else:
            f.write("#!/usr/bin/env bash\n")
            f.write("echo 'Mock simulator running with config: $@'\n")
    os.chmod(sim_exec, 0o755)

    if platform.system() == "Windows":
        run_cmd(["pwsh", os.path.join(SCRIPTS_DIR, "run_simulator.ps1")])
    else:
        run_cmd(["bash", os.path.join(SCRIPTS_DIR, "run_simulator.sh")])

    print("[PASS] run_simulator")

# -----------------------------------------------------------------
# Main
# -----------------------------------------------------------------
if __name__ == "__main__":
    test_export_behavior()
    test_profile_memory()
    test_run_simulator()
    print("\nAll script tests passed ✅")
