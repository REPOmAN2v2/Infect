#include <string.h>
#include <ctype.h>
#include "generation.h"
#include "display.h"

int X = 79, Y = 20;

Board ** parseArgs(int argc, char **argv)
{
	Board **board = NULL;
	int map = 0, size = 0, bDoctor = 0, bInfected = 0, bSoldier = 0,
	bNurse = 0, bWood = 0;

	for (size_t i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-x") == 0) {
			checkArg(i, argc, argv, &size, map, &X);
		} else if (strcmp(argv[i], "-y") == 0) {
			checkArg(i, argc, argv, &size, map, &Y);
		} else if (strcmp(argv[i], "-d") == 0) {
			checkArg(i, argc, argv, &bDoctor, map, &countDoc);
		} else if (strcmp(argv[i], "-i") == 0) {
			checkArg(i, argc, argv, &bInfected, map, &countInf);
		} else if (strcmp(argv[i], "-s") == 0) {
			checkArg(i, argc, argv, &bSoldier, map, &countSol);
		} else if (strcmp(argv[i], "-n") == 0) {
			checkArg(i, argc, argv, &bNurse, map, &countNur);
		} else if (strcmp(argv[i], "-w") == 0) {
			checkArg(i, argc, argv, &bWood, map, &countWood);
		} else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
			printHelp();
		} else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
			printVersion();
		} else if (strcmp(argv[i], "-m") == 0 || strcmp(argv[i], "--map") == 0) {
			if (i + 1 <= argc - 1) {
				if (!size && !bDoctor && !bInfected && !bSoldier && !bNurse) {
					++i;
					board = getMap(argv[i]);
					map = 1;
				} else {
					printError("Conflict between -m and -x/-y/-d/-i/-s/-n\n");
				}
			}
		} else if (strcmp(argv[i], "--slow") == 0) {
			refreshRate = 10;
		} else if (strcmp(argv[i], "--fast") == 0) {
        	refreshRate = 5;
        } else if (strcmp(argv[i], "--fastest") == 0) {
        	refreshRate = 1;
        } else if (strcmp(argv[i], "--debug") == 0) {
        	stepthrough = 1;
        }
	}

	if (!map) {
		if (!bDoctor) {
			countDoc = (X*Y*0.01)+1;
		}
		if (!bInfected) {
			countInf = (X*Y*0.005)+1;
		}
		if (!bSoldier) {
			countSol = (X*Y*0.02)+1;
		}
		if (!bNurse) {
			countNur = (X*Y*0.05)+1;
		}
		if (!bWood) {
			countWood = (X*Y*0.5);
		}
		board = initDefault(); 
	}

	return board;
}

int checkArg(int i, int argc, char **argv, int *check, int map, int *constant)
{
	if (i + 1 <= argc - 1) {
		if (!map) {
			++i;
			char *end;
			int val = strtol(argv[i], &end, 10);
			if (end != NULL && !end[0] && val >= 0) {
				*constant = val;
				*check = 1;
				return 1;
			} else {
				printError("Unrecognised option(s)\n");
			}
		} else {
			printError("Conflict between -m and -x/-y/-d/-i/-s/-n\n");
		}
	}
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
			case 'W':
				board[y][i].character = WALL;
				break;
			default :
				board[y][i].character = EMPTY;
				break;
		}
	}
}

Board **initDefault()
{
	countCit = X*Y - countDoc - countSol - countInf - countNur;
	if (countCit < 0) {
		endwin();
		fprintf(stderr, "Too many units for the board size.\n");
		exit(EXIT_FAILURE);
	}

	total = X*Y;

	Board **board = initBoard();

	defaultBoard(board);	
	generateCoord(board, countSol, SOL);
	generateCoord(board, countInf, INF);
	generateCoord(board, countDoc, DOC);
	generateCoord(board, countNur, NUR);

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