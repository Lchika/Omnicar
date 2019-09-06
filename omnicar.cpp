#include "omnicar.hpp"
#include <stdlib.h>
#include "esp32-hal-ledc.h"
#include "Arduino.h"

/*==============================*/
/*======= モータクラス ===========*/
/*==============================*/
/* コンストラクタ */
/* 引数:出力A/Bのチャネル，ピン */
Motor::Motor(byte inputA_pin, byte inputB_pin, byte inputC_pin, byte inputD_pin, int chanel_val) {
	chanel = chanel_val;
	pinA = inputA_pin;
	pinB = inputB_pin;
	state = inputD_pin;
	ledcSetup(chanel, 12800, 8);
	ledcAttachPin(inputC_pin, chanel);
	ledcWrite(chanel, 255);
	pinMode(pinA, OUTPUT);
	pinMode(pinB, OUTPUT);
	pinMode(state, OUTPUT);
	/* 初期状態を停止状態にしておく */
	Pause();
}

/* デストラクタ */
Motor::~Motor(void) {
	/* モータを停止してから終了 */
	Pause();
}

/* 設定されたデューティ値で回転させる */
/* 引数：設定するデューティ値, 回転方向 */
void Motor::RotSettedSpeed(byte abs_duty_ratio, char dir) {
	byte duty;
	/* 最小クランプ値未満の場合クランプ */
	duty = int(float(abs_duty_ratio) / 100.0 * MAX_DUTY);
	if(duty < CLAMP_DUTY) {
		duty = CLAMP_DUTY;
	}
	ledcWrite(chanel, duty);
	digitalWrite(state, HIGH);
	/* CW */
	if(dir == 1) {
		digitalWrite(pinA, HIGH);
		digitalWrite(pinB, LOW);
	}
	/* CCW */
	else {
		digitalWrite(pinA, LOW);
		digitalWrite(pinB, HIGH);
	}
}

/* 停止する */
/* 一時停止状態 */
void Motor::Stop(void) {
	digitalWrite(pinA, LOW);
	digitalWrite(pinB, LOW);
	digitalWrite(state, HIGH);
}

/* 停止する */
/* 完全停止状態 */
void Motor::Pause(void) {
	digitalWrite(pinA, LOW);
	digitalWrite(pinB, LOW);
	digitalWrite(state, LOW);
}


/*==============================*/
/*======= エンコーダクラス ========*/
/*==============================*/
/* コンストラクタ */
/* 引数：カウンタの桁数, エンコーダA相/B相の割り当てピン */
Encoder::Encoder(pcnt_unit_t pcnt_unit, byte gpioA, byte gpioB) {
	pcnt_config_t pcnt_confA;
	pcnt_config_t pcnt_confB;
	
	pcnt_confA.unit = pcnt_unit;
	pcnt_confA.channel = PCNT_CHANNEL_0;
	pcnt_confA.pulse_gpio_num = gpioA;
	pcnt_confA.ctrl_gpio_num = gpioB;
	pcnt_confA.pos_mode = PCNT_COUNT_INC;
	pcnt_confA.neg_mode = PCNT_COUNT_DEC;
	pcnt_confA.lctrl_mode = PCNT_MODE_REVERSE;
	pcnt_confA.hctrl_mode = PCNT_MODE_KEEP;
	pcnt_confA.counter_h_lim = 32767;
	pcnt_confA.counter_l_lim = -32768;

	pcnt_confB.unit = pcnt_unit;
	pcnt_confB.channel = PCNT_CHANNEL_1;
	pcnt_confB.pulse_gpio_num = gpioB;
	pcnt_confB.ctrl_gpio_num = gpioA;
	pcnt_confB.pos_mode = PCNT_COUNT_INC;
	pcnt_confB.neg_mode = PCNT_COUNT_DEC;
	pcnt_confB.lctrl_mode = PCNT_MODE_KEEP;
	pcnt_confB.hctrl_mode = PCNT_MODE_REVERSE;
	pcnt_confB.counter_h_lim = 32767;
	pcnt_confB.counter_l_lim = -32768;
	
	/* Initialize PCNT unit */
	pcnt_unit_config(&pcnt_confA);
	pcnt_unit_config(&pcnt_confB);
	
	pcnt_counter_pause(pcnt_unit);
	pcnt_counter_clear(pcnt_unit);
	pcnt_counter_resume(pcnt_unit);
}

