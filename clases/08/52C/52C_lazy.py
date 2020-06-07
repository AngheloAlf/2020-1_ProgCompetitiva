from math import log, ceil


def construir_arbol(arbol, datos, izquierda, derecha, pivote=1):
    if izquierda == derecha:
        arbol[pivote] = datos[derecha-1]
        return

    mitad = (izquierda + derecha) // 2
    pivote_izquierdo = 2*pivote
    pivote_derecho = 2*pivote + 1

    construir_arbol(arbol, datos, izquierda, mitad, pivote_izquierdo)
    construir_arbol(arbol, datos, mitad+1, derecha, pivote_derecho)

    arbol[pivote] = min(arbol[pivote_izquierdo], arbol[pivote_derecho])


def calcular_postergado(arbol, lazy, pivote):
    if lazy[pivote] == 0:
        return
    
    arbol[pivote] += lazy[pivote]

    pivote_izquierdo = 2*pivote
    pivote_derecho = 2*pivote + 1
    if pivote_derecho < len(lazy):
        lazy[pivote_izquierdo] += lazy[pivote]
        lazy[pivote_derecho] += lazy[pivote]
    
    lazy[pivote] = 0


def menor_en_rango(arbol, lazy, lf, rg, izquierda, derecha, pivote=1):
    if rg < izquierda or derecha < lf:
        return float("inf")

    calcular_postergado(arbol, lazy, pivote)
    if lf <= izquierda and derecha <= rg:
        return arbol[pivote]
    
    mitad = (izquierda + derecha) // 2
    pivote_izquierdo = 2*pivote
    pivote_derecho = 2*pivote + 1

    val_izq = menor_en_rango(arbol, lazy, lf, rg, izquierda, mitad, pivote_izquierdo)
    val_der = menor_en_rango(arbol, lazy, lf, rg, mitad+1, derecha, pivote_derecho)
    return min(val_izq, val_der)


def sumar_al_rango(arbol, lazy, lf, rg, v, izquierda, derecha, pivote=1):
    calcular_postergado(arbol, lazy, pivote)

    if rg < izquierda or derecha < lf:
        return

    if lf <= izquierda and derecha <= rg:
        lazy[pivote] = v
        calcular_postergado(arbol, lazy, pivote)
        return

    mitad = (izquierda + derecha) // 2
    pivote_izquierdo = 2*pivote
    pivote_derecho = 2*pivote + 1

    sumar_al_rango(arbol, lazy, lf, rg, v, izquierda, mitad, pivote_izquierdo)
    sumar_al_rango(arbol, lazy, lf, rg, v, mitad+1, derecha, pivote_derecho)

    arbol[pivote] = min(arbol[pivote_izquierdo], arbol[pivote_derecho])


n = int(input())
lista_a = list(map(int, input().split()))

tamano = 2*(2**ceil(log(n+1, 2)))
arbol = [None for i in range(tamano)]
lazy = [0 for i in range(tamano)]

construir_arbol(arbol, lista_a, 1, n)

m = int(input())

for i in range(m):
    operation = list(map(int, input().split()))
    if len(operation) == 2:
        lf, rg = operation
        lf += 1
        rg += 1
        if rg < lf:
            a = menor_en_rango(arbol, lazy, lf, n, 1, n)
            b = menor_en_rango(arbol, lazy, 1, rg, 1, n)
            print(min(a, b))
        else:
            print(menor_en_rango(arbol, lazy, lf, rg, 1, n))
    else:
        lf, rg, v = operation
        lf += 1
        rg += 1
        if rg < lf:
            sumar_al_rango(arbol, lazy, lf, n, v, 1, n)
            sumar_al_rango(arbol, lazy, 1, rg, v, 1, n)
        else:
            sumar_al_rango(arbol, lazy, lf, rg, v, 1, n)
