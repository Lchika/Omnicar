#include "com.hpp"
#include "serial.hpp"
#include "command.hpp"
#include "omnicar.hpp"



/* 初期設定 */
void setup() {
}

void set_omnicar_pin(OMNI_CAR_PIN* pin_struct) {
  /* モータ出力用ピン */
  pin_struct->motor1_pinA   = 15;
  pin_struct->motor1_pinB   = 2;
  pin_struct->motor1_pinC   = 0;
  pin_struct->motor2_pinA   = 5;
  pin_struct->motor2_pinB   = 21;
  pin_struct->motor2_pinC   = 19;
  pin_struct->motor3_pinA   = 4;
  pin_struct->motor3_pinB   = 16;
  pin_struct->motor3_pinC   = 17;
  /* モータドライバ状態設定用ピン */
  pin_struct->driver_D_pin  = 23;
  pin_struct->driver_E_pin  = 22;
  /* エンコーダ入力用ピン */
  pin_struct->encoder1_pinA = 0;//35;
  pin_struct->encoder1_pinB = 0;//34;
  pin_struct->encoder2_pinA = 0;//2;
  pin_struct->encoder2_pinB = 0;//4;
  pin_struct->encoder3_pinA = 0;//22;
  pin_struct->encoder3_pinB = 0;//23;
}

/* 処理の流れ:
①ラズパイからコントローラ入力or画像処理結果をシリアル通信で受信する
TODO: 何msec周期(何周期に1回)受信するか
TODO: 画像処理結果の受信データフォーマット
②受信データからオムニカーへの指令情報を生成する
TODO: 画像処理結果から指令生成する方法
③オムニカーオブジェクトに生成した指令情報を入力する
④オムニカーオブジェクトが各モータへの指令情報を生成する
⑤生成した指令情報からエンコーダからのフィードバック情報を加味した指令情報を生成する
⑥モータへ指令する
 */
void loop() {
  static Serialrecieve SerialrecieveX(115200);
  static Command CommandX;
  OMNI_CAR_PIN omni_car_pin;
  set_omnicar_pin(&omni_car_pin);
  static Omnicar OmnicarX(omni_car_pin);
  // if(OmnicarX.setup_f == 1) {
  //   SerialrecieveX.Finish_setup();
  //   OmnicarX.setup_f = 0;
  //   delay(1000);
  // }
  /* 指令タイプ: ボタン */
  if(CommandX.Get_command_type() == BUTTON) {
    /* ① */
    SerialrecieveX.Recieve_character();
    /* ② */
    CommandX.Generate_command_from_button(SerialrecieveX.Get_now_character(), SerialrecieveX.Get_before_character());
  } else
  /* 指令タイプ: アナログスティック */
  if(CommandX.Get_command_type() == AXIS) {
    /* ① */
    SerialrecieveX.Recieve_axis_data();
    /* ② */
    CommandX.Generate_command_from_axis(SerialrecieveX.Get_now_axis_data(0), 
                                        SerialrecieveX.Get_now_axis_data(1), 
                                        SerialrecieveX.Get_before_axis_data(0), 
                                        SerialrecieveX.Get_before_axis_data(1), 
                                        SerialrecieveX.Get_now_character());
  } 
  /* 指令タイプ: 画像処理結果 */
  else {
    //TODO
  } 
  /* ③ */
  OmnicarX.Set_command(CommandX.Get_command());
  /* ④ */
  OmnicarX.Generate_motor_command();
  // /* ⑤ */
  OmnicarX.Regenerate_command_with_feedback();
  /* ⑥ */
  OmnicarX.Drive();
}