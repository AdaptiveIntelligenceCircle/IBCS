#!/usr/bin/env bash 

# ===========================
# IBCS Run Simulator Script === 
#=============================
# Dung de build va chay simualator trong IBCS project 
# Ket hop core, runtime, plugin va config.. 

set -e #dung khi loi .. 

# --- Paths ---- #

ROOT_DIR = "$(cd "$( dirname "${BASH_SOURCE[0]}")/.. "&&pwd)"
BUILD_DIR = "$ROOT_DIR/build"
CONFIG_FILE = "$ROOT_DIR/config/default.yaml"
LOG_FILE = "$ROOT_DIR/logs/simulator_$(date + %Y%m%d_%H%M%S).log"

#---- Ensure dirs ---- #
mkdir -p "$BUILD_DIR"
mkdir -p "$ROOT_DIR/logs"

echo "[INFO] Root dir : $ROOT_DIR"
echo "[INFO] Build dir :$BUILD_DIR"
echo "[INFO] Log file: $LOG_FILE"

# --- Step 1 : Build (C++/rust/python hybrid)

echo "[INFO] Building project...."
cmake -S "$ROOT_DIR" -B "$BUILD_DIR"
cmake --build "$BUILD_DIR" -j$(nproc)

# neu co rust thuc nghiem ---- 
if [-d "$ROOT_DIR/plugin/rust"];  then 
echo "[INFO] Buidling RUST plugins.... "
cargo build --manifest-path "$ROOT_DIR/plugin/rust/Cargo.toml" -- release 
fi 

# step 2 : Run simulator ---- // 
echo "[INFO] Running IBCS simulator..."
EXECUTABLE = "$BUILD_DIR/ibcs_simulator"

if [ ! -f "$EXECUTABLE" ]; then
  echo "[ERROR] Simulator binary not found: $EXECUTABLE"
  exit 1
fi

# Chạy simulator với config, redirect log
"$EXECUTABLE" --config "$CONFIG_FILE" 2>&1 | tee "$LOG_FILE"

# --- Step 3: Optional Python post-process ---
if [ -f "$ROOT_DIR/scripts/profile_memory.py" ]; then
  echo "[INFO] Profiling memory usage..."
  python3 "$ROOT_DIR/scripts/profile_memory.py" "$LOG_FILE"
fi

echo "[INFO] Simulation finished successfully."
