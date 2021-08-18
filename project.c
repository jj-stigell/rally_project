#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define N 45

/*lastname and team limited to N = 45 because longest word in english dictionary is: 
pneumonoultramicroscopicsilicovolcanoconiosis
No one is stupid enough to have longer name than that.
*/

struct driver {
    char lastname[N]; 
    char team[N];
    int hours;
	int minutes;
	int seconds;
	int total_time; //used for sorting drivers from fastest to slowest.
	struct driver *next; //pointer for next in linked list.
};
struct driver *head = NULL;
struct driver *current = NULL;

int new_driver(struct driver *add_new, char *input_lastname, char *input_team) {
	
	//keeping track of how many drivers have been created.
	static int number_of_drivers = 0;

	//add a new driver, time set to 0. updated time using function update_total_time.
	strcpy(add_new->lastname , input_lastname);
	strcpy(add_new->team , input_team);
	add_new->hours = 0;
	add_new->minutes = 0;
	add_new->seconds = 0;
	add_new->total_time = 0;
	add_new->next = head;
	head = add_new;
	number_of_drivers++;
	
	return number_of_drivers;
}

void update_total_time(char *lastname, int hours, int minutes, int seconds) {
	
	int driver_exist = 0; //used for checking if any drivers exist, set 1 in while loop if lastname found.
	struct driver *ptr = head;
	while (ptr != NULL) {
		if(strcmp(ptr->lastname, lastname) == 0) {
			driver_exist = 1;
			//sum together current recorded time and new time.
			hours += ptr->hours;
			minutes += ptr->minutes;
			seconds += ptr->seconds;
			
			//add one minute for every 60 seconds.
			while (seconds >= 60) {
				seconds -= 60;
				minutes++;
			}
			
			//add one hour for every 60 minutes.
			while (minutes >= 60) {
				minutes -= 60;
				hours++;
			}
			
			//update the time to driver
			ptr->hours = hours;
			ptr->minutes = minutes;
			ptr->seconds = seconds;
			ptr->total_time= hours * 3600 + minutes * 60 + seconds;
		}
		ptr = ptr->next;
	}
	if (!driver_exist) {
		printf("Driver does not exist, please initialize the driver with command: \"A Lastname Team\"\n");
	}
}

void print_situation(int number_of_drivers) {
	
	struct driver *ptr = head;
	if (ptr == NULL) {
		printf("No drivers added\n");
	} else {
		while(ptr != NULL) {
			printf("Driver: %s, Team: %s, Time: %d:%02d:%02d\n",ptr->lastname, ptr->team, ptr->hours, ptr->minutes, ptr->seconds);
			ptr = ptr->next;
		}
	}
}

void save_results(char *save_name) {
	
	FILE *file_ptr = NULL;
	char file_name[1000] = "";
	snprintf(file_name, sizeof(file_name), "%s.txt", save_name); //create file name from user input ja add .txt to create text file.
	
    file_ptr = fopen(file_name, "w");
    if (NULL != file_ptr) {
		
		struct driver *ptr = head;
		if (ptr == NULL) {
			printf("No drivers added\n"); //if the first points to NULL then there is no drivers.
		} else {
			while(ptr != NULL) {
				char line[1000] = ""; 
				snprintf(line, sizeof(line), "Driver: %s, Team: %s, Time: %d:%02d:%02d\n",ptr->lastname, ptr->team, ptr->hours, ptr->minutes, ptr->seconds);
				fputs(line, file_ptr);
				ptr = ptr->next;
			}
			printf("File saved successfully\n");
			fclose(file_ptr);
		}
	} else {
		printf("File open error\n");
	}
}

void open_results() {
}

int main(void) {
	int quit = 1, hours, minutes, seconds, number_of_drivers;
	char input[40], lastname[20], command[1], team[20];
	
	while(quit) {
		
		printf("COMMANDS:\nA: Add driver\nU: Update total time\nL: Print results\nW: Save results\nO: Load results\nQ: Exit program\n");
		
		//input from user, limited to max 1000 characters.
		fgets(input, 1000, stdin);
		
		//Memory allocation for the struct.
		struct driver *add_new = (struct driver*) malloc(sizeof(struct driver));
		
		switch(input[0])
			{
				case 'A':
					sscanf(input, "%s %s %s %d %d %d", command, lastname, team, &hours, &minutes, &seconds);
					number_of_drivers = new_driver(add_new, lastname, team);
					//printf("number of drivers in the game is: %d\n\n", number_of_drivers);
					break;
				case 'U':
					sscanf(input, "%s %s %d %d %d", command, lastname, &hours, &minutes, &seconds);
					update_total_time(lastname, hours, minutes, seconds);
					break;
				case 'L':
					print_situation(number_of_drivers);
					break;
				case 'W':
					sscanf(input, "%s %s", command, lastname);
					save_results(lastname);
					break;
				case 'O':
					sscanf(input, "%s %s", command, lastname);
					open_results();
					break;
				case 'Q':
					free(add_new); //Free struct memory allocation.
					quit = 0; //to exit while loop make "quit" = False.
					break;				
				default:
					printf("Wrong input, not A, U, L W O or Q\n");
			}
	}
}
