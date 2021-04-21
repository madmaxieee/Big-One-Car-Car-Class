import shortest_path_floyd as spf
maze_path = "data/maze.csv"


def main():
    maze = spf(maze_path)
    now = 1
    treasure = maze.treasure
    while True:
        maze.remove_treasure(now)
        min_dist = 9999
        min_node = 0
        for t in treasure:
            if maze.distance[now][t] < min_dist:
                min_dist = maze.distance[now][t]
                min_node = t
        path=maze.path_reconstruction(now,min_node)
        now=min_node


if __name__ == '__main__':
    main()
