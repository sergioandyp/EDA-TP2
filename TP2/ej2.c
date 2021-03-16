#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_color.h>
#include "../../ParseCmdLib/ParseCmdLib/parseCmdLib.h"

#define DISP_WIDTH	1080
#define DISP_HEIGHT	720


typedef struct {
	int x;
	int y;
} point_t;

typedef struct {
	unsigned int step;
	point_t point;
} moire_t;


ALLEGRO_DISPLAY* display;
ALLEGRO_EVENT_QUEUE* queue;

int parseCallback(char* key, char* value, void* moire);

void drawMoire(moire_t *moire);

void initAll();

void destroyAll();

int main (int argc, char* argv[]) {

	moire_t moire;

	initAll();

	parseCmdLine(argc, argv, parseCallback, &moire);

	printf("n: %d\ncx: %d\ncy: %d\n", moire.step, moire.point.x, moire.point.y);

	drawMoire(&moire);

	destroyAll();
}

int parseCallback(char* key, char* value, void* moire) {

	printf("Entro con key=%s, value=%s\n", key, value);

	if (key == NULL) {	// Parametro
		return 0;
	}
	else if (!strcmp("n", key)) {
		((moire_t*)moire) -> step = atoi(value);
	}
	else if (!strcmp("cx", key)) {
		((moire_t*)moire) -> point.x = atoi(value);
	}
	else if (!strcmp("cy", key)) {
		((moire_t*)moire) -> point.y = atoi(value);
	}

	return 1;

}

void drawMoire(moire_t *moire) {

	al_clear_to_color(al_map_rgb(0, 0, 0));

	int n = moire->step;
	point_t point = moire->point;

	for (int i = 0; i <= DISP_WIDTH / n; i++) {
		al_draw_line(i * n, 0, point.x, point.y, al_color_name("white"), 1);
		al_flip_display();
	}
	for (int i = 0; i <= DISP_HEIGHT / n; i++) {
		al_draw_line(DISP_WIDTH, i * n, point.x, point.y, al_color_name("white"), 1);
		al_flip_display();
	}
	for (int i = 0; i <= DISP_WIDTH / n; i++) {
		al_draw_line(DISP_WIDTH - i*n, DISP_HEIGHT, point.x, point.y, al_color_name("white"), 1);
		al_flip_display();
	}
	for (int i = 0; i <= DISP_HEIGHT / n; i++) {
		al_draw_line(0, DISP_HEIGHT - i*n, point.x, point.y, al_color_name("white"), 1);
		al_flip_display();
	}

	ALLEGRO_EVENT event;
	do {
		al_wait_for_event(queue, &event);
	} while (event.type != ALLEGRO_EVENT_DISPLAY_CLOSE);

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
}