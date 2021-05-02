import interface
dir=['f','f','f','d','f','r','l','r','b','f']
def main():
    interf = interface.interface()
    interf.send_dir(dir[0])
    for i in range(1,10):#1~9
        interf.pass_msg()
        if i==3 or i==7 or i==9:
            uid=interf.get_UID()
            print(uid)
        interf.send_dir(dir[i])        

if __name__ == '__main__':
    main()

