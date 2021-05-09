import pandas
import numpy as np
import requests
import socketio
import sys


# ================================================================
# Scoreboard
#   add_UID(UID_str)
#     UID_str : UID string ("0x" excluded)
#   getCurrentScore()
#     return current score (int)
# ================================================================


class Scoreboard:
    '''
    The Scoreboard class connects to the server socket and enables updating score by sending UID.
    '''
    def __init__(self, filepath, teamName, host="http://192.168.50.14:3000"):

        # INIT VARIABLES

        self.totalScore = 0
        self.team = teamName
        self.game = 0
        self.ip = host
        #self.ip = 'https://creative.ntuee.org'

        print(f"{self.team} wants to play!")
        print(f"connecting to server......{self.ip}")

        # CONNECT WITH SERVER SOCKET

        # create socket.io instance and connect to server
        self.socket = Socket(self.ip)

        # GET INFORMATION FROM SERVER
        
        res = requests.get(self.ip + '/game_status')
        if res.status_code == 404:
            raise ConnectionError('Response[404] from /game_status')
        playing_team = res.json()['current_team']
        if playing_team is None:
            print("Game is starting.....")
            self.socket.start_game({ 
                'gamemode': self.game, 
                'team': self.team })
        elif playing_team != teamName:
            self.socket.disconnect()
            raise ConnectionError(f"{g.json()['current_team']} is current playing.\nPlease wait {g.json()['time_remain']} seconds for retry.")                

    def add_UID(self, UID_str):
        '''Send {UID_str} to server to update score. Returns nothing.'''
        UID_type = type(UID_str)
        UID_len = len(UID_str)
        print("In add_UID, UID = {}".format(UID_str))
        if(UID_type != type('')):
            print("    UID type error! (Your type: {})".format(UID_type.__name__))
        if(UID_len != 8):
            print("    UID length error! (Your length: {})".format(UID_len))
        self.socket.add_UID({'uid_str': UID_str})

    def getCurrentScore(self):
        try:
            r = requests.get(self.ip+"/current_score")
            return r.json()['current_score']
        except:
            print("Failed to fetch current score")
            return None
        # return int(self.totalScore)


# ================================================================
# Socket interface
#   mySocket = Socket(ip)
# ================================================================


class Socket(socketio.ClientNamespace):
    sio = socketio.Client()

    def __init__(self, ip):
        #***********************#
        # pass in the namespace '/'
        # https://python-socketio.readthedocs.io/en/latest/client.html#class-based-namespaces
        #***********************#
        super().__init__('/')
        self.ip = ip
        try:
            self.sio.connect(self.ip)
        except Exception as e:
            print('Failed to connect to server, ', e)
            sys.exit(1)
        self.sio.register_namespace(self)

    def on_connect(self):
        print("connected")
    
    def on_invalid_mode(self):
        print("Error:invalid gamemode!!")

    def on_game_end(self, data=None):
        print("game_end")
        self.disconnect()

    def on_game_started(self, data):
        print("Game started!")
        print("Playing game as\nTeam: {}\nMode: {}".format(
            data['current_team'], data['gamemode']))
        print("Please checkout {} for more information.".format(self.ip))

    def on_UID_added(self, message):
        print(message)

    def start_game(self, gamenum):
        self.emit("start_game", gamenum)

    def add_UID(self, UID_str):
        self.emit("add_UID", UID_str)
         
    #secret backdoor for TA
    def stop_game(self):
        self.emit("stop_game")


if __name__ == '__main__':
    myScoreboard = Scoreboard(None,'Test')
    myScoreboard.add_UID("D0B373A2")
    print(myScoreboard.getCurrentScore())