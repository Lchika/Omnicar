#include "command.hpp"
#include <stdlib.h>

/*==============================*/
/*======= 指令生成クラス =========*/
/*==============================*/
/* コンストラクタ */
Command::Command(void) {
    command.front_back_ratio = 0;
    command.left_right_ratio = 0;
    command.turn_ratio       = 0;
    command.dir              = NONE_DIR;
    command.speed_ratio      = 50;
    command_type             = AXIS;
}

/* デストラクタ */
Command::~Command(void) {
    command.front_back_ratio = 0;
    command.left_right_ratio = 0;
    command.turn_ratio       = 0;
    command.dir              = NONE_DIR;
    command.speed_ratio      = 0;
}

/* セットされている指令を取得 */
/* 返り値: セットされている指令値 */
OMNICAR_COMMAND Command::Get_command(void) {
    return command;
}

/* 現在の指令タイプを取得 */
/* 返り値: セットされている指令値タイプ*/
COMMAND_TYPE Command::Get_command_type(void) {
    return command_type;
}

/* コントローラのキー入力から指令生成 */
/* 引数: 今周期のコントローラの十字キー/○×△□入力情報 */
/*       前周期のコントローラの十字キー/○×△□入力情報 */
void Command::Generate_command_from_button(char now_input, char before_input) {
    if(now_input == OPTIONS) {
        command_type = AXIS;
    }
    if((now_input == TRIANGLE) && (before_input != TRIANGLE)) {
        command.speed_ratio += 5;
        if(command.speed_ratio > 100) {
            command.speed_ratio = 100;
        }
    } else
    if((now_input == CROSS) && (before_input != CROSS)){
        command.speed_ratio -= 5;
        if(command.speed_ratio < 0) {
            command.speed_ratio = 0;
        }
    } else
    if(now_input == FRONT) {
        command.front_back_ratio = command.speed_ratio;
        command.left_right_ratio = 0;
        command.turn_ratio       = 0;
        command.dir              = FRONT_DIR;
    } else 
    if(now_input == BACK) {
        command.front_back_ratio = command.speed_ratio;
        command.left_right_ratio = 0;
        command.turn_ratio       = 0;
        command.dir              = BACK_DIR;
    } else 
    if(now_input == LEFT) {
        command.front_back_ratio = 0;
        command.left_right_ratio = command.speed_ratio;
        command.turn_ratio       = 0;
        command.dir              = LEFT_DIR;
    } else 
    if(now_input == RIGHT) {
        command.front_back_ratio = 0;
        command.left_right_ratio = command.speed_ratio;
        command.turn_ratio       = 0;
        command.dir              = RIGHT_DIR;
    } else 
    if(now_input == CIRCLE) {
        command.front_back_ratio = 0;
        command.left_right_ratio = 0;
        command.turn_ratio       = command.speed_ratio;
        command.dir              = TLEFT_DIR;
    } else 
    if(now_input == SQUARE) {
        command.front_back_ratio = 0;
        command.left_right_ratio = 0;
        command.turn_ratio       = command.speed_ratio;
        command.dir              = TRIGHT_DIR;
    } else {
        command.front_back_ratio = 0;
        command.left_right_ratio = 0;
        command.turn_ratio       = 0;
        command.dir              = NONE_DIR;
    }
}

/* コントローラのアナログスティック入力から指令生成 */
/* 引数: コントローラのアナログスティック入力情報 */
void Command::Generate_command_from_axis(char front_back_input_now, char left_right_input_now, 
                                         char front_back_input_before, char left_right_input_before, char now_input) {
    char fb_value, lf_value;
    if(now_input == OPTIONS) {
        command_type = BUTTON;
    }

    /* 旋回動作はしない */
    command.turn_ratio = 0;

    set_fb_command_from_axis(front_back_input_now);
    set_lr_command_from_axis(left_right_input_now);

    generate_radial_command();
}

