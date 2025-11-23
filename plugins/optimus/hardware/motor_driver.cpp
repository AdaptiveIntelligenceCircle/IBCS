#include "motor_driver.h"
#include <iostream> 

#include <unordered_map> 
#include <mutex> 

#include <algorithm>
using namespace std; 

namespace optimus :: hardware
{
    class StubMotorDriver : public IMotorDriver
    {
        public: 
        StubMotorDriver(){
            // create 12 stub motors..
            for (int i = 0 ; i < 12 ; i++){
                MotorSpec s; 
                s.id = i; 
                s.max_torque = 8.0; 
                s.max_velocity = 6.0; 
                s.position_lower = -3.0; 
                s.position_upper = 3.0; 
                s.model = "stub-motor-v1"; 
                specs_.push_back(s); 

                JoinState js; 
                js.id = i; 
                js.position = 0.0; 
                js.velocity = 0.0; 
                js.torque = 0.0; 
                states_[i] = js; 
            }
        }

        bool connect(const string &endpoint) override 
        {
            lock_guard<mutex> lk(mu_); 
            endpoint_ = endpoint; 
            connected_ = true; 
            cout <<"Connected to " << endpoint << endl; 
            return true; 
        }

        void disconnect() override 
        {
            lock_guard<mutex> lk(mu_); 
            connected_ = false; 
            cout <<"Disconnected" << endl; 
        }

        bool set_positions(const vector<int> &joint_ids, const vector<double> &positions) override 
        {
            lock_guard<mutex> lk(mu_); 
            if (joint_ids.size() != positions.size()) return false; 
            for (size_t i = 0 ; i < joint_ids.size() ; ++i)
            {
                int id = joint_ids[i]; 
                double pos = positions[i]; 
                auto it = states_.find(id); 
                if (it == states_.end()) continue; 

                // clamp by spec.. 
                const MotorSpec &ms = specs_[id]; 
                if (pos < ms.position_lower)
                pos = ms.position_lower;
                if (pos > ms.position_upper)
                pos = ms.position_upper; 

                // set (stub : instant).. 
                it -> second.position = pos; 
                it -> second.velocity = 0.0; 
                it -> second.torque = 0.0; 
            }
            return true; 
        }

        bool set_torques(const vector<int> &joint_ads, const vector<double> &torques) override
        {
            lock_guard<mutex> lk(mu_); 
            if (joint_ads.size() != torques.size()) return false; 

            for (size_t i = 0 ; i < torques.size(); ++i)
            {
                int id = joint_ads[i]; 
                double tq = torques[i]; 
                auto it = states_.find(id); 
                if (it == states_.end()) continue; 

                const MotorSpec &ms = specs_[id]; 
                // clamp torque.. 

                if (tq > ms.max_torque) tq = ms.max_torque; 
                if (tq < -ms.max_torque) tq = -ms.max_torque; 
                it -> second.torque = tq; 

                // update positions / velocity in a naive way (not physical).. 
                it -> second.velocity += tq *0.01; 
                it -> second.position += it -> second.velocity *0.01; 

                // clamp positions limits.. 
                if (it->second.position < ms.position_lower) it->second.position = ms.position_lower;
                if (it->second.position > ms.position_upper) it->second.position = ms.position_upper;
            }
            return true; 
        }

        optional <JoinState> read_joint(int joint_id) override
        {
            lock_guard<mutex> lk(mu_); 
            auto it = states_.find(joint_id); 
            if (it == states_.end()) return nullopt; 
            return it -> second; 
        }

        vector<JoinState> read_all() override 
        {
            // lock_guard<mutex> lk(mu_);
            // vector<JointState> out;
            // out.reserve(states_.size());
            // for (auto &kv : states_) out.push_back(kv.second);
            // return out;
        }

        vector<MotorSpec> specs() const override
        {
            return specs_ ; 
        }

        private: 
        vector<MotorSpec> specs_; 
        string endpoint_; 
        mutable mutex mu_; 
        unordered_map<int, JoinState> states_; 
        bool connected_ = false; 
    }; 

    IMotorDriver *make_stub_mototr_driver(){
        return new StubMotorDriver(); 
    }
}