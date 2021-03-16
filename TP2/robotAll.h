#ifndef ROBOT_ALL_H
#define ROBOT_ALL_H
#include <stdbool.h>

// Estas estructura tomarlas de otro lado

typedef struct {
	int width;
	int height;
	bool* tiles;
} Floor;

typedef struct {
	double x;
	double y;
	double angle;
} Robot;

typedef struct {
	Floor floor;
	Robot* robots;
	int cantRobots;
} Simulacion;

// Callback para avanzar la simulacion de todos los robots 1 paso
typedef int (*simCallback)(Simulacion* simu);

// inicializa los recursos necesarios para la interfaz gráfica
// Devuelve 0 si se completo exitosamente, 1 si hubo error
int initUI();

// Destruccion de los recursos empleados
void destroyUI();

// Corre la simulacion solicitada de a intervalos de tick definidos segun el tamaño de
// la habitacion. Por cada avance de tick, dibuja las baldosas del piso segun su estado,
// los robots representados como flechas desde su ubicación actual hasta un paso de 1 unidad
// en su ángulo correspondiente. En caso de que el robot caiga nuevamente dentro del piso, se
// reemplaza su ubicación por la nueva. Luego de cada tick, se llama a la callback nextStep,
// que se debe encargar de otorgarle un nuevo ángulo a cada robot. La callback debe devolver 0 si 
// la simulacion debe continuar, y otro valor para detenerla. Si el usuario presiona cualquier tecla
// o cierra la ventana, la simulacion tambien se detiene.
void drawRobots(Simulacion* simu, simCallback nextStep);

// Realiza un grafico de los distintos values en funcion de su posicion en el arreglo
// para una correcta visualizacion, los valores deben ser no negativos
void drawChart(double* values, int size);

#endif	// ROBOT_ALL_H