void Command::set_fb_command_from_axis(char fb_command) {

    if(fb_command == PLUS_10) {
        command.front_back_ratio = 100;
        command.fb_dir          = FRONT_DIR;
    } else 
    if(fb_command == PLUS_9) {
        command.front_back_ratio = 90;
        command.fb_dir          = FRONT_DIR;
    } else 
    if(fb_command == PLUS_8) {
        command.front_back_ratio = 80;
        command.fb_dir          = FRONT_DIR;
    } else 
    if(fb_command == PLUS_7) {
        command.front_back_ratio = 70;
        command.fb_dir          = FRONT_DIR;
    } else 
    if(fb_command == PLUS_6) {
        command.front_back_ratio = 60;
        command.fb_dir          = FRONT_DIR;
    } else 
    if(fb_command == PLUS_5) {
        command.front_back_ratio = 50;
        command.fb_dir          = FRONT_DIR;
    } else 
    if(fb_command == PLUS_4) {
        command.front_back_ratio = 40;
        command.fb_dir          = FRONT_DIR;
    } else 
    if(fb_command == PLUS_3) {
        command.front_back_ratio = 30;
        command.fb_dir          = FRONT_DIR;
    } else 
    if(fb_command == PLUS_2) {
        command.front_back_ratio = 20;
        command.fb_dir          = FRONT_DIR;
    } else 
    if(fb_command == PLUS_1) {
        command.front_back_ratio = 10;
        command.fb_dir          = FRONT_DIR;
    } else 
    if(fb_command == MINUS_10) {
        command.front_back_ratio = 100;
        command.fb_dir          = BACK_DIR;
    } else 
    if(fb_command == MINUS_9) {
        command.front_back_ratio = 90;
        command.fb_dir          = BACK_DIR;
    } else 
    if(fb_command == MINUS_8) {
        command.front_back_ratio = 80;
        command.fb_dir          = BACK_DIR;
    } else 
    if(fb_command == MINUS_7) {
        command.front_back_ratio = 70;
        command.fb_dir          = BACK_DIR;
    } else 
    if(fb_command == MINUS_6) {
        command.front_back_ratio = 60;
        command.fb_dir          = BACK_DIR;
    } else
    if(fb_command == MINUS_5) {
        command.front_back_ratio = 50;
        command.fb_dir          = BACK_DIR;
    } else
    if(fb_command == MINUS_4) {
        command.front_back_ratio = 40;
        command.fb_dir          = BACK_DIR;
    } else
    if(fb_command == MINUS_3) {
        command.front_back_ratio = 30;
        command.fb_dir          = BACK_DIR;
    } else
    if(fb_command == MINUS_2) {
        command.front_back_ratio = 20;
        command.fb_dir          = BACK_DIR;
    } else
    if(fb_command == MINUS_1) {
        command.front_back_ratio = 10;
        command.fb_dir          = BACK_DIR;
    } else
    if(fb_command == PLUS_0) {
        command.front_back_ratio = 0;
        command.fb_dir          = NONE_DIR;
    }
}

