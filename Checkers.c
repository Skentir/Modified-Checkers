#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#define AMAX 8

enum B {
	True = 1,
	False = 0,
};
enum Colors
{
	Black = 0,
	Gray = 7,
	Blue = 9,
	Green = 10,
	Cyan = 11,
	Red = 12,
	Magenta = 13,
	Yellow = 14,
	White = 15	
};
void printColored (char s[301], int color)
{
	/* Given a string and a color value, print out that string in that color/bakground*/
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);
	printf ("%s", s);
	SetConsoleTextAttribute(hConsole, White);
}
char arrowKey ()
{
	/* Function for getting an arrow key/enter input for menus */
	int key;
	do
	{
		key = getch();
		switch (key)
		{
			case 72: return 'u';
			case 80: return 'd';
			case 75: return 'l';
			case 77: return 'r';
			case 13: return 'e';
			default: key = 0;
		}
	} while (key == 0);
	return '\0';
}

typedef int coords[AMAX + 1][AMAX + 1];

int inSetA (int n)
{
	/* Returns whether the number is within the bounds of A */
	if (n >= 1 && n <= AMAX)
		return True;
	else
		return False;
}
int inSetP (int x, int y)
{
	/* Returns whether x and y is in Set P */
	if (inSetA (x) && inSetA (y))
		return True;
	else
		return False;
}
int inSetS (int x, int y)
{
	/* Returns whether x and y is in Set S */
	if (inSetP (x, y) && ((x % 2 == 0 && y % 2 == 1) || (x % 2 == 1 && y % 2 == 0)) )
		return True;
	else
		return False;
}
void DisplayBoard (coords inAlpha, coords inBeta, coords inFree, int currX, int currY)
{
	
	/* Displays the current state of the board with the current highlighted piece*/
	int x, y;
	
	char line[31];
	char hold[11];
	char piece;
	strcpy (line, "+---+---+---+---+---+---+---+---+");

	printf ("\t  %s\n", line);
	for (y = AMAX; y >= 1; y--)
	{
		printf ("\t%i |", y);
			
		for (x = 1; x <= AMAX; x++)
		{
			if (inFree[x][y])
				piece = '*';
			else if (inAlpha[x][y])
				piece = 'A';
			else if (inBeta[x][y])
				piece = 'B';
			else
				piece = ' ';
			sprintf (hold, " %c ", piece);
			
			if (x == currX && y == currY)
				printColored (hold, (White + 1) * White);
			else
				printColored (hold, White);
			printf ("|");
		}
		printf ("\n");
		if (y != 5)
			printf ("\t  %s\n", line);
		else
			printf ("      y   %s\n", line);
	}
	printf ("\t    ");
	for (x = 1; x <= AMAX; x++)
		printf ("%i   ", x);
	printf ("\n\t\t\t  x\n\n");
}
int TestValidMoves (coords inAlpha, coords inBeta, coords inFree, int aTurn)
{
	/* Tests each piece of the current player's turn if it has any valid moves and returns how many */
	int x, y;
	int availableMove = 0;
	for (x = 1; x <= AMAX; x++)
		for (y = 1; y <= AMAX; y++)
		{
			if (aTurn && inAlpha[x][y] && x + 1 <= AMAX)
			{
				if (y + 1 <= AMAX && (inBeta[x + 1][y + 1] || inFree[x + 1][y + 1]))
					availableMove++;
				else if (y - 1 >= 1 && (inBeta[x + 1][y - 1] || inFree[x + 1][y - 1]))
					availableMove++;
			}
			else if (!aTurn && inBeta[x][y] && x - 1 >= 1)
			{
				if (y + 1 <= AMAX && (inAlpha[x - 1][y + 1] || inFree[x - 1][y + 1]))
					availableMove++;
				else if (y - 1 >= 1 && (inAlpha[x - 1][y - 1] || inFree[x - 1][y - 1]))
					availableMove++;
			}
		}
		
	return availableMove;
	printf ("availableMoves: %i\n", availableMove);
	system ("pause");
}
void NextPlayerMove (int *ok, coords inAlpha, coords inBeta, coords inFree, int *aTurn, int a, int b, int c, int d)
{
	/* Assuming that the variable indicating validity (ok) is false */
	
	int prev_x = a;
	int prev_y = b;
	int next_x = c;
	int next_y = d;
	
	
	/* Validation for coordinates*/
	
	if (*aTurn && inAlpha[prev_x][prev_y] && c == a + 1 && (d == b + 1 || b == d + 1))
	/* If it's Alpha's turn, and Alpha's 'piece' is selected, and the piece is towards positive x diagonally */
		*ok = !(*ok);
	else if (!(*aTurn) && inBeta[prev_x][prev_y] && a == c + 1 && (d == b + 1 || b == d + 1))
	/* If it's Beta's turn, and Beta's 'piece' is selected, and the piece is towards negative x diagonally */
		*ok = !(*ok);
		
	if (*ok && *aTurn && inFree[next_x][next_y])
	/* If coords are valid, and it's Alpha's turn and the destination coord is not occupied */
	/* "Move" the 'piece' towards the destination coord and toggle the turn to Beta, and toggle validity to false for later */
	{
		inAlpha[prev_x][prev_y] = False;
		inAlpha[next_x][next_y] = True;
		*aTurn = !(*aTurn);
		*ok = !(*ok);
	}
	else if (*ok && !(*aTurn) && inFree[next_x][next_y])
	{
	/* If coords are valid, and it's Beta's turn and the destination coord is not occupied */
	/* "Move" the 'piece' towards the destination coord and toggle the turn to Alpha, and toggle validity to false for later */
		inBeta[prev_x][prev_y] = False;
		inBeta[next_x][next_y] = True;
		*aTurn = !(*aTurn);
		*ok = !(*ok);
	}
	else if (*ok && (*aTurn) && inBeta[next_x][next_y])
	{
	/* If coords are valid, and it's Alpha's turn and the destination coord is occupied by Beta's 'piece' */
	/* "Move" the 'piece' towards the destination coord and toggle the turn to Alpha, and toggle validity to false for later */
	/* While doing so, remove the "piece" of Beta */
		inBeta[next_x][next_y] = False;
		inAlpha[prev_x][prev_y] = False;
		inAlpha[next_x][next_y] = True;
		*ok = !(*ok);
	}
	else if (*ok && !(*aTurn) && inAlpha[next_x][next_y])
	/* If coords are valid, and it's Beta's turn and the destination coord is occupied by Alpha's 'piece' */
	/* "Move" the 'piece' towards the destination coord and toggle the turn to Alpha, and toggle validity to false for later */
	{
		inAlpha[next_x][next_y] = False;
		inBeta[prev_x][prev_y] = False;
		inBeta[next_x][next_y] = True;
		*ok = !(*ok);
	}
	else if (*ok)
	/* If the coords are valid but non-intended coords were picked, toggle validity to false for later */
		*ok = !(*ok);
}

