#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;

char currentCommand   ='\0'; //Luu tru lenh hien tai
bool commandReceived  = false ; // kien soat khi nhan lenh moi ( dang ko co lenh nao)

void setup(){
  servo1.attach(2);
  servo2.attach(3);
  servo3.attach(4);
  servo4.attach(5);
  servo5.attach(6);

  // Khoi tao vi tri ban dau
  servo1.write(38);
  servo2.write(150);
  servo3.write(180);
  servo4.write(60);
  servo5.write(-90);  // Open grips

  Serial.begin(115200);
  
  
}
void DelayServo( Servo &servo , int startAngle, int endAngle){      // Chia nho cac goc de servo chay muot
  if(startAngle > endAngle){
    for(int i=startAngle ; i >= endAngle ; i--){
      servo.write(i);
      delay(14);    // Moi goc chay trong 0.03s
    }
  }else {
    for(int i=startAngle ; i <= endAngle ; i++){
      servo.write(i);
      delay(14);
    }
  }
 }

void grips () {     // Gap vat the
  
  
  DelayServo( servo2, 150, 70);
  DelayServo( servo4 , 60 , 90);
  delay(100);
  DelayServo( servo5 , -90 , 90);
  
}

void coordinate1() {          // red
  
  DelayServo( servo2 , 70 ,145);
  DelayServo( servo4 , 90,-10);
  DelayServo( servo1 , 38,88);
  
  DelayServo( servo5 , 90 , -90); // tha vat
  
}

void coordinate2() {          //green
  
  DelayServo( servo2 , 70 ,160);
  DelayServo( servo4 , 90,-10);
  DelayServo( servo1 ,38,115);
  
  DelayServo( servo5 , 90 , -90);
  
}

void coordinate3() {          //yellow
  
  DelayServo( servo2 , 70 ,150);
  DelayServo( servo4 , 90,-10);
  DelayServo( servo1 ,38,144);
  
  DelayServo( servo5 , 90 , -90);
 
} 

void home() {                   // ve vi tri ban dau muot hon
  
  DelayServo( servo1 , servo1.read(),38);
  DelayServo( servo2 , servo2.read(),150);
  DelayServo( servo3 , servo3.read(),180);
  DelayServo(servo4 , servo4.read(),60);

}


void grips_coordinate (char coordinate) {
  grips();
  delay(500);

  if ( coordinate == '1')
    coordinate1();
  else if (coordinate =='2')
    coordinate2();
  else if (coordinate =='3')
    coordinate3();
  delay(100);
  home();
  Serial.println("done");
}



void loop ()  {
  // Đọc dữ liệu khi chưa có lệnh nào đang xử lý
  if( Serial.available() > 0 && !commandReceived) {
    currentCommand = Serial.read();

    if(currentCommand >= '1' && currentCommand <='3')  {
      commandReceived = true ;                 // Đánh dấu là có lệnh hợp lệ mới             
    }
  }
  if(commandReceived) {
    grips_coordinate(currentCommand);   // Thực hiện di chuyển
    commandReceived = false ;           // Đặt lại để chờ lệnh tiếp theo
    currentCommand='\0';                // Đặt lại lệnh sau khi hoàn thành
     
     // Xóa Serial buffer
     while (Serial.available() > 0) {
      Serial.read();  // Đọc và bỏ qua mọi dữ liệu còn lại trong buffer
    }
  }
}
