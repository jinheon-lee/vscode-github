import random
import time
redo=True
while redo==True:
    n=0
    p=0
    q=0
    t=0
    HighScore=0
    while (n<10):

        a = random.randint(20,99)

        rd = random.randint(1,2)

        if rd == 1:
            rd="-"
            b = random.randint(-20,-1)

        if rd == 2:
            rd="+"
            b = random.randint(1,30)
        q=(str(a)+str(rd)+str(abs(b))+"=")
        try:
            t=int(input(q))
        except:
            print("숫자를 입력하세요")
            pass
        if t==a + b:
            print("정답")
            p+=1

        elif  input!=a+b:
            print("오답 정답은", a+b)

        n+=1
    print("처리중")
    time.sleep(0.7)
    print("[■■              ]")
    print("\n")  
    time.sleep(0.7)
    print("[■■■■            ]")
    print("\n")
    time.sleep(0.7)
    print("[■■■■■■          ]")
    print("\n")
    time.sleep(0.7)
    print("[■■■■■■■■        ]")
    print("\n")
    time.sleep(0.7)
    print("[■■■■■■■■■■      ]")
    print("\n")
    time.sleep(0.7)
    print("[■■■■■■■■■■■■    ]")
    print("\n")
    time.sleep(0.7)
    print("[■■■■■■■■■■■■■■  ]")
    print("\n")
    time.sleep(0.7)
    print("[■■■■■■■■■■■■■■■■]")
    print("\n")
    time.sleep(0.7)
    print('완료')
    print("점수=", p*10)
    score=p*10
    if (HighScore<score):
        HighScore=score
    time.sleep(2)
    redo=input("다시 하시겠습니까(Y,N)")
    if redo=="Y" or redo=="y":
        redo=True
print("최고점수:"+str(HighScore))