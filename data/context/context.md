

# **IBCS — Context Layer Documentation**

**Location:** `data/context/`

**Purpose:** Lưu giữ *ngữ cảnh thực thi* (execution context) của hệ thống IBCS tại từng thời điểm.

```
data/context/
   ├── environment_context.json
   ├── history_context.txt
   └── state_context.json
```

---

# **1. Context là gì?**

Trong IBCS, *context* là tập hợp thông tin nền tảng mô tả:

* Hệ môi trường mà runtime đang hoạt động
* Lịch sử tương tác, sự kiện và phiên làm việc
* Trạng thái nội bộ hiện tại của IBCS

> **Context không phải log**.
> Context là một *bức ảnh chụp hiện trạng hệ thống* — đầy đủ, rõ ràng, phục vụ cho introspection, deterministic behavior và phục hồi trạng thái.

3 file trong folder này tương ứng với 3 loại ngữ cảnh:

1. **environment_context.json** — mô tả môi trường
2. **history_context.txt** — lịch sử sự kiện
3. **state_context.json** — trạng thái nội bộ hiện tại

---

# **2. environment_context.json**

Mô tả **môi trường thực thi**, bao gồm cấu hình, phiên bản, nền tảng, và các ràng buộc an toàn.

Ví dụ cấu trúc:

```json
{
  "environment_id": "local_dev",
  "timestamp": "2025-08-29T14:05:00Z",
  "system": 
  {
    "os": "Windows 11",
    "cpu": "Intel core i7",
    "memory_gb": 32,
    "network": "offline"
  },
  "signals": [
    {
      "source": "user_input",
      "type": "command",
      "content": "compile idea seed_001",
      "confidence": 0.95
    },
    {
      "source": "sensor_mock",
      "type": "anomaly",
      "content": "low disk space warning",
      "confidence": 0.65
    }
  ],
  "constraints": {
    "timeout_sec": 10,
    "max_memory_mb": 2048
  }
}

```

**Ý nghĩa:**

* Cho phép Runtime quyết định cách phân bổ tài nguyên
* Hỗ trợ introspection xác định "vì sao hệ thống chọn hành vi A, không phải B"
* Bắt buộc trong mọi pipeline transform → evaluate

> Đây là gốc của **deterministic execution** trong IBCS.

---

# **3. history_context.txt**

Ghi **lịch sử tuyến tính** của các thao tác, sự kiện và thay đổi trạng thái.

Khác log ở chỗ:

* *History là dòng thời gian ổn định, không nhiễu*
* *History được sử dụng để tái tạo lại session giám định*

Ví dụ nội dung:

```
[2025-12-08 19:20:12] IBCS bootstrap complete
[2025-12-08 19:20:15] Loaded transformer: default_ruleset_v2
[2025-12-08 19:20:15] Evaluated input: driver_event(speed=32)
[2025-12-08 19:20:15] Behavior: switching to MODE_CAUTION
[2025-12-08 19:20:16] Introspection request captured
[2025-12-08 19:20:17] State updated: risk_level=0.32
[2025-12-08 19:20:19] Runtime warning: torque drift detected
[2025-12-08 19:20:20] Behavior: fallback engaged (motor_left)
```

**Ý nghĩa:**

* Cung cấp cho auditor một timeline để phân tích
* Cho phép IBCS xác định nguyên nhân gốc (root cause)
* Giúp tái tạo lại session trong simulation mode

---

# **4. state_context.json**

Lưu **trạng thái nội bộ hiện tại** của IBCS Runtime — một bản snapshot.

Ví dụ schema:

```json
{
    "agent_id": "ibcs_core_01" , 
    "timestamp": "2025-08-20T14 : 00:002", 
    "mode": "introspection", 
    "goals": [
        {
            "id": "g001", 
            "description" : "Compile incoming idea into executable behavior", 
            "priority": 0.9 , 
            "status" : "active"
        }, 
        {
            "id": "g002",
            "description": "Maintain logical consistency of context",
            "priority": 0.8,
            "status": "pending"
        }
    ], 
    "current_state": {
    "focus": "idea_translation",
    "confidence": 0.76,
    "energy_level": 0.64
  }
}
```

**Ý nghĩa:**

* Là trạng thái "tức thời" để Behavior Engine hoặc Debugger hiểu hệ thống đang ở đâu
* Dùng để:

  * reproducible introspection
  * state-handoff giữa các module
  * kiểm tra consistency

---

# **5. Vì sao Context Layer quan trọng trong IBCS?**

Context được sử dụng để:

### **1) Giữ tính nhất quán (consistency)**

Các module (runtime, introspection, transform) luôn nhìn cùng một bối cảnh.

### **2) Đảm bảo reproducibility**

Traces + context = tái tạo lại được pipeline y hệt.

### **3) Debug hiệu quả**

Lỗi IBCS đa số nằm ở sự khác biệt bối cảnh, không phải logic.

### **4) Tối ưu hoá Adaptive Behavior**

Behavior engine có thể đọc context để điều chỉnh chế độ.

### **5) An toàn (safety)**

Giảm rủi ro state drift ở vòng lặp dài.

---

# **6. Quy tắc quản lý context**

* Không lưu **dữ liệu nhạy cảm hoặc bí mật**
* Mỗi session phải có một version của 3 file
* Khi session kết thúc, context được lưu vào `archive/`
* Mọi context phải có:

  * checksum
  * timestamp
  * session_id

---

# **7. Vòng đời context**

```
session_start  
 → ghi environment_context  
 → runtime cập nhật state_context theo từng biến động  
 → history_context thêm theo sự kiện  
 → khi terminate: context được freeze  
```

---

# **8. Mối quan hệ giữa Context – Traces – Runtime**

* **Context**: trạng thái & môi trường
* **Trace**: từng bước chi tiết của xử lý
* **Runtime**: logic thực thi hành vi / biến đổi

Ba phần hỗ trợ nhau:

```
Context giải thích vì sao Runtime chọn hành vi X  
Trace cho biết Runtime thực hiện X thế nào  
```

---

# **9. Ngắn gọn nhất**

> `data/context/` cung cấp **bối cảnh đầy đủ** để hiểu, tái tạo và đánh giá chính xác mọi quyết định nội bộ của IBCS.

---

