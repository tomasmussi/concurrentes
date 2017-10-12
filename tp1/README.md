# tp1

## ¿Cómo correr el programa?

Una vez compilado el programa, es cuestión de, desde la carpeta `cmake_build_debug`, correr `./tp1` con los siguientes argumentos:

- `m`: Cantidad máxima de personas dentro de la playa. Es importante remarcar que este número debe ser mayor o igual a 10, ya que el torneo sólo empieza cuando hay 10 jugadores o más.
- `k`: Cantidad máxima de partidos que puede jugar un jugador.
- `rows`: Cantidad de filas de canchas en la playa.
- `columns`: Cantidad de columnas de canchas en la playa.
- `log_level`: Nivel de log deseado: 0 (`DEBUG`), 1 (`INFO`), 2 (`WARNING`), 3 (`ERROR`).

Con esto, una ejecución válida sería:

```bash
$ ./tp1 12 4 2 3 2
```

Es importante enviar siempre los 5 argumentos. Si no se quiere especificar uno, se puede enviar 0, para indicar que se quiere utilizar el default. En caso de no recibir 5 argumentos, se tomarán todos por defecto.

## ¿Cómo comenzar la simulación?

Una vez comenzada la simulación, lo que hay que hacer es comenzar a "enviar" jugadores al predio. Primero, debemos obtener el número de proceso principal viendo el archivo de log (Ver sección "Logging") o bien ejecutando:

```bash
$ ps -a
  PID TTY          TIME CMD
 5903 pts/2    00:00:00 tp1
 5904 pts/2    00:00:00 tp1
 5905 pts/2    00:00:00 tp1
 5906 pts/2    00:00:00 tp1
 5907 pts/2    00:00:00 tp1
 5931 pts/17   00:00:00 ps
```

Y viendo el número de proceso correspondiente al `CMD` de la aplicación. El `PID` del proceso `main` será el menor de todos. En el ejemplo, sería el 5903.

Hecho esto, se puede simular el arribo de jugadores al predio ejecutando por consola lo siguiente:

```
$ kill -USR1 <PID_main>
```

Reemplazando `<PID_main>` por el `PID` obtenido en el paso anterior, se envía la señal `USR1` (correspondiente con el número de señal 10), que se interpreta como el arribo de un jugador al predio. De esta manera y ejecutando _n_ veces este comando, se puede simular la llegada de varios jugadores al predio.

Una vez comenzado el programa, en el archivo de log o bien por pantalla, se mostraran los distintos `PID` de los procesos. Es importante tomar nota del `PID` del `Court Manager`, ya que es a él que se deberá enviar las señales para subir y bajar la marea. Para ello:

```bash
$ kill -USR2 <PID_CourtManager> # Sube la marea
$ kill -31 <PID_CourtManager> # Baja la marea
```

## Logging

Ya estando en ejecución, se encuentra a disposición un archivo de log llamado `log.txt` dentro de la carpeta `cmake_build_debug`. En este se pueden encontrar los diversos mensajes de log de la aplicación.

## Fin de la simulación

La simulación puede terminar de forma automática, pero si se desea cortar antes también es posible. Basta con enviar la señal `SIGINT` al `main`, para que terminen todos los procesos involucrados. No es necesario hacer esto con el comando `kill`, ya que apretando ctrl+c en la consola se envía dicha señal.