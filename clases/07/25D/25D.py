def u_find(padre, i):
    if padre[i] == -1:
        return i
    return u_find(padre, padre[i])

def union(padre, x, y):
    x_i = u_find(padre, x)
    y_j = u_find(padre, y)
    padre[x_i] = y_j

n = int(input())
grafo = [list() for i in range(n)]
caminos_iniciales = []
for i in range(n-1):
    x, y = map(lambda z: int(z)-1, input().split())
    grafo[x].append(y)
    grafo[y].append(x)
    caminos_iniciales.append((x, y))

cerrados = []
padre = [-1 for i in range(n)]
for x, y in caminos_iniciales:
    i = u_find(padre, x)
    j = u_find(padre, y)
    if i == j:
        cerrados.append((x+1, y+1))
    else:
        union(padre, x, y)

nuevos = []
for i in range(1, n):
    if u_find(padre, 0) != u_find(padre, i):
        union(padre, 0, i)
        nuevos.append((0+1, i+1))

print(len(cerrados))

for index in range(len(cerrados)):
    i, j = cerrados[index]
    u, v = nuevos[index]
    print(i, j, u, v)
