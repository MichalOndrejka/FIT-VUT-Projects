#!/usr/bin/env python3
class Polynomial(object):
    def __init__(self, *args, **kwargs):
        self.coefficient=list()
        for i in args:
            if type(i) is list:
                self.coefficient=i 
        
        if not self.coefficient:
            if args:
                self.coefficient = list(args) 
            else:
                for name,value in kwargs.items():
                    for i in range(1 + int(name.split("x")[1]) - len(self.coefficient)):
                        self.coefficient.append(0)
                    self.coefficient[int(name.split("x")[1])] = value 
        for i in range(len(self.coefficient)-1, 0, -1):
            if self.coefficient[i] == 0:
                del self.coefficient[i]
            else:
                break 

    def __str__(self):
        poly = ""
        if len(self.coefficient) == 1:
            poly = poly + str(self.coefficient[0])
            return poly 
        for i in reversed(range(len(self.coefficient))):
            if self.coefficient[i] == 0:
                continue 
            if poly:
                if self.coefficient[i] > 0: 
                    poly += " + "
                else:
                    poly += " - "
            if i == 0:
                poly += "{0}".format(str(abs(int((self.coefficient[0])))))
                return poly
            if i == 1:
                if abs(self.coefficient[1]) == 1:
                    poly += "x"
                else:
                    poly += "{0}x".format(str(abs(int(self.coefficient[1]))))
            else:
                if abs(self.coefficient[i]) == 1:
                    poly += "x^{0}".format(i)
                else:
                    poly += "{0}x^{1}".format(str(abs(int(self.coefficient[i]))), i)
        return poly
    def __eq__(self, other):
        return str(self) == str(other)

    def derivative(self):
        tmp=self.coefficient
        del tmp[0]
        for i in range(len(tmp)):
            tmp[i] = tmp[i]*(i+1)
        return Polynomial(tmp)

    def at_value(self, x1, x2 = None):
        res = self.coefficient[-1]
        for i in reversed(range(len(self.coefficient)-1)):
            res = (res*x1) + self.coefficient[i]
        if not x2:
            return res
        else:
            res2 = self.coefficient[-1]
        for i in reversed(range(len(self.coefficient)-1)):
            res2 = (res2*x2)+self.coefficient[i]
        return res2 - res

    def __mul__(self, other):
        tmp = [0]*(1 + len(self.coefficient) + len(other.coefficient))
        for i in range(len(self.coefficient)):
            for j in range(len(other.coefficient)):
                tmp[i+j] += self.coefficient[i] * other.coefficient[j]
        return Polynomial(tmp)

    def __pow__(self, n):
        if n < 0:
            raise ValueError("Wrong power value")
        elif n == 0: 
            return 1
        elif n == 1:
            return Polynomial(self.coefficient)
        elif n > 1:
            tmp = self
            for i in range(1,n):
                tmp *= self
            return Polynomial(tmp)