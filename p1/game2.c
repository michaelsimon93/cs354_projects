/*Assignment 1: Guessing Game (ascii file)
 * by Zach Myers, section 2
 * by Michael Simon, section 2
 *
 * This program reads ascii integers from a file and places the into
 * a linked list. Once integers are in a linked list the program will 
 * prompt the user to make an integer guess. Then the prorgam will check 
 * the list of numbers to see if the guess is in the list. The program 
 * will then let the player know if their gues was in the list. When the player
 * is finished playing the must enter 'q'.
 */ 
#include <stdio.h>
#include <fcntl.h>
#include <assert.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

/*node is a structure that contains a number and a pointer to another node.
 * and sets the structur to type Node. A node is used to create a linked list.
 *
 * There are no parameters for this struct.
 */ 
typedef struct node {
	int thenum;
	struct node *next;
}Node;

/*listadd will take the head, which is a pointer to the first item in a linked list,
 * and a new number that need to be added to the list to create a node and  add to it
 * the front of the linked list.
 *
 * Returns a pointer to the frist node, head.
 *
 * Prarmeters:
 * head: a pointer that points to the first node in a list
 * intforlist: the number that needs to be added to the linked list
 */
Node * listadd(Node *head, int intforlist){
	Node *newLink;

	newLink = malloc(sizeof(Node));/*allocates space for a node*/		
	newLink->thenum = intforlist;
	newLink->next = head;
	head = newLink;
	
	return head;
}

/*createlist will take a pointer to a FILE read the integers and place them into
 * an linked list. This function will create a linked list. Each node will
 * contain one of the number from the input file.
 *
 * Returns a pointer to the head of the linked list. NULL if empty.
 *
 * Parameters:
 * fd: a pointer the file the contains the integers
 */ 
Node * createlist(FILE *fd){
	/*Declaration of Variable*/
	Node *head;
	int filesize, itr, intfd;
	struct stat stbuf;
	char *numarray;
	
	/*Initializtion of Variable*/
	head = NULL;
	intfd = fd->_fileno;/*gets file directory number from file pointer*/
	
	fstat(intfd, &stbuf);
	filesize = stbuf.st_size;
	numarray = malloc(filesize);/*allocates space for the file input*/
	
	/*gets the information from the file and creates a linked list*/
	while(fgets(numarray, filesize, fd)){
		head = listadd(head, atoi(numarray));
	}
	
	/*closes and checks to make sure closed correctly*/
	if(fclose(fd) == EOF){
		fprintf(stderr, "error closing file -- quiting");
		exit(1);
	}

	return head;
}

/*inlist will take a pointer to the first node in a list, head and a value to 
 * search through the list. If the value is found in the list it will return
 * a 1 if not it will return a 0;
 *
 * Returns an int, 1 if value was found. 0 if it wasn't.
 *
 * Parameteres:
 * listhead: a pointer to the first node in the list, head.
 * invalue: the number to be search for in the linked list.
 */
int inlist(Node * listhead, int intvalue){
	/*searches through linked list*/
	while(listhead != NULL){
		if((listhead->thenum) == intvalue){
			return 1;/*will exit and return 1*/
		}
		listhead = listhead->next;
	}
	/*only gets to this code if value was not found*/
	return 0;
}

/*playgame will implement the user input and prompting for the game. This function 
 * will ask for user input, use the user input to search the list and return the 
 * user with an output to the screen if the value was found or not. If the user 
 * wants to quit the game they must enter 'q';
 *
 * Returns nothing
 *
 * Parameters:
 * listhead: a pointer to the first node in a linked list, head.
 */
void playgame(Node * listhead){
	/*Declaration of Variables*/
	/*11 beacuse largest 32 bit number has 10 deciaml digits and one for newline*/
	char  input[11];
	char *pinput;
	int guess, quit;
	
	/*Initializes Variables*/
	pinput = input;
	quit = 1;
	while(quit){
	
		printf("Integer guess: ");
		fgets(pinput, sizeof(input), stdin);/*gets the users input*/
		
		/*allows the user to guess something with q at the first locaton*/
		if(input[0] == 'q' && input[1] == '\n'){
			quit = 0;
		}else{
			guess = atoi(input);/*changes char array to int*/
			
			/* tells user if he/she was right or wrong*/
			if(inlist(listhead, guess) == 1){
				printf("%d is in the list.\n", guess);
			}else{
				printf("%d is not in the list.\n", guess);
			}
		}
	}
}

/*main will make sure there are enough command line arguments and open a file 
 * to be used in createlist. Then it will call playgame so the user can play the
 * guessing game. Implements the functions.
 *
 * Parameters:
 * arg: The number of white space seperated strings on command line
 * argv[]: Array of pointers to charaters.
 */
int main(int argc, char *argv[]){

	FILE *fd;
	char *numfile;
	Node *head;
		
	/*Checks command arguments*/
	if(argc != 2) {
		fprintf(stderr, "usage: stats <file>\n");
		exit(1);
	}
	
	numfile = argv[1];
	fd = fopen(numfile, "r");/*opens file for reading*/
		
	head = createlist(fd); /*creates list*/
	playgame(head);	/*implements game*/	
	
	return 0;	
}
