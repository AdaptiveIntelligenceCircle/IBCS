# **IBCS — Compiled Behavior Documentation**

**Location:** `data/compile/compiled_behavior/`

**Purpose:** Chứa **sản phẩm cuối cùng** sau quá trình biên dịch (compile) hành vi trong IBCS. Đây là đầu ra được Runtime sử dụng để thực thi.

---

# **1. Compiled Behavior là gì?**

**Compiled Behavior** là **Intermediate Representation (IR)** đã qua:

1. Parsing
2. AST Construction
3. Rewrite (canonicalization & safety transformation)
4. Optimization (opt passes)
5. Safety validation
6. Finalization

Kết quả trong thư mục này là **IR dạng cuối**, sẵn sàng được thực thi bởi:

* **Behavior Engine**
* **Sandbox Executor**
* **Simulation/Introspection Engine**

> Đây là **mã hành vi đã ổn định**, deterministic, an toàn và không còn metadata nội bộ.

---

# **2. Cấu trúc thư mục compiled_behavior/**

```
compiled_behavior/
   ├── behavior.final.ir.json
   ├── behavior.optimized.ir.json
   ├── ruleset.active.ir.json
   ├── fallback_behavior.ir.json
   └── metadata.json
```

Có thể có thêm file `.bin`, `cpp` hoặc `.msgpack`.

---

# **3. Nội dung file IR final**

Tất cả file IR trong thư mục này phải tuân thủ 3 tiêu chí:

### **1) Deterministic**

Cùng input → compiler phải sinh ra cùng một IR.

### **2) Không chứa thông tin thừa**

Không còn:

* debug info
* log pointer
* AST metadata
* non-essential fields

### **3) An toàn đã được thẩm định**

IR final đã qua kiểm tra bởi:

* Safety Pass
* Constraint Checker
* Policy Guard (nếu được bật)

---

# **4. Các loại file IR thường gặp**

## **4.1. behavior.final.ir.json**

IR hoàn chỉnh được Behavior Engine sử dụng.
Chỉ bao gồm:

* biểu diễn hành vi
* cây điều kiện
* tham số
* giới hạn an toàn đã khóa
* danh sách lệnh/tác vụ

Ví dụ cấu trúc:

```json
{
  "version": "1.0",
  "entry": "root",
  "nodes": [
    {
      "id": "root",
      "type": "if",
      "condition": "risk_level > 0.4",
      "then": "slow_down",
      "else": "normal_drive"
    }
  ]
}
```

---

## **4.2. behavior.optimized.ir.json**

Phiên bản IR sau tối ưu hóa nhưng **trước finalization**.
Giữ lại một số thông tin phụ để:

* phân tích hiệu suất
* so sánh tối ưu hóa
* audit pipeline

---

## **4.3. ruleset.active.ir.json**

Nếu behavior dựa trên rule, file này ghi IR của rule set đã kích hoạt.

---

## **4.4. fallback_behavior.ir.json**

IR của chế độ dự phòng trong trường hợp behavior chính không hợp lệ.

---

## **4.5. metadata.json**

Mô tả:

```json
{
  "compile_timestamp": 1733680000,
  "compiler_version": "0.3.1",
  "source_hash": "ad91f0e2c94...",
  "final_ir_hash": "903acbfa2c2...",
  "safety_verified": true
}
```

Mục đích:

* audit
* xác minh tính nguyên vẹn
* tiêu chuẩn compliance

---

# **5. Vai trò của Compiled Behavior trong IBCS**

### **1. Runtime Execution**

IBCS Runtime **chỉ** chạy IR trong compiled_behavior/.

### **2. Safety Guarantee**

IR final là kết quả của các pass an toàn → tránh hành vi rủi ro.

### **3. Deterministic Behavior**

Giúp tái tạo và kiểm chứng hành vi theo thời gian.

### **4. Debug & Simulation**

Kết hợp với:

* `irlogs/` → hiểu cách IR được sinh ra
* `traces/` → hiểu IR chạy thế nào

---

# **6. Quy tắc quản lý Compiled Behavior**

* Mỗi lần compile thành công → lưu 1 bản final IR
* Không chỉnh sửa thủ công
* Mỗi file final phải có:

  * Hash
  * Timestamp
  * Version
* IR cũ phải được lưu vào `archive/` nếu thay thế

---

# **7. Ngắn gọn nhất**

> **compiled_behavior/** chứa IR cuối cùng mà IBCS sử dụng để chạy.
> Đây là phần quan trọng nhất của pipeline compile → execute, đại diện cho hành vi đã được tối ưu, rewrite và kiểm tra an toàn.

---
