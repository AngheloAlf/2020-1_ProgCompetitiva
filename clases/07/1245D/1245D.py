
n = int(input())
#grafo = [list() for i in range(n)]
grilla = [None for i in range(n)]
for i in range(n):
    x, y = map(lambda z: int(z)-1, input().split())
    #grafo[x].append(y)
    #grafo[y].append(x)
    grilla[i] = (x, y)

c = list(map(lambda z: int(z), input().split()))
k = list(map(lambda z: int(z), input().split()))

min_yen = sum(c)
v_stations_to_build = []
e_connections = []








print(min_yen)

print(len(v_stations_to_build))
print("".join(map(lambda x: x.str(), v_stations_to_build)))

print(len(e_connections))
for x, y in e_connections:
    print(x, y)
