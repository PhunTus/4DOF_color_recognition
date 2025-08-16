#include <Servo.h>

// Servo mapping
Servo Servo1;  // pin 8
Servo Servo2;  // pin 5
Servo Servo3;  // pin 10
Servo Servo4;  // pin 11
Servo Servo5;  // pin 4

void setup() {
  Servo1.attach(8);
  Servo2.attach(5);
  Servo3.attach(10);
  Servo4.attach(11);
  Servo5.attach(4);

  // Vị trí ban đầu
  Servo1.write(0);
  Servo2.write(60);
  Servo3.write(110);
  Servo4.write(140);
  Servo5.write(0);

  Serial.begin(115200);
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

// Gắp vật (chính là code bạn viết trong grip cũ)
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
  Servo5.write(80);   // kẹp lại
  delay(1000);
  Servo4.write(120);
  delay(1000);
}

// Các tọa độ thả
void coordinate1() {   // red
  DelayServo(Servo2, Servo2.read(), 90);
  DelayServo(Servo1, Servo1.read(), 65);
  DelayServo(Servo4, Servo4.read(), 15);
  DelayServo(Servo5, Servo5.read(), 0); // mở kẹp
}

void coordinate2() {   // green
  DelayServo(Servo2, Servo2.read(), 90);
  DelayServo(Servo1, Servo1.read(), 90);
  DelayServo(Servo4, Servo4.read(), 15);
  DelayServo(Servo5, Servo5.read(), 0);
}

void coordinate3() {   // blue
  DelayServo(Servo2, Servo2.read(), 90);
  DelayServo(Servo1, Servo1.read(), 120);
  DelayServo(Servo4, Servo4.read(), 15);
  DelayServo(Servo5, Servo5.read(), 0);
}

// Thực hiện trọn chu trình gắp + đưa đến vị trí
void grips_coordinate(char command) {
  grip();
  delay(500);

  if (command == '1') coordinate1();
  else if (command == '2') coordinate2();
  else if (command == '3') coordinate3();

  delay(500);
  home();
  Serial.println("done");  // báo về Python
}

void loop() {
  if (Serial.available() > 0) {
    char cmd = Serial.read();
    if (cmd == '1' || cmd == '2' || cmd == '3') {
      grips_coordinate(cmd);
    }
  }
}
