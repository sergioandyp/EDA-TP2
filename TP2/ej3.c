#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_color.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <math.h>
#include "../../ParseCmdLib/ParseCmdLib/parseCmdLib.h"

#define DISP_WIDTH		640
#define DISP_W_H_PROP	0.75
#define DISP_HEIGHT		(DISP_WIDTH*DISP_W_H_PROP)

#define XM				4.0
#define PX_TO_M(x)		((x)*XM/DISP_WIDTH)		// Transforma de pixel a mtero
#define M_TO_PX(x)		((x)*DISP_WIDTH/XM)		// Transforma de mtero a pixel

#define BALL_RAD_M		0.1

ALLEGRO_DISPLAY* display;
ALLEGRO_EVENT_QUEUE *queue;
ALLEGRO_TIMER *timer;

typedef struct {
	double x;
	double y;
} Point;

typedef struct {
	Point pos;		// Coordenadas (x, y) de la posicion de la pelota
	Point vel;		// Vector (x,y) de la velocidad de la pelota
} Ball;

typedef struct {
	double g;		// Valor de la gravedad g
	double deltaT;	// Tiempo entre frames
	double k;		// Constante de elasticidad
	Ball ball;		// La pelota
} Simulacion;

void initAll();
void destroyAll();
int parseSimul(char* key, char* value, void* simul);
void simular(Simulacion *simu);

int main(int argc, char *argv[]) {

	initAll();

	Simulacion simul = {
		.g = 9.8,
		.deltaT = 0.1,
		.k = 0.9,
		.ball = {.pos = {1.5,1}, .vel = {1.0, 1.0}}
	};

	parseCmdLine(argc, argv, parseSimul, &simul);

	simular(&simul);
	/*
	al_clear_to_color(al_color_name("white"));
	al_draw_circle(100, 100, 20, al_color_name("black"), 5);
	al_flip_display();
	ALLEGRO_EVENT event;
	do {
		al_wait_for_event(queue, &event);
	} while (event.type != ALLEGRO_EVENT_DISPLAY_CLOSE);
	*/

	destroyAll();
	return 0;
}

void simular(Simulacion *simu) {
	al_clear_to_color(al_color_name("white"));
	timer = al_create_timer(simu->deltaT);
	al_register_event_source(queue, al_get_timer_event_source(timer));
	al_start_timer(timer);

	Ball *ball = &(simu->ball);

	ALLEGRO_EVENT event;
	bool redraw = 1;
	do {
		al_wait_for_event(queue, &event);
		al_flip_display();
		if (event.type == ALLEGRO_EVENT_TIMER) {

			if (M_TO_PX(ball->pos.x) - M_TO_PX(BALL_RAD_M) <= 0 ||
				M_TO_PX(ball->pos.x) + M_TO_PX(BALL_RAD_M) >= DISP_WIDTH) {	// Pelota toca pared lateral
				ball->vel.x *= -1 * simu->k;
			}

			if (M_TO_PX(ball->pos.y) - M_TO_PX(BALL_RAD_M) <= 0 ||
				M_TO_PX(ball->pos.y) + M_TO_PX(BALL_RAD_M) >= DISP_HEIGHT) {	// Pelota toca techo o piso
				ball->vel.y *= -1*simu->k;
			}

			ball->pos.x += ball->vel.x * simu->deltaT;
			ball->pos.y += (-ball->vel.y) * simu->deltaT;
			ball->vel.y -= simu->g * simu->deltaT;

			redraw = true;
		}

		if (redraw && al_event_queue_is_empty(queue)) {
			al_draw_circle(M_TO_PX(ball->pos.x), M_TO_PX(ball->pos.y), M_TO_PX(BALL_RAD_M), al_color_name("black"), 3);
			al_flip_display();
			redraw = false;
		}
	} while (event.type != ALLEGRO_EVENT_DISPLAY_CLOSE);

}


int parseSimul(char* key, char* value, void* simul) {
	if (key != NULL) {
		if (!strcmp(key, "x0")) {
			((Simulacion*)simul) -> ball.pos.x = atof(value);
		}
		else if (!strcmp(key, "y0")) {
			((Simulacion*)simul) -> ball.pos.y = atof(value);
		}
		else if (!strcmp(key, "vx0")) {
			((Simulacion*)simul) -> ball.vel.x = atof(value);
		}
		else if (!strcmp(key, "vy0")) {
			((Simulacion*)simul) -> ball.vel.y = atof(value);
		}
		else if (!strcmp(key, "deltaT")) {
			((Simulacion*)simul) -> deltaT = atof(value);
		}
		else if (!strcmp(key, "g")) {
			((Simulacion*)simul) -> g = atof(value);
		}
		else if (!strcmp(key, "k")) {
			((Simulacion*)simul) -> k = atof(value);
		}
		else {
			printf("Error en los parametros");
			return 0;
		}
		return 1;
	}
	return 0;
}

void initAll() {

	al_init();

	al_init_primitives_addon();

	display = al_create_display(DISP_WIDTH, DISP_HEIGHT);

	queue = al_create_event_queue();

	al_register_event_source(queue, al_get_display_event_source(display));

}

void destroyAll() {
	al_destroy_display(display);
	al_destroy_event_queue(queue);
	al_destroy_timer(timer);
}