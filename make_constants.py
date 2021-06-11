# coding=utf-8
import requests
from bs4 import BeautifulSoup

def gen_alignment_locations():
    tb = BeautifulSoup(requests.get('https://www.thonky.com/qr-code-tutorial/alignment-pattern-locations').text,'html.parser').find_all('table')[1];
    rows = tb.find_all('tr')
    print('static int qr_alignment_location [][7] =')
    print('{\n    { },')
    for i in range(1, len(rows)):
        cols = rows[i].find_all('td')
        if len(cols) < 2:
            continue
        print('    {', end = ' ')
        for j in range(1, len(cols)):
            if cols[j].string :
                print(cols[j].string, end = ',')
        print('},')
    print('};')

print('#pragma once\n\n')
print('/* gen by make_constants.py */\n\n')


def gen_capacities():
    print('/* version < EC_level < Mode */')
    print('/* Mode : Num AlphaNum Byte Kanji */')
    print('static int qr_capacities[40][4][4] = \n{')
    table =  BeautifulSoup(requests.get('https://www.thonky.com/qr-code-tutorial/character-capacities').text,'html.parser').find_all('table')[1]
    versions = table.find_all('tr') 
    span = 0
    level = "LMQH"
    for idx in range(1, len(versions)):
        version = versions[idx].find_all('td')
        if len(version) < 5:
            continue
        if span == 0:
            print('    { //v'+str(idx) + ' Num AlphaNum Byte Kanji')
        print('        {', end = ' ')
        start = 1
        if len(version) > 5:
            start = 2
        for j in range(start, len(version)):
            print(version[j].string, end = ',')
        print('}, // '+level[span])
        span = span + 1
        if span == 4: 
            print('    },')
            span = 0 
    print('};\n')

gen_capacities()

tb = BeautifulSoup(requests.get('http://www.thonky.com/qr-code-tutorial/error-correction-table').text,'html.parser').find_all('table')[1]
level = "LMQH"
p = 0;
print('typedef struct')
print('{')
print('    int ec_codeword_per_block;')
print('    int group1_blocks;')
print('    int data_codeword_per_block1;')
print('    int group2_blocks;')
print('    int data_codeword_per_block2;')
print('}qr_error_correction_parameter_t;')
print('')
print('static qr_error_correction_parameter_t qr_error_correction_parameter [40][4] =')
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
print('static int qr_remainder_bits [] = {', end = '')
for tr in tb.find_all('tr'):
    tds =  tr.find_all('td')
    if len(tds) > 1:
        print(tds[1].string+', ',end = '')
print('};')
        

gen_alignment_locations()

html = requests.get('http://www.thonky.com/qr-code-tutorial/format-version-tables').text
soup = BeautifulSoup(html,'html.parser');
tb = soup.find_all('tr')
td = soup.find_all('td')

print("")

mn = 0;
ec = 1;

print('static const char * format [] = {')
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
print('};')
