#include "sudoku.h"

//-------------------------------------------------------------------------------------------------
// Start here to work on your MP7
//-------------------------------------------------------------------------------------------------

// ----intro----
//
// when solve_sudoku is called the program checks for any empty cells. if it doesnt find one it signals that all cells have been 
// assigned numbers. if it does find an empty cell is stops looking for empty cells and starts searching for the number that
// goes in the current cell. it calls is_val_valid() which double checks that the cell is empty. and calls is_val_in_row(), 
// is_val_in_col(), and is_val_in_3x3_zone(). if the results of all 3 functions are false then is_val_valid() returns 1, otherwise
// it returns 0. if 1 is returned that means that val/num is the correct value for the cell and the program fills the cell.
// then the program calls itself with this new value in the cell. if it doesnt work out somewhere during the program 
// the program sets the cells back to 0 and does recursive backtracking.

// You are free to declare any private functions if needed.

// Function: is_val_in_row
// Return true if "val" already existed in ith row of array sudoku.
// 
// this function check every column in the given row i to see if it has the same value as val
// col is the column currently being checked
int is_val_in_row(const int val, const int i, const int sudoku[9][9]) {

  assert(i>=0 && i<9);

  // BEG TODO
  int col;
  
  for (col=0;col<9;col++){ //for every column in the given row
    if (sudoku[i][col]==val) return true; //the value val already exists in the row
  }
  return 0;
  // END TODO
}

// Function: is_val_in_col
// Return true if "val" already existed in jth column of array sudoku.
//
// this function checks every row in the given column j to see if it has the same value as val
// row is the row currently being checked
int is_val_in_col(const int val, const int j, const int sudoku[9][9]) {

  assert(j>=0 && j<9);

  // BEG TODO
  int row;

  for (row=0;row<9;row++){ //for every row in the column
    if (sudoku[row][j]==val) return true; //the value val already exists in the column
  }
  return 0;
  // END TODO
}

// Function: is_val_in_3x3_zone
// Return true if val already existed in the 3x3 zone corresponding to (i, j)
//
// this function starts by doing integer division with the i and 3 and j and 3 to get which of the 9 zones the cell is in.
// then this number is multiplied by 3 so row and col now point to the upper left side of the zone the cell is in.
// then the program goes through each cell within the zone and checks if the val occurs anywhere in the zone.
// then it returns 1 if val does occur and 0 if it doesnt.
// r is the row counter
// c is the column counter
// row is the starting row of the zone
// col is the starting column of the zone
int is_val_in_3x3_zone(const int val, const int i, const int j, const int sudoku[9][9]) {
   
  assert(i>=0 && i<9);
  
  // BEG TODO
  int r, c, row, col;
  row = (i/3)*3; //get starting row of 3x3 zone
  col = (j/3)*3; //get starting col of 3x3 zone
  for (r=row;r<row+3;r++){ //go through the 3 rows of the zone
    for (c=col;c<col+3;c++){ //go through the 3 cols of the zone
      if (sudoku[r][c]==val) return true; //val is in the zone
    }
  }
  return 0; //val isnt in the zone
  // END TODO
}

// Function: is_val_valid
// Return true if the val is can be filled in the given entry.
int is_val_valid(const int val, const int i, const int j, const int sudoku[9][9]) {

  assert(i>=0 && i<9 && j>=0 && j<9);

  // BEG TODO
  if (sudoku[i][j]!=0) return 0; //check if cell already had a number
    if ((is_val_in_row(val,i,sudoku)!=1)&&(is_val_in_col(val,j,sudoku)!=1)&&(is_val_in_3x3_zone(val,i,j,sudoku)!=1)){
    //^check if the val occurs in the row column or zone of (i, j) 
      return 1; //it does occur
    }
   return 0; //the number is good to go
          
  // END TODO
}

// Procedure: solve_sudoku
// Solve the given sudoku instance.
int solve_sudoku(int sudoku[9][9]) {

  // BEG TODO.
  int i, j, x, z, u;
  u=1; //set tracker for if all cells are assigned numbers
  for (x=0;x<9;x++){ //for every row
    for (z=0;z<9;z++){ //for every column
      if (sudoku[x][z]==0) { //if the cell is empty
	i=x; //set i to current row
        j=z; //set j to current column
        u=0; //there are non filled cells
	continue; //dont need to keep going
      }
    if (u==0) continue; //dont need to keep going
    }      
  }
  if (u==1) return 1; //no cells changed so all cells have been assigned numbers

  for (int num=1;num<=9;num++){ //for every number 1-9
    if(is_val_valid(num,i,j,sudoku)==1){ // if cell(i, j) can be filled with num
      sudoku[i][j] = num; //fill cell
      if (solve_sudoku(sudoku)) { 
        return true; //we solved it!
      }
      sudoku[i][j] = 0; //that number wasnt right, reset the cell
    }
  }
  return 0; //backtracking step
  // END TODO.
}

// Procedure: print_sudoku
void print_sudoku(int sudoku[9][9])
{
  int i, j;
  for(i=0; i<9; i++) {
    for(j=0; j<9; j++) {
      printf("%2d", sudoku[i][j]);
    }
    printf("\n");
  }
}

// Procedure: parse_sudoku
void parse_sudoku(const char fpath[], int sudoku[9][9]) {
  FILE *reader = fopen(fpath, "r");
  assert(reader != NULL);
  int i, j;
  for(i=0; i<9; i++) {
    for(j=0; j<9; j++) {
      fscanf(reader, "%d", &sudoku[i][j]);
    }
  }
  fclose(reader);
}





