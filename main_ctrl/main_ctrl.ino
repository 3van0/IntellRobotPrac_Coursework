#define STOP 0
#define FORWARD 1
#define BACKWARD 2
#define TURNLEFT 3
#define TURNRIGHT 4

#include <Servo.h>
Servo Servo1,Servo2;

int pin1 = 11; // 控制左右两个电机
int pin2 = 8;
int pin3 = 13;
int pin4 = 12;
int ENA = 3; //控制PWM
int ENB = 5;
int sensor1 = 14; //四路传感器
int sensor2 = 15;
int sensor3 = 16;
int sensor4 = 17;
int s = 0;

int stopcount=0;

void Write(int num,int s,int d)
{
  if (num == 1)
  {
    if (d >= s)
    {
      for(int i=s;i<=d;i++)
      {
        Servo1.write(i);
        delay(50); 
      }
      
    }
    else if(d<s)
    {
      for(int i=s;i>=d;i--)
      {
        Servo1.write(i);
        delay(50); 
      } 
    }
  }
  else if (num == 2)
  {
    if (d >= s)
    {
      for(int i=s;i<=d;i++)
      {
        Servo2.write(i);
        delay(50); 
      }
    }
    else if(d<s)
    {
      for(int i=s;i>=d;i--)
      {
        Servo2.write(i);
        delay(50); 
      } 
    }
  }
}



void setup()
{
  //Serial.begin(9600);
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  pinMode(pin3, OUTPUT);
  pinMode(pin4, OUTPUT);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
  pinMode(sensor3, INPUT);
  pinMode(sensor4, INPUT);

  Servo1.attach(10);
  Servo2.attach(9);

  Servo1.write(0);
  Servo2.write(8);
}

void loop()
{
  tracing();
}
void motorRun(int cmd, int value) //定义函数，两个形参分别：cmd控制方向，value是速度值
{

  switch (cmd)
  {
  case FORWARD:
    digitalWrite(pin2, HIGH);
    digitalWrite(pin1, LOW);
    digitalWrite(pin4, HIGH);
    digitalWrite(pin3, LOW);
    analogWrite(ENA, value); //设置PWM输出，value设置速度
    analogWrite(ENB, value);
    break;
  case BACKWARD:
    digitalWrite(pin1, HIGH);
    digitalWrite(pin2, LOW);
    digitalWrite(pin3, HIGH);
    digitalWrite(pin4, LOW);
    analogWrite(ENA, value); //设置PWM输出，value设置速度
    analogWrite(ENB, value);
    break;
  case TURNLEFT:
    digitalWrite(pin1, HIGH);
    digitalWrite(pin2, LOW);
    digitalWrite(pin3, LOW);
    digitalWrite(pin4, HIGH);
    analogWrite(ENA, value+25); //设置PWM输出，value设置速度
    analogWrite(ENB, value);
    break;
  case TURNRIGHT:
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, HIGH);
    digitalWrite(pin3, HIGH);
    digitalWrite(pin4, LOW);
    analogWrite(ENA, value); //设置PWM输出，value设置速度
    analogWrite(ENB, value+25);
    break;
  case STOP:
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, LOW);
    digitalWrite(pin3, LOW);
    digitalWrite(pin4, LOW);
    analogWrite(ENA, value); //设置PWM输出，value设置速度
    analogWrite(ENB, value);
  }
}

void tracing()
{
  int data[4];
  data[0] = digitalRead(sensor1); //读取四个传感器值
  data[1] = digitalRead(sensor2);
  data[2] = digitalRead(sensor3);
  data[3] = digitalRead(sensor4);

  if ((data[0] && !data[1]) || (data[3] && !data[2]) || (!data[0] && data[1] && !data[2] && !data[3])) //左右都没有检测到
  {
    delay(s);
    motorRun(FORWARD, 120);
  }

  if ((data[1] && data[0] && !data[3]) || (!data[0] && !data[1] && data[2] && !data[3])) //右边检测到黑线
  {
    delay(s);
    motorRun(TURNRIGHT, 130);
  }

  if (data[3] && data[2] && !data[0]) //左边检测到黑线
  {
    delay(s);
    motorRun(TURNLEFT, 135);
  }
  if ((!data[1] && !data[2] && !data[3] && !data[0]) || (!data[0] && data[1] && data[2] && !data[3])) //停止
  {

    motorRun(STOP, 0);
    delay(1000);
    stopcount += 1;
    if(stopcount%2 == 1)
    {
      fetchObj();
    }else if (stopcount%1 == 0)
    {
      dropObj();
    } 
    motorRun(FORWARD, 130);
    delay(1000);
    motorRun(STOP, 0);
  }
  if (data[1] && data[2] && data[3] && data[0]) //后退
  {
    delay(s);
    motorRun(BACKWARD, 130);
  }
  /*
  Serial.print(data[0]);
  Serial.print("---");
  Serial.print(data[1]);
  Serial.print("---");
  Serial.print(data[2]);
  Serial.print("---");
  Serial.print(data[3]);
  Serial.print("---");
*/
}

void fetchObj()
{
  Write(1, 0, 180);
  Write(2, 8, 55);
}

void dropObj()
{
  Write(2, 55, 8);
  Write(1, 180, 0);
}
