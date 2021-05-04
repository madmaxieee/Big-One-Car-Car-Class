import interface
def main():
    interf = interface.interface()
    for i in range(3):
        print("RFID!!!!!!")
        interf.get_i()
        uid=interf.get_UID()
        print(uid)
if __name__ == '__main__':
    main()