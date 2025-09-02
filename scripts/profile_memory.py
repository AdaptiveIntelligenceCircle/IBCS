print("profile memory")

#!/usr/bin/env python3
import psutil, time, argparse, csv, matplotlib.pyplot as plt

def profile_memory(pid, duration, interval, outlog=None, plot=False):
    process = psutil.Process(pid)
    data = []
    start = time.time()
    while time.time() - start < duration:
        mem = process.memory_info().rss / (1024 * 1024)  # MB
        t = time.time() - start
        print(f"[{t:.2f}s] {mem:.2f} MB")
        data.append((t, mem))
        time.sleep(interval)

    if outlog:
        with open(outlog, "w", newline="") as f:
            writer = csv.writer(f)
            writer.writerow(["time_sec", "memory_mb"])
            writer.writerows(data)

    if plot:
        ts, ms = zip(*data)
        plt.plot(ts, ms, marker="o")
        plt.xlabel("Time (s)")
        plt.ylabel("Memory (MB)")
        plt.title(f"Memory profile PID {pid}")
        plt.show()

def main():
    parser = argparse.ArgumentParser(description="Profile memory usage of IBCS runtime")
    parser.add_argument("--process", type=int, required=True, help="Target process ID")
    parser.add_argument("--duration", type=int, default=10)
    parser.add_argument("--interval", type=float, default=1.0)
    parser.add_argument("--log", help="Output log CSV file")
    parser.add_argument("--plot", action="store_true")
    args = parser.parse_args()

    profile_memory(args.process, args.duration, args.interval, args.log, args.plot)

if __name__ == "__main__":
    main()


