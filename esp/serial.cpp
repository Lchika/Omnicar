#include "serial.hpp"
#include "com.hpp"
#include <string.h>

/*==============================*/
/*======= シリアル通信クラス =======*/
/*==============================*/
/* コンストラクタ */
Serialrecieve::Serialrecieve(int baudrate) {
  Serial.begin(baudrate);
  before_char = '0';
  before_axis_ud = '0';
  before_axis_lf = '0';
}

/* デストラクタ */
Serialrecieve::~Serialrecieve(void) {}

/* セットアップ完了通知 */
void Serialrecieve::Finish_setup(void) {

}

/* シリアル受信 */
void Serialrecieve::Recieve_character(void) {
  if (Serial.available() > 0) {
    recieve_char = Serial.read();
  }
  if(recieve_char != before_char){
    if(recieve_char == NONE) {
      Send_character('N');
    } else {
      Send_character(recieve_char);
    }
  }
  before_char = recieve_char;
}

/* アナログスティック情報を受信する */
void Serialrecieve::Recieve_axis_data(void) {
  char buffer[2];
  int i = 0;
  while (1) {
    if(Serial.available() > 1) {
      recieve_axis_ud = Serial.read();
      recieve_axis_lf = Serial.read();
      break;
    }
  }

  Send_character('Z');

  before_axis_ud = recieve_axis_ud;
  before_axis_lf = recieve_axis_lf;
}

/* 今周期の受信データのgetter */
char Serialrecieve::Get_now_character(void) {
  return recieve_char;
}

/* 前周期の受信データのgetter */
char Serialrecieve::Get_before_character(void) {
  return before_char;
}

/* アナログスティック受信データのgetter */
/* 引数: 軸方向　　（0：上下 1: 左右)*/
char Serialrecieve::Get_now_axis_data(char dir) {
  if(dir == 0) {
    return recieve_axis_ud;
  } else {
    return recieve_axis_lf;
  }
}

/* 引数: 軸方向　　（0：上下 1: 左右)*/
char Serialrecieve::Get_before_axis_data(char dir) {
  if(dir == 0) {
    return before_axis_ud;
  } else {
    return before_axis_lf;
  }
}

/* シリアル送信 */
/* 文字 */
void Serialrecieve::Send_character(char send_char) {
  Serial.write(send_char);
}
/* 数値 */
void Serialrecieve::Send_number(char dir) {
  if(dir == 0) {
    Serial.write(recieve_axis_ud);
  } else {
    Serial.write(recieve_axis_lf);   
  }
}