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
        result[c[0]] = find_truthtable(lable, c)
        
    return result

def find_truthtable(lable, c):
    temp = []
    
    # 建立gate的input
    temp.append([lable[c[1][0]][0], lable[c[1][1]][0]])
    temp.append([lable[c[1][0]][0], lable[c[1][1]][1]])
    temp.append([lable[c[1][0]][1], lable[c[1][1]][0]])
    temp.append([lable[c[1][0]][1], lable[c[1][1]][1]])

    # 建立gate的table
    lableID = c[0] + _BITSIZE * 2
    for i in range(4):
        if i in c[2]:
            temp[i] = [hash(str([encrypt(temp[i][0]), encrypt(temp[i][1]),\
                                 lable[lableID][1]])), lable[lableID][1]]
        else:
            temp[i] = [hash(str([encrypt(temp[i][0]), encrypt(temp[i][1]),\
                                 lable[lableID][0]])), lable[lableID][0]]

    # 打亂table
    random.shuffle(temp)

    return temp

# 計算hash
def get_hash(I1, I2, O):
    return hash(str([encrypt(I1), encrypt(I2), O]))

# 加密
def encrypt(s):
  m = hashlib.md5()
  m.update(s.encode('utf-8'))
  return m.hexdigest()

# 驗證答案
def decide(a, b, t):
    for i in range(4):
        temp = get_hash(a, b, t[i][1])
        if temp == t[i][0]:
            print(t[i][1])
            return t[i][1]


# 計算結果  
def calculate(values, C_in, C_mid, C_out, table, lable):

    # input數量
    size = _BITSIZE * 2
    # 輸入的值加上gate輸出的值
    values = values + ([0] * (len(C_in) + len(C_mid) + len(C_out)))
    
    # 完成 IN
    for c in C_in:
        print('GateID：' + str(c[0]) + ' --->', end=' ')
        # 找出上面的值
        a = values[c[1][0]]
            
        # 找出下面的值
        b = values[c[1][1]]

        # 算出輸出值
        t = table[c[0]]
        
        # 放回去values
        values[size + c[0]] = decide(a, b, t)

        # 完成 MID
    for c in C_mid:
        print('GateID：' + str(c[0]) + ' --->', end=' ')
        # 找出上面的值
        a = values[c[1][0]]
            
        # 找出下面的值
        b = values[c[1][1]]

        # 算出輸出值
        t = table[c[0]]
        
        # 放回去values
        values[size + c[0]] = decide(a, b, t)

    # 完成 OUT
    answer = ''
    for c in C_out:
        print('GateID：' + str(c[0]) + ' --->', end=' ')
        # 找出上面的值
        a = values[c[1][0]]
            
        # 找出下面的值
        b = values[c[1][1]]

        # 算出輸出值
        t = table[c[0]]

        # 放回去values
        tempAns = decide(a, b, t)
        if tempAns == lable[size + c[0]][0]:
            answer += '0'
        else:
            answer += '1'

    return answer

def main():
    C_in, C_mid, C_out = circuit()
    input_lable = create_input_lable()
    gate_lable = create_gate_lable(C_in, C_mid, C_out)
    lable = input_lable + gate_lable
    table = create_table(lable, C_in, C_mid, C_out)
    
    print(lable)

    print('\n' + str(_BITSIZE) + '-bit g^x mod 3')
    
    # 輸入g和x，並加密
    values = []
    count = 0    
    g = input('請輸入 ' + str(_BITSIZE) + '-bit 的 g：')
    for w in g:
        values.append(lable[count][int(w)])
        count += 1
    x = input('請輸入 ' + str(_BITSIZE) + '-bit 的 x：')
    for w in x:
        values.append(lable[count][int(w)])
        count += 1
        
    # 計算結果
    answer = calculate(values, C_in, C_mid, C_out, table, lable)
    print('Output： ' + answer)

while(True):
    main()
