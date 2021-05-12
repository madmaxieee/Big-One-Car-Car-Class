import shortest_path_floyd as spf
maze_path = "data/final_map_109.csv"
dir = [['', 'f', 'b', 'l', 'r'], ['', 'l', 'r', 'b', 'f'],
       ['', 'b', 'f', 'r', 'l'], ['', 'r', 'l', 'f', 'b']]
all_path = []
# hint: You may design additional functions to execute the input command, which will be helpful when debugging :)


def main():
    # print("")
    # print("Arduino Bluetooth Connect Program.")
    # print("")
    all_path_length = 0
    maze = spf.shortest_path_floyd(maze_path)
    now = 1
    treasure = maze.treasure
    front = 0
    while True:
        maze.remove_treasure(now)
        if len(maze.treasure) == 0:
            print("You have found everything!!!")
            break
        min_dist = 99999
        min_node = 0
        for t in treasure:
            if maze.distance[now][t] < min_dist:
                min_dist = maze.distance[now][t]
                min_node = t
        path = maze.path_reconstruction(now, min_node)
        p = 1
        while(now != min_node):

            if now == 1:
                way = maze.get_way(now, path[p])
            else:
                way = maze.get_way(now, path[p])
                all_path.append(dir[front][way])
                print(dir[front][way])
            now = path[p]
            p += 1
            if dir[front][way] == 'r':
                front = (front+1) % 4
            elif dir[front][way] == 'l':
                front = (front-1) % 4
            elif dir[front][way] == 'b':
                front = (front+2) % 4
        all_path_length += (p-1)
if __name__ == '__main__':
    main()