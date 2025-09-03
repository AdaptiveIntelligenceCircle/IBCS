# Runtime Engine

Bộ xử lý thời gian thực thực thi các hành vi đã biên dịch.

### Modules:
- `executor`: Chạy hành vi đã được compile.
- `reaction_loop`: Vòng phản xạ liên tục xử lý sự kiện (async support).
- `memory`: Hệ thống bộ nhớ ngắn và dài hạn.
- `behavior_sandbox`: Môi trường thử nghiệm hành vi trước khi triển khai.

``` plaintext

auto mem = make_shared<Memory>();
mem->define("x", 42, false, false);

auto exec = make_shared<Executor>(mem);
exec->register_behavior("inspect", [](const ExecutionContext& ctx) {
    cout << "[Behavior:inspect] Executed.\n";
});

string log = exec->run("inspect");
cout << "Run result:\n" << log << endl;




```
