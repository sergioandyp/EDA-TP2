#include "simulacion.h"
#include <stdlib.h>
#include <time.h>
#include "ej11.h"
#include "robotAll.h"
#define N_ROBOTS 1000

static int simulCallback(Simulacion* simu);

int simulacion(int n, int height, int width, bool mode)//mode 1 = false ; mode 2 = true
{
	srand((unsigned int)time(NULL));

	bool* pPiso;
	pPiso = (bool*)calloc(height*width, sizeof(bool));//genero el piso. se inicializa con todos 0. 
											//uso que 0 = sucio
	if (pPiso == NULL) return -1;

	Robot* pRoomba;
	pRoomba = (Robot*)calloc(n, sizeof(Robot));

	if (pRoomba == NULL) return -1;
	for (int i = 0; i < n; i++)//asigno valores al azar para los roombas
	{
		pRoomba[i].position.x = rand()*(double)width/RAND_MAX;//columna al azar
		pRoomba[i].position.y = rand()* (double)height/RAND_MAX;//fila al azar
		pRoomba[i].angle = rand() * 360.0/RAND_MAX;//angulo al azar
	}

	int ticks = 0; //cuantos loops tarda
	int baldosas_sucias = height * width;//la cantidad de baldosas sucias
	if (!mode)
	{
		Floor piso;
		piso.width = width;
		piso.height = height;
		piso.tiles = pPiso;


		Simulacion simu;
		simu.floor = piso;
		simu.robots = pRoomba;
		simu.cantRobots = n;

		ticks = drawRobots( &simu, simulCallback);

	}
	else
	{
		while (baldosas_sucias > 0)
		{


			ticks++;
			for (int j = 0; j < n; j++)
			{
				//                 posicion de la balosa en x  + posicion de la baldosa en y 
				if (pPiso[(int)pRoomba[j].position.x + (int)pRoomba[j].position.y * width] == 0)
				{
					pPiso[(int)pRoomba[j].position.x + (int)pRoomba[j].position.y * width] = 1;
					baldosas_sucias--;
				}

				Point newPos = translatePoint((pRoomba + j)->position, 1,pRoomba[j].angle );//(pRoomba + j)->angle = rand() * 360.0 / RAND_MAX
				if (newPos.x < width && newPos.x > 0 && newPos.y < height && newPos.y > 0) {
					pRoomba[j].position = newPos;
				}
				else
				{
					(pRoomba + j)->angle = rand() * 360.0 / RAND_MAX;
				}

			}
		}
	}

	free(pRoomba);
	free(pPiso);

	return ticks;
}

static int simulCallback (Simulacion* simu)
{
	static int ticks = 0;
	static long baldosas_limpias = 0;

	ticks++;
	for (int j = 0; j < simu->cantRobots; j++)
	{
		//                 posicion de la balosa en x  + posicion de la baldosa en y 
		if (simu->floor.tiles[(int)simu->robots[j].position.x + (int)(simu->robots[j].position.y) * simu->floor.width] == 0)
		{
			simu->floor.tiles[(int)simu->robots[j].position.x + (int)(simu->robots[j].position.y) * simu->floor.width] = 1;
			baldosas_limpias++;
		}
		//no se si me conviene crear un punto p de prueba para encontrar un punto dentro del rectangulo o7
		//si me coniene asignarle al robot varias veces valores distintos hasta pegarle


		Point newPos = translatePoint((simu->robots + j)->position, 1, simu->robots[j].angle);
		if (newPos.x < simu->floor.width && newPos.x > 0 && newPos.y < simu->floor.height && newPos.y > 0) {
			simu->robots[j].position = newPos;
		}
		else
		{
			simu->robots[j].angle = rand() * 360.0 / RAND_MAX;
		}

	}

	if (baldosas_limpias < simu->floor.height * simu->floor.width)
		return 0;
	else
		return ticks;
}