#ifndef _COM_HPP_
#define _COM_HPP_

typedef unsigned char byte;

/* シリアル受信情報 */
/* ボタン */
#define FRONT    'F'
#define BACK     'B'
#define LEFT     'L'
#define RIGHT    'R'
#define CIRCLE   'C'
#define SQUARE   'S'
#define TRIANGLE 'T'
#define CROSS    'X'
#define L1       'H'
#define R1       'I'
#define L2       'J'
#define R2       'K'
#define SHARE    'Y'
#define OPTIONS  'Z'
#define NONE     'N'
/* アナログ */
#define PLUS_10  'A'
#define PLUS_9   'B'
#define PLUS_8   'C'
#define PLUS_7   'D'
#define PLUS_6   'E'
#define PLUS_5   'F'
#define PLUS_4   'G'
#define PLUS_3   'H'
#define PLUS_2   'I'
#define PLUS_1   'J'
#define PLUS_0   'K'
#define MINUS_1  'L'
#define MINUS_2  'M'
#define MINUS_3  'N'
#define MINUS_4  'O'
#define MINUS_5  'P'
#define MINUS_6  'Q'
#define MINUS_7  'R'
#define MINUS_8  'S'
#define MINUS_9  'T'
#define MINUS_10 'U'

/* 指令タイプ */
enum COMMAND_TYPE {
    BUTTON    = 0,
    AXIS      = 1,
    IMAGE     = 2,
};

/* 指令方向 */
enum COMMAND_DIR {
	/* 停止 */
	NONE_DIR   = 0,
	/* 1番ホイールを正面にして前後左右 */
	FRONT_DIR  = 1,
	BACK_DIR   = 2,
	LEFT_DIR   = 3,
	RIGHT_DIR  = 4,
	/* 旋回 */
	TLEFT_DIR  = 5,
	TRIGHT_DIR = 6,	
	/* 2番ホイールを正面にして前後左右 */
	R_FRONT_DIR = 7,
	R_BACK_DIR  = 8,
	R_LEFT_DIR  = 9,
	R_RIGHT_DIR = 10,
	/* 3番ホイールを正面にして前後左右 */
	L_FRONT_DIR = 11,
	L_BACK_DIR  = 12,
	L_LEFT_DIR  = 13,
	L_RIGHT_DIR = 14,
};

/* オムニカーのピン設定構造体 */
typedef struct OMNI_CAR_PIN {
public:
	/* モータのピン割り当て */
	byte motor1_pinA;
	byte motor1_pinB;
	byte motor1_pinC;
	byte motor2_pinA;
	byte motor2_pinB;
	byte motor2_pinC;
	byte motor3_pinA;
	byte motor3_pinB;
	byte motor3_pinC;

	/* モータドライバの状態設定ピン */
	byte driver_D_pin;
	byte driver_E_pin;

	/* エンコーダの入力ピン割り当て */
	byte encoder1_pinA;
	byte encoder1_pinB;
	byte encoder2_pinA;
	byte encoder2_pinB;
	byte encoder3_pinA;
	byte encoder3_pinB;
}OMNI_CAR_PIN;

/* オムニカーへの指令情報 */
typedef struct OMNICAR_COMMAND {
public:
    /* 前後方向の速度比率 */
    char front_back_ratio;
    /* 左右方向の速度比率 */
    char left_right_ratio;
    /* 左右方向旋回の速度比率 */
    char turn_ratio;
	/* 移動方向 */
	COMMAND_DIR dir;
	/* 前後方向 */
	COMMAND_DIR fb_dir;
	/* 左右方向 */
	COMMAND_DIR lr_dir;
	
	char speed_ratio;
}OMNICAR_COMMAND;

/* 変数の符号を返す */
extern char Return_value_sign(char);

/* 文字の数字を数値に変換 */
extern char ctoi(char);

#endif
