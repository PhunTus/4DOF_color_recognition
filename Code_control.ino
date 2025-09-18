#include <Servo.h>

// Servo mapping
Servo Servo1;  // pin 7
Servo Servo2;  // pin 5
Servo Servo3;  // pin 4
Servo Servo4;  // pin 2 
Servo Servo5;  // pin 6 Gripper

String inputString = "";

void setup() {
  Servo1.attach(7);
  Servo2.attach(5);
  Servo3.attach(4);
  Servo4.attach(2);
  Servo5.attach(6);

  // Vị trí ban đầu
  Servo1.write(0);
  Servo2.write(60);
  Servo3.write(110);
  Servo4.write(140);
  Servo5.write(0);

  Serial.begin(9600);   // ⚠ khớp baudrate với Python (9600)
}

// Di chuyển mượt
void DelayServo(Servo &servo, int startAngle, int endAngle) {
  if (startAngle > endAngle) {
    for (int i = startAngle; i >= endAngle; i--) {
      servo.write(i);
      delay(14);
    }
  } else {
    for (int i = startAngle; i <= endAngle; i++) {
      servo.write(i);
      delay(14);
    }
  }
}

// Về home
void home() {
  DelayServo(Servo1, Servo1.read(), 0);
  DelayServo(Servo2, Servo2.read(), 60);
  DelayServo(Servo3, Servo3.read(), 110);
  DelayServo(Servo4, Servo4.read(), 150);
  DelayServo(Servo5, Servo5.read(), 0);
}

// Gắp vật
void grip() {
  Servo4.write(160);
  delay(500);
  Servo1.write(20);  
  delay(1000);
  Servo2.write(35);
  delay(1000);
  Servo3.write(35);
  delay(1000);
  Servo4.write(70);
  delay(1000);
  Servo5.write(80);   // kẹp
  delay(1000);
  Servo4.write(120);
  delay(1000);
}

// Vị trí thả theo màu/hình
void coordinateBlue() {
  DelayServo(Servo2, Servo2.read(), 90);
  DelayServo(Servo1, Servo1.read(), 65);
  DelayServo(Servo4, Servo4.read(), 15);
  DelayServo(Servo5, Servo5.read(), 0);
}

void coordinateRed() {
  DelayServo(Servo2, Servo2.read(), 100);
  DelayServo(Servo1, Servo1.read(), 80);
  DelayServo(Servo4, Servo4.read(), 20);
  DelayServo(Servo5, Servo5.read(), 0);
}

void coordinateYellow() {
  DelayServo(Servo2, Servo2.read(), 120);
  DelayServo(Servo1, Servo1.read(), 100);
  DelayServo(Servo4, Servo4.read(), 25);
  DelayServo(Servo5, Servo5.read(), 0);
}

void loop() {
  if (Serial.available()) {
    inputString = Serial.readStringUntil('\n'); // đọc chuỗi Python gửi
    inputString.trim(); // bỏ khoảng trắng

    if (inputString.length() > 0) {
      Serial.println("Đã nhận: " + inputString);

      home();
      delay(500);
      grip();
      delay(500);

      // Xử lý theo nhãn
      if (inputString.startsWith("Blue")) {
        coordinateBlue();
      } else if (inputString.startsWith("Red")) {
        coordinateRed();
      } else if (inputString.startsWith("Yellow")) {
        coordinateYellow();
      }

      delay(1000);
      home();

      Serial.println("done"); // báo cho Python biết đã xong
    }
  }
}
