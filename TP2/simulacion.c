#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "ej11.h"
#define N_ROBOTS 1000

typedef struct {
	Point position;
	float angle;
}Roomba;

int simulacion(int n, int height, int width, bool mode)//mode 1 = false ; mode 2 = true
{
	srand(time(NULL));

	bool* pPiso;
	pPiso = (bool*)calloc(height * width, sizeof(bool));//genero el piso. se inicializa con todos 0. 
											//uso que 0 = sucio
	if (pPiso == NULL) return -1;

	Roomba* pRoomba;
	pRoomba = (Roomba*)calloc(n, sizeof(Roomba));

	if (pRoomba == NULL) return -1;

	for (int i = 0; i < n; i++)//asigno valores al azar para los roombas
	{
		(pRoomba + i)->position.x = (double)(rand() % width);//columna al azar
		(pRoomba + i)->position.y = (double)(rand() % height);//fila al azar
		(pRoomba + i)->angle = (double)(rand() % 360);//angulo al azar
	}

	int ticks = 0; //cuantos loops tarda
	int baldosas_sucias = height * width;//la cantidad de baldosas sucias
	while (baldosas_sucias > 0)
	{
		/*if(mode)
			allegro_print(bool* Piso, Roomba* pRoomba);*/
		ticks++;
		for (int j = 0; j < n; j++)//miro todos los roombas a ver si estan sobre piso para limpiar
		{
			//                 posicion de la balosa en x  + posicion de la baldosa en y 
			if (*(pPiso + ((int)((pRoomba + j)->position.x)) + (int)((pRoomba + j)->position.y) * width) == 0)//si esta sobre piso sucio
			{
				*(pPiso + ((int)((pRoomba + j)->position.x)) + (int)((pRoomba + j)->position.y) * width) = 1;//limpialo
				baldosas_sucias--;//indica que se limpio una baldosa
			}
			bool is_in_piso = 0;//spanglish papá
			//no se si me conviene crear un punto p de prueba para encontrar un punto dentro del rectangulo o7
			//si me coniene asignarle al robot varias veces valores distintos hasta pegarle
			while (!is_in_piso)//me fijo 
			{
				Point p;
				p = translatePoint((pRoomba + j)->position, 1, (pRoomba + j)->angle = rand() % 360);
				if (!(p.x > width || p.x < 0 || p.y > height || p.y < 0)) {
					(pRoomba + j)->position = p;
					is_in_piso = 1;
				}
			}


		}
	}
	return ticks;
}