#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "maze.h"


/*
 * The createMaze() function opens the file and reads the column and row size. then it allocated memory for the maze
 * struct and assigns the column and row size to weidth and height in the struct. then it allocated memory for the
 * array in cells. then the program goes through each place in the .txt file and (while skipping every \n space)
 * saves the character there to maze->cells. it also detects if the character is the start or end symbol and 
 * assigns the row and column locations to the maze struct. then it closes the file and returns the filled maze
 * struct. 
 * The destroyMaze() function frees all the cells of the array and then frees the maze struct. 
 * The printMaze() function goes through every cell in the array row by row and prints the characters and a 
 * \n at the end of every row.
 * The solveMazeDFS() function mostly follows the psuedocode from the course wiki with a few changes. I check if (col,row)
 * is at the end before i check if it's empty to prevent the end from not being recognized. I also check if (col,row) is
 * the start at the same time as empty since that is the first location that needs to be checked for a path from.
 * I also have adiffernt order for marking cells as visited or path from trial and error. 
*/

/*
 * createMaze -- Creates and fills a maze structure from the given file
 * INPUTS:       fileName - character array containing the name of the maze file
 * OUTPUTS:      None 
 * RETURN:       A filled maze structure that represents the contents of the input file
 * SIDE EFFECTS: None
 */
maze_t * createMaze(char * fileName)
{
  // Your code here. Make sure to replace following line with your own code.
  //open file  
  FILE * file;
  int i, j, rows, cols, h;
  file = fopen(fileName,"r"); 
  fscanf(file,"%d %d ", &cols, &rows); //get width and height from document

  maze_t * maze = malloc(sizeof(maze_t)); //allocate memory for maze struct
  maze->width = cols; //assign width to maze struct
  maze->height = rows; //assign heigh to maze struct
  maze->cells = (char**)malloc(rows*cols*sizeof(char*)); //allocate memory for cells
  for (i=0; i<=rows; i++) { //for every row
    for (j=0; j<=cols; j++) { //for every column
      if((h = fgetc(file)) != EOF) { //if theres a character in the file

        if (h == '\n') { //exclude newline characters
          i++;
          j = -1;
          continue;
        }

        if (h != '\n') { //if not a newline character
          maze->cells[i*cols+j] = h; //add it to struct
          if (maze->cells[i*cols+j] == START) { //if the character is 'S'
            maze->startColumn = j; //assign struct start column
            maze->startRow = i; //assign struct start row
          }
          if (maze->cells[i*cols+j] == END) { //if character is 'E'
            maze->endColumn = j; //assign struct end column
            maze->endRow = i; //assign struct end row
          }
        }
      }
    }
  }
  fclose(file); //close the file
  return maze; // return the maze struct

}

/*
 * destroyMaze -- Frees all memory associated with the maze structure, including the structure itself
 * INPUTS:        maze -- pointer to maze structure that contains all necessary information 
 * OUTPUTS:       None
 * RETURN:        None
 * SIDE EFFECTS:  All memory that has been allocated for the maze is freed
 */
void destroyMaze(maze_t * maze)
{
  // Your code here.
  free(maze->cells); //free memory from cells
  free(maze); //free memory from maze struct
  maze = NULL; //remove maze 
  return;
}

/*
 * printMaze --  Prints out the maze in a human readable format (should look like examples)
 * INPUTS:       maze -- pointer to maze structure that contains all necessary information 
 *               width -- width of the maze
 *               height -- height of the maze
 * OUTPUTS:      None
 * RETURN:       None
 * SIDE EFFECTS: Prints the maze to the console
 */
void printMaze(maze_t * maze)
{
  // Your code here.
  int i, j; //inits
  maze->cells[(maze->startRow)*(maze->width)+(maze->startColumn)] = START; //make sure start char is still there

  for (i=0; i<(maze->height); i++) { //for every row
    for (j=0; j<(maze->width); j++) { //for every column
      printf("%c", maze->cells[i*(maze->width)+j]); //print the char
    }
    printf("\n"); //print a new line at the end of every row
  }

}

/*
 * solveMazeManhattanDFS -- recursively solves the maze using depth first search,
 * INPUTS:               maze -- pointer to maze structure with all necessary maze information
 *                       col -- the column of the cell currently beinging visited within the maze
 *                       row -- the row of the cell currently being visited within the maze
 * OUTPUTS:              None
 * RETURNS:              0 if the maze is unsolvable, 1 if it is solved
 * SIDE EFFECTS:         Marks maze cells as visited or part of the solution path
*/
int solveMazeDFS(maze_t * maze, int col, int row)
{
  if (row<0 || col<0 || row>=(maze->height) || col>=(maze->width)){ //if (col, row) is within the maze bounds
    return false;
  }
  if (maze->cells[row*(maze->width)+col] == END){ //if (col,row) is at the end of the maze
    return true;
  }
  else if (maze->cells[row*(maze->width)+col] == EMPTY || maze->cells[row*(maze->width)+col] == START){ //if cell is empty or start
    maze->cells[row*(maze->width)+col] = VISITED; //mark as visited
    if (solveMazeDFS(maze, col-1, row)){ //left of (col,row)
      maze->cells[row*(maze->width)+col] = PATH; //mark as path
      return true;
    }
    if (solveMazeDFS(maze, col+1, row)){ //right of (col,row)
      maze->cells[row*(maze->width)+col] = PATH; //mark as path
      return true;
    }
    if (solveMazeDFS(maze, col, row-1)){ //up of (col,row)
      maze->cells[row*(maze->width)+col] = PATH; //mark as path
      return true;
    }
    if (solveMazeDFS(maze, col, row+1)){ //down of (col,row)
      maze->cells[row*(maze->width)+col] = PATH; //mark as path
      return true;
    }
   }
  return false;

}
  
