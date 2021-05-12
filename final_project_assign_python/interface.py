import shortest_path_floyd as spf
import bluetooth as BT
import shortest_path_floyd as spf
import score
from time import sleep
maze_path = "data/final_map_109.csv"
dir=[['','f','b','l','r'],['','l','r','b','f'],['','b','f','r','l'],['','r','l','f','b']];
all_path=[]
# hint: You may design additional functions to execute the input command, which will be helpful when debugging :)

class interface:
    def __init__(self):
        # print("")
        # print("Arduino Bluetooth Connect Program.")
        # print("")
        all_path_length=0
        maze = spf.shortest_path_floyd(maze_path)
        self.ser = BT.bluetooth()
        port = "com17"
        while(not self.ser.do_connect(port)):
            if(port == "quit"):
                self.ser.disconnect()
                quit()
            port = "com17"
        input("Press enter to start.")
        self.ser.write('s')
        now = 1
        treasure = maze.treasure
        front=0
        while True:
            maze.remove_treasure(now)
            if len(maze.treasure)==0:
                print("You have found everything!!!")
                break
            min_dist = 99999
            min_node = 0
            for t in treasure:
                if maze.distance[now][t] < min_dist:
                    min_dist = maze.distance[now][t]
                    min_node = t
            path=maze.path_reconstruction(now,min_node)
            p=1
            while(now!=min_node):

                if now==1:
                    way=maze.get_way(now,path[p])
                else:
                    way=maze.get_way(now,path[p])
                    all_path.append(dir[front][way])
                now=path[p]
                p+=1
                if dir[front][way]=='r' :
                    front=(front+1)%4
                elif dir[front][way]=='l':
                    front=(front-1)%4
                elif dir[front][way]=='b':
                    front=(front+2)%4
            all_path_length+=(p-1)
        str_apl=str(all_path_length-1)
        self.ser.write(str_apl[::-1])
        for i in all_path:
            sleep(0.005)
            self.send_action(i)
        self.ser.write('s')

    def get_UID(self):
        return self.ser.SerialReadByte()

    def send_action(self,dirc):
        #print(dirc)
        self.ser.write(dirc)
        return
    def get_i(self):
        while True:
            msg=self.ser.readByte()
            print(msg)
            if msg=='i':#pass
                break
    def end_process(self):
        self.ser.write('e')
        self.ser.disconnect()