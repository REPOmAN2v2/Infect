#include <string.h>
#include <ctype.h>
#include "generation.h"

void parseArgs(int argc, char **argv)
{
	if (argc == 1)	return;

	for (size_t i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-x") == 0) {
			if (i + 1 <= argc - 1) {
				++i;
				char *end;
				int val = strtol(argv[i], &end, 10);
				if (end != NULL && !end[0] && val >= 0) {
					X = val;
					continue;
				} else {
					goto error;
				}
			}
		} else if (strcmp(argv[i], "-y") == 0) {
			if (i + 1 <= argc - 1) {
				++i;
				char *end;
				int val = strtol(argv[i], &end, 10);
				if (end != NULL && !end[0] && val >= 0) {
					Y = val;
					continue;
				} else {
					goto error;
				}
			}
		} else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
			printHelp();
		}
	}

	return;

	error:
		fprintf(stdout, "Unrecognised option(s)\n");
		printHelp();
}

void printHelp()
{
	//fprintf(stdout, "--map <str>\t\tSpecify a file to read from containing a map\n");
    fprintf(stdout, "--x <int>\t\tUse in conjunction with --y <int> to specify dimensions of auto-generated map\n");
    fprintf(stdout, "--y <int>\t\tSee above\n");
    //fprintf(stdout, "--slow\t\t\tRun the simulation with slow speed. Very slow.\n");
    //fprintf(stdout, "--fast\t\t\tRun the simulation with fast speed. Almost real-time!\n");
    //fprintf(stdout, "--fastest\t\tRun the simulation at fastest speed.\n");
    fprintf(stdout, "Default values are x=100, y=50, fast speed\n");
    exit(EXIT_FAILURE);
}

void initialise(Board board[][X])
{
	countDoc = 10, countSol = 1, countInf = 3;
	countCit = X*Y - countDoc - countSol - countInf;

	defaultBoard(board);	
	generateCoord(board, countSol, SOL);
	generateCoord(board, countInf, INF);
	generateCoord(board, countDoc, DOC);
}

void defaultBoard(Board board[][X])
{
	for (size_t i = 0; i < Y; i++) {
		for (size_t j = 0; j < X; j++) {
			board[i][j].character = CIT;
			board[i][j].direction = NONE;
		}
	} 
}

void generateCoord(Board board[][X], const int count, Characters type)
{
	int x = 0, y = 0;

	for (size_t i = 0; i < count; i++) {
		do {
			x = rand()%X;
			y = rand()%Y;
		} while(board[y][x].character != CIT);
		
		board[y][x].character = type;
	} 
}