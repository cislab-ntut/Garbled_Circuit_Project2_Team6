import random
import json
import hashlib

# k的值
_BITSIZE = 2
# label的長度
_LABELSIZE = 8
# 亂數種子
randomSeed = ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',\
              'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z']

def circuit():
    # 讀電路檔案
    infile = open('circuit.json', 'r')
    r = infile.readline()
    infile.close()
    dic = json.loads(r)
    C_in = dic['input']
    C_mid = dic['mid']
    C_out = dic['output']
    return C_in, C_mid, C_out

def create_gate_lable(C_in, C_mid, C_out):
    result = []
    
    # gate數量
    size = len(C_in) + len(C_mid) + len(C_out)

    # 產生所有gate的0,1標籤
    for i in range(size):
        one = ''
        zero = ''
        while(len(one) < _LABELSIZE):
            one += str(random.choice(randomSeed))
        while(len(zero) < _LABELSIZE):
            zero += str(random.choice(randomSeed))
        result.append([zero, one]) # gate 的 0和1
    
    return result

def create_input_lable():
    result = []

    # 產生所有input的0,1標籤
    for i in range(_BITSIZE * 2):
        one = ''
        zero = ''
        while(len(one) < _LABELSIZE):
            one += str(random.choice(randomSeed))
        while(len(zero) < _LABELSIZE):
            zero += str(random.choice(randomSeed))
        result.append([zero, one]) # g0 的 0和1
        
    return result

def create_table(lable, C_in, C_mid, C_out):
    result = {}

    for c in C_in:
        result[c[0]] = find_truthtable(lable, c)

    for c in C_mid:
        result[c[0]] = find_truthtable(lable, c)

    for c in C_out:
        result[c[0]] = find_truthtable(lable, c)
