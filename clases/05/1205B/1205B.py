from collections import deque 

n = int(input())
numbers = list(map(int, input().split()))

graph = [list() for i in range(n)]
for i in range(n):
    a = numbers[i]
    for j in range(i+1, n):
        b = numbers[j]
        if a & b:
            graph[i].append(j)
            graph[j].append(i)

ciclo = float("inf")
for i in range(n):
    distancias = [float("inf")]*n
    distancias[i] = 0
    padre = [-1]*n
    cola = deque([i])

    while len(cola):
        nodo = cola.popleft()
        for conectado in graph[nodo]:
            if distancias[conectado] == float("inf"):
                distancias[conectado] = distancias[nodo] + 1
                padre[conectado] = nodo
                cola.append(conectado)
            elif padre[nodo] != conectado and padre[conectado] != nodo:
                ciclo = min(ciclo, distancias[nodo] + distancias[conectado] + 1)

if ciclo == float("inf"):
    print(-1)
else:
    print(ciclo)
