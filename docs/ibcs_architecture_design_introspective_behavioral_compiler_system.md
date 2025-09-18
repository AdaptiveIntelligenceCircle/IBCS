# IBCS — Architecture Design

**Tóm tắt:**  
Tài liệu này mô tả kiến trúc của **IBCS (Introspective Behavioral Compiler System)** — một hệ thống kết hợp compiler, runtime, sandboxed execution, và cơ chế introspection. Mục tiêu: cho phép mô tả, biến đổi, kiểm soát và thử nghiệm hành vi phần mềm dưới dạng một ngôn ngữ/mô hình nội bộ, với tính an toàn và khả năng mở rộng.

---

## 1. Mục tiêu thiết kế
1. **Introspection-first:** Hỗ trợ introspection (tự phân tích) ở mọi tầng: ngôn ngữ, IR, runtime, hành vi.
2. **Behavior-as-code:** Hành vi được viết bằng ngôn ngữ riêng (IBCS-Lang), biên dịch thành IR (BIR), rồi chạy trong sandbox.
3. **Safety & Trust:** Sandbox + policy enforcement + capability model bảo vệ hệ thống.
4. **Extensibility:** Plugin architecture mở rộng parser, optimizer, runtime hooks, backend.
5. **Observability & Replay:** Log, snapshot, replay để debug và học meta-learning.
6. **Distributed & Federated:** Hỗ trợ nhiều node, đồng bộ và phối hợp triển khai hành vi.
7. **Human-in-the-loop:** Cho phép con người giám sát, phê duyệt hoặc rollback hành vi.

---

## 2. Các tầng chính
1. **Language Layer (IBCS-Lang):** Cú pháp, ngữ nghĩa, typing, macros introspection.
2. **Compiler Layer:** Lexer, parser, AST, IR generator, optimizer, introspection hooks.
3. **IR Layer (BIR):** Behavior Intermediate Representation với metadata.
4. **Runtime Layer:** VM/Executor, sandbox, behavior engine, introspection API.
5. **Management Layer:** Plugin manager, node manager, distributed sync.
6. **Security & Policy Layer:** Policy engine, attestation, capability.
7. **Telemetry Layer:** Logging, snapshot, replay.
8. **Human Interface Layer:** REPL, UI, approval workflow, simulation panel.

---

## 3. Thành phần cốt lõi
### 3.1 Ngôn ngữ (IBCS-Lang)
- Cú pháp ngắn gọn, hỗ trợ introspection.
- Có annotations như `@policy`, `@rollback`, `@observe`.

### 3.2 Compiler
- Frontend: lexer, parser, AST.
- Passes: typecheck, optimization, policy injection.
- Output: BIR + manifest.

### 3.3 Behavior IR (BIR)
- Control-flow, data-flow, effect annotations, rollback-points.
- Có metadata: provenance, confidence, policy tags.

### 3.4 Runtime / VM / Sandbox
- Executor: interpreter hoặc JIT.
- Sandbox: giới hạn CPU, memory, syscalls.
- Behavior Engine: scheduling, composition, persistence.
- Introspection API: snapshot, trace, invariant check.

### 3.5 Introspection Engine
- Thu thập runtime signals, state summaries.
- Gợi ý sửa lỗi và tối ưu dựa trên trace.

### 3.6 Plugin System
- Loại plugin: parser, optimizer, runtime hook, backend.
- ABI ổn định (C API), manifest, versioning.

### 3.7 Security & Policy
- Policy engine: enforce luật `no-network`, `rate-limit`, `require-approval`.
- Attestation: signed artifacts, provenance.
- Trust model: capability tokens, roles.

### 3.8 Telemetry & Replay
- Structured logs, compressed traces.
- Deterministic replay để debug và học.

---

## 4. Lifecycle điển hình
1. Viết behavior `.ibcsl`.
2. Compiler → AST → passes → BIR + manifest.
3. CI/CD test + simulation.
4. Artifact ký số, deploy sang node.
5. Runtime load artifact vào sandbox.
6. Hành vi chạy; introspection ghi trace.
7. Nếu có vi phạm, rollback và notify.
8. Trace feedback vào optimizer/meta-learning.

---

## 5. Cấu trúc thư mục
- `language/` — grammar, parser.
- `compiler/` — frontend & passes.
- `runtime/` — VM, sandbox, executor.
- `introspection/` — trace, analyzer, replay.
- `plugins/` — plugin manager & API.
- `security/` — policy engine, attestation.
- `tools/` — CLI (`ibcs-compile`, `ibcs-deploy`, `ibcs-replay`).

---

## 6. APIs & Interfaces
- Compiler CLI: `ibcs-compile src.ibcsl -o behavior.bir`
- Runtime API: deploy, start, snapshot, trace, rollback.
- Plugin ABI: `init(HostApi*)`, `shutdown()`, `hook_pass()`.
- Introspection SDK: client libs (C++, Rust, Lua).

---

## 7. Distributed Design
- Node roles: controller, executor, simulator, observer.
- Sync: CRDTs cho metadata; Raft cho control-plane.
- Federation: chia sẻ capability, policy, telemetry.

---

## 8. Roadmap (gợi ý)
- **M0 (0–2m):** Ngôn ngữ + compiler tối thiểu → BIR.
- **M1 (2–6m):** Introspection primitives, snapshot, replay.
- **M2 (6–12m):** Plugin system, policy engine, sandbox.
- **M3 (12–18m):** Distributed deployment, simulation backend.
- **M4 (18m+):** Formal verification, production tooling.

---

## 9. Rủi ro & Giảm thiểu
- **Complexity creep:** Giữ core nhỏ gọn.
- **Security bugs:** Default-deny sandbox, signed artifacts.
- **Spec drift:** Freeze core semantics, versioning.
- **Performance:** Hỗ trợ interpreter + JIT.

---

## 10. Tiêu chí thành công
- % behaviors qua test/simulation.
- Thời gian debug trung bình.
- Tỷ lệ vi phạm policy/runtime.
- Quy mô plugin ecosystem.

---

## 11. Ví dụ chuỗi thao tác
```bash
ibcs-compile drive.ibcsl -o drive.bir
ibcs-deploy --node node1 drive.bir
ibcs-run --behavior drive_assist --mode sandboxed
ibcs-trace --behavior drive_assist --from t0 --to t1 > trace.evt
ibcs-replay trace.evt --with-mutation mutated_input.json
```

---

## 12. Next Steps
- Xây dựng vertical slice nhỏ: IBCS-Lang subset → BIR → interpreter → introspection.
- Thiết kế plugin ABI dựa trên WASM.
- Viết test suite + simulation harness ngay từ đầu.

