#ifndef EJ_10_H
#define EJ_10_H

#include <stdbool.h>

#define STANDARD_ANGLE(angle)	((angle)-360.0*(int)((angle)/360.0))

typedef struct {
	double x;
	double y;
} Point;

typedef struct {
	Point infIzq;
	Point supDer;
} Rect;


double getDistanceBetweenPoints(Point p1, Point p2);

double getAngleBetweenPoints(Point p1, Point p2);

Point translatePoint(Point p, double distance, double angle);

bool isPointEqual(Point p1, Point p2);

double getRectArea(Rect r);

bool isPointInRect(Point p, Rect r);

#endif