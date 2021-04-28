import shortest_path_floyd as spf
import bluetooth as BT
import threading
maze_path = "small_maze.csv"
dir=[['','f','b','l','r'],['','l','r','b','f'],['','b','f','r','l'],['','r','l','f','b']];
def main():
    maze = spf.shortest_path_floyd(maze_path)
    # for i in range(1,maze.n_of_node+1):
    #     for j in range(1,maze.n_of_node+1):
    #         print(maze.distance[i][j],end = ' ')
    #     print('\n')
    bt=BT.bluetooth("COM6")
    while not bt.is_open(): pass
    print("BT Connected!")
    now = 1
    treasure = maze.treasure
    last=0
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
        #print(min_node)
        path=maze.path_reconstruction(now,min_node)
        #print(path)
        p=1
        while(now!=min_node):
            way=maze.get_way(now,path[p])
            #print(way)
            #print(dir[front][way])
            bt.write(dir[front][way])
            while True:
                msg=bt.readByte()
                if msg!=None:
                    print(msg)
                if msg=="p":#pass
                    break
            now=path[p]
            p+=1
            if dir[front][way]=='r' :
                front=(front+1)%4
            elif dir[front][way]=='l':
                front=(front-1)%4
            elif dir[front][way]=='b':
                front=(front+2)%4
if __name__ == '__main__':
    main()
