/*
 * Hi there welcome to my code. there are individual explanation paragraphs before each function that 
 * explain the general process of the code and list out the variables that i created and 
 * what purpose they serve in the function.
 *
 */


/*
 * countLiveNeighbor
 * Inputs:
 * board: 1-D array of the current game board. 1 represents a live cell.
 * 0 represents a dead cell
 * boardRowSize: the number of rows on the game board.
 * boardColSize: the number of cols on the game board.
 * row: the row of the cell that needs to count alive neighbors.
 * col: the col of the cell that needs to count alive neighbors.
 * Output:
 * return the number of alive neighbors. There are at most eight neighbors.
 * Pay attention for the edge and corner cells, they have less neighbors.
 */

/* 
 * countLiveNeighbor starts at the row above the given row (or row 0 if the given row is 0) and
 * the column to the left of the given column (or column 0 is the given column is 0). it then checks
 * that it isnt currently located in both the given row and column location and if its not it checks 
 * if the current cell is alive or dead. if the cell is alive it adds one to the count and moves to 
 * the next column. it checks that the current column isnt outside the bounds of the array and that the 
 * curremt location isnt the given row and column. if both are false it checks if the current cell is alive
 * or dead. the program moves onto the next column and repeats the same process from moving to the last column
 * and after it determines if the cell is alive or dead the program moves onto the next row and repeats everything
 * until the current row is <= the given row + 1. the final count is then returned.
 *
 * currRow - current row being checked
 * currCol - current column being checked
 * liveneighbors - the count of the live neighbors
 */

int countLiveNeighbor(int* board, int boardRowSize, int boardColSize, int row, int col){
  int liveneighbors = 0; // initilaize count to 0
  for (int currRow = (row - 1); currRow <= (row + 1); currRow++){ // count from 1 row below to 1 row 
                                                                  //above the given cell
    if (currRow >= 0 && currRow < boardRowSize) { // check if row >= 0 to deal with -1 and 
                                                  //check if within the bounds of the array
      for (int currCol = (col - 1); currCol <= (col + 1); currCol++ ){ // count from 1 column to the left to 1
                                                                       //column to the right of the given cell
        if (currCol >= 0 && currCol < boardColSize) { // check is column >= 0 to deal with -1 
                                                      // and check if within the bound of the array
          if (row == currRow && col == currCol) { // check if the row and 
                                                  //column being checked arent the cell location
            continue; // they are so we want to skip counting it since we only want to count the neighbors
          }
          else // not currently at the cell 
            if (board[currRow*boardColSize + currCol] == 1) { // so check if the neighboring cell is alive or not
              liveneighbors++; // the cell us alive so add 1 to the alive count
          }
        }
      }
    }
  } 
  return liveneighbors; //return count of live neighbor cells at a array location
}

/*
 * Update the game board to the next step.
 * Input: 
 * board: 1-D array of the current game board. 1 represents a live cell.
 * 0 represents a dead cell
 * boardRowSize: the number of rows on the game board.
 * boardColSize: the number of cols on the game board.
 * Output: board is updated with new values for next step.
 */

/* 
 * updateBoard copies the given board into oldBoard then goes through each cell of the board
 * calling countLiveNeighbor with oldBoard. using the number of alive neighbors and wether the current
 * cell is alive or dead determines what the new status of the cell will be and sets the cell in board 
 * to that value.
 *
 * row - current row
 * col - current column
 * live - count of live neighbors to cell at (row, column), aka output of countLiveNeighbor
 * l - location in array
 * i - counter
 * oldBoard - array where current generations board is stored to compare to create new generations board
 */


void updateBoard(int* board, int boardRowSize, int boardColSize) {
int row, col, live, l, i; // inits
int oldBoard[boardRowSize*boardColSize]; //init array to copy board into
for (i = 0; i < boardRowSize*boardColSize; i++) { // for every cell in the board
  oldBoard[i] = board[i];  // copy the cell to the same location is oldBoard
}
for (row = 0; row < boardRowSize; row++) {  // row counter (within the bounds of the array)
  for (col = 0; col < boardColSize; col++) { // colum counter (within bounds of the array)
    live = countLiveNeighbor(oldBoard, boardRowSize, boardColSize, row, col); // call countLiveNeighbor iwth oldBoard
    l = row*boardColSize + col; // location of the cell in the array
    if (live < 2 || live > 3){  // cell has < 2 or > 3 live neighbors, has no affect on already dead cells
                                // and causes live cells to die from under-population and over-population respectively
      board[l] = 0; // cell either stays dead or dies
    }
    if (oldBoard[l] == 1) { // cell started alive
      if (live == 2 || live == 3){ // the live cell has 2 or 3 live neighbors
        board[l] = 1; // it lives on to the next generation
      }
      else // first if statmenet should make this redundant but prevents possible error
        board[l] = 0; // cell dies
    }
    if (oldBoard[l] == 0) { // cell starts dead
      if (live == 3) { // the cell has 3 live neighbors
        board[l] = 1; // and meets the requiremnts to be born
      }
      else // should be redundant but again better to prevent possible error
        board[l] = 0; // cell stays dead
     }
  }
}
return board[l]; // return the board
}

/*
 * aliveStable
 * Checks if the alive cells stay the same for next step
 * board: 1-D array of the current game board. 1 represents a live cell.
 * 0 represents a dead cell
 * boardRowSize: the number of rows on the game board.
 * boardColSize: the number of cols on the game board.
 * Output: return 1 if the alive cells for next step is exactly the same with 
 * current step or there is no alive cells at all.
 * return 0 if the alive cells change for the next step.
 */

/*
 * aliveStable copies the current board into an array called nextBoard then runs the updateBoard function
 * with nextBoard to get the state of the board for the next generation. then the program goes through each
 * location in nextBoard and sees if it has the same living state as the same location in board.
 * if it doesnt 0 is returned and if it does 1 is returned. 
 *
 * i - counter
 * j - counter
 * k - counter
 * nextBoard - array with the next generation of the given board
 */
 
int aliveStable(int* board, int boardRowSize, int boardColSize){
int i, k, j; // inits
int nextBoard[boardRowSize*boardColSize]; // init nextBoard array
for (j = 0; j < boardRowSize*boardColSize; j++) { // for every cell in board
  nextBoard[j] = board[j]; // copy board into nextBoard
}
updateBoard(nextBoard,boardRowSize, boardColSize); // get the next state of the board
for (i = 0; i < boardRowSize; i++){ // for every row
  for (k = 0; k < boardColSize; k++) { // for every column
    if (nextBoard[i*boardColSize+k] != board[i*boardColSize+k]){ // check if the board stays the same 
      return 0; // the board changes so 0 is returned
    }
  }
}
return 1; // the board doesnt change so 1 is returned
}





