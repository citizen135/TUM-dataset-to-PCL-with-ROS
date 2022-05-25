#!/usr/bin/env python3
import pandas as pd

path='~/data/rgbd_dataset_freiburg2_desk/'
ground = pd.read_csv(path+'groundtruth.txt',sep=" ")
depth = pd.read_csv(path+'depth.txt',sep=" ")
rgb = pd.read_csv(path+'rgb.txt',sep=" ")

merge=pd.merge(ground,depth,how='outer',on='timestamp')
merge=pd.merge(merge,rgb,how='outer',on='timestamp')
merge=merge.sort_values(by='timestamp')
merge.to_csv(path+'associate.csv')
merge=merge.fillna(method='backfill')
merge=merge.drop_duplicates(['depthfile'])
merge=merge.drop_duplicates(subset=['tx','qx'])
list1=[]
for i in range(len(merge)-1,1,-1):
    if (merge.iloc[i]['timestamp']-merge.iloc[i-1]['timestamp'])>0.2:
        list1.append(i)
for i in list1:
    merge=merge.drop(merge.index[i-1])
merge.to_csv(path+'fill.csv')

