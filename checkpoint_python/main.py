import interface
dir=['f','f','f','d','f','f','r','l','r','b','f']
def main():
    interf = interface.interface()
    interf.send_dir(dir[0])
    for i in range(1,11):
        if i==5 or i==9 :
            interf.get_i()
            uid=interf.get_UID()
            print(uid)
            interf.send_dir(dir[i]) 
        else:
            interf.get_p() 
            interf.send_dir(dir[i])      
    interf.get_i()
    uid=interf.get_UID()
    print(uid)
if __name__ == '__main__':
    main()

