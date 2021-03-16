# EDA-TP2
Simulación de robots de limpieza para practicar el uso de la interfaz gráfica de Allegro en C

Ejercicio:

12. Este ejercicio se debe resolver sin emplear objetos ni clases. El objetivo del TP es que el
alumno pueda observar cómo se estructura un programa correctamente para luego pasar
a emplear objetos y clases. Por favor no utilicen los recursos de objetos y clases en este
ejercicio

• La habitación es rectangular, de largo h y ancho w. El tamaño de la habitación es pasado
por línea de comandos y es tal que w <= 100 y h <=70.
• El piso está dividido en baldosas de una unidad por una unidad. Las baldosas pueden
estar limpias o sucias. Inicialmente todo el piso está sucio.
• Hay n robots limpiando en simultáneo. La cantidad de robots se pasa por línea de
comando.
• Para simplificar las cosas asumiremos que los robots son puntos y se pueden cruzar sin
interferirse.
• La posición de un robot se describe con dos números reales, (x, y), y su dirección con un
ángulo real d (0 ≤ d < 360). La baldosa en que se encuentra es (floor(x), floor(y)).
• Inicialmente los robots se encuentran dentro de la habitación, en posiciones y
direcciones al azar.
• La simulación avanza de a una unidad de tiempo (“tick”). En cada unidad de tiempo un
robot avanza una unidad de distancia desde donde se encuentra, en la dirección en la
que está.
• Cuando un robot choca contra una pared conserva su posición anterior, y selecciona
una nueva dirección al azar.
• Cuando un robot pisa una baldosa, se considera que limpió esa baldosa.
• El programa termina cuando todas las baldosas quedaron limpias.
• Se pide desarrollar un programa que le permita elegir al usuario entre dos modos a
ingresar por línea de comandos:
  1. Modo 1: El programa mostrará en tiempo real como se va limpiando el piso;
graficando las baldosas limpias y sucias, a los robots y sus direcciones.
  2. Modo 2. El programa realizará un gráfico de tiempo medio de demora en la
limpieza del pido en función de cantidad de robots para el tamaño de piso dado.
De esta forma comenzará con n = 1 y continuará iterando hasta que el tiempo
medio para n y n+1 sea menor a 0.1. Por cada step en n (cada incremento en n)
se deberán correr 1000 simulaciones para estimar el tiempo medio que demora
limpiar el piso con dicha cantidad de robots.

Notas:
• El programa recibe por línea de comandos, el tamaño del piso (Width y Heigh), la
cantidad de robots (Robots) y el modo de la simulación (Modo).
• En Modo 2 se ignora el parámetro Robots (en el caso de recibirlo).
• El programa en el Modo 1 informa la cantidad de ticks necesarios para limpiar el piso
según los parámetros recibidos. En el Modo 2 muestra el gráfico según se describe
arriba.
• Es sumamente útil re-usar el módulo del ejercicio 11. 
• Recuerden separar ideas separadas en módulos separados. Reducir dependencias, para
el código quede poco ligado, sea fácil de entender y se pueda reciclar. Y “as simple as
possible”. 
