# coding:utf-8
import controller
import serial
from time import sleep


class Raspi():
    def __init__(self):
        self.controller = controller.PS4Controller()

    def loop(self):
        # コントローラ入力読み込み
        self.controller.listen_all_button()
        # self.controller.listen_hat()
        # if self.controller.hat_input == 0:
        # self.controller.listen_geometry()
        self.controller.listen_axis()

    def __del__(self):
        del self.controller
        self.port.close()


if __name__ == "__main__":
    raspi = Raspi()
    try:
        while True:
            raspi.loop()
            # sleep(0.1)
    except KeyboardInterrupt:
        pass
    finally :
        del raspi
