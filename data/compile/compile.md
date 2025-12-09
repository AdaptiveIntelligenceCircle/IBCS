# **IBCS — Compile Data Directory**

**Location:** `data/compile/`

**Purpose:** Lưu lại toàn bộ artifact được tạo ra trong quá trình *biên dịch hành vi* (behavior compilation) của IBCS.

Thư mục bao gồm 3 phần cốt lõi:

```
compiled_behavior/  → kết quả IR cuối cùng, dùng để chạy  
irlogs/             → nhật ký các bước compiler  
traces/             → lịch sử thực thi IR dùng cho mô phỏng & introspection
```

---

# **1. compiled_behavior/**

**Nơi chứa sản phẩm cuối cùng của quá trình biên dịch.**

Compiler của IBCS nhận behavior code (hoặc introspection rules) → tạo ra một **IR đã chuẩn hóa** và sẵn sàng chạy trong Runtime hoặc Behavior Sandbox.

### Các loại file thường có:

```
behavior.optimized.ir.json
behavior.sanitized.ir.json
behavior.final.ir.bin
ruleset_v3.ir.json
```

### Chức năng:

* Là output *duy nhất* mà runtime sử dụng
* Giúp behavior engine chạy ở chế độ deterministic
* Phục vụ versioning (mỗi IR final có thể được hash để kiểm định)

### Nguyên tắc:

* “Compiled Behavior” luôn **ổn định**, không chứa metadata thừa
* Chỉ chứa IR final sau khi: parsing → rewrite → optimize → safety pass

---

# **2. irlogs/**

**Nhật ký từng bước của compiler.**

Irlogs mô tả rõ **compiler đã làm gì**, bao gồm:

* Tokenization
* AST build
* Rewrite passes
* Optimization
* Safety decisions
* Error & warning details

Ví dụ:

```
2025-12-08_21-10-32_parse.log
2025-12-08_21-10-32_rewrite.log
2025-12-08_21-10-32_opt_pass.log
2025-12-08_21-10-32_safety.log
```

### Ý nghĩa:

* Tái hiện toàn bộ pipeline biên dịch
* Dễ kiểm tra vì sao compiler đưa ra quyết định A thay vì B
* Là nguồn dữ liệu quan trọng cho debugging và audit

> “IR logs giúp đọc được suy nghĩ của Compiler.”

---

# **3. traces/**

**Kết quả mô phỏng hoặc thực thi IR trong Evaluation Engine.**

Không giống IR logs (chỉ theo compiler), traces phản ánh:

* IR được thực thi *thế nào*
* Kết quả từng bước
* Các biến trung gian
* Các quyết định hành vi
* State changes

Ví dụ:

```
trace_2025-12-08_eval.json
trace_2025-12-08_simulation.json
trace_2025-12-08_risk_transition.json
```

### Ý nghĩa:

* Tái tạo pipeline chạy thực tế của hành vi
* Phục vụ demo, audit, replay, introspection
* Cho phép “step-by-step execution analysis”

**Traces là nền tảng để xác định behavior có an toàn hay không.**

---

# **Pipeline tổng quát**

```
source behavior
       ↓
   compiler
       ↓         → irlogs/
intermediate IR
       ↓
rewrite + optimize + safety
       ↓
 final IR → compiled_behavior/
       ↓
evaluation / simulation → traces/
```

---

# **Ngắn gọn nhất**

> `data/compile/` lưu trữ 3 lớp dữ liệu cốt lõi của IBCS:
>
> * **compiled_behavior**: IR cuối cùng để runtime dùng
> * **irlogs**: mọi bước trong quá trình compiler xử lý và tối ưu
> * **traces**: quá trình IR được thực thi trong mô phỏng và đánh giá

Ba phần này giúp IBCS đạt được **tính minh bạch, reproducibility và kiểm chứng an toàn** — đúng với triết lý ban đầu của Adaptive Intelligence Circle.

---


