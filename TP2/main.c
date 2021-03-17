#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simulacion.h"
#include "robotALL.h"
#include "libparse.h"
#include "userData.h"

#define ON 1
#define OFF 0
int isanumber(char* value);
int isanint(char* value);
int isauint(char* value);
int Callback(char *key, char *value, void *uData);
void help(void);

int main(int argc, char* argv[]) // Param 1 (mandatorio) = IP propia
{								 // Param 2 (opcional) = Inicial
		
	userData Flags = { OFF, OFF, OFF, OFF, NULL};				//Init flags for argument validation
	userData uData = { 0, 0, 1, 0, &Flags };					//Default values
	int timeticks = 0;
	double averageticks = 0;
	double prev_average = 1; //Initial value to assure the for in mode 2 is run at least once.
	unsigned int optimal=0;
	double* averages=NULL; 
	double* aux_ptr=NULL;
	if (argc == 1) {//If the program was called with no cmd line arguments, print the user help.
		help();
		system("PAUSE");
		return -1;
	}
	int error = OFF; //Error state, for exit purposes.
	if (parseCmdLine(argc, argv, &Callback, &uData) != PARSECMD_FAIL) { //Mandatory cmd line arguments must be filled for program to run.
		if (!initUI()) {//Init all stuff needed for allegro
			if (!uData.mode) { //if mode 1, single fire simulation.

				timeticks = simulacion(uData.robots, uData.height, uData.width, uData.mode);
				printf("With %d robots, a %d by %d floor took %d ticks or steps to clean.\n", uData.robots, uData.height, uData.width, timeticks);
			}
			else {

				averages = malloc(sizeof(double));
				if (!averages) {
					error = ON;
					printf("ERROR ALLOCATING MEMORY.\n");
					exit(-1);
				}

				for (unsigned int n = 1; prev_average - averageticks > 0.1; n++) { //if mode 2, compute averages.
					prev_average = averageticks;
					averageticks = 0;
					for (unsigned int i = 0; i < 1000; i++) {
						timeticks = simulacion(n, uData.height, uData.width, uData.mode);
						averageticks += timeticks / 1000.0;
					}
					
					//reallocate
					aux_ptr = realloc(averages, n * sizeof(double));
					if (!aux_ptr) {
						free(averages);
						error = ON;
						printf("ERROR ALLOCATING MEMORY.\n");
						exit(-1);
					}
					else {
						averages = aux_ptr;
						averages[n - 1] = averageticks;
					}
					drawChart(averages, n);
					if (!prev_average) { prev_average = averageticks + 1;}
					if (averageticks > prev_average)
						optimal = n - 1;
					else
						optimal = n;
				}
				printf("After %d simulations, the graph presented is the average amount of ticks till floor is clean for each number of robots until optimal number is reached. The optimal number of robots to clean a floor %d by %d is %d.\n", 1000 * optimal, uData.height, uData.width, optimal);
			}
			system("PAUSE");
			if (averages != NULL) { free(averages); }
			destroyUI();
		}
		else {
			printf("Error: Allegro failed to start.\n");
			error = ON;
		}
	}
	else{
		error = ON;
		help();		//Ante un error en los parametros, se muestra el texto para nuevos usuarios.
	}

	if (error) {
		system("PAUSE"); 
		return -1;
	}
	else return 0;
}


int Callback(char *key, char *value, void *uData) {

	int errfl = ON;						//Dado que no se aceptan más de una misma llamada a un parametro, se utiliza un flag 
										//para verificar que el parametro no fue llamado anteriormente.
	userData *uD = (userData*)uData;	//Casteo el puntero a void que estaba por portabilidad.	

	if (key == NULL) {
		if (!strcmp(value, "help")) {		//Se agrego un help para el uso del programa. Este es su llamado explicito, pero se llama
			return PARAMS_INVALID;			//ante cualquier error en los parametros de entrada.
		}
		else {
			printf("Error: Program only accepts \"-key value\" type arguments, help being the only exception.\n");
			return PARAMS_INVALID;
		}
	}
	else if (!strcmp(key, "h") || !strcmp(key, "H")) {
		if (uD->Flags->height == OFF) {
			if (isauint(value)) {
				uD->height = strtol(value, NULL, 10);
				if ((uD->height > 70) || (uD->height < 1)) {
					printf("Error: Value for key -%s is out of valid range!\n", key);
					return PARAMS_INVALID;
				}
				uD->Flags->height = ON;
				errfl = OFF;
				
			}
			else {
				printf("Error: Value for key -%s in not an unsigned interger number!\n",key);
				return PARAMS_INVALID;
			}
		}else{
			printf("Error: Multiple -%s arguments!\n", key); 
			return PARAMS_INVALID;
		}
	}
	else if (!strcmp(key, "w") || !strcmp(key, "W")) {
		if (uD->Flags->width == OFF) {
			if (isauint(value)) {
				uD->width = strtol(value, NULL, 10);
				if ((uD->width > 100) || (uD->width < 1)) {
					printf("Error: Value for key -%s is out of valid range!\n", key);
					return PARAMS_INVALID;
				}
				uD->Flags->width = ON;
				errfl = OFF;
			}
			else {
				printf("Error: Value for key -%s in not an unsigned interger number!\n", key);
				return PARAMS_INVALID;
			}
		}
		else {
			printf("Error: Multiple -%s arguments!\n", key);
			return PARAMS_INVALID;
		}
	}
	else if (!strcmp(key, "r") || !strcmp(key, "R")) {
		if (uD->Flags->robots == OFF) {
			if (isauint(value)) {
				uD->robots = strtol(value, NULL, 10);
				if (!uD->robots) {
					printf("Error: Value for key -%s can't be zero!\n", key);
					return PARAMS_INVALID;
				}
				uD->Flags->robots = ON;
				errfl = OFF;
			}
			else {
				printf("Error: Value for key -%s in not an unsigned interger number!\n", key);
				return PARAMS_INVALID;
			}
		}
		else {
			printf("Error: Multiple -%s arguments!\n", key);
			return PARAMS_INVALID;
		}
	}
	else if (!strcmp(key, "m") || !strcmp(key, "M") || !strcmp(key, "mode") || !strcmp(key, "Mode") || !strcmp(key, "mOde") || !strcmp(key, "MOde") || !strcmp(key, "moDe") || !strcmp(key, "MoDe") || !strcmp(key, "mODe") || !strcmp(key, "MODe") || !strcmp(key, "modE") || !strcmp(key, "ModE") || !strcmp(key, "mOdE") || !strcmp(key, "MOdE") || !strcmp(key, "moDE") || !strcmp(key, "MoDE") || !strcmp(key, "mODE") || !strcmp(key, "MODE")) {
		if (uD->Flags->mode == OFF) {
			if (!strcmp(value, "1") || !strcmp(value, "2")) {
				uD->mode = strtol(value, NULL, 10)-1;
				uD->Flags->mode = ON;
				errfl = OFF;
			}
			else {
				printf("Error: Value for key -%s is an invalid mode!\n", key);
				return PARAMS_INVALID;
			}
		}
		else {
			printf("Error: Multiple -%s arguments!\n", key);
			return PARAMS_INVALID;
		}
	}
	

	if(errfl == ON) {
		printf("Error: Invalid key -%s!\n", key);
		return PARAMS_INVALID;
	}
	return PARAMS_VALID;
}

