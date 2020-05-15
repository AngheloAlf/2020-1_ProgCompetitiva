from collections import deque

def es_valido(grafo, secuencia):
    nodo_inicial = secuencia[0]
    if nodo_inicial != 0:
        return "No"
    cola = deque([nodo_inicial])
    
    i = 1
    while cola and i < len(secuencia):
        siguiente = cola.popleft()
        while i < len(secuencia):
            nodo = secuencia[i]
            if nodo not in grafo[siguiente]:
                break
            cola.append(nodo)
            i += 1
    return "Yes" if i == len(secuencia) else "No"

n = int(input())
grafo = [set() for i in range(n)]
for i in range(n-1):
    x, y = map(lambda z: int(z)-1, input().split())
    grafo[x].add(y)
    grafo[y].add(x)
secuencia =  list(map(lambda z: int(z)-1, input().split()))
print(es_valido(grafo, secuencia))
