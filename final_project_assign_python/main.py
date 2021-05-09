import shortest_path_floyd as spf
import bluetooth as BT
import score_local
import interface
import threading
import sys
def main():
    point = score_local.Scoreboard("data/UID.csv", "team_3")
    interf = interface.interface()
    for i in range(11):
        interf.get_i()
        uid=interf.get_UID()
        print(uid)
    # point.add_UID(uid)
    # print(point.getCurrentScore())  


if __name__ == '__main__':
    main()

