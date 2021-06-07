#!/usr/bin/env python
# coding=utf-8
import requests
from bs4 import BeautifulSoup


print('#pragma once\n\n')
print('/* 脚本自动生成的表格 */\n\n')


def gen_capacities():
    
    print('/* 每个版本四个纠错等级四个模式的容量 */')
    print('static int qr_capacities[][][][4] = \n{\n')
    table =  BeautifulSoup(requests.get('https://www.thonky.com/qr-code-tutorial/character-capacities').text,'html.parser').find_all('table')[1]
    print(table)
    rows = table.find_all('tr') 
    for r in range(1, len(rows)):
        print('{')
        row = rows[r]
        cols = row.find_all('td')
        for c in range(1, len(cols)):
            col = cols[c]
            print(col, end = ' ')
        print('},')
    print('};\n')

gen_capacities()

tb = BeautifulSoup(requests.get('http://www.thonky.com/qr-code-tutorial/error-correction-table').text,'html.parser').find_all('table')[1]
level = "LMQH"
p = 0;
print('static int qr_error_correction_parameter [][][5] =')
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


tb = BeautifulSoup(requests.get('http://www.thonky.com/qr-code-tutorial/structure-final-message').text,'html.parser').find_all('table')[5];
print('int a = {')
for tr in tb.find_all('tr'):
    tds =  tr.find_all('td')
    if len(tds) > 1:
        print(tds[1].string+', ',end = '')
print('};')
        

html = requests.get('http://www.thonky.com/qr-code-tutorial/format-version-tables').text
soup = BeautifulSoup(html,'html.parser');
tb = soup.find_all('tr')
td = soup.find_all('td')

print("")

mn = 0;
ec = 1;
for i in range(2,(32)*3+2):
    if ((i-1)%3==0):
        print("        \"%s\","%(td[i].string))
        mn = mn+1
        if mn%8 == 0:
            ec = ec +1
            print("")
v = 7
for td in tb:
    tr = td.find_all('td')
    if  len(tr) > 1 and tr[1].string :
        #print(tr[0].string, end = ', ')
        print("    \"%s\", // v%d"%(tr[1].string,v))
        v = v+1
