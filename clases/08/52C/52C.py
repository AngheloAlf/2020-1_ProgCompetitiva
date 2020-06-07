n = int(input())
lista_a = list(map(int, input().split()))

m = int(input())

for i in range(m):
    operation = list(map(int, input().split()))
    if len(operation) == 2:
        lf, rg = operation
        if rg < lf:
            print(min(lista_a[lf:]+lista_a[:rg+1]))
        else:
            print(min(lista_a[lf:rg+1]))
    else:
        lf, rg, v = operation
        if rg < lf:
            for j in range(lf, len(lista_a)):
                lista_a[j] += v
            for j in range(rg+1):
                lista_a[j] += v
        else:
            for j in range(lf, rg+1):
                lista_a[j] += v
