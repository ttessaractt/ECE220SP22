#include "game.h"


game * make_game(int rows, int cols)
/*! Create an instance of a game structure with the given number of rows
    and columns, initializing elements to -1 and return a pointer
    to it. (See game.h for the specification for the game data structure) 
    The needed memory should be dynamically allocated with the malloc family
    of functions.
*/
{
  //Dynamically allocate memory for game and cells (DO NOT modify this)
  game * mygame = malloc(sizeof(game));
  mygame->cells = malloc(rows*cols*sizeof(cell));

  //YOUR CODE STARTS HERE:  Initialize all other variables in game struct
  int k;
  
  for (k=0; k<rows*cols; k++){ //for every cell in the game
    mygame->cells[k] = -1; //set each element to -1
  }
  mygame->rows = rows; //set rows initial value
  mygame->cols = cols; //set cols initial value
  mygame->score = 0; //set score initial value
  return mygame;
}

void remake_game(game ** _cur_game_ptr,int new_rows,int new_cols)
/*! Given a game structure that is passed by reference, change the
	game structure to have the given number of rows and columns. Initialize
	the score and all elements in the cells to -1. Make sure that any 
	memory previously allocated is not lost in this function.	
*/
{
  /*Frees dynamically allocated memory used by cells in previous game,
	 then dynamically allocates memory for cells in new game.  DO NOT MODIFY.*/
  free((*_cur_game_ptr)->cells);
  (*_cur_game_ptr)->cells = malloc(new_rows*new_cols*sizeof(cell));

  //YOUR CODE STARTS HERE:  Re-initialize all other variables in game struct
  int k;

  for (k=0; k<new_rows*new_cols; k++){ //for every cell in the game
    (*_cur_game_ptr)->cells[k] = -1; //reset the cells to -1
  }
  (*_cur_game_ptr)->rows = new_rows; //change rows to given # of rows
  (*_cur_game_ptr)->cols = new_cols; //change cols to given # of columns
  (*_cur_game_ptr)->score = 0; //set score to 0
  return;	
}

void destroy_game(game * cur_game)
/*! Deallocate any memory acquired with malloc associated with the given game instance.
    This includes any substructures the game data structure contains. Do not modify this function.*/
{
    free(cur_game->cells);
    free(cur_game);
    cur_game = NULL;
    return;
}