int isanumber(char* value) {		//No standard function to check if the value string was a double unless using sscanf. We made our own. 
	int fldash = OFF;
	int flpoint = OFF;
	for (int i = 0; i< (int)strlen(value); i++) {
		if (isdigit(value[i])); else if (value[i] == '.') if (flpoint == OFF) flpoint = ON; else return 0; else if (value[i] == '-') if (fldash == OFF) fldash = ON; else return 0; else return 0;
	}
	return 1;
}

int isanint(char* value) {		//Tweaked "isanumber" to detect intergers instead of doubles. 
	int fldash = OFF;
	int flpoint = ON;
	for (int i = 0; i< (int)strlen(value); i++) {
		if (isdigit(value[i])); else if (value[i] == '.') if (flpoint == OFF) flpoint = ON; else return 0; else if (value[i] == '-') if (fldash == OFF) fldash = ON; else return 0; else return 0;
	}
	return 1;
}

int isauint(char* value) {		//Tweaked "isanumber" even further to detect unsigned intergers. 
	int fldash = ON;
	int flpoint = ON;
	for (int i = 0; i< (int)strlen(value); i++) {
		if (isdigit(value[i])); else if (value[i] == '.') if (flpoint == OFF) flpoint = ON; else return 0; else if (value[i] == '-') if (fldash == OFF) fldash = ON; else return 0; else return 0;
	}
	return 1;
}

void help(void) {									//Created for easy use to new users.
	printf("Welcome user!\n");
	printf("This text was written to help new users with the parameters and limitations of the program.\n");
	printf("\n Key Parameters: (Non case sensitive)\n");
	printf("\n Use -m or mode to choose the simulation mode of the program. This argument is mandatory. The two modes available are:\n");
	printf("Mode 1: Single fire simulation. A number of robots are set randomly on the floor to clean it. The simulation provides a visual progress of the test. The program returns the time the simulation took to complete.\n");
	printf("Mode 2: Optimization mode. Simulations with ascending number of robots are run 1000 times over to retrieve the average time to completion. The program stops once the last simulation improved in less than 0.1 seconds from the previous. A histogram detailing the different averages of each number of robots is then presented to the user.\n");
	printf("Use -h  to define the height of the floor to be cleaned during simulation. The value is an interger with the range [1, 100]. This argument is mandatory.\n");
	printf("Use -w  to define the width of the floor to be cleaned during simulation. The value is an interger with the range [1, 70]. This argument is mandatory.\n");
	printf("Use -r  to define the number of robots to be used during simulation in single fire mode (Mode 1). The value is an positive interger. This argument is optional. The default is 1. This argument will be ignored if Optimization mode is chosen.\n");
	printf("\nNon-Key Parameters: (Non case sensitive)\n");
	printf("\nUse 'help' to access this menu. But you already knew that, right?\n");
	printf("\n The program runs a simulation of cleaner robots cleaning tiles on a dirty floor.\n");
	printf("Each tile measures 1x1 units, and the robots start in random locations represented by doubles on the entire floor.\n");
	printf("Robots begin with a random orientation, and change to a new random orientation within a type double angle bewteen 0 and 360 whenever they would move out of boundaries.\n");
	printf("Each tick of the simulation robots move a single unit and clean the tile they are standing on.\n");
	printf("Once the floor is fully cleaned a simulation is considered finished, and the number of ticks it took is returned.\n");
	printf("Once the program is done press any key to exit.\n");
}