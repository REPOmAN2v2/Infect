/*
 * https://github.com/REPOmAN2v2/Infect
 * 
 * These functions deal mainly with argument parsing, file I/O and updating
 * the game variables and the board. Implementation of each function doesn't
 * involve anything special.
 */

#include <string.h>
#include <ctype.h>
#include "generation.h"
#include "display.h"

/*
 * Internal prototypes
 */

static void checkArg(int i, const int argc, const char * const * const argv, int *check, const int map, int *variable);
static Board ** getMap(const char *path);
static Board ** initBoard();
static void fillBoard(Board * const * const board, const int y, const char * const line);
static void defaultBoard(Board * const * const board);
static void generateCoord(Board * const * const board, const int count, const Characters type);


/*
 * parseArgs() parses the command line arguments.
 *
 * It takes in as arguments argc and argv and returns a pointer to the board.
 * Freeing the board is then left to the caller.
 *
 * It's a big if/else switchboard parsing argv until it finds an argument
 * it recognises, which is then checked for a valid value through checkArg().
 * width of the screen, the string to print and the colour to print it in.
 *
 * Those arguments may update the game variables, which are then used to generate
 * the map.
 */
Board ** parseArgs(const int argc, const char * const * const argv)
{
	Board **board = NULL;
	int map = 0, size = 0, bDoctor = 0, bInfected = 0, bSoldier = 0,
	bNurse = 0, bWood = 0;

	for (size_t i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-x") == 0) {
			checkArg(i, argc, argv, &size, map, &gameVar.dim.x);
		} else if (strcmp(argv[i], "-y") == 0) {
			checkArg(i, argc, argv, &size, map, &gameVar.dim.y);
		} else if (strcmp(argv[i], "-d") == 0) {
			checkArg(i, argc, argv, &bDoctor, map, &gameVar.units.doctors);
		} else if (strcmp(argv[i], "-i") == 0) {
			checkArg(i, argc, argv, &bInfected, map, &gameVar.units.infected);
		} else if (strcmp(argv[i], "-s") == 0) {
			checkArg(i, argc, argv, &bSoldier, map, &gameVar.units.soldiers);
		} else if (strcmp(argv[i], "-n") == 0) {
			checkArg(i, argc, argv, &bNurse, map, &gameVar.units.nurses);
		} else if (strcmp(argv[i], "-w") == 0) {
			checkArg(i, argc, argv, &bWood, map, &gameVar.units.wood);
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
			gameVar.time.refreshRate = SLOW;
		} else if (strcmp(argv[i], "--fast") == 0) {
        	gameVar.time.refreshRate = FAST;
        } else if (strcmp(argv[i], "--fastest") == 0) {
        	gameVar.time.refreshRate = FASTEST;
        } else if (strcmp(argv[i], "--debug") == 0) {
        	gameVar.time.steps = 1;
        }
	}

	if (!map) {
		if (!bDoctor) {
			gameVar.units.doctors = (gameVar.dim.x * gameVar.dim.y * 0.01) + 1;
		}
		if (!bInfected) {
			gameVar.units.infected = (gameVar.dim.x * gameVar.dim.y * 0.005 ) + 1;
		}
		if (!bSoldier) {
			gameVar.units.soldiers = (gameVar.dim.x * gameVar.dim.y * 0.02) + 1;
		}
		if (!bNurse) {
			gameVar.units.nurses = (gameVar.dim.x * gameVar.dim.y * 0.05) + 1;
		}
		if (!bWood) {
			gameVar.units.wood = (gameVar.dim.x * gameVar.dim.y * 0.5);
		}

		board = initDefault(); 
	}

	return board;
}

/*
 * checkArg() check the value behind the cli arguments.
 *
 * It takes in as parameters argv, argc, the index of the argument in argv, two
 * boolean flags (check and map) as well as the game variable to modify.
 * 
 * If the argument is followed by an invalid value, it prints an error and exits.
 */
void checkArg(int i, const int argc, const char * const * const argv, int *check, const int map, int *variable)
{
	if (i + 1 <= argc - 1) {
		if (!map) {
			++i;
			char *end;
			int val = strtol(argv[i], &end, 10);
			if (end != NULL && !end[0] && val >= 0) {
				*variable = val;
				*check = 1;
			} else {
				printError("Unrecognised option(s)\n");
			}
		} else {
			printError("Conflict between -m and -x/-y/-d/-i/-s/-n\n");
		}
	}
}

