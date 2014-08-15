#include <string.h>
#include <ctype.h>
#include "generation.h"

unsigned int X = 79, Y = 20;

Board ** parseArgs(int argc, char **argv)
{
	Board **board = NULL;

	if (argc == 1)	return initDefault();

	int map = 0, size = 0;

	for (size_t i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-x") == 0) {
			if (i + 1 <= argc - 1) {
				if (!map) {
					++i;
					char *end;
					int val = strtol(argv[i], &end, 10);
					if (end != NULL && !end[0] && val >= 0) {
						X = val;
						size = 1;
						continue;
					} else {
						goto error;
					}
				} else {
					goto conflict;
				}
			}
		} else if (strcmp(argv[i], "-y") == 0) {
			if (i + 1 <= argc - 1) {
				if (!map) {
					++i;
					char *end;
					int val = strtol(argv[i], &end, 10);
					if (end != NULL && !end[0] && val >= 0) {
						Y = val;
						size = 1;
						continue;
					} else {
						goto error;
					}
				} else {
					goto conflict;
				}
			}
		} else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
			printHelp();
		} else if (strcmp(argv[i], "-m") == 0 || strcmp(argv[i], "--map") == 0) {
			if (i + 1 <= argc - 1) {
				if (!size) {
					++i;
					board = getMap(argv[i]);
					map = 1;
				} else {
					goto conflict;
				}
			}
		} else if (strcmp(argv[i], "--slow") == 0) {
			refreshRate = 10;
		} else if (strcmp(argv[i], "--fast") == 0) {
        	refreshRate = 5;
        } else if (strcmp(argv[i], "--fastest") == 0) {
        	refreshRate = 1;
        }
	}

	if (!map) { board = initDefault(); }

	return board;

	error:
		fprintf(stderr, "Unrecognised option(s)\n");
		printHelp();

	conflict:
		fprintf(stderr, "Conflict between -m and -x and/or -y\n");
		exit(EXIT_FAILURE);
}

void printHelp()
{
	fprintf(stdout, "--map <str>\t\tSpecify a file to read from containing a map\n");
    fprintf(stdout, "--x <int>\t\tUse in conjunction with --y <int> to specify dimensions of auto-generated map\n");
    fprintf(stdout, "--y <int>\t\tSee above\n");
    fprintf(stdout, "--slow\t\t\tRun the simulation with slow speed. Very slow.\n");
    fprintf(stdout, "--fast\t\t\tRun the simulation with fast speed. Almost real-time!\n");
    fprintf(stdout, "--fastest\t\tRun the simulation at fastest speed.\n");
    fprintf(stdout, "Default values are x=79, y=20, fast speed\n");
    exit(EXIT_FAILURE);
}

Board ** getMap(const char* path)
{
	FILE *map = NULL;
	char *line = NULL;
	size_t len = 0;
	int read, i = 0;

	map = fopen(path, "r");
	if (map == NULL) {
		fprintf(stderr, "The file is empty, cannot generate the map\n");
		exit(EXIT_FAILURE);
	}

	if ((read = getline(&line, &len, map)) != -1) {
		X = read - 1;	// Remove \n
		fseek(map, 0L, SEEK_END);
		Y = ftell(map)/read;
		rewind(map);
	} else {
		fprintf(stderr, "Could not read the first line\n");
		exit(EXIT_FAILURE);
	}

	Board **board = initBoard();

	while (getline(&line, &len, map) != -1) {
		fillBoard(board, i, line);
		++i;
	}

	return board;
}

Board ** initBoard()
{
	Board **board;
	board = calloc(Y, sizeof(Board *));
	if (board == NULL) {
		fprintf(stderr, "Couldn't allocate enough memory for the array");
		exit(EXIT_FAILURE);
	} 

	for (size_t j = 0; j < Y; j++) {
		board[j] = calloc(X, sizeof(Board));
		if (board[j] == NULL) {
			fprintf(stderr, "Couldn't allocate enough memory for the array");
			exit(EXIT_FAILURE);
		} 
	}

	return board;
}

void fillBoard(Board **board, int y, const char* line)
{
	for (size_t i = 0; i < X; i++) {
		switch (line[i]) {
			case 'I':
				board[y][i].character = INF;
				++countInf;
				++total;
				break;
			case 'D':
				board[y][i].character = DOC;
				++countDoc;
				++total;
				break;
			case 'S':
				board[y][i].character = SOL;
				++countSol;
				++total;
				break;
			case 'O':
				board[y][i].character = CIT;
				++countCit;
				++total;
				break;
			case 'X':
				board[y][i].character = DEAD;
				++countDea;
				++total;
				break;
			case 'N':
				board[y][i].character = NUR;
				++countNur;
				++total;
				break;
			default :
				board[y][i].character = EMPTY;
				break;
		}
	}
}

Board **initDefault()
{
	countDoc = 10, countSol = 1, countInf = 3;
	countCit = X*Y - countDoc - countSol - countInf;
	total = X*Y;

	Board **board = initBoard();

	defaultBoard(board);	
	generateCoord(board, countSol, SOL);
	generateCoord(board, countInf, INF);
	generateCoord(board, countDoc, DOC);

	return board;
}

void defaultBoard(Board **board)
{
	for (size_t i = 0; i < Y; i++) {
		for (size_t j = 0; j < X; j++) {
			board[i][j].character = CIT;
			board[i][j].direction = NONE;
		}
	} 
}

void generateCoord(Board **board, const int count, Characters type)
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