cell * get_cell(game * cur_game, int row, int col)
/*! Given a game, a row, and a column, return a pointer to the corresponding
    cell on the game. (See game.h for game data structure specification)
    This function should be handy for accessing game cells. Return NULL
	if the row and col coordinates do not exist.
*/
{
  //YOUR CODE STARTS HERE
  if (row>=0 && col>=0 && (cur_game->rows)>row && (cur_game->cols)>col) { //make sure cell exists
    return &(cur_game->cells[row * cur_game->cols + col]); //return pointer to the cell
  }  
  else return NULL; //cell does not exist
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////


//start move functions
//for all of my move functions i first define 2 other functions, one to move the 
//tiles in the correct direction and one to combine pairs of tiles. the move_() 
//function defines the rows and columns of the playing board and then calls the 
//combining function. the combining function first calls the move function to 
//shift all the tiles to the correct side of the board. Then checks if each pair
//of cells is equal and combines them if they are. then once all the combinations
//have been made the function calls the move function again to shift all the newly
//combined cells properly. then the function is finished and waits for the next move.

//------------------------------------------------------------------------------------------
//start move_w() functions
//the function searches for an empty cell starting in the first row of the board (row=0)
//and if it finds an empty cell it checks if the cell below it is a tile. if the cell
//is a tile then it moves the value from the tile to the cell above it and clears the tile.
//then it moves on to check the next cell in the row. and each row in the board moving from
//the top of the board to the bottom.

//variable - what it does
//  i   - the row that is being checked for a value if there is an empty cell
//  j   - what column that the cells being checked are in
//empty - the row that is being checked for empty cells 

void slide_up(game * cur_game, int rows, int cols){
  int i, j;
  for (j=0;j<cols;j++){ //for all columns
    
    //get first empty cell
    int empty;
    //do stuff to get first empty cell
    for (empty=0;empty<rows && *get_cell(cur_game,empty,j)!=-1;empty++) ;

    for (i=empty+1;i<rows;i++){ //for every cell that could shift up
      if (*get_cell(cur_game,i,j)!=-1){ //if it needs to shift up do it
        *get_cell(cur_game,empty,j) = *get_cell(cur_game, i, j); //shift value up
        *get_cell(cur_game,i,j) = -1; //clear original
        empty++;
      }
    }
  }
  return;
}

//variable - what it does
//i - the row that is the top of each pair
//j - the column that the pair is in
//h - the row that is the bottom of each pair

void slide_smush_up(game * cur_game, int rows, int cols){

  slide_up(cur_game, rows, cols); //shift (all nonempty now touching)
  //smush matching adjacent pairs upwards (and clear out lower of two)
  int i, j, h;
  for (i=0;i<rows;i++) { //iterate through the top of each pair

    for (j=0;j<cols;j++) { //iterate through the columns
      h=i+1;
      if (*get_cell(cur_game,i,j) == -1) continue; //skip empty cells
      // check if it matches the one below it
      if (*get_cell(cur_game,i,j) == *get_cell(cur_game,h,j)) {
        *get_cell(cur_game,i,j) *= 2; //double the top
        *get_cell(cur_game,h,j) = -1; //clear out the bottom
        cur_game->score += *get_cell(cur_game,i,j); //add to score
      }
    }
  }
  slide_up(cur_game, rows, cols); //shift (everything now in final location)
  return;
}

int move_w(game * cur_game)
/*!Slides all of the tiles in cur_game upwards. If a tile matches with the
  one above it, the tiles are merged by adding their values together. When
  tiles merge, increase the score by the value of the new tile. A tile can
  not merge twice in one turn. If sliding the tiles up does not cause any
  cell to change value, w is an invalid move and return 0. Otherwise, return 1.
*/
{
  //YOUR CODE STARTS HERE
  int rows, cols, k, h, a;
  rows = cur_game->rows; //define rows
  cols = cur_game->cols; //define cols
  int check[rows*cols]; //create array to store old board

  for (k=0;k<rows*cols;k++){ //for every cell in the board
    check[k] = cur_game->cells[k]; //copy current board to check[]
  }
  slide_smush_up(cur_game, rows, cols);  //update board
  for (h=0;h<rows*cols;h++){ //for every cell
    if (check[h] == cur_game->cells[h]) a +=1; //check if current and old board are the same
  }
  if (a == rows*cols) return 0; //invaliud move
  else return 1; //valid move
};

//------------------------------------------------------------------------------------------
//start move_s() function
//the function searches for an empty cell starting in the last row of the board (row=rows-1)
//and if it finds an empty cell it checks if the cell above it is a tile. if the cell
//is a tile then it moves the value from the tile to the cell below it and clears the tile.
//then it moves on to check the next cell in the row. and each row in the board moving from
//the bottom of the board to the top.

//variable - what it does
//  i   - the row that is being checked for a value if there is an empty cell
//  j   - the column that the cells being checked are in
//empty - the row that is being checked for empty cells

void slide_down(game * cur_game, int rows, int cols){
  int i, j;
  for (j=0;j<cols;j++){ //for all columns

    //get first empty cell
    int empty;
    //do stuff to get first empty cell
    for (empty=rows-1;empty>0 && *get_cell(cur_game,empty,j)!=-1;empty--);

    for (i=empty-1;i>=0;i--){ //for every cell that could shift down
      if (*get_cell(cur_game,i,j)!=-1){ //if it needs to shift down do it
        *get_cell(cur_game,empty,j) = *get_cell(cur_game,i,j); //shift value down
        *get_cell(cur_game,i,j) = -1; //clear original
        empty--;
      }
    }
  }
  return;
}

//variable - what it does
//i - the row that is the bottom of each pair
//j - the column that the pair is in
//h - the row that is the top of each pair

void slide_smush_down(game *cur_game, int rows, int cols){

  slide_down(cur_game, rows, cols); //shift (all nonempty now touching)
  //smush matching adjacent pairs downwards (and clear out upper of two)
  int i, j, h;
  for (i=rows-1;i>0;i--) { //iterate through the bottom of each pair

    for (j=0;j<cols;j++) { //iterate through the columns
      h=i-1;
      if (*get_cell(cur_game,i,j) == -1) continue; //skip empty cells
      // check if it matches the one below it
      if (*get_cell(cur_game,i,j) == *get_cell(cur_game,h,j)) {
        *get_cell(cur_game,i,j) *= 2; //double the bottom
        *get_cell(cur_game,h,j) = -1; //clear out the top
        cur_game->score += *get_cell(cur_game,i,j); //add to score
      }
    }
  }
  slide_down(cur_game, rows, cols); //shift (everything now in final location)
}

int move_s(game * cur_game) //slide down
/*!Slides all of the tiles in cur_game down. If a tile matches with the
  one below it, the tiles are merged by adding their values together. When
  tiles merge, increase the score by the value of the new tile. A tile can
  not merge twice in one turn. If sliding the tiles down does not cause any
  cell to change value, s is an invalid move and return 0. Otherwise, return 1.
*/
{
    //YOUR CODE STARTS HERE
  int rows, cols, k, h, a; //ints
  rows = cur_game->rows; //define rows
  cols = cur_game->cols; //dedfine cols
  int check[rows*cols]; //create array to store old board

  for (k=0;k<rows*cols;k++){ //for every cell
    check[k] = cur_game->cells[k]; //copy cur_board to check
  }
  slide_smush_down(cur_game, rows, cols); //update board
  for (h=0;h<rows*cols;h++){ //for every cell
    if (check[h] == cur_game->cells[h]) a +=1; //check if new and old board are the same
  }
  if (a == rows*cols) return 0; //invalid move
  else return 1; //valid move
};

//------------------------------------------------------------------------------------------
//start move_a() function
//the function searches for an empty cell starting in the first column of the board (col=0)
//and if it finds an empty cell it checks if the cell to the right of it is a tile. if the cell
//is a tile then it moves the value from the tile to the cell to the left of it and clears the tile.
//then it moves on to check the next cell in the column. and each column in the board moving from
//the left of the board to the right.

//variable - what it does
//  i   - the column that is being checked for a value if there is an empty cell
//  j   - the row that the cells being checked are in
//empty - the column that is being checked for empty cells

void slide_left(game * cur_game, int rows, int cols){
  int i, j;
  for (j=0;j<rows;j++){ //for all rows

    //get first empty cell
    int empty;
    //do stuff to get first empty cell
    for (empty=0;empty<cols && *get_cell(cur_game,j,empty)!=-1;empty++);
      
    for (i=empty+1;i<cols;i++){ //for every cell that could shift left
      if (*get_cell(cur_game,j,i)!=-1){ //if it needs to shift left do it
        *get_cell(cur_game,j,empty) = *get_cell(cur_game,j,i); //shift value left
        *get_cell(cur_game,j,i) = -1; //clear original
        empty++;
      }
    }
  }
  return;
}

//variable - what it does
//i - the column that is the left of each pair
//j - the row that the pair is in
//h - the column that is the right of each pair

void slide_smush_left(game *cur_game, int rows, int cols){

  slide_left(cur_game, rows, cols); //shift (all nonempty now touching)
  //smush matching adjacent pairs left (and clear out right of two)
  int i, j, h;
  for (i=0;i<cols-1;i++) { //iterate through the left of each pair

    for (j=0;j<rows;j++) { //iterate through the rows
      h=i+1;
      if (*get_cell(cur_game,j,i) == -1) continue; //skip empty cells
      // check if it matches the one next to it
      if (*get_cell(cur_game,j,i) == *get_cell(cur_game,j,h)) {
        *get_cell(cur_game,j,i) *= 2; //double the left
        *get_cell(cur_game,j,h) = -1; //clear out the right
        cur_game->score += *get_cell(cur_game, j, i); //add to score
      }
    }
  }
  slide_left(cur_game, rows, cols); //shift (everything now in final location)
}

int move_a(game * cur_game) //slide left
/*!Slides all of the tiles in cur_game left. If a tile matches with the
  one to the right of it, the tiles are merged by adding their values together. 
  When tiles merge, increase the score by the value of the new tile. A tile can
  not merge twice in one turn. If sliding the tiles left does not cause any
  cell to change value, a is an invalid move and return 0. Otherwise, return 1.
*/
{
    //YOUR CODE STARTS HERE
  int rows, cols, k, h, a; //inits
  rows = cur_game->rows; //define rows
  cols = cur_game->cols; //define cols
  int check[rows*cols]; //create array to store old board
  
  for (k=0;k<rows*cols;k++){ //for every cell
    check[k] = cur_game->cells[k]; //copy cur_board to check
  }
  slide_smush_left(cur_game, rows, cols); //update board 
  for (h=0;h<rows*cols;h++){ //for every cell
    if (check[h] == cur_game->cells[h]) a +=1; //check if current and old board are the same
  }
  if (a == rows*cols) return 0; //invalid move
  else return 1; //valid move
};

//------------------------------------------------------------------------------------------
//start move_d() function
//the function searches for an empty cell starting in the last column of the board (col=cols-1)
//and if it finds an empty cell it checks if the cell to the left of it is a tile. if the cell
//is a tile then it moves the value from the tile to the cell to the right of it and clears the tile.
//then it moves on to check the next cell in the column. and each column in the board moving from
//the right of the board to the left.

//variable - what it does
//  i   - the column that is being checked for a value if there is an empty cell
//  j   - the row that the cells being checked are in
//empty - the column that is being checked for empty cells

void slide_right(game * cur_game, int rows, int cols){
  int i, j;
  for (j=0;j<rows;j++){ //for all rows

    //get first empty cell
    int empty;
    //do stuff to get first empty cell
    for (empty=cols-1;empty>0 && *get_cell(cur_game,j,empty)!=-1;empty--);
      
    for (i=empty-1;i>=0;i--){ //for every cell that could shift right
      if (*get_cell(cur_game,j,i)!=-1){ //if it needs to shift right do it
        *get_cell(cur_game,j,empty) = *get_cell(cur_game,j,i); //shift value right
        *get_cell(cur_game,j,i) = -1; //clear original
        empty--;
      }
    }
  }
  return;
}

//variable - what it does
//i - the column that is the right of each pair
//j - the row that the pair is in
//h - the column that is the left of each pair

void slide_smush_right(game *cur_game, int rows, int cols){

  slide_right(cur_game, rows, cols); //shift (all nonempty now touching)
  //smush matching adjacent pairs right (and clear out left of two)
  int i, j, h;
  for (i=cols-1;i>0;i--) { //iterate through the right of each pair

    for (j=0;j<rows;j++) { //iterate through the rows
      h=i-1;
      if (*get_cell(cur_game,j,i) == -1) continue; //skip empty cells
      // check if it matches the one next to it
      if (*get_cell(cur_game,j,i) == *get_cell(cur_game,j,h)) {
        *get_cell(cur_game,j,i) *= 2; //double the right
        *get_cell(cur_game,j,h) = -1; //clear out the left
        cur_game->score += *get_cell(cur_game,j,i); //add to score
      }
    }
  }
  slide_right(cur_game, rows, cols); //shift (everything now in final location)
}

int move_d(game * cur_game) //slide to the right
/*!Slides all of the tiles in cur_game right. If a tile matches with the
  one to the left of it, the tiles are merged by adding their values together. 
  When tiles merge, increase the score by the value of the new tile. A tile can
  not merge twice in one turn. If sliding the tiles right does not cause any
  cell to change value, d is an invalid move and return 0. Otherwise, return 1.
*/{
  //YOUR CODE STARTS HERE
  int rows, cols, k, h, a; //inits
  rows = cur_game->rows; //define rows
  cols = cur_game->cols; //define cols
  int check[rows*cols]; //create array to store old board

  for (k=0;k<rows*cols;k++){ //for every cell
    check[k] = cur_game->cells[k]; //copy cur_board to check
  }
  slide_smush_right(cur_game, rows, cols); //update board
  for (h=0;h<rows*cols;h++){ //for every cell
    if (check[h] == cur_game->cells[h]) a +=1; //check if current and old board are the same
  }
  if (a == rows*cols) return 0; //invalid move
  else return 1;  //valid move
};


//end move_() functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////


int legal_move_check(game * cur_game)
/*! Given the current game check if there are any legal moves on the board. There are
    no legal moves if sliding in any direction will not cause the game to change.
	Return 1 if there are possible legal moves, 0 if there are none.
 */
{
  //YOUR CODE STARTS HERE
  int rows, cols, k, i; //inits
  rows = cur_game->rows; //define rows
  cols = cur_game->cols; //define cells
  i=0; //create valid move counter
  for (k=0;k<rows*cols;k++){ //for every cell
    if (cur_game->cells[k] == -1){ //check if any cell is empty
      i = 1; //at least one cell is empty return 1
    }
  }
  if (i==0) return 0; //no possible legal moves
  else return 1; //possible legal moves
}


/*! code below is provided and should not be changed */

void rand_new_tile(game * cur_game)
/*! insert a new tile into a random empty cell. First call rand()%(rows*cols) to get a random value between 0 and (rows*cols)-1.
*/
{
	
	cell * cell_ptr;
    cell_ptr = 	cur_game->cells;
	
    if (cell_ptr == NULL){ 	
        printf("Bad Cell Pointer.\n");
        exit(0);
    }
	
	
	//check for an empty cell
	int emptycheck = 0;
	int i;
	
	for(i = 0; i < ((cur_game->rows)*(cur_game->cols)); i++){
		if ((*cell_ptr) == -1){
				emptycheck = 1;
				break;
		}		
        cell_ptr += 1;
	}
	if (emptycheck == 0){
		printf("Error: Trying to insert into no a board with no empty cell. The function rand_new_tile() should only be called after tiles have succesfully moved, meaning there should be at least 1 open spot.\n");
		exit(0);
	}
	
    int ind,row,col;
	int num;
    do{
		ind = rand()%((cur_game->rows)*(cur_game->cols));
		col = ind%(cur_game->cols);
		row = ind/cur_game->cols;
    } while ( *get_cell(cur_game, row, col) != -1);
        //*get_cell(cur_game, row, col) = 2;
	num = rand()%20;
	if(num <= 1){
		*get_cell(cur_game, row, col) = 4; // 1/10th chance
	}
	else{
		*get_cell(cur_game, row, col) = 2;// 9/10th chance
	}
}

int print_game(game * cur_game) 
{
    cell * cell_ptr;
    cell_ptr = 	cur_game->cells;

    int rows = cur_game->rows;
    int cols = cur_game->cols;
    int i,j;
	
	printf("\n\n\nscore:%d\n",cur_game->score); 
	
	
	printf("\u2554"); // topleft box char
	for(i = 0; i < cols*5;i++)
		printf("\u2550"); // top box char
	printf("\u2557\n"); //top right char 
	
	
    for(i = 0; i < rows; i++){
		printf("\u2551"); // side box char
        for(j = 0; j < cols; j++){
            if ((*cell_ptr) == -1 ) { //print asterisks
                printf(" **  "); 
            }
            else {
                switch( *cell_ptr ){ //print colored text
                    case 2:
                        printf("\x1b[1;31m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 4:
                        printf("\x1b[1;32m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 8:
                        printf("\x1b[1;33m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 16:
                        printf("\x1b[1;34m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 32:
                        printf("\x1b[1;35m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 64:
                        printf("\x1b[1;36m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 128:
                        printf("\x1b[31m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 256:
                        printf("\x1b[32m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 512:
                        printf("\x1b[33m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 1024:
                        printf("\x1b[34m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 2048:
                        printf("\x1b[35m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 4096:
                        printf("\x1b[36m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 8192:
                        printf("\x1b[31m%04d\x1b[0m ",(*cell_ptr));
                        break;
					default:
						printf("  X  ");

                }

            }
            cell_ptr++;
        }
	printf("\u2551\n"); //print right wall and newline
    }
	
	printf("\u255A"); // print bottom left char
	for(i = 0; i < cols*5;i++)
		printf("\u2550"); // bottom char
	printf("\u255D\n"); //bottom right char
	
    return 0;
}

int process_turn(const char input_char, game* cur_game) //returns 1 if legal move is possible after input is processed
{ 
	int rows,cols;
	char buf[200];
	char garbage[2];
    int move_success = 0;
	
    switch ( input_char ) {
    case 'w':
        move_success = move_w(cur_game);
        break;
    case 'a':
        move_success = move_a(cur_game);
        break;
    case 's':
        move_success = move_s(cur_game);
        break;
    case 'd':
        move_success = move_d(cur_game);
        break;
    case 'q':
        destroy_game(cur_game);
        printf("\nQuitting..\n");
        return 0;
        break;
	case 'n':
		//get row and col input for new game
		dim_prompt: printf("NEW GAME: Enter dimensions (rows columns):");
		while (NULL == fgets(buf,200,stdin)) {
			printf("\nProgram Terminated.\n");
			return 0;
		}
		
		if (2 != sscanf(buf,"%d%d%1s",&rows,&cols,garbage) ||
		rows < 0 || cols < 0){
			printf("Invalid dimensions.\n");
			goto dim_prompt;
		} 
		
		remake_game(&cur_game,rows,cols);
		
		move_success = 1;
		
    default: //any other input
        printf("Invalid Input. Valid inputs are: w, a, s, d, q, n.\n");
    }

	
	
	
    if(move_success == 1){ //if movement happened, insert new tile and print the game.
         rand_new_tile(cur_game); 
		 print_game(cur_game);
    } 

    if( legal_move_check(cur_game) == 0){  //check if the newly spawned tile results in game over.
        printf("Game Over!\n");
        return 0;
    }
    return 1;
}
