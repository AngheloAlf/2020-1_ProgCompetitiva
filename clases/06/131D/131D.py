from collections import deque 


# https://www.geeksforgeeks.org/detect-cycle-in-the-graph-using-degrees-of-nodes-of-graph/
def findCycle(grafo):
    n = len(grafo)
    cantidad_conexiones = [len(destinos) for destinos in grafo]
    visitados = [False for i in range(n)]

    cola = deque(i for i in range(n) if cantidad_conexiones[i] == 1)
    while cola:
        while cola:
            nodo = cola.popleft()
            visitados[nodo] = True
            for destino in grafo[nodo]:
                cantidad_conexiones[destino] -= 1
        cola = deque(i for i in range(n) if cantidad_conexiones[i] == 1 and not visitados[i])

    nodos_ciclo = []
    for i in range(n):
        if not visitados[i]:
            nodos_ciclo.append(i)
    return nodos_ciclo

def distanciasAlCiclo(grafo, ciclo, numero_grande=float("inf")):
    distancias = [numero_grande for i in range(n)]
    for nodo in ciclo:
        distancias[nodo] = 0
    cola = deque(ciclo)

    while len(cola):
        nodo = cola.popleft()
        for conectado in grafo[nodo]:
            if distancias[conectado] == numero_grande:
                distancias[conectado] = distancias[nodo] + 1
                if conectado not in ciclo:
                    cola.append(conectado)
    return distancias

n = int(input())
grafo = [list() for i in range(n)]
for i in range(n):
    x, y = map(lambda z: int(z)-1, input().split())
    grafo[x].append(y)
    grafo[y].append(x)

nodos_ciclo = findCycle(grafo)
print(" ".join(map(str, distanciasAlCiclo(grafo, nodos_ciclo))))
