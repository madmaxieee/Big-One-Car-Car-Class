import shortest_path_floyd as spf
import bluetooth as BT
import score
import interface
import threading
import sys
maze_path = "data/small_maze.csv"
dir=[['','f','b','l','r'],['','l','r','b','f'],['','b','f','r','l'],['','r','l','f','b']];
def main():
    maze = spf.shortest_path_floyd(maze_path)
    point = score.Scoreboard("data/UID.csv", "team_3")
    interf = interface.interface()
    now = 1
    treasure = maze.treasure
    front=0
    while True:
        maze.remove_treasure(now)
        if len(maze.treasure)==0:
            print("You have found everything!!!")
            interf.end_process()
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
            way=maze.get_way(now,path[p])
            interf.send_action(dir[front][way])
            interf.pass_msg()
            now=path[p]
            p+=1
            if dir[front][way]=='r' :
                front=(front+1)%4
            elif dir[front][way]=='l':
                front=(front-1)%4
            elif dir[front][way]=='b':
                front=(front+2)%4
        uid=interf.get_UID()
        point.add_UID(uid)
        print(point.getCurrentScore())  

if __name__ == '__main__':
    main()

