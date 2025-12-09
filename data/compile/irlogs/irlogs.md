# **IBCS — IR Logs Documentation**

**Location:** `data/compile/irlogs/`

**Purpose:** Lưu toàn bộ *nhật ký biên dịch* (Intermediate Representation Logs) tạo ra trong quá trình compiler phân tích và tối ưu hành vi.

---

# **1. IR Logs là gì?**

**IR Logs (Intermediate Representation Logs)** là tập hợp các file ghi lại:

* Mỗi bước compiler đã thực hiện
* Quyết định rewrite
* Các cảnh báo và lỗi semantic
* Mọi tối ưu hóa được thực thi
* Sự thay đổi của IR theo từng giai đoạn

Khác với logs runtime hay traces:

| Loại                  | Nguồn    | Mục đích                  | Thời điểm        |
| --------------------- | -------- | ------------------------- | ---------------- |
| **irlogs**            | Compiler | Hiểu pipeline biên dịch   | *Trước khi chạy* |
| **compiled_behavior** | Compiler | Output cuối, dùng để chạy | *Sau biên dịch*  |
| **traces**            | Runtime  | Ghi lại thực thi IR       | *Trong khi chạy* |

**IR Logs chỉ nói về suy nghĩ của compiler**, không phản ánh execution.

---

# **2. Cấu trúc thư mục irlogs/**

```
data/compile/irlogs/
   ├── 2025-12-08_21-10-32_parse.log
   ├── 2025-12-08_21-10-32_ast.log
   ├── 2025-12-08_21-10-32_rewrite.log
   ├── 2025-12-08_21-10-32_opt_pass.log
   └── 2025-12-08_21-10-32_safety.log
```

Mỗi lần compiler chạy → tạo một nhóm file cùng timestamp.

---

# **3. Các loại IR Log chính**

## **3.1. parse.log**

Ghi lại:

* tokenization
* quá trình đọc input
* lỗi cú pháp
* vị trí dòng/chỉ mục

Ví dụ:

```
[INFO] Tokenizing ✓
[WARN] Unknown symbol '@' at col 14
[ERROR] Unclosed block at line 22
```

---

## **3.2. ast.log**

Ghi lại quá trình xây dựng **AST (Abstract Syntax Tree)**:

* các node được tạo
* cấu trúc semantic ban đầu
* điểm bất thường trong cây

Ví dụ:

```
ASTNode(FunctionCall, name="adjust_torque")
ASTNode(If, condition="wheel_slip > 0.3")
```

---

## **3.3. rewrite.log**

Ghi quyết định của **rewrite engine**:

* tối ưu rule
* loại bỏ nodes thừa
* thay đổi logic để đảm bảo ổn định
* canonicalization
* transformation passes

Ví dụ:

```
Rewrite: Flatten nested-if → merge branch (complexity reduction)
Rewrite: Replace unsafe_constraint with safe variant
```

---

## **3.4. opt_pass.log**

Ghi lại các pass tối ưu:

* dead-code elimination
* constant folding
* branch prediction hints
* micro-optimizations
* inline hóa hành vi

Ví dụ:

```
ConstantFold: speed * 0 became 0
DeadCode: removing unreachable path (branch=3)
```

---

## **3.5. safety.log**

Ghi các bước kiểm tra an toàn:

* safety contract validation
* kiểm tra hành vi nguy hiểm
* đảm bảo compliance với DIP / IBCS Safety Model

Ví dụ:

```
[SAFETY] Risk: torque adjust allowed max range = 0.4
[SAFETY] Blocked unsafe expression: raw_motor_access
```

---

# **4. Vai trò của IR Logs**

### **1) Là cơ sở để audit và chứng minh tính an toàn**

Một behavior được chứng minh là an toàn khi toàn bộ pipeline của nó minh bạch.

### **2) Hỗ trợ debugging sâu**

Lỗi behavior thường nằm ở rewrite logic hoặc safety passes — IR logs cho thấy rõ nguyên nhân.

### **3) Xác nhận tính determinism**

IR logs giúp đảm bảo rằng cùng một input → compiler sẽ sinh ra cùng một IR final.

### **4) Tái hiện quá trình biên dịch**

Kết hợp IR + IR logs cho phép tái dựng lại y hệt pipeline phiên bản trước.

---

# **5. Vòng đời IR Logs**

```
source input
    ↓
tokenize → parse.log
    ↓
build AST → ast.log
    ↓
rewrite → rewrite.log
    ↓
optimize → opt_pass.log
    ↓
safety pass → safety.log
    ↓
final IR → compiled_behavior/
```

---

# **6. Quy tắc lưu trữ IR Logs**

* Không xoá tự động (mang tính audit)
* Mỗi lần compile → tạo một nhóm file theo timestamp
* Các logs phải **bất biến**, không được chỉnh tay
* Mỗi nhóm log gắn với 1 IR final

---

# **7. Ngắn gọn nhất**

> **IR Logs** phản ánh từng suy nghĩ của compiler khi biến đổi, tối ưu và đảm bảo an toàn cho behavior.
> Đây là công cụ quan trọng nhất để audit và tái tạo quá trình compile trong IBCS.

---