/*
 * getMap() opens a map file.
 *
 * It takes in as a parameter the path to the file then opens it and gets its
 * dimensions. It then iterates over every line and calls fillBoard() to fill
 * our game board.
 *
 * It returns a pointer to our board. Freeing the board is then left to the 
 * caller.
 */
Board ** getMap(const char* path)
{
	FILE *map = NULL;
	char *line = NULL;
	size_t len = 0;
	int read, i = 0;

	map = fopen(path, "r");
	if (map == NULL) {
		printError("The file is empty, cannot generate the map\n");
	}

	if ((read = getline(&line, &len, map)) != -1) {
		gameVar.dim.x = read - 1;	// Remove \n
		fseek(map, 0L, SEEK_END);
		gameVar.dim.y = ftell(map)/read;
		rewind(map);
	} else {
		printError("Could not read the first line\n");
	}

	Board **board = initBoard();

	while (getline(&line, &len, map) != -1) {
		fillBoard(board, i, line);
		++i;
	}

	return board;
}

/*
 * initBoard() allocates memory for our game board.
 *
 * It exits if no memory is available, otherwise it returns a pointer to the
 * board. Freeing the board is then left to the caller.
 */
Board ** initBoard()
{
	Board **board;
	board = calloc(gameVar.dim.y, sizeof(Board *));
	if (board == NULL) {
		printError("Couldn't allocate enough memory for the array\n");
	} 

	for (size_t j = 0; j < gameVar.dim.y; j++) {
		board[j] = calloc(gameVar.dim.x, sizeof(Board));
		if (board[j] == NULL) {
			printError("Couldn't allocate enough memory for the array\n");
		} 
	}

	return board;
}

/*
 * fillBoard() fills our board array from a map file line by line.
 *
 * It takes in as parameters the board array, the line number and the line
 * itself. It then parses the line and depending on the character, sets the
 * unit at the given coordinates and updates the unit counters.
 */
void fillBoard(Board * const * const board, const int y, const char* const line)
{
	Units *units = &gameVar.units;
	for (size_t i = 0; i < gameVar.dim.x; i++) {
		switch (line[i]) {
			case 'I':
				board[y][i].character = INF;
				++units->infected;
				++units->total;
				break;
			case 'D':
				board[y][i].character = DOC;
				++units->doctors;
				++units->total;
				break;
			case 'S':
				board[y][i].character = SOL;
				++units->soldiers;
				++units->total;
				break;
			case 'O':
				board[y][i].character = CIT;
				++units->citizens;
				++units->total;
				break;
			case 'X':
				board[y][i].character = DEAD;
				++units->dead;
				++units->total;
				break;
			case 'N':
				board[y][i].character = NUR;
				++units->nurses;
				++units->total;
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

/*
 * initDefault() initialises the default map.
 *
 * It fills the board with default values through defaultBoard() then generates
 * random coordinates for the non-citizens, as long as there are any left to
 * place on the map, through generateCoord().
 *
 * It returns a pointer to the board. Freeing the board is then left to the caller.
 */
Board **initDefault()
{
	Units *units = &gameVar.units;

	units->total = gameVar.dim.x * gameVar.dim.y;
	units->citizens = units->total - units->doctors - units->soldiers - units->infected - units->nurses;
	if (units->citizens < 0) {
		printError("Too many units for the board size.\n");
	}

	Board **board = initBoard();

	defaultBoard(board);	
	generateCoord(board, units->soldiers, SOL);
	generateCoord(board, units->infected, INF);
	generateCoord(board, units->doctors, DOC);
	generateCoord(board, units->nurses, NUR);

	return board;
}

/*
 * defaultBoard() defaults the board array.
 *
 * It takes in as a parameter the board, then fills it with default value.
 */
void defaultBoard(Board * const * const board)
{
	for (size_t i = 0; i < gameVar.dim.y; i++) {
		for (size_t j = 0; j < gameVar.dim.x; j++) {
			board[i][j].character = CIT;
			board[i][j].direction = 0;
		}
	} 
}

/*
 * generateCoord() generates random coordinates for special units.
 *
 * It takes in as parameters the board, the count of the unit and its type.
 */
void generateCoord(Board * const * const board, const int count, const Characters type)
{
	int x = 0, y = 0;

	for (size_t i = 0; i < count; i++) {
		do {
			x = rand()%gameVar.dim.x;
			y = rand()%gameVar.dim.y;
		} while(board[y][x].character != CIT);
		
		board[y][x].character = type;
	} 
}