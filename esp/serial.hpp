#ifndef _SERIAL_HPP_
#define _SERIAL_HPP_

#include "BluetoothSerial.h"

class Serialrecieve {
private:
    char recieve_char;
    char before_char = '0';
    char string1[10];
    char string2[10];
    char recieve_axis_ud;
    char recieve_axis_lf;
    char before_axis_ud;
    char before_axis_lf;
    BluetoothSerial SerialBT;
public:
    Serialrecieve(int);
    ~Serialrecieve(void);
    void Finish_setup(void);
    void Recieve_character(void);
    char Get_now_character(void);
    char Get_before_character(void);
    void Recieve_axis_data(void);
    char Get_now_axis_data(char);
    char Get_before_axis_data(char);
    void Send_character(char);
    void Send_number(char);
};

#endif