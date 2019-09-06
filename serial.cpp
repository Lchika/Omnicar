#include "serial.hpp"
#include "com.hpp"
#include <M5Stack.h>
#include <string.h>

/*==============================*/
/*======= シリアル通信クラス =======*/
/*==============================*/
/* コンストラクタ */
Serialrecieve::Serialrecieve(int baudrate) {
  Serial.begin(baudrate);
  dacWrite(25, 0); // Speaker OFF
  // SerialBT.begin("M5Stack");
  M5.begin();
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(RED);
  M5.Lcd.setTextSize(3);
  M5.Lcd.drawString("    SETUP ~_~", 0, 100);
  delay(1000);
  M5.Lcd.clear();
  // Finish_setup();
  before_char = '0';
  before_axis_ud = '0';
  before_axis_lf = '0';
}

/* デストラクタ */
Serialrecieve::~Serialrecieve(void) {}

/* セットアップ完了通知 */
void Serialrecieve::Finish_setup(void) {
  M5.Lcd.fillScreen(WHITE);
  M5.Lcd.setTextColor(BLUE);
  M5.Lcd.drawString("    START +v+", 0, 100);
  delay(1000);
  M5.Lcd.clear();

  M5.Lcd.fillScreen(WHITE);
  M5.Lcd.setTextColor('WHITE');
  M5.Lcd.fillRect(0, 0, 400, 40, BLUE);
  M5.Lcd.drawString("MODE: BUTTON", 5, 10);

  M5.Lcd.fillRect(0, 40, 200, 200, WHITE);
  M5.Lcd.setCursor(10, 60);
  M5.Lcd.setTextColor(BLACK);
  M5.Lcd.drawString("INPUT: ", 10 ,60);
  M5.Lcd.drawString("ACTION: ", 10 ,100);
}

/* シリアル受信 */
void Serialrecieve::Recieve_character(void) {
  if (Serial.available() > 0) {
    recieve_char = Serial.read();
  }
  if(recieve_char != before_char){
    if(recieve_char == LEFT) {
      M5.Lcd.setTextColor(RED);
      strcpy(string1, "LEFT");
      strcpy(string2, "GO LEFT");
    } else if(recieve_char == RIGHT) {
      M5.Lcd.setTextColor(BLUE);
      strcpy(string1, "RIGHT");
      strcpy(string2, "GO RIGHT");
    } else if(recieve_char == FRONT) {
      M5.Lcd.setTextColor(GREEN);
      strcpy(string1, "UP");
      strcpy(string2, "GO FORWARD");
    } else if(recieve_char == BACK) {
      M5.Lcd.setTextColor(ORANGE);
      strcpy(string1, "DOWN");
      strcpy(string2, "GO BACK");
    } else if(recieve_char == CIRCLE) {
      M5.Lcd.setTextColor(CYAN);
      strcpy(string1, "CIRCLE");
      strcpy(string2, "TURN RIGHT");
    } else if(recieve_char == SQUARE) {
      M5.Lcd.setTextColor(MAGENTA);
      strcpy(string1, "SQUARE");
      strcpy(string2, "TURN LEFT");
    } else if(recieve_char == TRIANGLE) {
      M5.Lcd.setTextColor(DARKGREY);
      strcpy(string1, "TRIANGLE");
      strcpy(string2, "NOT ASSIGN");
    } else if(recieve_char == CROSS) {
      M5.Lcd.setTextColor(DARKGREY);
      strcpy(string1, "CROSS");
      strcpy(string2, "NOT ASSIGN");
    } else if(recieve_char == L1) {
      M5.Lcd.setTextColor(DARKGREY);
      strcpy(string1, "L1");
      strcpy(string2, "NOT ASSIGN");
    } else if(recieve_char == R1) {
      M5.Lcd.setTextColor(DARKGREY);
      strcpy(string1, "R1");
      strcpy(string2, "NOT ASSIGN");
    } else if(recieve_char == L2) {
      M5.Lcd.setTextColor(DARKGREY);
      strcpy(string1, "L2");
      strcpy(string2, "NOT ASSIGN");
    } else if(recieve_char == R2) {
      M5.Lcd.setTextColor(DARKGREY);
      strcpy(string1, "R2");
      strcpy(string2, "NOT ASSIGN");
    } else if(recieve_char == SHARE) {
      M5.Lcd.setTextColor(DARKGREY);
      strcpy(string1, "SHARE");
      strcpy(string2, "NOT ASSIGN");
    } else if(recieve_char == OPTIONS) {
      M5.Lcd.setTextColor(DARKGREY);
      strcpy(string1, "OPTIONS");
      strcpy(string2, "NOT ASSIGN");
    } else if(recieve_char == NONE){
      M5.Lcd.setTextColor(BLACK);
      strcpy(string1, "NONE");
      strcpy(string2, "-");
    }
    M5.Lcd.fillRect(120, 60, 400, 40, WHITE);
    M5.Lcd.fillRect(130, 100, 400, 40, WHITE);
    M5.Lcd.setTextSize(2);
    M5.Lcd.drawString(string1, 120, 60);
    M5.Lcd.drawString(string2, 135, 100);
    Send_character(recieve_char);
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
    // if(i > 1) {
    //   break;
    // }
  }
  // buffer[0] = recieve_axis_ud;
  // buffer[1] = recieve_axis_lf;

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