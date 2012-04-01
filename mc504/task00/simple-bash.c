/**
 * Tarefa 00 - Interpretador de comandos
 * Turma: MC504A
 * Data: 16/03/2012
 *
 * 103520 - Matheus Smythe Svolenski
 * 106228 - Rafael Timbó Matos
 */

// INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// ERROR CONSTANTS
#define ERR_CMD_NOT_FOUND -1	// Command was not found
#define ERR_MISSING_DIR   -2	// Missing directory in program arguments

// GENERAL CONSTANTS
#define TOKEN               ":"
#define MAX_STR_SIZE        150


/**
 *  Deallocs the memory designated
 *
 *	@param char** pointer	Pointer to the memory to be cleaned
 */
void mem_free(char** pointer) {
	// Declaring variables
	int i;
	
	// Cleaning memory
	for (i = 0; pointer[i] != NULL; i++)
		free(pointer[i]);
	free(pointer);
}

/**
 *  Get the parameters for the command
 *
 *  @return char** All the parameters
 */
char** get_parameters() {
	// Declaring variables
	char c;
	char **param = NULL;
	int i = 0;
	
	// Separating parameters
	c = getchar();
	while (c != '\n') {
		if (!(c == '\t' || c == ' ')) {                                 // Verifies if the character is not a space or a tab
			ungetc(c, stdin);                                           // Returns to the last letter
			param = (char**) realloc(param, sizeof(char*) * (i + 1));   // Adds a string pointer to the list
			param[i] = (char*) malloc(MAX_STR_SIZE * sizeof(char));     // Allocates the pointer to get the string
			scanf("%s", param[i]);
			param[i] = (char*) realloc(param[i], sizeof(char) * (strlen(param[i]) + 1)); // Resizes the string space
			i++;
		}
		c = getchar();
	}
	param = (char**) realloc(param, sizeof(char*) * (i + 1));       // Adds a string pointer to the list
	param[i] = NULL;                                                // Last pointer null, as required.
		
	return param;
}

int main (int argc, char *argv[]) {
	// Declaring variables
	char conc[MAX_STR_SIZE];
	char *cur_dir, *path = argv[1];
	char **args = get_parameters();
	int tmp = 0;

	// Checking arguments
	if (argc <= 1) {
		printf("usage: path/simple-bash path1:path2:...:pathn\n");
		return ERR_MISSING_DIR;
	}

	// Searching for the paths
	cur_dir = strtok(path, TOKEN);
	while (cur_dir != NULL) {
		tmp = strlen(cur_dir) - 1;
		if (cur_dir[tmp] == '/') cur_dir[tmp] = '\0';   //Remove final /, if it exists, in cur_dir to avoid repetition of it.
		sprintf(conc, "%s/%s", cur_dir, args[0]);		// Concatenates complete path, inserting '/'
		tmp = execv(conc, args);						//System Call
		cur_dir = strtok(NULL, TOKEN);                  //Search for the command in the next directory.
	}
	
	mem_free(args);
	
	if (tmp == ERR_CMD_NOT_FOUND)
		printf("Command not found in the provided paths.\n");
	
	return 0;
}
