#include <Arduino.h>
#include <Servo.h>

/*################################變數宣告區################################*/
// IR sansor A1~A5
int IR[5] = {A1, A2, A3, A4, A5};

// 速度控制:左邊馬達為PWM5，右邊馬達為PWM6
int mL = 5;
int mR = 6;
// 方向控制:左邊馬達為4，右邊馬達為7
int sL = 4;
int sR = 7;
Servo claw;
/*################################函數宣告區################################*/
void motor(int, int);
void trail();
void small_turn_left();
void small_turn_right();
void big_turn_left();
void big_turn_right();
void forward();
void stop();
void IR_test();
void motor_test();
bool cross();
/*################################程式初始化################################*/
void setup() // 程式初始化
{
    Serial.begin(9600);
    Serial.println("hello world");
    for (int i = 0; i < 5; i++)
    {
        pinMode(IR[i], INPUT);
    }

    pinMode(mL, OUTPUT);
    pinMode(mR, OUTPUT);
    pinMode(sL, OUTPUT);
    pinMode(sR, OUTPUT);

    claw.attach(9);

    /*################################程式循環################################*/

    while (!cross())
    {
        trail();
    }
    // 判斷IR1~IR3有沒有同時小於450，如果有的話就停止
    while (!(analogRead(IR[1]) < 450 && analogRead(IR[2]) < 450 && analogRead(IR[3]) < 450))
    {
        trail();
    }
    stop();

    // 旋轉(big_turn_left)直到IR2大於450，如果有的話就停止
    while (!(analogRead(IR[0]) > 450))
    {
        big_turn_left();
    }
    stop();
    while (!cross())
    {
        trail();
    }
    stop();
    // 直走直到IR1~IR3其中一個小於450，如果有的話就停止
    while (!(analogRead(IR[1]) < 450 or analogRead(IR[2]) < 450 or analogRead(IR[3]) < 450))
    {
        forward();
    }
    stop();
    while (!cross())
    {
        trail();
    }
    stop();
    while (!(analogRead(IR[1]) < 450 or analogRead(IR[2]) < 450 or analogRead(IR[3]) < 450))
    {
        forward();
    }
    stop();
    big_turn_left();
    delay(300);
    while (!(analogRead(IR[0]) > 450))
    {
        big_turn_left();
    }
    stop();
    while (!cross())
    {
        trail();
    }
    stop();

    // 判斷IR1~IR3有沒有同時大於450，如果有的話就停止

    // 判斷IR1~IR3有沒有同時大於450，如果有的話就停止
}

/*################################程式循環################################*/
void loop() // 程式循環
{

    // claw.write(0);
    // delay(250);
    // claw.write(90);
    // delay(250);
    // claw.write(180);
    // delay(250);
    // claw.write(90);
    // delay(250);
}

/*################################函數定義區################################*/
void trail()
{
    if (analogRead(IR[2]) > 450)
    {
        if (analogRead(IR[1]) > 450)
        {
            small_turn_left();
        }
        else if (analogRead(IR[3]) > 450)
        {
            small_turn_right();
        }
        else if (analogRead(IR[1]) < 450 && analogRead(IR[3]) < 450)

        {
            forward();
        }
    }
    else
    {
        if (analogRead(IR[1]) > 450)
        {
            // while (!(analogRead(IR[2]) > 450))
            {
                big_turn_left();
            }
        }
        else if (analogRead(IR[3]) > 450)
        {
            // while (!(analogRead(IR[2]) > 450))
            {
                big_turn_right();
            }
        }
    }
}

bool cross()
{
    // 判斷IR1~IR3有沒有同時大於450，如果有的話就停止

    if (analogRead(IR[1]) > 450 && analogRead(IR[2]) > 450 && analogRead(IR[3]) > 450)
    {
        stop();
        return true;
    }
    else
    {
        return false;
    }
}

// AND=&&
// OR=||
void forward()
{
    motor(100, 100);
}

void small_turn_left()
{
    motor(0, 120);
}

void small_turn_right()
{
    motor(120, 0);
}

void big_turn_left()
{
    motor(-120, 120);
}

void big_turn_right()
{
    motor(120, -120);
}

void stop()
{
    motor(0, 0);
}

void motor(int speedL, int speedR)
{

    // 判斷speedL跟speedR正負號決定方向
    digitalWrite(sL, speedL < 0 ? LOW : HIGH); // 左邊馬達方向設定
    digitalWrite(sR, speedR < 0 ? LOW : HIGH); // 右邊馬達方向設定

    analogWrite(mL, abs(speedL)); // 左邊馬達速度設定
    analogWrite(mR, abs(speedR)); // 右邊馬達速度設定
}

void IR_test()
{
    for (int i = 0; i < 5; i++)
    {
        Serial.print(analogRead(IR[i]));
        Serial.print(" ");
    }
    Serial.println("");
}
//
void motor_test()
{
    forward();
    delay(1000);
    stop();
    delay(1000);
    small_turn_left();
    delay(1000);
    small_turn_right();
    delay(1000);
    big_turn_left();
    delay(1000);
    big_turn_right();
    delay(1000);
}