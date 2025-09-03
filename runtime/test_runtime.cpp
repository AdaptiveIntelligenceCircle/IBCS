#include <iostream>
#include <memory>

#include "memory.cpp"
#include "executor.cpp"

#include "behavior_sandbox.cpp"
#include "reaction_loop.cpp"

#include "memory.h"
#include "executor.h"

#include "behavior_sandbox.h"
#include "reaction_loop.h"

#include "../core/introspection_engine.h"
#include "../core/intropection_parser.cpp"

using namespace std;
using namespace ibcs::runtime;
// using namespace runtime;// Executor, ReactionLoop nằm ở namespace runtime

int main() 
{

    cout << "[TEST] Runtime module integration\n";

    // ===== 1. Test Memory =====
    {
        cout << "\n[Case1] Memory basic operations\n";
        auto mem = make_shared<Memory>();
        mem->define("x", 10, false, false);
        mem->define("y", string("hello"), false, true);

        cout << "Dump state:\n" << mem->dumpState() << endl;

        mem->set("x", 20);
        cout << "x updated to " << get<int>(mem->get("x")) << endl;
    }

    // ===== 2. Test Executor =====
    {
        cout << "\n[Case2] Executor register + execute\n";
        auto mem = make_shared<Memory>();
        auto exec = make_shared<Executor>(mem);
        exec->enable_debug(true);

        exec->register_behavior("greet", [](const ExecutionContext &ctx) {
            cout << "[Behavior:greet] Hello, "
                 << (ctx.parameters.count("name") ? ctx.parameters.at("name") : "world")
                 << "!\n";
        });

        ExecutionContext ctx;
        ctx.behavior_id = "greet";
        ctx.parameters["name"] = "Tri";
        exec->execute(ctx);
    }

    // ===== 3. Test BehaviorSandBox =====
    {
        cout << "\n[Case3] BehaviorSandBox executeBehavior\n";
        auto mem = make_shared<Memory>();
        auto exec = make_shared<Executor>(mem);
        shared_ptr<ibcs::IntrospectionEngine> introspection = nullptr; // giả lập

        exec->register_behavior("echo", [](const ExecutionContext &ctx) {
            cout << "[Behavior:echo] params=" << ctx.parameters.size() << endl;
        });

        BehaviorSandBox sandbox(exec, mem, introspection);

        BehaviorContext bctx;
        bctx.behavior_id = "echo";
        bctx.parameters["msg"] = "test message";
        bctx.allow_side_effect = false;

        string result = sandbox.executeBehavior(bctx);
        cout << "Sandbox result: " << result << endl;

        cout << "Logs collected:\n";
        for (auto &log : sandbox.getLogs()) {
            cout << " - " << log << endl;
        }
    }

    // ===== 4. ReactionLoop basic =====
    {
        cout << "\n[Case4] ReactionLoop start/stop demo\n";
        auto mem = make_shared<Memory>();
        auto exec = make_shared<Executor>(mem);
        auto sandbox = make_shared<BehaviorSandBox>(exec, mem, nullptr);

        ReactionLoop loop(sandbox, exec, mem, 200); // interval 200ms
        loop.start();
        this_thread::sleep_for(chrono::milliseconds(600)); // chạy ~3 ticks
        loop.stop();
    }

    cout << "\n[TEST] Finished all runtime cases.\n";
    return 0;
}
