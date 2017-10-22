#!/usr/bin/env python
# coding=utf-8
import requests
from bs4 import BeautifulSoup

tb = BeautifulSoup(requests.get('http://www.thonky.com/qr-code-tutorial/error-correction-table').text,'html.parser').find_all('table')[1]



level = "LMQH"
p = 0;
print('static \nint ec_parameter [][][5]')
print("=")
print('{')
trs = tb.find_all('tr')
for j in range(1,len(trs)):
    rtds = trs[j].find_all('td')
    if p%4 == 0:
        print("    { //v%d"%((p/4)+1))
    if len(rtds) != 1:
        p = p+1;
        print("        {",end='')
        for i in range(2,len(rtds)-1):
            cell = rtds[i].string;
            if cell == None:
                cell = '0'
            print('%3s'%cell,end='')
            if i != 6:
                print(',',end='')
            print('',end='')
        print("}",end='')
        if p%4 !=  0:
            print(',',end = '')
        else:
            print(' ',end = '')
        print('// level '+level[p%4-1])
    if p%4 == 0:
        print("    }",end='')
        if p/4+0.5<40:
            print(',')
        else:
            print(' ')

print('};')

