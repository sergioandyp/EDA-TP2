#include "robotAll.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_font.h>
#include <stdio.h>
#include <stdlib.h>
#include "ej11.h"	// Cambiar por archivo donde estas las funciones matematicas del ej11

#define DISP_WIDTH	1080
#define DISP_HEIGHT	720

#define GAP_BTW_TILES	1.0	// La distancia entre baldosas

// Obtengo la medida del lado de la baldosa en base a la medida de pantalla y la cantidad de baldosas.
// EspaioParaBaldosas/CantidadDeBaldosas
#define CALC_TILE_LENGTH(dispSize, nTiles)	( (double)( (dispSize) - GAP_BTW_TILES*((nTiles)+1) ) / (nTiles) )

// Cambia la coordenada de un punto para adaptarla a las coordenadas de Allegro (eje Y invertido)
#define	Y_TO_ALL(y)		(DISP_HEIGHT-(y))

#define CHART_OFFSET	60

// Static functions prototypes

static void drawArrow(Point* orig, Point* dest, ALLEGRO_COLOR color);
static void drawFloor(Floor* floor);
static double getMaxValue(double* values, int size);
static Point pointToAll(Point p, int w, int h);

// Static variables

static ALLEGRO_DISPLAY* display = NULL;
static ALLEGRO_EVENT_QUEUE* queue = NULL;
static ALLEGRO_TIMER* timer = NULL;
static ALLEGRO_FONT* font = NULL;


//#define TEST

#ifdef TEST

#include <time.h>
#define TEST_W		1
#define TEST_H		2
#define TEST_NUM	(TEST_W*TEST_H)

int nextStep(Simulacion* simu) {
	for (int i = 0; i < simu->cantRobots; i++) {
		simu->robots[i].angle = rand() * 360.0 / RAND_MAX;
	}
	return 0;
}

int main() {

	if (initUI()) {
		return 1;
	}

	srand(time(NULL));

	//Point p0 = { 3.5, 1.5 };
	//Point p1 = { 1.5, 2.5 };
	//
	//bool tiles[TEST_NUM];
	//Floor floor = { 5, 5, tiles };
	//drawFloor(&floor);
	//p0 = pointToAll(p0, 5, 5);
	//p1 = pointToAll(p1, 5, 5);
	//drawArrow(&p0,&p1);
	//al_flip_display();

// Modo 1:

	bool tiles[TEST_NUM];
	
	for (int i = 0; i < TEST_NUM; i++) {
		tiles[i] = 0;// rand() % 2;
	}
	
	Floor floor = { TEST_W, TEST_H, tiles };
	Robot robot[5] = {	{.x = rand()*(double)TEST_W/RAND_MAX, .y = 4.0, .angle = 0 },
						{.x = rand() * (double)TEST_W / RAND_MAX, .y = rand() * (double)TEST_H / RAND_MAX, .angle = rand() * 360.0 / RAND_MAX },
						{.x = rand() * (double)TEST_W / RAND_MAX, .y = rand() * (double)TEST_H / RAND_MAX, .angle = rand() * 360.0 / RAND_MAX },
						{.x = rand() * (double)TEST_W / RAND_MAX, .y = rand() * (double)TEST_H / RAND_MAX, .angle = rand() * 360.0 / RAND_MAX },
						{.x = rand() * (double)TEST_W / RAND_MAX, .y = rand() * (double)TEST_H / RAND_MAX, .angle = rand() * 360.0 / RAND_MAX },
	};
	Simulacion simu = {
		.cantRobots = 5,
		.floor = floor,
		.robots = robot
	};

	drawRobots(&simu, nextStep);


// Modo 2:

	Point p1 = { DISP_WIDTH/2.0, DISP_HEIGHT/2.0 };
	Point p2 = {DISP_WIDTH/2.0, DISP_HEIGHT-100};
	
	double values[10];

	for (int i = 0; i < 10; i++) {
		values[i] = (rand()*1000.0/RAND_MAX);
		printf("values[%d]: %f\n", i, values[i]);
		drawChart(values, i+1);
	}

	ALLEGRO_EVENT event;
	do {
		al_wait_for_event(queue, &event);
	} while (event.type != ALLEGRO_EVENT_DISPLAY_CLOSE);

	destroyUI();

	return 0;
}
#endif

// Global function Definition

