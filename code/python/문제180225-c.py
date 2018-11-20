import random
import time
redo=True
while redo==True:#문제 수만큼 반복
    n=0
    p=0
    Question=0
    answer=0#입력한 답
    HighScore=0
    for num in range(1, 11): #문제 10개

        a = random.randint(20,99) #처음수

        rd = random.randint(1,2)  #+, - 정하기

        if rd == 1:
            rd="-"
            b = random.randint(-20,-1) #나중수

        if rd == 2:
            rd="+"
            b = random.randint(1,30) #나중수
        Question=(str(num)+" : "+str(a)+str(rd)+str(abs(b))+"=") #문제번호+
        answer=str(input(Question))
        if answer == str(a + b):
            print("정답")
            p+=1

        else:
            print("오답, 정답은", a+b)

        n+=1
    print("처리중")#처리 이펙트
    print("[■■              ]\n")
    time.sleep(0.7)
    print("[■■■■            }\n")
    time.sleep(0.7)
    print("[■■■■■■          ]\n")
    time.sleep(0.7)
    print("[■■■■■■■■        ]\n")
    time.sleep(0.7)
    print("[■■■■■■■■■■      ]\n")
    time.sleep(0.7)
    print("[■■■■■■■■■■■■    ]\n")
    time.sleep(0.7)
    print("[■■■■■■■■■■■■■■  ]\n")
    time.sleep(0.7)
    print("[■■■■■■■■■■■■■■■■]\n")
    print('완료')
    print("점수=", p*10)
    score=p*10
    if HighScore<score:#최고 점수
        HighScore=score
    time.sleep(1)
    redo=input("다시 하시겠습니까(Y,N)")
    if redo=="Y" or redo=="y":
        redo = True
print("최고점수:"+str(HighScore))
time.sleep(10)