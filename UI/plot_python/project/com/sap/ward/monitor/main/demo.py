# coding:utf-8
import matplotlib.pyplot as plt 
from datetime import datetime
import matplotlib.dates as mdate
import time  
import os

RECORD_NAME = "05150844-kichen-hermetic-after-24h-partly"
# ROOM = "slaveBedroom"
# ROOM = "mainBedroom"
# ROOM = "livingRoom"
ROOM = "kitchen"

AVERAGE = 10

PLOT_PYTHON_PATH = os.path.abspath('../../../../../../')
RECORD_TXT_FILE_NAME = PLOT_PYTHON_PATH + "\\records" + "\\" + ROOM + "\\" + RECORD_NAME + ".TXT"
PICTURE_PNG_FILE_NAME = PLOT_PYTHON_PATH + "\\pictures" + "\\" + ROOM + "\\" + RECORD_NAME + "-p" + str(AVERAGE) + ".png" 
# print(RECORD_PATH)
 
num = []
date_time = []
hcho = []
temp = []
lux = []
sum_lux = 0
sum_hcho = 0
sum_temp = 0
index = 0
security_hcho = []
SECURITY_HCHO = 0.08
 
f = open(RECORD_TXT_FILE_NAME)  
line = f.readline()  
while line:  
    index = index + 1
    print(line)  
    line = f.readline() 
    lineData = line.split(',') 
    if(len(lineData) > 3):
        if(lineData[0].isdigit()):
            sum_hcho += float(lineData[2])
            sum_temp += float(lineData[4])
            sum_lux += float(lineData[6])
#             print(lineData[0],lineData[2],lineData[4])
            if(0 == index % AVERAGE):
                security_hcho.append(SECURITY_HCHO)
                # Formatter time
                timeArray = time.strptime(lineData[1], "%Y-%m-%d %H:%M:%S")  
                timestamp = time.mktime(timeArray)  # 时间戳  
                date_time.append(datetime.fromtimestamp(timestamp))  # Time
                num.append(index / AVERAGE)  # Number
                hcho.append(round((sum_hcho / float(AVERAGE)), 2))  # Hcho
                temp.append(round((sum_temp / float(AVERAGE)), 2))  # Temperature
                lux.append(round((sum_lux / float(AVERAGE)), 2))  # Lux
                sum_hcho = 0  
                sum_temp = 0
                sum_lux = 0
print(num)
print(hcho)
print(temp)
print(lux)
f.close() 
 
#### Plot 1
plt.subplot(2, 1, 1)
plt.title('Environment Monitor', size=14, color='green')  
plt.xlabel('Time', size=12)  
plt.ylabel('Temperature & HCHO', size=12)
plt.grid()  
 
plt.plot(date_time, temp, color='b', linestyle='-', label='Temperature')  
# ax2.plot(date_time, lux, color='r', linestyle='-', label='Lux')  
plt.legend(loc='lower left')
 
# 设置X轴的坐标刻度线显示间隔
plt.gca().xaxis.set_tick_params(rotation=45, labelsize=12)  
plt.gca().xaxis.set_major_formatter(mdate.DateFormatter('%m-%d %H:%M:%S'))  # 设置时间标签显示格式
 
# Right y 
ax2 = plt.twinx()  # this is the important function
ax2.plot(date_time, security_hcho, color='g', linestyle='--', label='Security HCHO') 
ax2.plot(date_time, hcho, color='g', linestyle='-', label='HCHO')  
ax2.legend(loc='lower right')  
 
# 设置X轴的坐标刻度线显示间隔
ax2.xaxis.set_tick_params(rotation=45, labelsize=12)  
ax2.xaxis.set_major_formatter(mdate.DateFormatter('%m-%d %H:%M:%S'))  # 设置时间标签显示格式
 
#### Plot 2
plt.subplot(2, 1, 2)
plt.xlabel('Time', size=12)  
plt.ylabel('Temperature & Lux', size=12)
plt.grid()  
 
plt.plot(date_time, temp, color='b', linestyle='-', label='Temperature')  
# ax2.plot(date_time, lux, color='r', linestyle='-', label='Lux')  
plt.legend(loc='lower left')
 
# 设置X轴的坐标刻度线显示间隔
plt.gca().xaxis.set_tick_params(rotation=45, labelsize=12)  
plt.gca().xaxis.set_major_formatter(mdate.DateFormatter('%m-%d %H:%M:%S'))  # 设置时间标签显示格式
 
# Right y 
ax2 = plt.twinx()  # this is the important function
ax2.plot(date_time, lux, color='r', linestyle='-', label='Lux')  
ax2.legend(loc='lower right')  
 
# 设置X轴的坐标刻度线显示间隔
ax2.xaxis.set_tick_params(rotation=45, labelsize=12)  
ax2.xaxis.set_major_formatter(mdate.DateFormatter('%m-%d %H:%M:%S'))  # 设置时间标签显示格式

plt.gcf().set_size_inches(18.5, 10.5)
plt.savefig(PICTURE_PNG_FILE_NAME, format='png', dpi=300)   
print("ploting*****************************")
plt.show() 
