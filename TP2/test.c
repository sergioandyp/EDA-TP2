#include <stdio.h>
#include <stdlib.h>
#include "simulacion.h"
#include "robotAll.h"

//int main() {
//
//	if (initUI()) return -1;
//	double values[1000];
//	int robotCount = 0;
//	unsigned long long ticks;
//
//	//ticks = simulacion(2, 5, 5, 0);
//	//printf("ticks: %d\n", ticks);
//
//	do {
//		ticks = 0;
//		printf("Nro de robots: %d\n", robotCount+1);
//		for (int i = 0; i < 1000; i++) {
//			ticks += simulacion(robotCount+1, 20, 20, 0);
//			//ticks = rand()*1000.0/RAND_MAX;
//		}
//		printf("\tticks: %f\n", ticks/1000.0);
//		values[robotCount] = ticks/1000.0;
//		robotCount++;
//		drawChart(values, robotCount);
//	} while (robotCount < 2 || values[robotCount-2] - values[robotCount-1] >= 0.1);
//
//
//	printf("Termina2\n");
//	while (getchar() != '\n');
//	destroyUI();
//}