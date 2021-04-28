import shortest_path_floyd as spf
import bluetooth as BT
import threading
maze_path = "small_maze.csv"
dir=[['','f','r','b','l'],['','l','f','r','b'],['','b','l','f','r'],['','r','b','l','f']];
def main():
    maze = spf.shortest_path_floyd(maze_path)
    bt=BT.bluetooth("COM6")
    while not bt.is_open(): pass
    print("BT Connected!")
    
    now = 1
    treasure = maze.treasure
    last=0
    front=0
    while True:
        maze.remove_treasure(now)
        min_dist = 9999
        min_node = 0
        for t in treasure:
            if maze.distance[now][t] < min_dist:
                min_dist = maze.distance[now][t]
                min_node = t
        path=maze.path_reconstruction(now,min_node)
        p=1

        while(now!=min_node):
            way=maze.get_way(now,path[p])
            bt.write(dir[front][way])
            while True:
                msg=bt.readByte()
                print("aaa")
                #if msg!=None:
                print(msg)
                if msg=="p":#pass
                    break
            now=path[p]
            p+=1
            if way==2 :
                front=(front+1)%4
            elif way==4:
                front=(front-1)%4
            elif way==3:
                front=(front+2)%4
if __name__ == '__main__':
    main()
