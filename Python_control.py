import cv2
import serial
import time
from ultralytics import YOLO

# ===== Kết nối với Arduino =====
arduino = serial.Serial(port='COM7', baudrate=115200, timeout=1)
time.sleep(2)

# ===== Load model YOLO (thay 'best.pt' bằng model bạn train) =====
model = YOLO("color.pt")

# ===== Biến cờ để ngắt khi Arduino đang xử lý =====
processing_flag = False

# ===== Mapping tên nhãn -> số để gửi cho Arduino =====
mapping = {
    "red": "1",
    "green": "2",
    "blue": "3"
}

def send_label(label):
    """Gửi số (theo mapping) qua Serial cho Arduino"""
    global processing_flag
    if label in mapping:
        data = mapping[label]
        arduino.write(data.encode())
        print(f"Gửi nhãn: {label} -> {data}")
        processing_flag = True

# ===== Camera =====
cap = cv2.VideoCapture(1)

while True:
    ret, frame = cap.read()
    if not ret:
        break

    if not processing_flag:
        # Nhận diện bằng YOLO
        results = model(frame, stream=True)

        for r in results:
            for box in r.boxes:
                x1, y1, x2, y2 = map(int, box.xyxy[0])
                conf = float(box.conf[0])
                cls = int(box.cls[0])
                label = model.names[cls]

                if conf > 0.2:  # ngưỡng tin cậy
                    # Vẽ bounding box
                    cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 2)
                    cv2.putText(frame, f"{label} {conf:.2f}", (x1, y1 - 10),
                                cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 255, 0), 2)

                    # Gửi số thay vì nhãn chữ
                    send_label(label)
                    break   # chỉ gửi 1 đối tượng tại 1 thời điểm

    # ===== Kiểm tra phản hồi từ Arduino =====
    if arduino.in_waiting > 0:
        response = arduino.readline().decode().strip()
        if response == "done":
            processing_flag = False
            print("Arduino trả về: 'done' - Sẵn sàng nhận nhãn mới.")

    # ===== Hiển thị video =====
    cv2.imshow("YOLO Detection", frame)

    # Thoát bằng phím 'q'
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# ===== Giải phóng tài nguyên =====
cap.release()
cv2.destroyAllWindows()
arduino.close()
