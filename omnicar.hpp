#ifndef _OMNI_CAR_HPP_
#define _OMNI_CAR_HPP_

#include "driver/pcnt.h"
#include "com.hpp"

typedef unsigned char byte;

#define RESOLUTION 636 //エンコーダ分解能
#define MAX_DUTY   255 //最大デューティ値
#define CLAMP_DUTY 20 //指定デューティ値最小クランプ値
#define MIN_DUTY    0 //最小デューティ値

/*======= モータクラス =======*/
class Motor{
	private:
		int chanel;
		byte duty;
		byte pinA, pinB;
		byte state;
	public:
		Motor(byte, byte, byte, byte, int);
		~Motor(void);
		void RotSettedSpeed(byte, char);
		void Stop(void);
		void Pause(void);
};

/*======= エンコーダクラス =======*/
class Encoder {
	private:
		int16_t count;
	public:
		/* コンストラクタ */
		/* 引数：カウンタの桁数, エンコーダA相/B相の割り当てピン */
		Encoder(pcnt_unit_t, byte, byte);
		~Encoder(void);
		int16_t GetEncoderValue(void);
};

/*======= オムニカークラス =======*/
class Omnicar {
private:
	OMNICAR_COMMAND command;
	Motor Motor1, Motor2, Motor3;
	Encoder Encoder1, Encoder2, Encoder3;
/*			 
			↑	 1
	前進方向 ↑	  _
	        ↑   / \
	           /   \
	          /     \
	         /       \
	     2  |_________|  3
*/
	void StopAll(void);
	void PauseAll(void);
	void MoveFB(char, char);
	void MoveLR(char, char);
	void MoveFBLR(char, char);
	void TurnLR(char);
public:
	char setup_f = 0;
	Omnicar(OMNI_CAR_PIN);
	~Omnicar(void);
	void Set_command(OMNICAR_COMMAND);
	void Generate_motor_command(void);
	void Regenerate_command_with_feedback(void);
	void Drive(void);
};

#endif