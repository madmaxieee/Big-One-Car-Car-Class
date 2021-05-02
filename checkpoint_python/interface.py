import bluetooth as BT

class interface:
    def __init__(self):
        print("")
        print("Arduino Bluetooth Connect Program.")
        print("")
        self.ser = BT.bluetooth()
        port = input("PC bluetooth port name: ")
        while(not self.ser.do_connect(port)):
            if(port == "quit"):
                self.ser.disconnect()
                quit()
            port = input("PC bluetooth port name: ")
        input("Press enter to start.")
        self.ser.write('s')

    def get_UID(self):
        return self.ser.SerialReadByte()
    def pass_msg(self):
        while True:
            msg=self.ser.readByte()
            print(msg)
            if msg=='P':#pass
                break
    def end_process(self):
        self.ser.write('e')
        self.ser.disconnect()