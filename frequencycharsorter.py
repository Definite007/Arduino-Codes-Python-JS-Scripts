s = 'feeble' # word to be tested against
l1 = list(s)
s1 = set(s)
d1 = {}
for i in s1:
    d1[i] = 0
    for j in l1:
        if i == j:
            d1[i] +=1
d1 = dict(sorted(d1.items(), key=lambda item: item[1], reverse=True))
s2 = ''
for key,value in d1.items():
    for i in range(value):
        s2 += key
print(s2)
