import random
import time
redo=True
while redo==True:
    n=0
    p=0
    q=0
    while (n<10):

        a = random.randint(10,81)

        rd = random.randint(1,2)

        if rd == 1:
            rd="-"
            b = random.randint(-9,-1)

        if rd == 2:
            rd="+"
            b = random.randint(1,9)
            q=(str(a)+str(rd)+str(abs(b))+"=")
        t=(input(q))
        if t==str(a + b):
            print("정답")
            p+=1

        elif  input!=a+b:
            print("오답 정답은", a+b)

        n+=1
    print("처리중")
    time.sleep(2)
    print("점수=", p*10)
    redo=input("다시 하시겠습니까(Y,N)")
    if redo=="Y" or redo=="y":
        redo=True
    else:
        redo=False
