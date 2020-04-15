from collections import Counter

entrada = input()
nineteen = Counter("nineteen")
countEntrada = Counter(entrada)

resultado = float("inf")
for letra, cant in nineteen.items():
    if letra == "n" and letra in countEntrada:
        multiplos = (countEntrada[letra]-1)//(cant-1)
    else:
        multiplos = countEntrada[letra]//cant
    if multiplos < resultado:
        resultado = multiplos

print(resultado)
