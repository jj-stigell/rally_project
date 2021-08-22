#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define N 45

struct driver {
	//database for drivers
    char lastname[N]; 
    char team[N];
    int hours;
	int minutes;
	int seconds;
	struct driver *next; //pointer for next in linked list.
};
struct driver *head = NULL;

int driver_exist(char *lastname) {
	
	/*checks if the driver already exist.*/
	struct driver *ptr = head;
	
	while (ptr != NULL) {
		if(strcmp(ptr->lastname, lastname) == 0) {
			return 1; //driver found.
		}
		ptr = ptr->next;
	}
	return 0; //if driver is not found from database.
}

void new_driver(struct driver *add_new, char *input_lastname, char *input_team, int arguments) {
	
	if (arguments < 3) { //check that user inputted correct amount of data.
		printf("A should be followed by exactly 2 arguments.\n");
	} else if (driver_exist(input_lastname)) { //check if driver exist. 
		printf("Driver \"%s\" is already in the database.\n", input_lastname);
	} else {
		/*add a new driver, initial time set to 0. 
		updated driver time using function update_total_time.*/
		strcpy(add_new->lastname , input_lastname);
		strcpy(add_new->team , input_team);
		add_new->hours = 0;
		add_new->minutes = 0;
		add_new->seconds = 0;
		add_new->next = head;
		head = add_new;
		printf("SUCCESS\n");
	}
}

void update_total_time(char *lastname, int hours, int minutes, int seconds, int arguments) {
	
	if (arguments < 5) { 
		printf("U should be followed by exactly 4 arguments.\n");
	}  else if (!driver_exist(lastname)) { //check if driver exist. 
		printf("Driver \"%s\" does not exist.\n", lastname);
	} else if (hours < 0) { //check if hours are negative.
		printf("Hour cannot be negative.\n");
	} else if (minutes < 0 || minutes > 59) { //check if minutes are negative or over 59
		printf("Minute cannot be negative or greater than 59.\n");
	} else if (seconds < 0 || seconds > 59) { //check if seconds are negative or over 59
		printf("Second cannot be negative or greater than 59.\n");
	} else {
	
		struct driver *ptr = head;
		
		while (ptr != NULL) {
			if(strcmp(ptr->lastname, lastname) == 0) {
				/*sum together current recorded time and new time.*/
				hours += ptr->hours;
				minutes += ptr->minutes;
				seconds += ptr->seconds;

				/*add one minute for every 60 seconds.*/
				while (seconds > 59) {
					seconds -= 60;
					minutes++;
				}

				/*add one hour for every 60 minutes.*/
				while (minutes > 59) {
					minutes -= 60;
					hours++;
				}

				/*update the time to drivers database*/
				ptr->hours = hours;
				ptr->minutes = minutes;
				ptr->seconds = seconds;
			}
			
			ptr = ptr->next;
		}
		printf("SUCCESS\n");
	}
}

void print_situation() {
	
	/*print current database and its drivers one by one. 
	printing order is the order drivers are inputted*/
	struct driver *ptr = head;
	if (ptr == NULL) {
		printf("No drivers added\n");
	} else {
		while(ptr != NULL) {
			printf("%s %s %d %02d %02d\n",ptr->lastname, ptr->team, ptr->hours, ptr->minutes, ptr->seconds);
			ptr = ptr->next;
		}
	}
}

void save_results(char *filename) {
	
	FILE *file_ptr = NULL;
    file_ptr = fopen(filename, "w"); //open or create file for writing.
    if (NULL != file_ptr) {
		
		struct driver *ptr = head;
		if (ptr == NULL) {
			printf("No drivers added\n"); //if the first points to NULL then there is no drivers.
		} else {
			while(ptr != NULL) {
				char line[1000] = ""; 
				snprintf(line, sizeof(line), "%s %s %d %02d %02d\n",ptr->lastname, ptr->team, ptr->hours, ptr->minutes, ptr->seconds);
				fputs(line, file_ptr);
				ptr = ptr->next;
			}
			printf("SUCCESS\n");
			fclose(file_ptr);
		}
	} else {
		printf("File open error\n");
	}
}

void open_results() {
	/*Not implemented*/
}

int main(void) {
	
	int quit = 1, hours, minutes, seconds, arguments;
	char input[40], command[1], lastname[20], team[20];
	
	while(quit) {
		
		//input from user, limited to max 1000 characters.
		fgets(input, 1000, stdin);
		
		//Memory allocation for the struct.
		struct driver *add_new = (struct driver*) malloc(sizeof(struct driver));
		
		switch(input[0])
			{
				case 'A':
					arguments = sscanf(input, "%s %s %s %d %d %d", command, lastname, team, &hours, &minutes, &seconds);
					new_driver(add_new, lastname, team, arguments);
					break;
				case 'U':
					arguments = sscanf(input, "%s %s %d %d %d", command, lastname, &hours, &minutes, &seconds);
					update_total_time(lastname, hours, minutes, seconds, arguments);
					break;
				case 'L':
					print_situation();
					break;
				case 'W':
					sscanf(input, "%s %s", command, lastname); //lastname will be the file name.
					save_results(lastname);
					break;
				case 'O':
					sscanf(input, "%s %s", command, lastname); //lastname will be the file name.
					open_results();
					break;
				case 'Q':
					free(add_new); //Free struct memory allocation.
					quit = 0; //to exit while loop make "quit" = False.
					break;				
				default:
					sscanf(input, "%s", command);
					printf("Invalid command %s\n", command);
			}
	}
	printf("SUCCESS\n");
}
