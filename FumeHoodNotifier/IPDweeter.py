#!/usr/bin/python
from datetime import datetime
import time
import subprocess
import dweepy
import socket
from apscheduler.schedulers.blocking import BlockingScheduler

def connect_type(word_list):
    """ This function takes a list of words, then, depeding which key word, returns the corresponding
    internet connection type as a string. ie) 'ethernet'.
    """
    if 'wlan0' in word_list or 'wlan1' in word_list:
        con_type = 'wifi'
    elif 'eth0' in word_list:
        con_type = 'ethernet'
    else:
        con_type = 'current'

    return con_type

def dweetIP():
    arg='ip route list'  # Linux command to retrieve ip addresses.
    # Runs 'arg' in a 'hidden terminal'.
    p=subprocess.Popen(arg,shell=True,stdout=subprocess.PIPE)
    data = p.communicate()  # Get data from 'p terminal'.

    # Split IP text block into three, and divide the two containing IPs into words.
    ip_lines = data[0].splitlines()
    split_line_a = ip_lines[1].split()

    # con_type variables for the message text. ex) 'ethernet', 'wifi', etc.
    ip_type_a = connect_type(split_line_a)

    """Because the text 'src' is always followed by an ip address,
    we can use the 'index' function to find 'src' and add one to
    get the index position of our ip.
    """
    ipaddr_a = split_line_a[split_line_a.index('src')+1]

    # my_ip_a = 'Your %s ip is %s' % (ip_type_a, ipaddr_a)
    # print my_ip_a

    dweepy.dweet_for(socket.gethostname(), {'ip': ipaddr_a})

if __name__ == '__main__':
    dweetIP()
    scheduler = BlockingScheduler()
    scheduler.add_job(dweetIP, 'interval', hours=1)
    # print('Press Ctrl+C to exit')

    try:
        scheduler.start()
    except (KeyboardInterrupt, SystemExit):
        pass
