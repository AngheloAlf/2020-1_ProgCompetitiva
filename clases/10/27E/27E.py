PRIMOS = [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31]

def solucion(n, prim_i=0, numero=1, divisores=1):
    if divisores == n:
        return numero
    sol = 10**18
    primo = PRIMOS[prim_i]
    for i in range(1, 64):
        if numero * primo > sol:
            break
        if divisores * (i+1) > n:
            break
        numero *= primo
        aux = solucion(n, prim_i+1, numero, divisores * (i+1))
        if aux < sol:
            sol = aux
    return sol


n = int(input())
if n == 1:
    print(1)
else:
    print(solucion(n))
