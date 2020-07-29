def revisar_poleras(tallas_voluntarios, poleras):
    if len(tallas_voluntarios) == 0:
        return "YES"
    
    for talla in tallas_voluntarios[0]:
        if talla not in poleras or poleras[talla] <= 0:
            continue
        poleras[talla] -= 1
        resultado = revisar_poleras(tallas_voluntarios[1:], dict(poleras))
        if resultado == "YES":
            return resultado
        poleras[talla] += 1

    return "NO"


tests = int(input())

for _ in range(tests):
    M, N = map(int, input().split())
    poleras_por_talla = {"XXL": M/6, "XL": M/6, "L": M/6, "M": M/6, "S": M/6, "XS": M/6}
    tallas_voluntarios = [tuple(input().split()) for _ in range(N)]
    print(revisar_poleras(tallas_voluntarios, dict(poleras_por_talla)))
