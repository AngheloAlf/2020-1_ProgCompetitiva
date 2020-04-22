# Excede el tiempo, habría que intentar portearlo a C

# `frase` está despues del "("
def contador_parentesis(frase):
    contador = 1
    caracteres = 1
    caracteres_ultimo_cero = 0
    for i in range(len(frase)):
        if frase[i] == "(":
            contador += 1
        elif frase[i] == ")":
            contador -= 1
        caracteres += 1
        if contador < 0:
            caracteres -= 1
            return caracteres
        if contador == 0:
            caracteres_ultimo_cero = caracteres
    #if contador != 0:
    #    return 0
    return caracteres_ultimo_cero

a = input()

maximo = 0
contados = 0
i = 0
while i <len(a):
    if a[i] == "(":
        nuevo = contador_parentesis(a[i+1:])
        if nuevo > 0 and nuevo % 2 == 0:
            if nuevo > maximo:
                maximo = nuevo
                contados = 1
            elif nuevo == maximo:
                contados += 1
            i += nuevo
        else:
            i += 1
    else:
        i += 1

if maximo == 0:
    print(0, 1)
else:
    print(maximo, contados)

