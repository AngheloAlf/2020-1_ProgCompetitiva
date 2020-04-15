from itertools import chain, combinations

# Conjunto potencia, todas las combinaciones posibles de los elementos del parametro.
# https://stackoverflow.com/a/1482316/6292472
def powerset(iterable):
    s = list(iterable)
    return chain.from_iterable(combinations(s, r) for r in range(1, len(s)+1))

n, l, r, x = map(int, input().split())

cantidad_de_formas = 0
difficulties = powerset(map(int, input().split()))
for combinacion in difficulties:
    if len(combinacion) > 1: # No nos interesan las combinaciones de un único elemento.
        if max(combinacion) - min(combinacion) >= x: # Cumple con la diferencia mínima de dificultad.
            suma = sum(combinacion)
            if suma >= l and suma <= r:
                cantidad_de_formas += 1
print(cantidad_de_formas)