int drawRobots(Simulacion * simu, simCallback nextStep) {

	timer = al_create_timer(0.5);
	al_register_event_source(queue, al_get_timer_event_source(timer));
	al_register_event_source(queue, al_get_keyboard_event_source());
	al_start_timer(timer);

	Rect room = {
		.infIzq = {0.0, 0.0},
		.supDer = {(double)simu->floor.width, (double)simu->floor.height}
	};

	ALLEGRO_EVENT event;
	int retVal = 0;		// Return value
	bool redraw = true;
	bool quit = false;
	do {
		al_wait_for_event(queue, &event);

		switch (event.type) {
			case ALLEGRO_EVENT_TIMER:
				redraw = true;
				break;
			case ALLEGRO_EVENT_DISPLAY_CLOSE:		// Presionar la X
			case ALLEGRO_EVENT_KEY_DOWN:			// O cualquier tecla
				quit = true;						// sale del programa
				break;
			default:
				break;
		}

		if (redraw && al_event_queue_is_empty(queue)) {
			drawFloor(&(simu->floor));
			for (int i = 0; i < simu->cantRobots; i++) {

				Point orig = simu->robots[i].position;
				Point dest = translatePoint(orig, 1.0, simu->robots[i].angle);

				orig = pointToAll(orig, simu->floor.width, simu->floor.height);
				dest = pointToAll(dest, simu->floor.width, simu->floor.height);
				drawArrow(&orig, &dest, al_color_name("black"));	// Robot modelado como flecha
			}
			al_flip_display();
			retVal = nextStep(simu);
			if (retVal) {	// Avanzo un paso en la simulacion
				quit = true;			// Y me fijo si hay que parar la simulacion
			}
			redraw = false;
		}

	} while (!quit);

	return retVal;

}

void drawChart(double *values, int size) {

	al_clear_to_color(al_color_name("white"));

	double maxVal = getMaxValue(values, size);

	double stepX = (double)(DISP_WIDTH - 2 * CHART_OFFSET) / (size+1.0);

	for (int i = 1; i <= size; i++) {

		double posX = CHART_OFFSET + stepX * i;
		double posY = CHART_OFFSET + (double)(DISP_HEIGHT - 2.0 * CHART_OFFSET - 30.0) * values[i-1] / maxVal;
		
		// Punto en el gráfico
		//al_draw_filled_circle(posX, Y_TO_ALL(posY), 10.0, al_color_name("darkcyan"));

		// Barra tipo histograma
		al_draw_line(posX, Y_TO_ALL(CHART_OFFSET), posX, Y_TO_ALL(posY), al_color_name("deepskyblue"), 0.8*stepX);
		
		// Marcas en el eje X
		al_draw_line(posX, Y_TO_ALL(CHART_OFFSET-5), posX, Y_TO_ALL(CHART_OFFSET+5), al_color_name("black"), 3);
		al_draw_textf(font, al_color_name("black"), posX, Y_TO_ALL(CHART_OFFSET-7), ALLEGRO_ALIGN_CENTRE, "%d", i);

		// Marcas en el eje Y
		//al_draw_line(CHART_OFFSET - 5, Y_TO_ALL(posY), CHART_OFFSET + 5, Y_TO_ALL(posY), al_color_name("black"), 3);
		//al_draw_textf(font, al_color_name("black"), CHART_OFFSET-30, Y_TO_ALL(posY), ALLEGRO_ALIGN_CENTRE, "%.2f", values[i-1]);

		// Texto sobre la barra
		al_draw_textf(font, al_color_name("black"), posX, Y_TO_ALL(posY+10), ALLEGRO_ALIGN_CENTRE, "%.2f", values[i-1]);
		
	}
	
	// Dibujo los ejes
	Point p0 = {CHART_OFFSET, Y_TO_ALL(CHART_OFFSET)};
	Point px = {DISP_WIDTH - CHART_OFFSET, Y_TO_ALL(CHART_OFFSET)};
	Point py = {CHART_OFFSET, Y_TO_ALL(DISP_HEIGHT - CHART_OFFSET)};
	drawArrow(&p0, &px, al_color_name("black"));
	drawArrow(&p0, &py, al_color_name("black"));

	al_flip_display();
}


