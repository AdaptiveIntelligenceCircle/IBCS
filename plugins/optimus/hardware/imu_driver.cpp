#include "imu_driver.h"
using namespace std;

#include <iostream>
#include <chrono>

#include <thread> 
#include <atomic>

namespace optimus :: hardware
{
    class StubImuDriver : public IIMuDriver
    {
        StubImuDriver() { running_ = false; }
        ~StubImuDriver() { disconnect(); }

    bool connect(const string &endpoint) override 
    {
        endpoint_ = endpoint;
        running_ = true;
        // spawn background generator
        worker_ = thread([this]() 
        {
            using namespace chrono;
            double t = 0.0;
            while (running_) 
            {
                ImuSample s;
                s.timestamp = chrono::duration<double>(steady_clock::now().time_since_epoch()).count();
                s.accel = {0.0, 0.0, -9.81};
                s.gyro = {0.0, 0.0, 0.0};
                s.msg = {0.0,0.0,0.0};
                {
                    lock_guard<std::mutex> lk(mu_);
                    last_ = s;
                }
                this_thread::sleep_for(milliseconds(10));
                t += 0.01;
            }
        });
        cout << "[IMU Stub] connected to " << endpoint << "\n";
        return true;
    }

    void disconnect() override 
    {
        running_ = false;
        if (worker_.joinable()) worker_.join();
        cout << "[IMU Stub] disconnected\n";
    }

    optional<ImuSample> read_sample() override 
    {
        lock_guard<mutex> lk(mu_);
        return last_;
    }

    bool calibrate(int duration_seconds) override 
    {
        cout << "[IMU Stub] calibrating for " << duration_seconds << "s...\n";
        this_thread::sleep_for(chrono::seconds(duration_seconds));
        cout << "[IMU Stub] calibration done\n";
        return true;
    }

private:
    string endpoint_;
    atomic<bool> running_;
    thread worker_;
    mutex mu_;
    ImuSample last_;
};

// IIMuDriver* make_stub_imu_driver() {
//     return new StubImuDriver();
// }
}
