#include "ej11.h"

#include <stdio.h>
#include <math.h>

#define PI 3.14159265358979323846
#define DELTA 0.0000001

//#define TEST

#ifdef TEST
int main() {
	Point p1 = {0.0, 0.0};
	Point p2 = {-10.0, 10.0};
	printf("Los puntos (%f,%f) y (%f,%f) forman un angulo de %f°\n", p1.x,p1.y, p2.x, p2.y, getAngleBetweenPoints(p1,p2));
}
#endif


double getDistanceBetweenPoints(Point p1, Point p2) {
	return sqrt( pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2) );
}

double getAngleBetweenPoints(Point p1, Point p2) {
	
	if (isPointEqual(p1, p2)) {		// Es el mismo punto, devuelvo 0
		return 0;
	}

	double angle = atan2((p2.x - p1.x), (p2.y - p1.y)) * (180.0 / PI);	// angulo entre -180 y 180

	return angle >=0 ? angle : angle+360;	// Si es negativo le sumo 1 vuelta
}

Point translatePoint(Point p, double distance, double angle) {

	p.x += distance * sin(angle * PI / 180.0);
	p.y += distance * cos(angle * PI / 180.0);

	return p;
}

bool isPointEqual(Point p1, Point p2) {
	return getDistanceBetweenPoints(p1,p2) < DELTA;	// Devuelve 1 si son iguales, 0 si no
}

double getRectArea(Rect r) {
	return (r.supDer.x - r.infIzq.x) * (r.supDer.y - r.infIzq.y);	// base*altura
}

bool isPointInRect(Point p, Rect r) {
	if (p.x >= r.infIzq.x && p.x<= r.supDer.x			// Dentro del x del rectangulo
		&& p.y >= r.infIzq.y && p.y <= r.supDer.y) {	// Y dentro del y
		return true;
	}
	return false;
}