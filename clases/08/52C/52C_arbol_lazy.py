import math

class ArbolLazy():
    def __init__(self, datos):
        self.n = len(datos)
        self.tamano = 2*(2**math.ceil(math.log(self.n+1, 2)))
        self.arbol = [None for i in range(self.tamano)]
        self.lazy = [0 for i in range(self.tamano)]
        self._construir(datos, 1, self.n)

    def _construir(self, datos, izquierda, derecha, pivote=1):
        if izquierda == derecha:
            self.arbol[pivote] = datos[derecha-1]
            return self.arbol[pivote]

        mitad = (izquierda + derecha) // 2
        pivote_izquierdo = 2*pivote
        pivote_derecho = 2*pivote + 1

        a = self._construir(datos, izquierda, mitad, pivote_izquierdo)
        b = self._construir(datos, mitad+1, derecha, pivote_derecho)

        self.arbol[pivote] = min(a, b)
        return self.arbol[pivote]

    def _calcular_postergado(self, pivote):
        if self.lazy[pivote] == 0:
            return

        self.arbol[pivote] += self.lazy[pivote]

        pivote_izquierdo = 2*pivote
        pivote_derecho = 2*pivote + 1
        if pivote_derecho < self.tamano:
            self.lazy[pivote_izquierdo] += self.lazy[pivote]
            self.lazy[pivote_derecho] += self.lazy[pivote]

        self.lazy[pivote] = 0
        return

    def _sumar_al_rango(self, lf, rg, v, izq, der, pivote=1):
        self._calcular_postergado(pivote)

        if rg < izq or der < lf:
            return

        if lf <= izq and der <= rg:
            self.lazy[pivote] += v
            self._calcular_postergado(pivote)
            return

        mitad = (izq + der) // 2
        pivote_izquierdo = 2*pivote
        pivote_derecho = 2*pivote + 1

        self._sumar_al_rango(lf, rg, v, izq, mitad, pivote_izquierdo)
        self._sumar_al_rango(lf, rg, v, mitad+1, der, pivote_derecho)

        self.arbol[pivote] = min(self.arbol[pivote_izquierdo], self.arbol[pivote_derecho])

    def inc(self, lf, rg, v):
        lf += 1
        rg += 1
        if rg < lf:
            self._sumar_al_rango(lf, self.n, v, 1, self.n)
            self._sumar_al_rango(1, rg, v, 1, self.n)
            return
        self._sumar_al_rango(lf, rg, v, 1, self.n)

    def _min_en_rango(self, lf, rg, izq, der, pivote=1):
        if rg < izq or der < lf:
            return float("inf")

        self._calcular_postergado(pivote)
        if lf <= izq and der <= rg:
            return self.arbol[pivote]

        mitad = (izq + der) // 2
        pivote_izquierdo = 2*pivote
        pivote_derecho = 2*pivote + 1

        a = self._min_en_rango(lf, rg, izq, mitad, pivote_izquierdo)
        b = self._min_en_rango(lf, rg, mitad+1, der, pivote_derecho)
        return min(a, b)

    def rmq(self, lf, rg):
        lf += 1
        rg += 1
        if rg < lf:
            a = self._min_en_rango(lf, self.n, 1, self.n)
            b = self._min_en_rango(1, rg, 1, self.n)
            return min(a, b)
        return self._min_en_rango(lf, rg, 1, self.n)


input()
arbol = ArbolLazy(list(map(int, input().split())))

m = int(input())
for i in range(m):
    operation = list(map(int, input().split()))
    if len(operation) == 2:
        print(arbol.rmq(*operation))
    else:
        arbol.inc(*operation)
