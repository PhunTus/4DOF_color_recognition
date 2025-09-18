#include <Servo.h>

// Servo mapping
Servo Servo1;  // pin 7
Servo Servo2;  // pin 5
Servo Servo3;  // pin 4
Servo Servo4;  // pin 2 
Servo Servo5;  // pin 6 Gripper

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

// Di chuyển đến tọa độ 1
void coordinate1() {   // red
  DelayServo(Servo2, Servo2.read(), 90);
  DelayServo(Servo1, Servo1.read(), 65);
  DelayServo(Servo4, Servo4.read(), 15);
  DelayServo(Servo5, Servo5.read(), 0); // mở kẹp
}

void loop() {
  // Về home
  home();
  delay(500);

  // Thực hiện grip
  grip();
  delay(500);

  // Di chuyển đến coordinate1
  coordinate1();
  delay(1000); // giữ 1 giây trước khi quay về home lần nữa
}
