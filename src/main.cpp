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
Servo claw; // 爪子伺服馬達
Servo arm;  // 手臂伺服馬達
unsigned long duration_time_diff;
/*################################函數宣告區################################*/
void motor(int, int);
void trail();
void small_turn_left();
void small_turn_right();
void mid_turn_left();
void mid_turn_right();
void big_turn_left();
void big_turn_right();
void forward();
void stop();
void back();
void re_turn_left();
void return_to_line_left();
void re_turn_right();
void return_to_line_right();
void return_to_line_2();
void trail_cross();
bool cross();
void my_init();
void pickup_middle();
void pickup_left();
void pickup_right();
void pick_up();
void pick_down();
void pick_down_2();
void arm_up();
void arm_down();
void back_to_center();
/*################################程式初始化################################*/
void setup() // 程式初始化
{
    my_init();
    trail_cross();
    arm_down();

    // pickup_middle(); // 取貨點(中)

    // back_to_center(); // 回到中心點

    pickup_left(); // 取貨點(左)

    back_to_center(); // 回到中心點

    pickup_right(); // 取貨點(右)

    back_to_center(); // 回到中心點
}

/*################################程式循環################################*/
void loop() // 程式循環
{
    // pickup_middle(); // 取貨點(中)

    // back_to_center(); // 回到中心點

    pickup_left(); // 取貨點(左)

    back_to_center(); // 回到中心點

    pickup_right(); // 取貨點(右)

    back_to_center(); // 回到中心點
}

/*################################函數定義區################################*/

void back_to_center()
{
    back();
    delay(425);
    stop();
    delay(100);
    return_to_line_left();
    delay(100);
    return_to_line_left();
    delay(100);
    pick_down_2();
    delay(100);
}

void arm_up()
{
    arm.write(80); // 調整手臂角度範例
}

void arm_down()
{
    // arm.write(120); // 調整手臂角度範例
    for (int i = 0; i < 3; i++)
    {
        arm.write(100 + 10 * i);
        delay(100);
    }
}

void pick_up()
{
    claw.write(100); // 調整爪子角度
    delay(100);
    arm_up();
}

void pick_down()
{
    arm_down();
    delay(100);
    claw.write(5); // 調整爪子角度
}

void pick_down_2()
{
    arm.write(120);
    delay(100);
    claw.write(20); // 調整爪子角度
}
void pickup_middle()
{
    /*################前往第一個取貨點(中)################*/
    pick_down_2();
    delay(100);

    // 繼續前進直到IR1~IR3全部小於450，到達取貨點
    while (!(analogRead(IR[1]) < 450 and analogRead(IR[2]) < 450 and analogRead(IR[3]) < 450))
    {
        trail();
        if (analogRead(IR[1]) < 450 and analogRead(IR[2]) < 450 and analogRead(IR[3]) < 450)
        {
            stop();
            delay(100);
        }
    }
    stop();

    /*################到達第一個取貨點(中)################*/

    pick_up();
    arm_up();
    delay(500);

    /*################前往卸貨點################*/
    return_to_line_left(); // 迴轉回到黑線上
    trail_cross();         // 循跡到中間十字路口
    trail_cross();         // 循跡到卸貨T字路口
    /*################到達卸貨點################*/
    pick_down();
    arm_up();
}

void pickup_left()
{
    /*################前往第二個取貨點(左)################*/
    pick_down_2();
    delay(100);

    return_to_line_left();
    delay(100);

    duration_time_diff = millis();
    while (!(millis() - duration_time_diff >= 500))
    {
        trail();
    }
    stop();
    while (!((analogRead(IR[1]) < 450) and (analogRead(IR[2]) < 450) and (analogRead(IR[3]) < 450)))
    {
        trail();
        if (analogRead(IR[1]) < 450 and analogRead(IR[2]) < 450 and analogRead(IR[3]) < 450)
        {
            stop();
            delay(100);
        }
    }
    stop();
    /*################到達第二個取貨點(左)################*/
    pick_up();
    delay(500);

    /*################前往卸貨點################*/
    return_to_line_left();
    trail_cross();

    return_to_line_right();
    while (!(analogRead(IR[4]) > 450))
    {
        big_turn_right();
    }
    stop();
    trail_cross(); // 循跡到卸貨T字路口
    /*################到達卸貨點################*/
    pick_down();
    arm_up();
}

