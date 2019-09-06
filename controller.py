# coding:utf-8
import pprint
import pygame
import serial
from time import sleep
import math
import struct

class PS4Controller():
    def __init__(self):
        pygame.init()
        pygame.joystick.init()
        self.controller = pygame.joystick.Joystick(0)
        self.controller.init()
        self.hat_data = {}
        self.button_data = {}
        self.axis_data = {}
        self.before_input_hat = 0 #0:NONE 1:LEFT 2:RIGHT 3:UP 4:DOWN
        self.before_input_button = 0  #0:NONE 1:× 2:○ 3:△ 4:□
        self.hat_input = 0
        self.button_input = 0
        for i in range(self.controller.get_numhats()):
            self.hat_data[i] = (0, 0)
        for i in range(self.controller.get_numbuttons()):
            self.button_data[i] = False

        self.port = serial.Serial(
            port="/dev/ttyUSB0",
            baudrate=115200,
            parity=serial.PARITY_NONE,
            stopbits=serial.STOPBITS_ONE,
            bytesize=serial.EIGHTBITS,
            timeout=0.001
        )

    #十字ボタン
    def listen_hat(self):
        for event in pygame.event.get():
            if event.type == pygame.JOYHATMOTION:
                self.hat_data[event.hat] = event.value

                if self.hat_data[0][0] == 1:
                    if self.before_input_hat != 2:
                        pprint.pprint("RIGHT(→)")
                        self.port.write("R".encode('utf-8'))
                    self.before_input_hat = 2
                    self.hat_input = 2
                elif self.hat_data[0][0] == -1:
                    if self.before_input_hat != 1:
                        pprint.pprint("LEFT(←)")
                        self.port.write("L".encode('utf-8'))
                    self.before_input_hat = 1
                    self.hat_input = 1
                elif self.hat_data[0][1] == 1:
                    if self.before_input_hat != 3:
                        pprint.pprint("FRONT(↑)")
                        self.port.write("F".encode('utf-8'))
                    self.before_input_hat = 3
                    self.hat_input = 3
                elif self.hat_data[0][1] == -1:
                    if self.before_input_hat != 4:
                        pprint.pprint("BACK(↓)")
                        self.port.write("B".encode('utf-8'))
                    self.before_input_hat = 4
                    self.hat_input = 4
                else:
                    if self.before_input_hat != 0:
                        pprint.pprint("NONE")
                        self.port.write("N".encode('utf-8'))
                    self.before_input_hat = 0
                    self.hat_input = 0

    #○×△□ボタン
    def listen_geometry(self):
        for event in pygame.event.get():
            if event.type == pygame.JOYBUTTONDOWN:
                self.button_data[event.button] = True
            elif event.type == pygame.JOYBUTTONUP:
                self.button_data[event.button] = False

            if self.button_data[0] == True:
                if self.before_input_button != 1:
                    pprint.pprint("NOT ASSIGN(×)")
                    self.port.write("X".encode('utf-8'))
                self.before_input_button = 1
                self.button_input = 1
            elif self.button_data[1] == True:
                if self.before_input_button != 2:
                    pprint.pprint("TURN RIGHT(○)")
                    self.port.write("C".encode('utf-8'))
                self.before_input_button = 2
                self.button_input = 2
            elif self.button_data[2] == True:
                if self.before_input_button != 3:
                    pprint.pprint("NOT ASSIGN(△)")
                    self.port.write("T".encode('utf-8'))
                self.before_input_button = 3
                self.button_input = 3
            elif self.button_data[3] == True:
                if self.before_input_button != 4:
                    pprint.pprint("TURN LEFT(□)")
                    self.port.write("S".encode('utf-8'))
                self.before_input_button = 4
                self.button_input = 4
            else :
                if self.before_input_button != 0:
                    pprint.pprint("NONE")
                    self.port.write("N".encode('utf-8'))
                self.before_input_button = 0
                self.button_input = 0

    #L1/L2/R1/R2ボタン
    def listen_left_right(self):
        for event in pygame.event.get():
            if event.type == pygame.JOYBUTTONDOWN:
                self.button_data[event.button] = True
            elif event.type == pygame.JOYBUTTONUP:
                self.button_data[event.button] = False

            if self.button_data[4] == True:
                if self.before_input_button != 1:
                    pprint.pprint("NOT ASSIGN(L1)")
                    self.port.write("X".encode('utf-8'))
                self.before_input_button = 1
                self.button_input = 1
            elif self.button_data[5] == True:
                if self.before_input_button != 2:
                    pprint.pprint("NOT ASSIGN(R1)")
                    self.port.write("C".encode('utf-8'))
                self.before_input_button = 2
                self.button_input = 2
            elif self.button_data[6] == True:
                if self.before_input_button != 3:
                    pprint.pprint("NOT ASSIGN(L2)")
                    self.port.write("T".encode('utf-8'))
                self.before_input_button = 3
                self.button_input = 3
            elif self.button_data[7] == True:
                if self.before_input_button != 4:
                    pprint.pprint("NOT ASSIGN(R2)")
                    self.port.write("S".encode('utf-8'))
                self.before_input_button = 4
                self.button_input = 4
            else :
                if self.before_input_button != 0:
                    pprint.pprint("NONE")
                    self.port.write("N".encode('utf-8'))
                self.before_input_button = 0
                self.button_input = 0

    #SHERE/OPTIONSボタン
    def listen_other_button(self):
        for event in pygame.event.get():
            if event.type == pygame.JOYBUTTONDOWN:
                self.button_data[event.button] = True
            elif event.type == pygame.JOYBUTTONUP:
                self.button_data[event.button] = False
            if self.button_data[8] == True:
                if self.before_input_button != 3:
                    pprint.pprint("NOT ASSIGN(SHARE)")
                    self.port.write("T".encode('utf-8'))
                self.before_input_button = 3
                self.button_input = 3
            elif self.button_data[9] == True:
                if self.before_input_button != 4:
                    pprint.pprint("NOT ASSIGN(OPTIONS)")
                    self.port.write("S".encode('utf-8'))
                self.before_input_button = 4
                self.button_input = 4
            else:
                if self.before_input_button != 0:
                    pprint.pprint("NONE")
                    self.port.write("N".encode('utf-8'))
                self.before_input_button = 0
                self.button_input = 0


    #全ボタン(アナログスティック以外)
    def listen_all_button(self):
        for event in pygame.event.get():
            if event.type == pygame.JOYHATMOTION:
                self.hat_data[event.hat] = event.value
                if self.hat_data[0][0] == 1:
                    if self.before_input_hat != 2:
                        pprint.pprint("RIGHT(→)")
                        self.port.write("R".encode('utf-8'))
                        recieve = self.port.read_all();
                        print(recieve);
                    self.before_input_hat = 2
                    self.hat_input = 2
                elif self.hat_data[0][0] == -1:
                    if self.before_input_hat != 1:
                        pprint.pprint("LEFT(←)")
                        self.port.write("L".encode('utf-8'))
                        recieve = self.port.read_all();
                        print(recieve);
                    self.before_input_hat = 1
                    self.hat_input = 1
                elif self.hat_data[0][1] == 1:
                    if self.before_input_hat != 3:
                        pprint.pprint("FRONT(↑)")
                        self.port.write("F".encode('utf-8'))
                        recieve = self.port.read_all();
                        print(recieve);
                    self.before_input_hat = 3
                    self.hat_input = 3
                elif self.hat_data[0][1] == -1:
                    if self.before_input_hat != 4:
                        pprint.pprint("BACK(↓)")
                        self.port.write("B".encode('utf-8'))
                        recieve = self.port.read_all();
                        print(recieve);
                    self.before_input_hat = 4
                    self.hat_input = 4
                else:
                    if self.before_input_hat != 0:
                        pprint.pprint("NONE")
                        self.port.write("N".encode('utf-8'))
                        recieve = self.port.read_all();
                        print(recieve);
                    self.before_input_hat = 0
                    self.hat_input = 0
                return

            elif event.type == pygame.JOYBUTTONDOWN:
                self.button_data[event.button] = True
            elif event.type == pygame.JOYBUTTONUP:
                self.button_data[event.button] = False
            if self.button_data[0] == True:
                if self.before_input_button != 5:
                    pprint.pprint("NOT ASSIGN(×)")
                    self.port.write("X".encode('utf-8'))
                    recieve = self.port.read_all();
                    print(recieve);
                self.before_input_button = 5
                self.button_input = 5
            elif self.button_data[1] == True:
                if self.before_input_button != 6:
                    pprint.pprint("TURN RIGHT(○)")
                    self.port.write("C".encode('utf-8'))
                    recieve = self.port.read_all();
                    print(recieve);
                self.before_input_button = 6
                self.button_input = 6
            elif self.button_data[2] == True:
                if self.before_input_button != 7:
                    pprint.pprint("NOT ASSIGN(△)")
                    self.port.write("T".encode('utf-8'))
                    recieve = self.port.read_all();
                    print(recieve);
                self.before_input_button = 7
                self.button_input = 7
            elif self.button_data[3] == True:
                if self.before_input_button != 8:
                    pprint.pprint("TURN LEFT(□)")
                    self.port.write("S".encode('utf-8'))
                    recieve = self.port.read_all();
                    print(recieve);
                self.before_input_button = 8
                self.button_input = 8
            elif self.button_data[4] == True:
                if self.before_input_button != 9:
                    pprint.pprint("NOT ASSIGN(L1)")
                    self.port.write("H".encode('utf-8'))
                    recieve = self.port.read_all();
                    print(recieve);
                self.before_input_button = 9
                self.button_input = 9
            elif self.button_data[5] == True:
                if self.before_input_button != 10:
                    pprint.pprint("NOT ASSIGN(R1)")
                    self.port.write("I".encode('utf-8'))
                    recieve = self.port.read_all();
                    print(recieve);
                self.before_input_button = 10
                self.button_input = 10
            elif self.button_data[6] == True:
                if self.before_input_button != 11:
                    pprint.pprint("NOT ASSIGN(L2)")
                    self.port.write("J".encode('utf-8'))
                    recieve = self.port.read_all();
                    print(recieve);
                self.before_input_button = 11
                self.button_input = 11
            elif self.button_data[7] == True:
                if self.before_input_button != 12:
                    pprint.pprint("NOT ASSIGN(R2)")
                    self.port.write("K".encode('utf-8'))
                    recieve = self.port.read_all();
                    print(recieve);
                self.before_input_button = 12
                self.button_input = 12
            elif self.button_data[8] == True:
                if self.before_input_button != 13:
                    pprint.pprint("NOT ASSIGN(SHARE)")
                    self.port.write("Y".encode('utf-8'))
                    recieve = self.port.read_all();
                    print(recieve);
                self.before_input_button = 13
                self.button_input = 13
            elif self.button_data[9] == True:
                if self.before_input_button != 14:
                    pprint.pprint("NOT ASSIGN(OPTIONS)")
                    self.port.write("Z".encode('utf-8'))
                    recieve = self.port.read_all();
                    print(recieve);
                self.before_input_button = 14
                self.button_input = 14
            else :
                if self.before_input_button != 0:
                    pprint.pprint("NONE")
                    self.port.write("N".encode('utf-8'))
                    recieve = self.port.read_all();
                    print(recieve);
                self.before_input_button = 0
                self.button_input = 0

    #アナログスティック
    def listen_axis(self):
        for event in pygame.event.get():
            if event.type == pygame.JOYAXISMOTION:
                self.axis_data[event.axis] = round(event.value, 2)
            # pprint.pprint(self.axis_data)
            # pprint.pprint(self.axis_data.get(0))
            left_right_value = self.axis_data.get(0)
            if left_right_value is None:
                left_right_value = 0
            up_down_value = self.axis_data.get(1)
            if up_down_value is None:
                up_down_value = 0

            up_down_value = int(up_down_value * 10)
            if up_down_value <= -9:
                self.port.write('A'.encode('utf-8'))
            elif up_down_value <= -8:
                self.port.write('B'.encode('utf-8'))
            elif up_down_value <= -7:
                self.port.write('C'.encode('utf-8'))
            elif up_down_value <= -6:
                self.port.write('D'.encode('utf-8'))
            elif up_down_value <= -5:
                self.port.write('E'.encode('utf-8'))
            elif up_down_value <= -4:
                self.port.write('F'.encode('utf-8'))
            elif up_down_value <= -3:
                self.port.write('G'.encode('utf-8'))
            elif up_down_value <= -2:
                self.port.write('H'.encode('utf-8'))
            elif up_down_value <= -1:
                self.port.write('I'.encode('utf-8'))
            elif up_down_value < 0:
                self.port.write('J'.encode('utf-8'))
            elif up_down_value == 0:
                self.port.write('K'.encode('utf-8'))
            elif up_down_value < 1:
                self.port.write('L'.encode('utf-8'))
            elif up_down_value < 2:
                self.port.write('M'.encode('utf-8'))
            elif up_down_value < 3:
                self.port.write('N'.encode('utf-8'))
            elif up_down_value < 4:
                self.port.write('O'.encode('utf-8'))
            elif up_down_value < 5:
                self.port.write('P'.encode('utf-8'))
            elif up_down_value < 6:
                self.port.write('Q'.encode('utf-8'))
            elif up_down_value < 7:
                self.port.write('R'.encode('utf-8'))
            elif up_down_value < 8:
                self.port.write('S'.encode('utf-8'))
            elif up_down_value < 9:
                self.port.write('T'.encode('utf-8'))
            elif up_down_value >= 9:
                self.port.write('U'.encode('utf-8'))
            # while self.port.in_waiting == 0:
            #     pass
            # recieve = self.port.read_all()
            # print(recieve)

            left_right_value = int(left_right_value * 10)
            if left_right_value <= -9:
                self.port.write('A'.encode('utf-8'))
            elif left_right_value <= -8:
                self.port.write('B'.encode('utf-8'))
            elif left_right_value <= -7:
                self.port.write('C'.encode('utf-8'))
            elif left_right_value <= -6:
                self.port.write('D'.encode('utf-8'))
            elif left_right_value <= -5:
                self.port.write('E'.encode('utf-8'))
            elif left_right_value <= -4:
                self.port.write('F'.encode('utf-8'))
            elif left_right_value <= -3:
                self.port.write('G'.encode('utf-8'))
            elif left_right_value <= -2:
                self.port.write('H'.encode('utf-8'))
            elif left_right_value <= -1:
                self.port.write('I'.encode('utf-8'))
            elif left_right_value < 0:
                self.port.write('J'.encode('utf-8'))
            elif left_right_value == 0:
                self.port.write('K'.encode('utf-8'))
            elif left_right_value < 1:
                self.port.write('L'.encode('utf-8'))
            elif left_right_value < 2:
                self.port.write('M'.encode('utf-8'))
            elif left_right_value < 3:
                self.port.write('N'.encode('utf-8'))
            elif left_right_value < 4:
                self.port.write('O'.encode('utf-8'))
            elif left_right_value < 5:
                self.port.write('P'.encode('utf-8'))
            elif left_right_value < 6:
                self.port.write('Q'.encode('utf-8'))
            elif left_right_value < 7:
                self.port.write('R'.encode('utf-8'))
            elif left_right_value < 8:
                self.port.write('S'.encode('utf-8'))
            elif left_right_value < 9:
                self.port.write('T'.encode('utf-8'))
            elif left_right_value >= 9:
                self.port.write('U'.encode('utf-8'))
            while self.port.inWaiting() == 0:
                pass
            # recieve = self.port.read_all()
            # print(recieve)