/* デストラクタ */
Encoder::~Encoder(void) {}	

/* エンコーダの値を取得する */
int16_t Encoder::GetEncoderValue(void) {
	pcnt_get_counter_value(PCNT_UNIT_2, &count);
	return count;
}


/*==============================*/
/*======= オムニカークラス =======*/
/*==============================*/
/* コンストラクタ */
/* 引数：オムニカーのピン設定構造体 */
Omnicar::Omnicar(OMNI_CAR_PIN omni) : 
	Motor1(omni.motor1_pinA, omni.motor1_pinB, omni.motor1_pinC, omni.driver_D_pin, 0),
	Motor2(omni.motor2_pinA, omni.motor2_pinB, omni.motor2_pinC, omni.driver_D_pin, 1),
	Motor3(omni.motor3_pinA, omni.motor3_pinB, omni.motor3_pinC, omni.driver_E_pin, 2),
	Encoder1(PCNT_UNIT_2, omni.encoder1_pinA, omni.encoder1_pinB),
	Encoder2(PCNT_UNIT_2, omni.encoder2_pinA, omni.encoder2_pinB),
	Encoder3(PCNT_UNIT_2, omni.encoder3_pinA, omni.encoder3_pinB)
{
	/* 起動直後は停止 */
	PauseAll();
	setup_f = 1;
}

/* デストラクタ */
Omnicar::~Omnicar(void) {
	/* モータを停止してから終了 */
	PauseAll();
}

/* 新しい指令をセットする */
/* 引数：新しい指令 */
void Omnicar::Set_command(OMNICAR_COMMAND newcommand) {
	command = newcommand;
}

/* モータへの指令情報を生成 */
void Omnicar::Generate_motor_command(void) {

}

/* エンコーダからのフィードバック情報を加味してモータへの指令情報を再生成 */
void Omnicar::Regenerate_command_with_feedback(void) {
}

/* 生成された指令情報を元にモータへ指令 */
void Omnicar::Drive(void) {
	/* 左右直進指令 */
	if((command.dir == LEFT_DIR) || (command.dir == RIGHT_DIR)) {
		MoveLR(command.left_right_ratio, 1);
	}
	else 
	/* 前後直進指令 */
	if((command.dir == FRONT_DIR) || (command.dir == BACK_DIR)) {
		MoveFB(command.front_back_ratio, 1);
	} else 
	/* 旋回指令 */
	if((command.dir == TLEFT_DIR) || (command.dir == TRIGHT_DIR)) {
		TurnLR(command.turn_ratio);
	} else 
	if(command.dir == R_FRONT_DIR) {
		MoveFB(command.left_right_ratio, 2);
	} else 
	if(command.dir == R_BACK_DIR) {
		MoveFB(command.left_right_ratio, 2);
	} else 
	if(command.dir == R_LEFT_DIR) {
		MoveLR(command.front_back_ratio, 2);
	} else 
	if(command.dir == R_RIGHT_DIR) {
		MoveLR(command.front_back_ratio, 2);
	} else 
	if(command.dir == L_FRONT_DIR) {
		MoveFB(command.left_right_ratio, 3);
	} else 
	if(command.dir == L_BACK_DIR) {
		MoveFB(command.left_right_ratio, 3);
	} else 
	if(command.dir == L_LEFT_DIR) {
		MoveLR(command.front_back_ratio, 3);
	} else 
	if(command.dir == L_RIGHT_DIR) {
		MoveLR(command.front_back_ratio, 3);
	} else {
		StopAll();
	}
}

/* 全モータを停止する(通電状態) */
/* 減速してから停止する */
void Omnicar::StopAll(void) {
	Motor1.Stop();
	Motor2.Stop();
	Motor3.Stop();
}

/* 全モータを停止する(非通電状態) */
/* 即時停止する */
void Omnicar::PauseAll(void) {
	Motor1.Pause();
	Motor2.Pause();
	Motor3.Pause();
}

