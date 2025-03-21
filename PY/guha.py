#Алгоритм GUHA на путоне

def norm(a):
    sm = 0.0
    for i in a:
        sm += i
    if(sm == 0):
        return [0]*len(a)
    for i in range(len(a)):
        a[i] /= sm
    return a
#функция нормализации

alla = 0.05 #частота пересечений
character = 3 #ассоциативность
depends = 1.5 #зависимость

def FIMPLE(a, b): 
	if(a < alla):
	    return 0.0
	return a / (a + b)
#ассоциативная функция FIMPLE

rules = [] #письменные правила
names = [] #названия атрибутов
nodes = [
    1 * 1 + 1 * 2 + 1 * 4,
	0 * 1 + 1 * 2 + 1 * 4,
	0 * 1 + 1 * 2 + 0 * 4,
	1 * 1 + 0 * 2 + 1 * 4,
	1 * 1 + 0 * 2 + 1 * 4,
	1 * 1 + 1 * 2 + 1 * 4,
	1 * 1 + 0 * 2 + 0 * 4,
	1 * 1 + 0 * 2 + 1 * 4,
	0 * 1 + 1 * 2 + 0 * 4,
	1 * 1 + 1 * 2 + 1 * 4] #объекты
msk=[1, 2, 4] #маски атрибутов

class noded: #класс в котором происходят все основные чудеса
    def __init__(self, a, b):
        self.rule = [0, 0, 0, 0] #правило (здесь хранятся количества [d, c, b, a])
        self.formatted = '' #письменное представление (для удобства восприятия)
        self.a = a #здесь хранится младшая маска
        self.b = b #здесь хранится старшая маска
        #во избежание повторов маски идут в порядке [меньшее, большее]
    
    def get_rule(self):
        return norm(self.rule) #возвращение правила (в форме поддержек)
    
    def get_mask(self):
        return self.a | self.b #возвращение формируемой маски
    
    def get_strength(self):
        AB = FIMPLE(self.rule[3], self.rule[2])
        AC = FIMPLE(self.rule[3], self.rule[1])
        DB = FIMPLE(self.rule[2], self.rule[0])
        DC = FIMPLE(self.rule[1], self.rule[0])
        t = (AB * AC) / (DB * DC) if (DB * DC > 0) else character + 1 #силы связи
        return t > character or t < 1.0 / character #если отношение в периоде то у связь слабо выражена
    
    def get_string(self):
        r = norm(self.rule) #относительное правило
        if(r[3] < alla):
            return '' #если пересечений между атрибутами мало то нет смысла говорит об ассоциациях
        return self.formatted #возвращает письменное представление
    
    def try_object(self, O):
        phi = ((self.a & O) == self.a) #наличие атрибута a
        psi = ((self.b & O) == self.b) #наличие атрибута b
        
        if(phi and psi):
            self.rule[3] += 1 #если оба то последний [d, c, b, A]
        if(not phi and psi):
            self.rule[2] += 1 #если только b то предпоследний [d, c, B, a]
        if(phi and not psi):
            self.rule[1] += 1 #если только a то второй [d, C, b, a]
        if(not phi and not psi):
            self.rule[0] += 1 #если нету то первый [D, c, b, a]
        #для более простого восприятия тут серия из if но можно и через формулу:
        #self.rule[phi | (psi << 1)] += 1
    
    def form_string(self):
        if((self.rule[3] + self.rule[1] == 0) or (self.rule[3] + self.rule[2] == 0)):
            return #B=a+c и A=a+b тогда если хоть один 0 то нет смысла искать связь
        nn = norm(self.rule) #относительное
        
        o1 = '' #название атрибута a
        for i in range(len(names)):
            o1 += names[i] if (self.a & msk[i] != 0) else '' #составление названия атрибута
        o2 = '' #название атрибута b
        for i in range(len(names)):
            o2 += names[i] if (self.b & msk[i] != 0) else '' #составление названия атрбута
        
        AB = FIMPLE(nn[3], nn[2])
        AC = FIMPLE(nn[3], nn[1])
        DB = FIMPLE(nn[2], nn[0])
        DC = FIMPLE(nn[1], nn[0])
        
        AD = (AB * AC) / (DB * DC) if (DB * DC > 0) else character + 1 #силы связи
        OT = AB / AC if (AC > 0) else depends + 1
        UB = DC / DB if (DB > 0) else depends + 1
        T = OT if (AD >= 1) else UB #направления зависимости
        
        if(AD > character): #если отношение за периодом в положительную сторону то это ассоциация
            if(T > depends): #если свободных A больше то B зависит
                self.formatted += o2 + '->' + o1 + '' #B---->A
            elif(T < 1.0 / depends): #если свободных B больше то A зависит
                self.formatted += o1 + '->' + o2 + '' #A---->B
            else: #если свободных B и A примерно поровну то они просто встречаются вместе
                self.formatted += '{edge [dir=none] ' + o1 + '->' + o2 + '}' #A-----B
        elif(AD < 1.0 / character): #если отношение за периодом в негативную сторону то это избегание
            if(T > depends): #если свободных A больше то B проигрывает конкуренцию
                self.formatted += '{edge [style=dashed] ' + o2 + '->' + o1 + '}' #B- - >A
            elif(T < 1.0 / depends): #если свободных B больше то A проигрывает конкуренцию
                self.formatted += '{edge [style=dashed] ' + o1 + '->' + o2 + '}' #A- - >B
            else: #если свободных A и B примерно поровну то они просто избегают друг друга
                self.formatted += '{edge [style=dashed, dir=none] ' + o1 + '->' + o2 + '}' #A- - -B
        else:#если отношение в периоде то оно настолько слабое что нет смысла считать
            self.formatted = '' #обнуление
            self.rule = [0.0, 0.0, 0.0, 0.0] #обнуление

