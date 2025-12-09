# **IBCS — Traces Documentation**

**Location:** `data/compile/traces/`
**Purpose:** Ghi lại *dòng suy luận và dòng hành vi* (behavior flow & introspective flow) trong quá trình thực thi IBCS Runtime.

---

# **1. Traces là gì?**

Traces là **bản ghi chi tiết từng bước** của:

* quá trình phân tích (introspection)
* quá trình biến đổi (transform)
* quá trình thực thi (runtime evaluation)
* quá trình phản ứng (reaction loop)

Traces được sinh ra nhằm:

* Kiểm tra hoạt động của Runtime
* Chẩn đoán lỗi (debugging / regression checks)
* Phân tích quá trình lý luận của hệ thống
* Đảm bảo tính minh bạch trong hành vi (behavior transparency)
* Theo dõi sai lệch hoặc các trường hợp bất thường trong logic

> **Traces không phải log thông thường** → chúng là *timeline có cấu trúc*, ghi lại toàn bộ pipeline xử lý của IBCS.

---

# **2. Traces sinh ra ở đâu trong hệ thống?**

Traces được tạo bởi ba thành phần chính:

### **a. Introspection Engine**

* ghi chuỗi các bước phân tích nội tại
* lưu reasoning, inference, metadata
* ghi cả đầu vào/đầu ra sau mỗi transformation

### **b. Transformer Layer**

* lưu lại từng AST transform
* ghi mapping: input-node → output-node
* ghi các rule được kích hoạt

### **c. Runtime Executor**

* ghi quá trình chạy behavior
* ghi phản ứng, sự kiện, giá trị intermediate
* ghi thời gian và thứ tự thực thi (timeline)

Tất cả được gom vào `data/compile/traces/`.

---

# **3. Định dạng trace file**

Một trace luôn theo cấu trúc JSON:

```
<timestamp>_<random-id>.trace.json
```

Ví dụ:

```
2025-12-08_18-22-41_c8f2b.trace.json
```

---

# **4. Cấu trúc một trace file**

Dưới đây là **schema chính xác** của trace:

```json
{
  "version": "1.0.0",
  "description": "Specification for IBCS compiler trace events",
  "fields": {
    "timestamp": "uint64",
    "category": "parse | transform | optimize | codegen | error | introspection",
    "message": "string",
    "meta": {
      "node": "string (optional)",
      "rule": "string (optional)",
      "extra": "object"
    }
  }
}
```

Giải thích từng trường:

* **trace_id**: định danh duy nhất của trace
* **timestamp**: thời gian tạo
* **source**: nguồn tạo trace
* **context**: mô tả file, component hoặc runtime session
* **steps**: từng bước nội bộ
* **summary**: tổng hợp sau khi pipeline kết thúc

---

# **5. Các loại trace**

Có 3 nhóm lớn:

---

## **1) Introspection Traces**

Ghi lại:

* phân tích inner-thought
* rút trích cấu trúc reasoning
* các bước inference
* summarization nội bộ
* điểm mismatch giữa insights và behavior

Sử dụng cho:

* kiểm chứng chain-of-thought theo logic
* phân tích sai khác reasoning
* xây dựng behavioral consistency

---

## **2) Transform Traces**

Sinh ra bởi `transformer.cpp` trong `language/`.

Ghi lại:

* mỗi lần biến đổi AST
* rule nào được kích hoạt
* node input → node output
* các tối ưu hóa hoặc cảnh báo
* thời gian transform từng bước

Dùng để:

* debug transformer
* xác định transform redundancy
* đảm bảo ngôn ngữ của IBCS hoạt động đúng đặc tả

---

## **3) Runtime Execution Traces**

Ghi lại pipeline:

```
parse → analyze → transform → compile → evaluate → react
```

Thông tin được ghi:

* input data của mỗi vòng lặp
* intermediate values
* events được trigger
* phản ứng của behavior engine
* thời gian thực thi từng đoạn
* dấu hiệu bất thường (latency tăng, loop drift)

Dùng cho:

* đo hiệu suất
* phát hiện bottleneck
* kiểm tra tính xác định (determinism)

---

# **6. Khi nào hệ thống tạo trace?**

Hệ thống tạo trace trong các trường hợp sau:

* mỗi lần chạy transformation phức tạp
* mỗi lần introspection engine hoạt động
* khi runtime gặp lỗi
* khi behavior có phản ứng bất thường
* khi được bật `diagnostic_mode`
* theo chu kỳ nếu bật `trace_interval` trong config

Traces không được ghi liên tục như log — chỉ khi cần để tránh lãng phí.

---

# **7. Sự khác nhau giữa trace và log**

| Đặc tính        | Trace                | Log                |
| --------------- | -------------------- | ------------------ |
| Mức độ chi tiết | Rất cao              | Trung bình         |
| Cấu trúc        | JSON có schema       | Text hoặc JSON đơn |
| Mục tiêu        | Phân tích pipeline   | Theo dõi hoạt động |
| Độ chính xác    | Định danh từng bước  | Ghi lại sự kiện    |
| Tính truy xuất  | Có timeline rõ ràng  | Thời gian-based    |
| Kích thước      | Lớn                  | Nhỏ–vừa            |
| Dùng cho        | Debug, introspection | Quan sát, audit    |

**Trace = Chụp MRI toàn pipeline**
**Log = Chụp X-ray 1 lớp**

---

# **8. Công cụ đọc trace**

IBCS hỗ trợ:

* `ibcs trace view <filename>`
* xuất PDF/HTML timeline
* diff giữa hai traces
* highlight anomaly hoặc bước bất thường

(Đây là tool bạn có thể viết sau, không bắt buộc trong repo hiện tại.)

---

# **9. Nguyên tắc an toàn (Hard Constraints)**

* Traces **không được chứa key bí mật**
* Không ghi raw user data nếu không cần thiết
* Tất cả trace phải có **checksum** để tránh sửa đổi
* Trace nghi ngờ anomalous phải được lưu riêng ở `traces/suspicious/`

---

# **10. Ý nghĩa của traces đối với dự án IBCS**

Traces là thành phần quan trọng nhất để:

1. **Kiểm chứng IBCS hoạt động đúng đặc tả**
2. **Theo dõi introspection và lý luận nội bộ**
3. **Tối ưu transformer và runtime**
4. **Kiểm soát sai lệch hành vi (behavior drift)**
5. **Xây dựng tính tin cậy và minh bạch**
6. **Phục vụ cho việc làm safety chứng nhận**

Traces là dữ liệu có tính kỹ thuật, khách quan, và có thể tái tạo (reproducible).

---


