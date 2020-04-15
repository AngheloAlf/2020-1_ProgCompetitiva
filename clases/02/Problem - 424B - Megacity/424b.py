def distanciaAlCero(x, y):
    return (x**2 + y**2)**0.5

POBLACION_MINIMA = 1_000_000

# Lista de tuplas
# Tupla: (distancia, k)
ubicaciones = []

# Asumiendo que todos los inputs son correctos
n, s = map(int, input().split())
for i in range(n):
    x, y, k = map(int, input().split())
    ubicaciones.append((distanciaAlCero(x, y), k))

ubicaciones.sort()
for distancia, k in ubicaciones:
    s += k
    if s >= POBLACION_MINIMA:
        print("%.7f" % distancia)
        exit(0)

print(-1)
