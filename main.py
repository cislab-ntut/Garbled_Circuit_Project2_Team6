
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
    
    g = input('請輸入 2-bit 的 g：')
    if check and g[:2] == '00':
        print('無意義')
        return
    g = [enc(g[:2], '10'), enc(g[:2], '01')]

    # 輸入的值
    values = x + g
    answer = calculate(values)
    print('Output： ' + answer)

while(True):
    main()