void Command::set_lr_command_from_axis(char lr_command) {
    if(lr_command == PLUS_10) {
        command.left_right_ratio = 100;
        command.lr_dir          = LEFT_DIR;
    } else 
    if(lr_command == PLUS_9) {
        command.left_right_ratio = 90;
        command.lr_dir          = LEFT_DIR;
    } else 
    if(lr_command == PLUS_8) {
        command.left_right_ratio = 80;
        command.lr_dir          = LEFT_DIR;
    } else 
    if(lr_command == PLUS_7) {
        command.left_right_ratio = 70;
        command.lr_dir          = LEFT_DIR;
    } else 
    if(lr_command == PLUS_6) {
        command.left_right_ratio = 60;
        command.lr_dir          = LEFT_DIR;
    } else 
    if(lr_command == PLUS_5) {
        command.left_right_ratio = 50;
        command.lr_dir          = LEFT_DIR;
    } else 
    if(lr_command == PLUS_4) {
        command.left_right_ratio = 40;
        command.lr_dir          = LEFT_DIR;
    } else 
    if(lr_command == PLUS_3) {
        command.left_right_ratio = 30;
        command.lr_dir          = LEFT_DIR;
    } else 
    if(lr_command == PLUS_2) {
        command.left_right_ratio = 20;
        command.lr_dir          = LEFT_DIR;
    } else 
    if(lr_command == PLUS_1) {
        command.left_right_ratio = 10;
        command.lr_dir          = LEFT_DIR;
    } else 
    if(lr_command == MINUS_10) {
        command.left_right_ratio = 100;
        command.lr_dir          = RIGHT_DIR;
    } else 
    if(lr_command == MINUS_9) {
        command.left_right_ratio = 90;
        command.lr_dir          = RIGHT_DIR;
    } else 
    if(lr_command == MINUS_8) {
        command.left_right_ratio = 80;
        command.lr_dir          = RIGHT_DIR;
    } else 
    if(lr_command == MINUS_7) {
        command.left_right_ratio = 70;
        command.lr_dir          = RIGHT_DIR;
    } else 
    if(lr_command == MINUS_6) {
        command.left_right_ratio = 60;
        command.lr_dir          = RIGHT_DIR;
    } else
    if(lr_command == MINUS_5) {
        command.left_right_ratio = 50;
        command.lr_dir          = RIGHT_DIR;
    } else
    if(lr_command == MINUS_4) {
        command.left_right_ratio = 40;
        command.lr_dir          = RIGHT_DIR;
    } else
    if(lr_command == MINUS_3) {
        command.left_right_ratio = 30;
        command.lr_dir          = RIGHT_DIR;
    } else
    if(lr_command == MINUS_2) {
        command.left_right_ratio = 20;
        command.lr_dir          = RIGHT_DIR;
    } else
    if(lr_command == MINUS_1) {
        command.left_right_ratio = 10;
        command.lr_dir          = RIGHT_DIR;
    } else
    if(lr_command == PLUS_0) {
        command.left_right_ratio = 0;
        command.lr_dir          = NONE_DIR;
    }
}

void Command::generate_radial_command(void) {
    int ratio;
    int comp;
    /* 左右方向のみの場合 */
    if(command.fb_dir == NONE_DIR) {
        if(command.lr_dir == NONE_DIR) {
            command.dir = NONE_DIR;
        } else {
            command.dir = command.lr_dir;
        }
    } 
    /* 前後方向のみの場合 */
    else if(command.lr_dir == NONE_DIR) {
        command.dir = command.fb_dir;
    }
    /* 前後左右共に移動成分がある場合 */
    /* 前後方向と左右方向の比率から進行方向を求める */
    else {
        if(command.front_back_ratio > command.left_right_ratio) {
            // ratio = float(command.front_back_ratio) / float(command.left_right_ratio);
            comp = 0;
        } else {
            // ratio = float(command.left_right_ratio) / float(command.front_back_ratio);
            comp = 1;
        }
        /* 前後方向の方が移動成分が大きい場合 */
        if(comp == 0) {
            if(command.fb_dir == FRONT_DIR) {
                 /* 前右方向 */
                if(command.lr_dir == RIGHT_DIR) {
                    command.dir = L_LEFT_DIR;
                } 
                /* 前左方向 */
                else {
                    command.dir = R_RIGHT_DIR;
                }
            } else {
                 /* 後右方向 */
                if(command.lr_dir == RIGHT_DIR) {
                    command.dir = R_LEFT_DIR;
                } 
                /* 後左方向 */
                else {
                    command.dir = L_RIGHT_DIR;
                }
            }
        }
        /* 前後方向の方が移動成分が小さい場合 */
        else if(ratio < 1 || ratio > (1/2)){
            if(command.fb_dir == FRONT_DIR) {
                 /* 前右方向 */
                if(command.lr_dir == RIGHT_DIR) {
                    command.dir = R_BACK_DIR;
                } 
                /* 前左方向 */
                else {
                    command.dir = L_BACK_DIR;
                }
            } else {
                 /* 後右方向 */
                if(command.lr_dir == RIGHT_DIR) {
                    command.dir = L_FRONT_DIR;
                } 
                /* 後左方向 */
                else {
                    command.dir = R_FRONT_DIR;
                }
            }
        } else {
            if(command.front_back_ratio >= command.left_right_ratio) {
                command.dir = command.fb_dir;
            } else {
                command.dir = command.lr_dir;
            }
        }
    }
}

/* 画像処理結果から指令生成 */
/* 引数: 画像処理結果情報 */
void Command::Generate_command_from_imageprocess(char* imageprocess_input) {

}