#include "generation.h"

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