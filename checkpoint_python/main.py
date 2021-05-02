import interface
dir=['f','f','f','d','f','r','l','r','b','f']
def main():
    interf = interface.interface()
    interf.send_dir(dir[0])
    for i in range(0,9):
        if i==3 or i==7 or i==9:
            interf.get_i()
            uid=interf.get_UID()
            print(uid)
            interf.send_dir(dir[i+1]) 
        else:
            interf.get_p() 
            interf.send_dir(dir[i+1])      

if __name__ == '__main__':
    main()

