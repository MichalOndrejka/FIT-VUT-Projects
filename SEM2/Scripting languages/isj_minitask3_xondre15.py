# minitask 3
def complex_func(x,y):
    return 2 ** x ** y
def first(x):
    return 2 ** x - 1
def second(x):
    return 3 ** x - 2
def third(x):
    return 5 ** x - 3
# list of functions first, second, third, complex_func
funlist = [first, second, third, complex_func]
a = 2
b = 3
# print the result of the last function in the list with parameters a and b
print(funlist[3](a, b))
# from the second to the third
for func in funlist[1:3]:
    # print the result of the function with parameter a
    print(func(a))