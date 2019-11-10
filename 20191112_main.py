import random
import json
import hashlib

# label的長度
_LABELSIZE = 16
# 亂數種子
randomSeed = ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',\
              'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z']
# K(64)
K = ['0x428a2f98', '0x71374491', '0xb5c0fbcf', '0xe9b5dba5', '0x3956c25b', '0x59f111f1', '0x923f82a4', '0xab1c5ed5',\
    '0xd807aa98', '0x12835b01', '0x243185be', '0x550c7dc3', '0x72be5d74', '0x80deb1fe', '0x9bdc06a7', '0xc19bf174',\
    '0xe49b69c1', '0xefbe4786', '0x0fc19dc6', '0x240ca1cc', '0x2de92c6f', '0x4a7484aa', '0x5cb0a9dc', '0x76f988da',\
    '0x983e5152', '0xa831c66d', '0xb00327c8', '0xbf597fc7', '0xc6e00bf3', '0xd5a79147', '0x06ca6351', '0x14292967',\
    '0x27b70a85', '0x2e1b2138', '0x4d2c6dfc', '0x53380d13', '0x650a7354', '0x766a0abb', '0x81c2c92e', '0x92722c85',\
    '0xa2bfe8a1', '0xa81a664b', '0xc24b8b70', '0xc76c51a3', '0xd192e819', '0xd6990624', '0xf40e3585', '0x106aa070',\
    '0x19a4c116', '0x1e376c08', '0x2748774c', '0x34b0bcb5', '0x391c0cb3', '0x4ed8aa4a', '0x5b9cca4f', '0x682e6ff3',\
    '0x748f82ee', '0x78a5636f', '0x84c87814', '0x8cc70208', '0x90befffa', '0xa4506ceb', '0xbef9a3f7', '0xc67178f2']
# H(8)
H = []
# W(64)
W = []
# i
It = 0

class Circuit():
    def __init__(self, c_in, c_out, lable, table, inputSize):
        self.C_in = c_in
        self.C_out = c_out
        self.Lable = lable
        self.Table = table
        self.InputSize = inputSize

def circuit(fileName, inputSize):
    # 讀電路檔案
    infile = open(fileName, 'r')
    r = infile.readline()
    infile.close()
    dic = json.loads(r)
    C_in = dic['input']
    C_out = dic['output']

    # 建立label
    input_lable = create_input_lable(inputSize)
    gate_lable = create_gate_lable(C_in, C_out)
    lable = input_lable + gate_lable
    
    # 建立table
    table = create_table(lable, C_in, C_out, inputSize) 
    
    # 建立結構
    resultC = Circuit(C_in, C_out, lable, table, inputSize)
    
    return resultC

def create_gate_lable(C_in, C_out):
    result = []
    
    # gate數量
    size = len(C_in) + len(C_out)

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

def create_input_lable(inputSize):
    result = []

    # 產生所有input的0,1標籤
    for i in range(inputSize * 32):
        one = ''
        zero = ''
        while(len(one) < _LABELSIZE):
            one += str(random.choice(randomSeed))
        while(len(zero) < _LABELSIZE):
            zero += str(random.choice(randomSeed))
        result.append([zero, one]) # g0 的 0和1
        
    return result

def create_table(lable, C_in, C_out, inputSize):
    result = {}

    for c in C_in:
        result[c[0]] = find_truthtable(lable, c, inputSize)

    for c in C_out:
        result[c[0]] = find_truthtable(lable, c, inputSize)
        
    return result

def find_truthtable(lable, c, inputSize):
    temp = []
    
    # 建立gate的input
    temp.append([lable[c[1][0]][0], lable[c[1][1]][0]])
    temp.append([lable[c[1][0]][0], lable[c[1][1]][1]])
    temp.append([lable[c[1][0]][1], lable[c[1][1]][0]])
    temp.append([lable[c[1][0]][1], lable[c[1][1]][1]])

    # 建立gate的table
    lableID = c[0] + (inputSize * 32)
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
    result = t[random.randint(0, 3)][1]
    print(result)
    return result

# 計算結果  
def calculate(values, CC):
    # 輸入的值加上gate輸出的值
    values = values + ([0] * (len(CC.C_in) + len(CC.C_out)))

    # 完成 IN
    for c in CC.C_in:
        print('GateID：' + str(c[0]) + ' --->', end=' ')
        # 找出上面的值
        a = values[c[1][0]]
            
        # 找出下面的值
        b = values[c[1][1]]

        # 算出輸出值
        t = CC.Table[c[0]]
        
        # 放回去values
        values[CC.InputSize + c[0]] = decide(a, b, t)

    # 完成 OUT
    answer = ''
    for c in CC.C_out:
        print('GateID：' + str(c[0]) + ' --->', end=' ')
        # 找出上面的值
        a = values[c[1][0]]
            
        # 找出下面的值
        b = values[c[1][1]]

        # 算出輸出值
        t = CC.Table[c[0]]

        # 放回去values
        tempAns = decide(a, b, t)
        values[CC.InputSize + c[0]] = tempAns
        if tempAns == CC.Lable[(CC.InputSize * 32) + c[0]][0]:
            answer += '0'
        else:
            answer += '1'

    return answer