/* 前後にすすむ */
/* 引数: 速度比率 */
/*    : 正面のホイール番号 */
void Omnicar::MoveFB(char speed_ratio, char front_wheel) {
	/* 1番ホイールが正面 */
	if(front_wheel == 1) {
		if(command.dir == BACK_DIR || command.dir == R_BACK_DIR || command.dir == L_BACK_DIR) {
			Motor1.Stop();
			Motor2.RotSettedSpeed(speed_ratio, -1);
			Motor3.RotSettedSpeed(speed_ratio, 1);
		} else {
			Motor1.Stop();
			Motor2.RotSettedSpeed(speed_ratio, 1);
			Motor3.RotSettedSpeed(speed_ratio, -1);
		}
	} else 
	/* 2番ホイールが正面 */
	if(front_wheel == 2) {
		if(command.dir == BACK_DIR || command.dir == R_BACK_DIR || command.dir == L_BACK_DIR) {
			Motor1.RotSettedSpeed(speed_ratio, -1);
			Motor2.Stop();
			Motor3.RotSettedSpeed(speed_ratio, -1);
		} else {
			Motor1.RotSettedSpeed(speed_ratio, 1);
			Motor2.Stop();
			Motor3.RotSettedSpeed(speed_ratio, 1);
		}
	} else 
	/* 3番ホイールが正面 */
	{
		if(command.dir == BACK_DIR || command.dir == R_BACK_DIR || command.dir == L_BACK_DIR) {
			Motor1.RotSettedSpeed(speed_ratio, 1);
			Motor2.RotSettedSpeed(speed_ratio, 1);
			Motor3.Stop();
		} else {
			Motor1.RotSettedSpeed(speed_ratio, -1);
			Motor2.RotSettedSpeed(speed_ratio, -1);
			Motor3.Stop();
		}
	}
}

/* 左右に進む */
/* 引数: 速度比率 */
/*    : 正面のホイール番号 */
void Omnicar::MoveLR(char speed_ratio, char front_wheel) {
	/* 1番ホイールが正面 */
	if(front_wheel == 1) {
		if(command.dir == RIGHT_DIR || command.dir == R_RIGHT_DIR || command.dir == L_RIGHT_DIR) {
			Motor1.RotSettedSpeed(speed_ratio * 1, -1);
			Motor2.RotSettedSpeed(speed_ratio * 0.58 , -1);
			Motor3.RotSettedSpeed(speed_ratio * 0.5, -1);
		} else {
			Motor1.RotSettedSpeed(speed_ratio, 1);
			Motor2.RotSettedSpeed(speed_ratio * 0.5, 1);
			Motor3.RotSettedSpeed(speed_ratio * 0.58 , 1);
		}
	} else 
	/* 2番ホイールが正面 */
	if(front_wheel == 2) {
		if(command.dir == RIGHT_DIR || command.dir == R_RIGHT_DIR || command.dir == L_RIGHT_DIR) {
			Motor2.RotSettedSpeed(speed_ratio * 1, 1);
			Motor3.RotSettedSpeed(speed_ratio * 0.58 , -1);
			Motor1.RotSettedSpeed(speed_ratio * 0.5, 1);
		} else {
			Motor2.RotSettedSpeed(speed_ratio, -1);
			Motor3.RotSettedSpeed(speed_ratio * 0.5, 1);
			Motor1.RotSettedSpeed(speed_ratio * 0.58 , -1);
		}
	} else 
	/* 3番ホイールが正面 */
	{
		if(command.dir == RIGHT_DIR || command.dir == R_RIGHT_DIR || command.dir == L_RIGHT_DIR) {	
			Motor3.RotSettedSpeed(speed_ratio * 1, 1);
			Motor1.RotSettedSpeed(speed_ratio * 0.58 , 1);
			Motor2.RotSettedSpeed(speed_ratio * 0.5, -1);
		} else {
			Motor3.RotSettedSpeed(speed_ratio, -1);
			Motor1.RotSettedSpeed(speed_ratio * 0.5, -1);
			Motor2.RotSettedSpeed(speed_ratio * 0.58 , 1);
		}
	}
}

/* 前後左右に進む */
/* 引数: 速度比率 */
void Omnicar::MoveFBLR(char speed_ratio_fb, char speed_ratio_lr) {
	//TODO: 動作不可
}

/* 左右に旋回する */
void Omnicar::TurnLR(char speed_ratio) {
	if(command.dir == TLEFT_DIR) {
		Motor1.RotSettedSpeed(speed_ratio, -1);
		Motor2.RotSettedSpeed(speed_ratio, 1);
		Motor3.RotSettedSpeed(speed_ratio, 1);
	} else {
		Motor1.RotSettedSpeed(speed_ratio, 1);
		Motor2.RotSettedSpeed(speed_ratio, -1);
		Motor3.RotSettedSpeed(speed_ratio, -1);
	}
}