void GameOver (int countAlpha, int countBeta, char result[])
{
	/* Assuming that the number of Alpha 'pieces' doesn't equal the number of Beta 'pieces' */
	
	if (countAlpha == 0)
	
		strcpy (result, "Alpha ");
	else if (countBeta == 0)
		strcpy (result, "Beta ");
		
	strcat (result, "Wins");
}
int main()
{
	int x, y; 
	int currRow = 0, currCol = 0;
	char key;
	
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, White);
	
	coords inAlpha;
	coords inBeta;
	coords inFree;
	int countAlpha, countBeta;
	int valid;
	char hold[51];
	char optionDialog[301];
	int prevSelect = True;
	
	int aTurn;
	int over;
	int ok;
	int prev_x, prev_y;
	int next_x, next_y;
	
	/* Initializes Alpha */
	for (x = 1; x <= AMAX; x++)
		for (y = 1; y <= AMAX; y++)
			if (inSetS(x, y) && x <= 3)
				inAlpha[x][y] = True;
			else
				inAlpha[x][y] = False;
	
	/* Initializes Beta */
	for (x = 1; x <= AMAX; x++)
		for (y = 1; y <= AMAX; y++)
			if (inSetS(x, y) && x >= 6)
				inBeta[x][y] = True;
			else
				inBeta[x][y] = False;
	
	/* Initializes Free */
	for (x = 1; x <= AMAX; x++)
			for (y = 1; y <= AMAX; y++)
				if (inSetS(x, y) && !inAlpha[x][y] && !inBeta[x][y])
					inFree[x][y] = True;
				else
					inFree[x][y] = False;
	
	/* Initializes the other system variables */	
	over = False;
	ok = False;
	aTurn = True;
	
	while (!over)
	{
		/* Fact Checks, check with every loop */
		valid = 0;
		prevSelect = True;
		while (!valid)
		{
			while (!valid)
			{	
				/* Prints whose turn it is */
				if (aTurn)
					strcpy (hold, "Alpha");
				else
					strcpy (hold, "Beta");
				
				/* Prints which part of the selection the player is at */
				if (prevSelect)
					strcpy (optionDialog, "Select the piece to be moved!");
				else
					strcpy (optionDialog, "Select the destination!");
				
				/* Loops while the player hasn't selected a coordinate */
				do
				{
					system ("cls");
					printf ("\n\t\t    %s's Turn!\n", hold);
					printf ("\t    %s\n\n", optionDialog);
					
					/* Initializes the current highlighted coordinate based on the location from arrow key input */
					x = currCol + 1;
					y = 8 - currRow;
					
					/* Displays the current highlighted coordinate and the full board */
					DisplayBoard (inAlpha, inBeta, inFree, x, y);
					
					/* Gets arrow key input and makes sure that scrolling past the edge loops back to the other side */
					key = arrowKey();
					switch (key)
					{
						case 'u': 
							if (currRow - 1 >= 0)
								currRow--;
							else
								currRow = 7;
							break;
						case 'd':
							if (currRow + 1 < 8)
								currRow++;
							else
								currRow = 0;
							break;
						case 'l':
							if (currCol - 1 >= 0)
								currCol--;
							else
								currCol = 7;
							break;
						case 'r':
							if (currCol + 1 < 8)
								currCol++;
							else
								currCol = 0;
							break;
					}
				} while (key != 'e');
				
				if (!prevSelect)
				/* If selected the destination coordinates */
				{
					next_x = x;
					next_y = y;
				}
				else
				/* If selected the 'piece' to be moved coordinates*/
				{
					prev_x = x;
					prev_y = y;
				}
				
				/* If coordinates of selection was valid */
				if (inSetS (x, y))
				{
					/* If the destination was valid, go through with second round of error-checking */
					if (!prevSelect)
						valid = 1;
					/* Else, goes back to selection of 'piece' coordinate */
					else
						prevSelect = 0;
				}
				/* If coordinates of selection wasn't valid, go back to selection of 'piece' coordinate */
				else
				{
					prevSelect = 1;
					printf ("Coordinate was not valid for selection.\n");
					system ("pause");
					system ("cls");
				}
			}	
			
			/* If it's Alpha's turn and Alpha's 'piece' wasn't selected or Beta's turn and Beta's 'piece' wasn't selected */
			if (!(aTurn && inAlpha[prev_x][prev_y]) && !(!aTurn && inBeta[prev_x][prev_y]))
			{
				valid = 0;
				prevSelect = 1;
				printf ("Selected piece is not yours.\n");
				system ("pause");
				system ("cls");
			}
			/* If the destination wasn't free or if the destination is occupied by the player's own 'piece' */
			else if (!inFree[next_x][next_y] && !(inBeta[next_x][next_y] && aTurn) && !(inAlpha[next_x][next_y] && !aTurn))
			{
				valid = 0;
				prevSelect = 1;
				printf ("Destination is not valid.\n");
				system ("pause");
				system ("cls");
			}
			else
				valid = 1;
		}	
		
		/* Inputs the player's move and updates the turn */
		NextPlayerMove(&ok, inAlpha, inBeta, inFree, &aTurn, prev_x, prev_y, next_x, next_y);
		system ("cls");
		
		/* Fact Checks, check with every loop*/
		/* Updating Free to contain coordinates in Set S not occupied by Alpha or Beta */
		for (x = 1; x <= AMAX; x++)
			for (y = 1; y <= AMAX; y++)
				if (inSetS(x, y) && !inAlpha[x][y] && !inBeta[x][y])
					inFree[x][y] = True;
				else
					inFree[x][y] = False;
					
		/* Counts the number of Alpha and Beta 'pieces' */
		countAlpha = 0;
		for (x = 1; x <= AMAX; x++)
			for (y = 1; y <= AMAX; y++)
				if (inAlpha[x][y])
					countAlpha++;
		countBeta = 0;
		for (x = 1; x <= AMAX; x++)
			for (y = 1; y <= AMAX; y++)
				if (inBeta[x][y])
					countBeta++;
					
		/* If either Alpha or Beta has a count of 0, but not both */
		if ((countAlpha == 0 || countBeta == 0) && (countAlpha != countBeta))
			over = True;
		/* If no more valid moves by the player whose turn it is */
		else if (!TestValidMoves (inAlpha, inBeta, inFree, aTurn))
			over = True;
	}
	system ("cls");
	fflush (stdin);
	
	/* Display the final state of the board*/
	DisplayBoard(inAlpha, inBeta, inFree, -1, -1);
	
	/* If there are no valid moves for the player whose turn it was */
	if (!TestValidMoves (inAlpha, inBeta, inFree, aTurn))
	{
		/* The winner is the one with more pieces*/
		if (countBeta < countAlpha)
			strcpy (hold, "Alpha");
		else if (countAlpha < countBeta)
			strcpy (hold, "Beta");
		
		/* Indicates whose turn made the game end due to having no valid moves */
		if (aTurn)
			printf ("Alpha ");
		else
			printf ("Beta ");
		printf ("has no more valid moves! ");
		
		/* If the count wasn't a tie */
		if (countBeta != countAlpha)
			printf ("%s wins the game due to having more pieces!", hold);
		/* If the count WAS a tie */
		else
			printf ("Draw!");
	}
	else 
	{
		/* If Beta runs out of 'pieces' */
		if (countBeta == 0)
			strcpy (hold, "Alpha");
		/* If Alpha runs out of 'pieces' */
		else if (countAlpha == 0)
			strcpy (hold, "Beta");
		printf ("Congratulations! %s wins the game!\n", hold);
	}
	return 0;
}