assoc=[] #лист с объектами noded

def mask_gen(name_array): #создание масок для атрибутов и наложения
    global msk #нелокальная переменная
    global names #нелокальная переменная
    msk=[] #обнуление
    names = name_array #присваивание имен
    for i in range(len(name_array)):
        msk.append(2 ** i) #формирование масок

def run_assoc(a, b): #заполнение правил
    if(a & b):
        return False #если маски накладываются то выходим
    if(a > b): #меняем маски местами во избежание повторов и обеспечиваем [меньшее, большее]
        t=a
        a=b
        b=t
    flag1 = False
    flag2 = False
    lol1 = False
    lol2 = False
    #флаги для выхода по слабой связи
    for i in assoc:
        if(i.a == a and i.b == b):
            return False #если правило уже есть то это повтор выходим
        elif(i.get_mask() == a): #если маска a составная
            flag1 |= i.get_strength() #смотрим силу связи
            lol1 = True #отмечаем что мы нашли такую маску
        elif(i.get_mask() == b): #если маска b составная
            flag2 |= i.get_strength() #смотрим силу связи
            lol2 = True #отмечаем что мы нашли эту маску
    if(lol1 and not flag1 or lol2 and not flag2):
        return False #если связь хоть одной из составных масок слабая то считать смысла нет
        
    O = noded(a, b) #формируем объект
    for i in nodes:
        O.try_object(i) #вставляем объект
    assoc.append(O) #добавляем правило
    return O.get_strength #возвращаем силу

def guha(a=0): #автоматон формирующий правила
    global rules #нелокальная переменная
    rules=[] #обнуление
    collect = msk #текущие маски
    temp = [] #следующие маски
    mask = msk #прогоняемые маски
    allowed = len(msk) - 1 if (a > len(msk) - 1 or not a) else allowed
    #дозволенная глубина (вплоть до 2 d cntgtyb (кол-во масок - 1))
    
    for i in range(allowed): #проход по уровням
        for j in collect: #проход по текущим маскам (они сравниваются)
            for k in mask: #проход по уже составленным маскам
                if(run_assoc(j, k)) and (j | k != 2 ** len(msk)):
                    temp.append(j | k) #если правило есть и маска содержит не все атрибуты то вставка
        collect = temp #текущие маски состоят из новых масок
        mask.extend(temp) #в общие маски вставляются новые маски
        temp=[] #новые маски обнуляем
    for i in assoc:
        i.form_string() #заполнение письменными правилами
        rules.append(i.get_string()) #добавление их к списку
    return rules #вывод правил (так как в нескольких файлах)

