#include "simulacion.h"
#include <stdlib.h>
#include <time.h>
#include "ej11.h"
#include "robotAll.h"
#define N_ROBOTS 1000

typedef struct {
	Point position;
	double angle;
} Roomba;

int simulacion(int n, int height, int width, bool mode)//mode 1 = true ; mode 2 = false
{
	srand(time(NULL));

	bool* pPiso;
	pPiso = (bool*)calloc(height*width, sizeof(bool));//genero el piso. se inicializa con todos 0. 
											//uso que 0 = sucio
	if (pPiso == NULL) return -1;

	Roomba* pRoomba;
	pRoomba = (Roomba*)calloc(n, sizeof(Roomba));

	if (pRoomba == NULL) return -1;
	for (int i = 0; i < n; i++)//asigno valores al azar para los roombas
	{
		pRoomba[i].position.x = rand()*(double)width/RAND_MAX;//columna al azar
		pRoomba[i].position.y = rand()* (double)height/RAND_MAX;//fila al azar
		pRoomba[i].angle = rand() * 360.0/RAND_MAX;//angulo al azar
	}

	int ticks = 0; //cuantos loops tarda
	int baldosas_sucias = height * width;//la cantidad de baldosas sucias
	while (baldosas_sucias > 0)
	{
		//if(mode)
		//	drawRobots(bool* Piso, Roomba* pRoomba);

		ticks++;
		for (int j = 0; j < n; j++)
		{
			//                 posicion de la balosa en x  + posicion de la baldosa en y 
			if ( pPiso[(int)pRoomba[j].position.x + (int)pRoomba[j].position.y * width] == 0)
			{
				pPiso[(int)pRoomba[j].position.x + (int)pRoomba[j].position.y * width] = 1;
				//*(pPiso + ((int)(pRoomba + j)->position.x) + width * (sizeof(bool))) = 1;
				baldosas_sucias--;
			}
			bool is_in_piso = 0;//spanglish papá
			//no se si me conviene crear un punto p de prueba para encontrar un punto dentro del rectangulo o7
			//si me coniene asignarle al robot varias veces valores distintos hasta pegarle
			
			Point newPos = translatePoint((pRoomba + j)->position, 1, (pRoomba + j)->angle = rand() * 360.0/RAND_MAX);
			if (newPos.x < width && newPos.x > 0 && newPos.y < height && newPos.y > 0) {
				is_in_piso = 1;		
				pRoomba[j].position = newPos;
			}


		}
	}

	free(pRoomba);
	free(pPiso);

	return ticks;
}