def operation(C_Con, C_Maj, C_LS0, C_LS1, C_Add):

    ## T1 0-31
    values = [C_LS1.Lable[t][int(H[4][t])] for t in range(len(H[4]))]
    T1_LS1 = calculate(values, C_LS1)
    
    tempV1 = [C_Add.Lable[t][int(H[7][t])] for t in range(len(H[7]))]
    tempV2 = [C_Add.Lable[t][int(T1_LS1[t])] for t in range(len(T1_LS1))]
    values = tempV1 + tempV2
    T1_ADD1 = calculate(values, C_Add)

    tempV1 = [C_Con.Lable[t][int(H[4][t])] for t in range(len(H[4]))]
    tempV2 = [C_Con.Lable[t][int(H[5][t])] for t in range(len(H[5]))]
    tempV3 = [C_Con.Lable[t][int(H[6][t])] for t in range(len(H[6]))]
    values = tempV1 + tempV2 + tempV3
    T1_Con = calculate(values, C_Con)

    tempV1 = [C_Add.Lable[t][int(T1_ADD1[t])] for t in range(len(T1_ADD1))]
    tempV2 = [C_Add.Lable[t][int(T1_Con[t])] for t in range(len(T1_Con))]
    values = tempV1 + tempV2
    T1_ADD2 = calculate(values, C_Add)

    tempV1 = [C_Add.Lable[t][int(T1_ADD2[t])] for t in range(len(T1_ADD2))]
    tempV2 = [C_Add.Lable[t][int(K[It][t])] for t in range(len(K[It]))]
    values = tempV1 + tempV2
    T1_ADD3 = calculate(values, C_Add)

    tempV1 = [C_Add.Lable[t][int(T1_ADD3[t])] for t in range(len(T1_ADD3))]
    tempV2 = [C_Add.Lable[t][int(W[It][t])] for t in range(len(W[It]))]
    values = tempV1 + tempV2
    T1 = calculate(values, C_Add)

    ## T2 0-31
    values = [C_LS0.Lable[t][int(H[0][t])] for t in range(len(H[0]))]
    T2_LS0 = calculate(values, C_LS0)

    tempV1 = [C_Maj.Lable[t][int(H[0][t])] for t in range(len(H[0]))]
    tempV2 = [C_Maj.Lable[t][int(H[1][t])] for t in range(len(H[1]))]
    tempV3 = [C_Maj.Lable[t][int(H[2][t])] for t in range(len(H[2]))]
    values = tempV1 + tempV2 + tempV3
    T2_Maj = calculate(values, C_Maj)

    tempV1 = [C_Add.Lable[t][int(T2_LS0[t])] for t in range(len(T2_LS0))]
    tempV2 = [C_Add.Lable[t][int(T2_Maj[t])] for t in range(len(T2_Maj))]
    values = tempV1 + tempV2
    T2 = calculate(values, C_Add)

    ## D + T1 0-31
    tempV1 = [C_Add.Lable[t][int(H[3][t])] for t in range(len(H[3]))]
    tempV2 = [C_Add.Lable[t][int(T1[t])] for t in range(len(T1))]
    values = tempV1 + tempV2
    D_T1 = calculate(values, C_Add)

    ## T1 + T2 0-31
    tempV1 = [C_Add.Lable[t][int(T1[t])] for t in range(len(T1))]
    tempV2 = [C_Add.Lable[t][int(T2[t])] for t in range(len(T2))]
    values = tempV1 + tempV2
    T1_T2 = calculate(values, C_Add)

    H[7] = H[6]
    H[6] = H[5]
    H[5] = H[4]
    H[4] = T1_LS1#D_T1
    H[3] = H[2]
    H[2] = H[1]
    H[1] = H[0]
    H[0] = T1_LS1#T1_T2

def main():
    
    C_Con = circuit('conditional.json', 3)
    C_Maj = circuit('Majority.json', 3)
    C_LS0 = circuit('LSigma_0.json', 1)
    C_LS1 = circuit('LSigma_1.json', 1)
    C_Add = circuit('adder.json', 2)
    
    # 讀入 H.txt 和 W.txt 和 I.txt
    global H
    infileH = open('H.txt')
    tempH = infileH.readline()
    H = tempH.split()
    H = ['{0:032b}'.format(int(h, 16)) for h in H]
    infileH.close()
    
    global W
    infileW = open('W.txt')
    tempW = infileW.readline()
    W = tempW.split()
    W = ['{0:032b}'.format(int(w, 16)) for w in W]
    infileW.close()
    
    global It
    infileI = open('I.txt')
    It = int(infileI.read())
    infileI.close()
    
    global K
    K = ['{0:032b}'.format(int(k, 16))for k in K]
        
    # 計算結果
    operation(C_Con, C_Maj, C_LS0, C_LS1, C_Add)
    H = ['{0:08x}'.format(int(h, 2)) for h in H]


main()
