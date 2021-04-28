import threading
import time
from time import sleep
import sys
import serial

class bluetooth:
    def __init__(self, port: str, baudrate: int=9600):
        """ Initialize an BT object, and auto-connect it. """
        # The port name is the name shown in control panel
        # And the baudrate is the communication setting, default value of HC-05 is 9600.
        self.ser = serial.Serial(port, baudrate=baudrate)
        
    def is_open(self) -> bool:
        return self.ser.is_open

    def waiting(self) -> bool:
        return self.ser.in_waiting

    def do_connect(self, port: str, baudrate: int=9600) -> bool:
        """ Connect to the specify port with particular baudrate """
        # Connection function. Disconnect the previous communication, specify a new one.
        self.disconnect()

        try:
            self.ser = serial.Serial(port, baudrate=baudrate)
            return True
        except:
            return False

    def disconnect(self):
        """ Close the connection. """
        self.ser.close()

    def write(self, output: str):
        # Write the byte to the output buffer, encoded by utf-8.
        send = output.encode("utf-8")
        self.ser.write(send)

    def readByte(self):
        # Scan the input buffer until meet a '\n'. return none if doesn't exist.
        sleep(0.05)
        waiting = self.ser.inWaiting()
        print(waiting)
        if waiting >= 0:
            rv = self.ser.read(1).decode("utf-8") 
            return rv
        return ""

def read():
    while True:
        if bt.waiting():
            print(bt.readString())

def write():
    while True:
        msgWrite = input()
        
        if msgWrite == "exit": sys.exit()
    
        bt.write(msgWrite + "\n")

if __name__ == "__main__":
    # Please modify the port name.
    bt = bluetooth("COM8")
    while not bt.is_open(): pass
    print("BT Connected!")

    readThread = threading.Thread(target=read)
    readThread.setDaemon(True)
    readThread.start()

    while True:
        msgWrite = input()
        if msgWrite == "exit": sys.exit()
        bt.write(msgWrite)
