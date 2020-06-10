# Tuorial 52C

El problema nos plantea un arreglo numérico circular. A este arreglo le debemos implementar 2 operaciones especiales:

- `inc(lf, rg, v)`. Esta operación le suma `v` a todos los números en el rango `[lf, rg]`.
- `rmq(lf, rg)`. Esta operación busca el valor mínimo en el rango `[lf, rg]`.

Como el arreglo es circular, si `rg` es menor a `lf`, la secuencia del indice empieza en `rg` hasta llegar al final del arreglo, y luego empieza en cero hasta llegar a `lf`.

Teniendo esto en cuenta, el problema nos dice que tenemos que procesar `m` de estas operaciones y mostrar los resultados por pantalla.

El problema de esto, es que si implementamos estas operaciones de forma _naive_, ambas tendrían una complejidad temporal **O(n)**, lo cual sumado a las `m` operaciones que debemos realizar, tendríamos una complejidad temporal de **O(n^2)** entre manos, lo cual es mucho para este problema.

Una posible optimización para la operación `rmq` sería simular el arreglo usando un _segment tree_, de modo que el criterio de este arbol sea que sus nodos contengan el menor valor del segmento al que representan. De este modo, la operación `rmq` tendría una complejidad temporal de **O(log n)**.

Para optimizar la operación `inc` usando _segment tree_ 's, habría que usar una implementación _lazy_. La idea detras de esto, sería tener una estructura secundaría que indique que valores no hemos actualizado todavía, y actualizar estos nodos cuando los vayamos a leer, no antes. Una forma de implementar esto, sería usar una estructura similar al arbol principal, de modo que cada vez que queramos sumar `v` al rango, le sumaríamos ese valor al nodo del arbol secundario (el _lazy_) en vez de sumarselo al nodo del arbol principal. De este modo, `inc` tendría una complejidad temporal de **O(log n)**. De este modo, al considerar las `m` operaciones, todo el proceso quedaría en **O(n*log(n))**

Al hacer este cambio, habría que modificar levemente la operación `rmq`, de modo que mientras que recorre el arbol en busca de el valor mínimo, también debería revisar ese mismo nodo en el arbol secundario, y si dicho nodo dice que este nodo tiene un valor pendiente por actualizar, lo actualizamos y luego le sumamos este valor a los hijos de este nodo en el arbol secundario (y no en principal).

Finalmente quedaría el detale de que este arreglo es un arreglo circular, pero esto se puede solucionar facilmente. En el caso de `rmq`, simplemente haríamos la consulta desde `rg` hasta el final del arreglo, y luego otra consulta desde el inicio del arreglo hasta `lf`, luego comparamos ambos resultados y nos quedamos con el menor. Para el caso de `inc` sería aún más simple, tan solo llamamos a `inc` desde `rg` hasta el final, y luego desde el inicio del arreglo hasta `lf`.
