import interface
#dir=['f','f','f','d','f','f','r','l','r','b','f']
dir=['l','l','b']
def main():
    interf = interface.interface()
    interf.send_dir(dir[0])
    interf.get_p()
    interf.send_dir(dir[1])
    interf.get_p()
    interf.send_dir(dir[2])
    interf.get_i()
    uid=interf.get_UID()
    print(uid)
    '''
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
    print(uid)'''
if __name__ == '__main__':
    main()