#!/usr/bin/env python
import datetime, time
import socket
from IPy import IP
import subprocess


print (time.ctime(),"startup!")

TCP_IP = '192.168.0.145'
TCP_PORT = 9999
BUFFER_SIZE = 20  # Normally 1024, but we want fast response

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1) #make socket reuseable, for debugging (enables you to rerun the program before the socket has timed out)
s.bind((TCP_IP, TCP_PORT))
s.listen(1)


print('Opening TCP port', TCP_PORT)

while True:
    try:
        conn, addr = s.accept()
        print(time.ctime(),'Connection from:', addr)
        #ask sardukar about disk usage
        sardukarDiskUsage = subprocess.check_output("ssh robotto@sardukar.moore.dk -p 222 df | grep 'vg00-root' | awk '{print $5}'", shell=True)
        sardukarPercent = int(sardukarDiskUsage.strip().replace("%", ""))
        print('Sardukar:',sardukarPercent)

        # read rocksteady's disk usage from file:
        with open('rocksteadyReport', 'r') as rocksteadyReportFile:
            for line in rocksteadyReportFile:
                rocksteadyPercent = int(line.strip().replace("%", ""))
                print('Rocksteady:',rocksteadyPercent)

        #txString = str(22)
        txString = str(sardukarPercent)+,str(rocksteadyPercent)+'\r'
        conn.send(txString)#+'\n')  # echo
        print ('TX: ', txString) #txString.replace('\r','\r\n')
        print ('Client disconnected.')
        print ('--------------------------')
        conn.close()
    except Exception as e:
        #print "hmm.. It looks like there was an error: " + str(e)
        print ('Client disconnected... :',str(e))
        print ('--------------------------')
        conn.close()
