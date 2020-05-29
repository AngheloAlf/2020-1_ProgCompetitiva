# Entrega semana 13-05-2020

Grupo 1

## Integrantes

- Anghelo Carvajal
- Juan Pablo L.
- Rodrigo Valenzuela

## Soluciones a los problemas

### 131D - Subway

[Link a la solución](https://codeforces.com/contest/131/submission/80024268)

#### Tutorial

El problema explica que tenemos un conjunto de $n$ estaciones y $n$ pasajes que los conectan, los cuales pueden ser recorridos en ambos sentidos. Dado esto, podemos modelar el problema como un grafo no-dirigido de $n$ nodos y $n$ vértices.

Además explica que siempre hay _exactamente_ un único anillo formado por algunos de los pasajes.

Lo que se nos pide es mostrar por pantalla la distancia que hay entre cada nodo y el nodo más cercano a este que sea parte del anillo. Todos los nodos que son parte del anillo tienen distancia 0.

Dado que sabemos que siempre hay un único ciclo (anillo), podemos usar un algoritmo que esté encargado de encontrar este ciclo, y conociendo este ciclo podemos usar otro algoritmo que se encargue de calcular las distancias de todos los nodos hasta este ciclo.

Para encontrar el ciclo, podemos usar un algoritmo BFS. Los **nodos iniciales** de este algoritmo serían todos los nodos que están en **los extremos del grafo** (los que tienen un único arco). Para determinar esto se recomienda tener en alguna estructura la cantidad de arcos que tiene cada nodo. Inicialmente agregariamos a la cola todos los nodos que tengan exactamente una única conexión. Recorremos la cola, y por cada nodo recorrido lo "removemos", anotandolo como visitado y **disminuyendo en 1 la cantidad de arcos** que tiene cada nodo que esté conectado con este nodo. Una vez terminamos de recorrer la cola, agregamos a la cola todos los nodos que tengan un único arco y que no hayan sido visitados, así sucesivamente hasta que no queden nodos que tengan un único arco. Finalmente, los nodos que conforman el ciclo son los que no han sido visitados, ya que estos siempre tendrán más de un arco, dado que están conectados con al menos 2 nodos (los otros que componen al ciclo) + los nodos que no son parte de un ciclo.

En otras palabras, este algoritmo inicia en los nodos de los extremos, removiendo los nodos recorridos y avanzando a los nodos que tienen un único camino por recorrer. Si después de remover un arco el nodo tiene más de un arco se entiende que es posible que sea parte del ciclo, por lo que no lo recorremos. Finalmente, los que no hemos recorrido son parte del ciclo.

Teniendo los nodos que conforman este ciclo, podemos realizar un algoritmo BFS clasico para calcular la distancia de cada nodo al nodo inicial. La única diferencia sería qué en vez de tener un único nodo inicial, tendríamos varios, los cuales serían los nodos que conforman el ciclo, el resto es igual.
