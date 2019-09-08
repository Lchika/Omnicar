#include "com.hpp"

char Return_value_sign(char value) {
    char return_val = 0;
    if(value > 0) {
        return_val =  1;
    }
    if(value < 0) {
        return_val = -1;
    }
    return return_val;
}

char ctoi(char input) {
    char output;
    switch(input) {
        case PLUS_10:
            output = 10;
            break;
        case PLUS_9:
            output = 9;
            break;
        case PLUS_8:
            output = 8;
            break;
        case PLUS_7:
            output = 7;
            break;
        case PLUS_6:
            output = 6;
            break;
        case PLUS_5:
            output = 5;
            break;
        case PLUS_4:
            output = 4;
            break;
        case PLUS_3:
            output = 3;
            break;
        case PLUS_2:
            output = 2;
            break;
        case PLUS_1:
            output = 1;
            break;
        case PLUS_0:
            output = 0;
            break;
        case MINUS_1:
            output = -1;
            break;
        case MINUS_2:
            output = -2;
            break;
        case MINUS_3:
            output = -3;
            break;
        case MINUS_4:
            output = -4;
            break;
        case MINUS_5:
            output = -5;
            break;
        case MINUS_6:
            output = -6;
            break;
        case MINUS_7:
            output = -7;
            break;
        case MINUS_8:
            output = -8;
            break;
        case MINUS_9:
            output = -9;
            break;
        case MINUS_10:
            output = -10;
            break;
        default:
            output = 0;
            break;
    }
    return output;
}