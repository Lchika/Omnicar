#ifndef _COMMAND_HPP_
#define _COMMAND_HPP_

#include "com.hpp"

class Command {
private:
    OMNICAR_COMMAND command;
    COMMAND_TYPE command_type;
    void set_fb_command_from_axis(char);
    void set_lr_command_from_axis(char);
    void generate_radial_command(void);
public:
    Command(void);
    ~Command(void);
    OMNICAR_COMMAND Get_command(void);
    COMMAND_TYPE    Get_command_type(void);
    void Generate_command_from_button(char, char);
    void Generate_command_from_axis(char, char, char, char, char);
    void Generate_command_from_imageprocess(char*);
};

#endif