void pickup_right()
{
    pick_down_2();
    delay(100);

    return_to_line_right();
    delay(100);

    duration_time_diff = millis();
    while (!(millis() - duration_time_diff >= 500))
    {
        trail();
    }
    stop();
    while (!((analogRead(IR[1]) < 450) and (analogRead(IR[2]) < 450) and (analogRead(IR[3]) < 450)))
    {
        trail();
        if (analogRead(IR[1]) < 450 and analogRead(IR[2]) < 450 and analogRead(IR[3]) < 450)
        {
            stop();
            delay(100);
        }
    }
    stop();
    /*################到達第三個取貨點(右)################*/
    pick_up();
    delay(500);
    /*################前往卸貨點################*/
    return_to_line_left();
    trail_cross();

    return_to_line_left();
    while (!(analogRead(IR[0]) > 450))
    {
        big_turn_left();
    }
    stop();
    trail_cross(); // 循跡到卸貨T字路口
    /*################到達卸貨點################*/
    pick_down();
    arm_up();
}

void my_init()
{
    for (int i = 0; i < 5; i++)
    {
        pinMode(IR[i], INPUT);
    }

    pinMode(mL, OUTPUT);
    pinMode(mR, OUTPUT);
    pinMode(sL, OUTPUT);
    pinMode(sR, OUTPUT);

    claw.attach(9);
    arm.attach(10);
}

void return_to_line_left()
{
    while (!(analogRead(IR[0]) > 450))
    {
        re_turn_left();
    }
    stop();
}
void return_to_line_right()
{
    while (!(analogRead(IR[4]) > 450))
    {
        re_turn_right();
    }
    stop();
}
void return_to_line_2()
{
    while (!(analogRead(IR[1]) > 450))
    {
        re_turn_left();
    }
    stop();
}

void trail_cross()
{
    while (!cross())
    {
        trail();
    }
    while (!(analogRead(IR[1]) < 450 or analogRead(IR[2]) < 450 or analogRead(IR[3]) < 450))
    {
        forward();
    }
    stop();
    delay(100);
}

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
        else if (analogRead(IR[1]) < 450 and analogRead(IR[3]) < 450)

        {
            forward();
        }
    }
    else
    {
        if (analogRead(IR[1]) > 450)
        {
            mid_turn_left();
        }
        else if (analogRead(IR[3]) > 450)
        {
            mid_turn_right();
        }
    }
}

bool cross()
{
    // 判斷IR1~IR3有沒有同時大於450，如果有的話就停止

    if (analogRead(IR[1]) > 450 and analogRead(IR[2]) > 450 and analogRead(IR[3]) > 450)
    {
        stop();
        return true;
    }
    else
    {
        return false;
    }
}

void forward()
{
    motor(100, 100);
}

void back()
{
    motor(-120, -120);
}
void small_turn_left()
{
    motor(80, 100);
}

void small_turn_right()
{
    motor(100, 80);
}

void mid_turn_left()
{
    motor(0, 110);
}
void mid_turn_right()
{
    motor(110, 0);
}

void big_turn_left()
{
    motor(-150, 100);
}

void big_turn_right()
{
    motor(100, -150);
}

void stop()
{
    motor(0, 0);
}

void re_turn_left()
{
    motor(-140, 100);
}
void re_turn_right()
{
    motor(100, -140);
}

void motor(int speedL, int speedR)
{
    // 判斷speedL跟speedR正負號決定方向
    digitalWrite(sL, speedL < 0 ? LOW : HIGH); // 左邊馬達方向設定
    digitalWrite(sR, speedR < 0 ? LOW : HIGH); // 右邊馬達方向設定

    analogWrite(mL, abs(speedL)); // 左邊馬達速度設定
    analogWrite(mR, abs(speedR)); // 右邊馬達速度設定
}