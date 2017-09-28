
import math
import random
from sets import Set

def puede_pareja(x, y):
	for a in range(2):
		for b in range(2):
			if (x[a] == y[b]):
				return False
	return True
	
n = 5
comb = math.factorial(n) / math.factorial(n - 2)
print "permutaciones: " + str(comb / 2)
partidos = comb / 4



posibles = Set()
for i in xrange(n):
	for j in xrange(n):
		if i != j:
			if (i,j) not in posibles and (j,i) not in posibles:
				posibles.add((i,j))

print str(len(posibles)) + ": " + str(posibles)
print str(comb/2) + " vs " + str(len(posibles))
print
print "partidos: " + str(partidos)

def fueron_usados (x,y, usados):
	x_inv = (x[1], x[0])
	y_inv = (y[1], y[0])
	if x in usados or x_inv in usados or y in usados or y_inv in usados:
		return True
	return False

usados = Set()
while (len(posibles) > 0):
	x2 = posibles.pop()
	if (len(posibles) > 0):
		x3 = posibles.pop()
		rnd = random.random()
		if (rnd <= 0.5):
			x = x2
			posibles.add(x3)
		else:
			x = x3
			posibles.add(x2)
	else:
		x = x2
	if (len(posibles) == 0):
		it = 100
	else:
		fou = False
		it2 = 0
		prueba = posibles.copy()
		while (len(prueba) > 0 and not fou):
			y = prueba.pop()		
			if (puede_pareja(x,y)):
				if (not fueron_usados(x,y,usados)):
					print str(x) + " vs " + str(y)
					fou = True
					usados.add(x)
					usados.add(y)
	


