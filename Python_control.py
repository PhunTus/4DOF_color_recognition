import cv2
import numpy as np
import serial
import time

# Kết nối với Arduino qua COM8
arduino = serial.Serial(port='COM8', baudrate=115200, timeout=1)
time.sleep(2)
# Định nghĩa các ngưỡng màu trong không gian HSV
color_ranges = {
    "red": ((0, 20, 233), (69, 255, 255)),
    "green": ((71, 66, 229), (86, 255, 255)),
    "blue": ((81, 69, 255), (179, 255, 255))

}

processing_flag = False # Biến cờ ngắt khi đang ko thực hiện j

def send_coordinate(coordinate):
    global processing_flag
    arduino.write(coordinate.encode())
    print(f"Gửi tọa độ: {coordinate}")
    processing_flag = True  # Bật cờ để ngừng nhận diện màu tiếp

def detect_color(frame):
    
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    # Tạo mặt nạ và làm sạch cho màu đỏ
    mask_red = cv2.inRange(hsv, *color_ranges["red"])
    mask_red = cv2.morphologyEx(mask_red, cv2.MORPH_OPEN, np.ones((5, 5), np.uint8))
    mask_red = cv2.morphologyEx(mask_red, cv2.MORPH_CLOSE, np.ones((5, 5), np.uint8))

    # Tạo mặt nạ và làm sạch cho màu xanh lá
    mask_green = cv2.inRange(hsv, *color_ranges["green"])
    mask_green = cv2.morphologyEx(mask_green, cv2.MORPH_OPEN, np.ones((5, 5), np.uint8))
    mask_green = cv2.morphologyEx(mask_green, cv2.MORPH_CLOSE, np.ones((5, 5), np.uint8))

    # Tạo mặt nạ và làm sạch cho màu xanh nước biển
    mask_blue = cv2.inRange(hsv, *color_ranges["blue"])
    mask_blue = cv2.morphologyEx(mask_blue, cv2.MORPH_OPEN, np.ones((5, 5), np.uint8))
mask_blue = cv2.morphologyEx(mask_blue, cv2.MORPH_CLOSE, np.ones((5, 5), np.uint8))

    # Tìm và xử lý contour cho từng màu
    for color, mask in [("red", mask_red), ("green", mask_green), ("blue", mask_blue)]:
        contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        for contour in contours:
            area = cv2.contourArea(contour)
            if area > 500:  # Kích thước tối thiểu của contour
                x, y, w, h = cv2.boundingRect(contour)
                cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)
                cv2.putText(frame, color, (x, y - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)

                # Gửi tọa độ nếu đang không xử lý
                if not processing_flag :
                    send_coordinate('1' if color == "red" else '2' if color == "green" else '3')
                    
                return frame

    return frame

# Bắt đầu xử lý ảnh từ camera
cap = cv2.VideoCapture(1)

while True:
    ret, frame = cap.read()
    if not ret:
        break

    # Nếu không đang xử lý tọa độ, tiếp tục nhận diện màu
    if not processing_flag:
        frame = detect_color(frame)

    # Hiển thị hình ảnh
    cv2.imshow("Frame", frame)
    # Kiểm tra phản hồi từ Arduino
    if arduino.in_waiting > 0:
        response = arduino.readline().decode().strip()
        if response == "done":
            processing_flag = False  # Cho phép nhận diện màu tiếp
            print("Arduino trả về: 'done' - Sẵn sàng nhận tọa độ mới.")  # Thông báo trạng thái
    # Nhấn 'q' để thoát
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Giải phóng tài nguyên
cap.release()
cv2.destroyAllWindows()
arduino.close()
