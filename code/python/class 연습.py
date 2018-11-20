import time
class cat(object):
    def __init__(self, name, weight):
        self.name = name
        self.weight = weight
    def eat(self, food):
        self.weight+=0.05
        print(self.name+" is eating "+food)
    def sleep(self, sec):
        print(self.name+" is sleeping")
        print("zzz")
        time.sleep(sec)
        print(self.name+" is awake")
    def excersise(self, t):
        print(self.name+" is excersise")
        time.sleep(t)
        if(self.weight>4):
            self.weight=self.weight-0.05
        

navi = cat("navi", 4.5)
navi.eat("tuna")
print(navi.weight)
navi.sleep(1)
navi.excersise(1)
print(navi.weight)

kitten =cat("kitten", 5)
kitten.eat("fish")
print(kitten.weight)
kitten.sleep(2)
kitten.excersise(3)
print(kitten.weight)