// inicializa los recursos necesarios para la interfaz gráfica
// Devuelve 0 si se completo exitosamente, 1 si hubo error
int initUI() {

	if (!al_init()) {
		printf("No se pudo inicializar Allegro\n");
		return 1;
	}

	if (!al_init_primitives_addon()) {
		printf("No se pudo inicializar el plugin de primitivas\n");
		return 1;
	}

	if (!al_init_font_addon()) {
		printf("No se pudo inicializar el plugin de fuentes\n");
		return 1;
	}

	if (!al_install_keyboard()) {
		printf("No se pudo inicializar el teclado\n");
		return 1;
	}

	font = al_create_builtin_font();
	if (!font) {
		printf("No se pudo cargar la fuente");
		return 1;
	}

	display = al_create_display(DISP_WIDTH, DISP_HEIGHT);
	if (!display) {
		printf("No se pudo inicializar el display\n");
		return 1;
	}

	queue = al_create_event_queue();
	if (!queue) {
		printf("No se pudo inicializar la event queue\n");
		return 1;
	}

	al_register_event_source(queue, al_get_display_event_source(display));

	return 0;
}

void destroyUI() {
	al_destroy_display(display);
	al_destroy_event_queue(queue);
	al_destroy_timer(timer);
	al_shutdown_primitives_addon();
	al_destroy_font(font);
	al_shutdown_font_addon();
	al_uninstall_keyboard();
}

// Static function definition

static void drawFloor(Floor* floor) {
	int w = floor->width;
	int h = floor->height;

	double tileLength = min(CALC_TILE_LENGTH(DISP_WIDTH, w), CALC_TILE_LENGTH(DISP_HEIGHT, h));	// Me quedo con la minima medida entre ancho y alto
																								// Y uso esa para la baldosa cuadrada

	double initGapX = (DISP_WIDTH - w*(tileLength+GAP_BTW_TILES) + GAP_BTW_TILES)/2.0;	// Calculo del gap inical, para centrar el piso
	double initGapY = (DISP_HEIGHT - h*(tileLength+GAP_BTW_TILES) + GAP_BTW_TILES)/2.0;

	al_clear_to_color(al_color_name("black"));

	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			double startX = initGapX + (tileLength + GAP_BTW_TILES) * j;
			double startY = initGapY + (tileLength + GAP_BTW_TILES) * i;
			bool state = floor->tiles[i*w+j];
			al_draw_filled_rectangle(startX, Y_TO_ALL(startY), startX+tileLength, Y_TO_ALL(startY + tileLength), state ? al_color_name("white") : al_color_name("gray"));
		}
	}

}

// Dibuja una flecha desde el punto de origen hasta el de destino
// con las coordenadas stadard de Allegro
static void drawArrow(Point* orig, Point* dest, ALLEGRO_COLOR color) {

	double angle = getAngleBetweenPoints(*dest, *orig);
	Point p2 = translatePoint(*dest, 10.0, STANDARD_ANGLE(angle+45.0));
	Point p3 = translatePoint(*dest, 10.0, STANDARD_ANGLE(angle-45.0));
	Point pMid = translatePoint(*dest, 5.0, STANDARD_ANGLE(angle));

	al_draw_line(orig->x, orig->y, pMid.x, pMid.y, color, 5);
	al_draw_filled_triangle(dest->x, dest->y, p2.x, p2.y, p3.x, p3.y, color);
	

}

static double getMaxValue(double* values, int size) {
	
	double max = values[0];

	for (int i = 1; i < size; i++) {
		if (values[i] > max) {
			max = values[i];
		}
	}

	return max;
}

// Transforma coordenadas del Floor en puntos para graficar en Allegro
static Point pointToAll(Point p, int w, int h) {

	// 1 unidad = medida del lado de la baldosa
	double tileLength = min(CALC_TILE_LENGTH(DISP_WIDTH, w), CALC_TILE_LENGTH(DISP_HEIGHT, h));	// Me quedo con la minima medida entre ancho y alto
																								// Y uso esa para la baldosa cuadrada

	double gapX = (DISP_WIDTH - w * (tileLength + GAP_BTW_TILES) + GAP_BTW_TILES) / 2.0;	// Calculo del gap inical y final, para centrar el piso
	double gapY = (DISP_HEIGHT - h * (tileLength + GAP_BTW_TILES) + GAP_BTW_TILES) / 2.0;

	p.x = gapX + tileLength*p.x + GAP_BTW_TILES*(int)p.x;
	p.y = gapY + tileLength*p.y + GAP_BTW_TILES*(int)p.y;

	p.y = Y_TO_ALL(p.y);	// Eje Y invertido en Allegro
	
	return p;

}