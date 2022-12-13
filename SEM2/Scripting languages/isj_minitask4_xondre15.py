# minitask 4
mcase = {'a': 10, 'b': 34, 'A': 7, 'Z': 3}
#wanted = {'a': 17, 'b': 34, 'z': 3}
wanted = {}
for key,item in mcase.items():
    key = key.lower()
    if key in wanted.keys():
        wanted[key] += item
    else:
        wanted[key] = item
    
print(wanted)