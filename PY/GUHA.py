from guha import *
#для заполнения nodes можно создать свой алгоритм чтения из файла
#sss.nodes = список
names=['A', 'B', 'C'] #названия атрибутов
mask_gen(names); #внесение названий
nn=''
for i in names:
    nn += i + ' ' #вывод в формате A B C
print(nn)
for i in nodes:
    l = ''
    for j in msk:
        l += '1 ' if (i & j != 0) else '0 ' #вывод объектов в бинарной форме
    print(l)
rules = guha() #заполнение правил без ограничений
for i in rules:
    if(len(i) != 0):
        print(i) #вывод всех правил за исключением недостаточных    

