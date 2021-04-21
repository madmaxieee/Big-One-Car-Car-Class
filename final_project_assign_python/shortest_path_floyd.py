import csv
import pandas
import numpy as np
import math

class shortest_path_floyd():

    def __init__(self, file_path):
        # get how many nodes in the map
        reader=csv.reader(open(file_path))
        n_of_node=len(list(reader))
        #initialize the distance matrix
        self.distance=np.zeros((n_of_node,n_of_node),dtype=np.int)
        self.neighbor=np.zeros((n_of_node,5),dtype=np.int)#0 for up/1 for right/2 for down/3 for left
        self.next=np.zeros((n_of_node,n_of_node),dtype=np.int)
        self.treasure=list()
        for i in range(1,n_of_node+1):
            for j in range(1,n_of_node+1):
                if i!=j:
                    self.distance[i][j]=9999
        #read csv file
        csv_raw_data = pandas.read_csv(file_path).values
        self.initialize_maze_with_csv_raw_data(csv_raw_data)

    def initialize_maze_with_csv_raw_data(self, csv_raw_data):
        # process csv_raw_data:
        # for each csv_data_row,
        #     csv_data_row[0] is the node index;
        #     csv_data_row[i] is, if exists, the index of the neighboring node to the ith direction, with 1 <= i <= 4.
        
        for csv_data_row in csv_raw_data:
            n1=csv_data_row[0]
            cnt=0
            for i in range(1,5):
                if not math.isnan(csv_data_row[i]):
                    cnt+=1
                    n2=csv_data_row[i]
                    self.neighbor[n1][i]=n2
                    self.distance[n1][n2]=csv_data_row[i+4]
                    self.next[n1][n2]=n2
                else:
                    self.neighbor[n1][i]=0
            if cnt==1:
                self.treasure.append(n1)
        floyd()
    def floyd(self):
        for k in range(1,self.n_of_node+1):
            for i in range(1,self.n_of_node+1):
                for j in range(i,self.n_of_node+1):
                    if self.distance[i][j]>self.distance[i][k]+self.distance[j][k]:
                        self.distance[i][j]=self.distance[i][k]+self.distance[j][k]
                        self.next[i][j]=self.next[i][k]
    def path_reconstruction(self,start,end):
        path=[start]
        nxt=start
        while nxt!=end:
            this=nxt
            nxt=self.next[nxt][end]
            for i in range(1,5):
                if self.neighbor[this][i]==nxt:
                    path.append(i)
        return path
    def remove_treasure(self,idx):
        self.treasure.remove(idx)
#to do
#轉彎方向轉換
#和bluetooth結合
#github desktop