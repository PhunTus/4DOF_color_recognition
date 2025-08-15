#include <Servo.h>

// Servo mapping:
// Servo 1 - pin 8
// Servo 2 - pin 5 (Careful)
// Servo 3 - pin 10
// Servo 4 - pin 9
// Servo 5 (Gripper) - pin 4

Servo Servo1;
Servo Servo2;
Servo Servo3;
Servo Servo4;
Servo Servo5;
bool start, end;
void setup() {
  Servo1.attach(8);
  Servo2.attach(5);
  Servo3.attach(10);
  Servo4.attach(11);
  Servo5.attach(4);
  start = true;
  end = false;
  // Vị trí ban đầu
  Servo1.write(0);
  Servo2.write(60);
  Servo3.write(110);
  Servo4.write(140);
  Servo5.write(0);
}

// Hàm di chuyển mượt
void DelayServo(Servo &servo, int startAngle, int endAngle) {
  if (startAngle > endAngle) {
    for (int i = startAngle; i >= endAngle; i--) {
      servo.write(i);
      delay(14); // mỗi bước ~14ms
    }
  } else {
    for (int i = startAngle; i <= endAngle; i++) {
      servo.write(i);
      delay(14);
    }
  }
}

// Về vị trí "home" mượt
void home() {
  DelayServo(Servo1, Servo1.read(), 0);
  DelayServo(Servo2, Servo2.read(), 60);
  DelayServo(Servo3, Servo3.read(), 110);
  DelayServo(Servo4, Servo4.read(), 150);
  DelayServo(Servo5, Servo5.read(), 0);
}

// Động tác gắp
void grip() {
  if (start){
  Servo4.write(160);
  delay(500);
  start = false;
  end = true;
  }
  if (end){
  Servo1.write(20);  
  delay(1000);
  Servo2.write(35);
  delay(1000);
  Servo3.write(35);
  delay(1000);
  Servo4.write(70);
  delay(1000);
  Servo5.write(80);
  delay(1000);
  Servo4.write(120);
  delay(1000);
  }
}
void coordinate1() {          // red
  DelayServo( Servo2 , Servo2.read(),90);
  DelayServo( Servo1 , Servo1.read(),65);
  
  DelayServo( Servo4 , Servo4.read(),15);
  
  DelayServo( Servo5 , Servo5.read(),0); // tha vat
  
}

void coordinate2() {          //Blue
  DelayServo( Servo2 , Servo2.read(),90);
  DelayServo( Servo1 , Servo1.read(),90);
  
  DelayServo( Servo4 , Servo4.read(),15);
  
  DelayServo( Servo5 , Servo5.read(),0); // tha vat
  
}

void coordinate3() {          //Blue
  DelayServo( Servo2 , Servo2.read(),90);
  DelayServo( Servo1 , Servo1.read(),120);
  
  DelayServo( Servo4 , Servo4.read(),15);
  
  DelayServo( Servo5 , Servo5.read(),0); // tha vat
  
}

// void grips_coordinate (char coordinate) {
//   grips();
//   delay(500);

//   if ( coordinate == '1')
//     coordinate1();
//   else if (coordinate =='2')
//     coordinate2();
//   else if (coordinate =='3')
//     coordinate3();
//   delay(100);
//   home();
//   Serial.println("done");
// }

void loop() {
  grip();
  Servo4.write(160);
  delay(2000);
  coordinate1();
  delay(1000);
  Servo4.write(160);
  delay(2000);
  home();
}
