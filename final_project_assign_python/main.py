import shortest_path_floyd as spf
import bluetooth as BT
import score
import interface
import threading
import sys
import threading
point=None
def start_socket():
    global point
    point=score.Scoreboard("data/UID.csv","Team 3 ")
def main():
    # point = score.Scoreboard("data/UID.csv", "team_3")
    thread=threading.Thread(target=start_socket)
    thread.start()
    interf = interface.interface()
    for i in range(12):
        interf.get_i()
        uid=interf.get_UID()
        print(uid)
        point.add_UID(uid)
        print(point.getCurrentScore())  
if __name__ == '__main__':
    main()

