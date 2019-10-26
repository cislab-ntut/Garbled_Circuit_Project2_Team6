import random
import json

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

def getValue(Coz):
    re = int(Coz[-1])
    if re % 2:
        re = int(Coz[1])%2
    else:
        re = int(Coz[0])%2
    return re

def decide(index, a, b):
    for j in index:
        if j == 0:
            if a == 0 and b == 0:
                return 1
        if j == 1:
            if a == 0 and b == 1:
                return 1
        if j == 2:
            if a == 1 and b == 0:
                return 1
        if j == 3:
            if a == 1 and b == 1:
                return 1
    return 0
    
def calculate(values):
    C_in, C_mid, C_out = circuit()

    # input數量
    size = len(values)
    # 輸入的值加上gate輸出的值
    values = values + ([0] * (len(C_in) + len(C_mid) + len(C_out)))
    
    # 完成 IN
    for i in range(len(C_in)):
        
        # 找出上面的值
        a = getValue(values[C_in[i][1][0]])
            
        # 找出下面的值
        b = getValue(values[C_in[i][1][1]])

        # 算出輸出值
        temp = decide(C_in[i][2], a, b)

        # 放回去values
        values[size + C_in[i][0]] = enc(str(temp) + str(random.randint(0,1)), '10')
    
    # 完成 MID
    for i in range(len(C_mid)):
        
        # 找出上面的值
        a = getValue(values[C_mid[i][1][0]])
            
        # 找出下面的值
        b = getValue(values[C_mid[i][1][1]])

        # 算出輸出值
        temp = decide(C_mid[i][2], a, b)

        # 放回去values
        values[size + C_mid[i][0]] = enc(str(random.randint(0,1)) + str(temp), '01')

    # 完成 OUT
    answer = ''
    for i in range(len(C_out)):
        
        # 找出上面的值
        a = getValue(values[C_out[i][1][0]])
            
        # 找出下面的值
        b = getValue(values[C_out[i][1][1]])

        # 算出輸出值
        temp = decide(C_out[i][2], a, b)

        # 放回去values
        answer += str(temp)

    return answer
    
def enc(value, index):
    # value -> 10 ； index -> 10
    # 36946870
    s = ''
    
    # 值(1 -> 奇數 ； 0 -> 偶數)
    temp = random.randint(0,9)
    while temp % 2 != int(value[0]) % 2:
        temp = random.randint(0,9)
    s += str(temp)
    temp = random.randint(0,9)
    while temp % 2 != int(value[1]) % 2:
        temp = random.randint(0,9)
    s += str(temp)
    
    # 4個亂數
    for i in range(4):
        s += str(random.randint(0,9))
        
    # 位置(1 -> 奇數 ； 0 -> 偶數)   
    temp = random.randint(0,9)
    while temp % 2 != int(index[0]) % 2:
        temp = random.randint(0,9)
    s += str(temp)
    temp = random.randint(0,9)
    while temp % 2 != int(index[1]) % 2:
        temp = random.randint(0,9)
    s += str(temp)
    
    return s

def main():
    print('\n2-bit g^x mod 3')

    # 防呆 (00, 00)
    check = False
    
    # 輸入x和p，並加密
    # 10 -> ['58307567', '90154381']
    x = input('請輸入 2-bit 的 x：')
    if x[:2] == '00':
        check = True
    x = [enc(x[:2], '10'), enc(x[:2], '01')]
    
    p = input('請輸入 2-bit 的 p：')
    if check and p[:2] == '00':
        print('無意義')
        return
    p = [enc(p[:2], '10'), enc(p[:2], '01')]

    # 輸入的值
    values = x + p
    answer = calculate(values)
    print('Output： ' + answer)

while(True):
    main()
