# supermercado-moderno

#### Declaramos las variables globales:
- Dos de tipo int, para contar el número de cajas y el número de clientes.
- Un array dinámico de cajas de tipo bool para saber si está ocupada o no.
- Un array dinámico de int para el tiempo de espera de clientes.
- Un mutex de tipo pthread_mutex_t para controlar la cola.
- Un tipo pthread_cond_t para la condición del mutex.

#### Programa principal (función main):
Primero declaramos las variables:
- Una de tipo int como contador.
- Un array dinámico para los identificadores de los clientes.
- Un array dinámico de threads que serán los clientes.
- Tres de tipo float para calcular la desviación típica, la media y un auxiliar.

Si el número de argumentos es 2 significa que sólo nos han pasado el número de cajas y por lo tanto el número de clientes deberá ser 20 veces mayor. 

Si el número de argumentos es 3 asignamos el valor a las variables. Si no es ninguno de eso casos mostrara por pantalla un error.

Reservamos memoria para los arrays dinámicos.

Inicializamos el mutex, que hará de cola.

Entramos en un bucle inicializando el array de las cajas del supermercado a false, vacías.

Inicializamos la condición del mutex, que una caja se quede vacía.

Creamos los threads que serán los clientes y realizarán la función cliente.

A continuación se sincronizarán los threads.

Destruimos los mutex.

Inicializamos la variable media y la calculamos sacando los valores del tiempo de espera del array de espera de clientes y dividiendo la suma entre el número de clientes.

Se inicializa la variable desviación y se calcula. Liberamos la memoria.

Para finalizar se imprime un mensaje mostrando que el supermercado se ha cerrado, otro mostrando el tiempo medio de espera y otro para la desviación típica.

#### Función que ejecutaran los threads( cliente): 

Primero declaramos las variables:
- Cuatro variables de tipo int, una para la caja a la que irá el cliente, otra para el identificador del cliente, y dos para los cálculos del tiempo de espera.

Creamos una semilla con la hora del ordenador para que sea más aleatorio el uso de rand(). Imprimimos por pantalla que el cliente está comprando.

Hacemos que espere un tiempo aleatorio mientras compra.

Inicializamos una variable con el tiempo de inicio.

Imprimimos por pantalla que al cliente está en la cola.

El cliente hace un lock del mutex si puede, si no espera a que se libere el mutex.

Espera a que una caja se quede libre, entra en un while que haciendo uso de la función cajasOcupadas, espera a la condición del mutex, hasta que una caja no esté libre no saldrá del bucle.

Entra en la sección crítica.

Inicializamos la variable caja con una caja libre usando la función auxiliar cajaLibre.

Inicializamos una variable con el tiempo que ha esperado.

Imprimimos que el cliente ha sido atendido, por lo tanto estará en una caja pagando. El tiempo que espera es aleatorio.

Marcamos en el array de cajas la caja en la que estamos como true, para saber que está ocupada.

Y liberamos el mutex de la cola para que el siguiente pueda hacer uso de la primera caja libre.

Finaliza la sección crítica.

Esperamos un tiempo aleatorio para simular que está siendo atendido.

Calculamos el tiempo que ha esperado antes de ser atendido y se guarda en el array dinámico de espera_clientes.

Entramos en otra sección crítica.

Se bloquea el mutex para que ningún cliente entre a ninguna caja mientras.

Se marca como false en el array de cajas la actual para que se vea como libre. Mandamos una señal al mutex de que hay una caja libre.

Se hace un unlock del mutex para que los clientes puedan seguir buscando una caja. Finaliza la sección crítica.

Después se imprime que el cliente ha salido después de esperar el tiempo calculado anteriormente.
 
#### Función auxiliar cajasOcupadas:
Esta función devuelve true si todas las cajas están ocupadas. 

#### Función auxiliar cajaLibre:
Esta función devuelve una caja que esté libre.
