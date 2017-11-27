# tp2

## ¿Cómo correr el programa?

Una vez compilado el programa, es cuestión de, desde la carpeta `cmake_build_debug`, correr `./tp2` con alguno de los siguientes argumentos:

- `s`: Comienza el servidor.
- `a`: Comienza un administrador, de forma de poder agregar datos a los servicios correspondientes. Las instrucciones correspondientes para ejecutar un alta o modificación sobre algún dato serán mostradas cuando se ejecute el administrador.
- `c`: Comienza un cliente, que podrá hacer consultas sobre los distintos servicios que ofrece el portal. Para ejecutar un cliente válido, se debe ejecutar con el tipo de consulta que se quiere hacer. Por ejemplo:
  * Para una consulta sobre el tiempo de una ciudad: `c 1 BSAS`
  * Para una consulta sobre el cambio de una moneda: `c 2 USD`

## Fin de la simulación

La forma de finalizar el servidor es tan simple como enviar la señal `SIGINT` al servidor. No es necesario hacer esto con el comando `kill`, ya que apretando ctrl+c en la consola se envía dicha señal.

Una vez finalizada la ejecución, dentro de la carpeta `cmake_build_debug` se encontrarán los archivos `monedas.csv` y `tiempo.csv` con los datos que se fueron agregando en la ejecución del programa. Dicho archivo será levantado la próxima vez que se ejecute el servidor.