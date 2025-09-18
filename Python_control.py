import cv2
import serial
import time
from ultralytics import YOLO

arduino = serial.Serial(port='COM3', baudrate=9600, timeout=.1)
time.sleep(2)
model = YOLO('Prediction_code.pt')
processing_flag = False
mapping = {
    "0": "Blue Circle",
    "1": "Blue Cylinder",
    "2": "Blue Square",
    "3": "Red Circle",
    "4": "Red Cylinder",
    "5": "Red Square",
    "6": "Yellow Circle",
    "7": "Yellow Cylinder",
    "8": "Yellow Square"
}


def send_label_to_arduino(label):
    global processing_flag
    if label in mapping and not processing_flag:
        arduino.write(mapping[label].encode())
        print(f"Sent to Arduino: {mapping[label]}")
        processing_flag = True
        time.sleep(2)
        processing_flag = False

cap = cv2.VideoCapture(0)

while True:
    ret, frame = cap.read()
    if not ret:
        break

    if not processing_flag:
        results = model(frame)[0]
        for result in results.boxes.data.tolist():
            x1, y1, x2, y2, score, class_id = result
            if score > 0.5:
                label = str(int(class_id))
                send_label_to_arduino(label)  # Send only once
                cv2.rectangle(frame, (int(x1), int(y1)), (int(x2), int(y2)), (0, 255, 0), 2)
                cv2.putText(frame, f"{mapping[label]} {score:.2f}", (int(x1), int(y1)-10), 
                           cv2.FONT_HERSHEY_SIMPLEX, 0.9, (36,255,12), 2)
                break

    # Check Arduino response within the main loop
    if arduino.in_waiting > 0:
        arduino_data = arduino.readline().decode('utf-8').rstrip()
        if arduino_data == "done":
            processing_flag = False
            print("Arduino is ready for next input")

    cv2.imshow('YOLOv8 Detection', frame)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
arduino.close()#layer 
