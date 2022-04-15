#include "sparsemat.h"

#include <stdio.h>
#include <stdlib.h>

//swap all data values in linked list
void swap(sp_tuples_node *a, sp_tuples_node *b)
{
  int tempr = a->row;
  a->row = b->row;
  b->row = tempr;

  int tempc = a->col;
  a->col = b->col;
  b->col = tempc;

  double tempv = a->value;
  a->value = b->value;
  b->value = tempv;
}

//sort the linked list
void sort(sp_tuples_node * start)
{
  int swapped;
  sp_tuples_node *ptr1 = NULL;
  sp_tuples_node *lptr = NULL;

  if (start== NULL) return; //check for empty list
  do
  {
    swapped = 0;
    ptr1 = start;
    while (ptr1->next != lptr){
      if (ptr1->row > ptr1->next->row){
        swap(ptr1, ptr1->next);
        swapped = 1;
      }
      if (ptr1->row == ptr1->next->row){
        if (ptr1->col > ptr1->next->col){
          swap(ptr1, ptr1->next);
          swapped = 1;
        }
      }
      ptr1 = ptr1->next;
    }
    lptr = ptr1;
  }
  while (swapped);
}

void combine(sp_tuples * mat_t)
{
  int combine;
  sp_tuples_node *ptr1 =NULL;
  sp_tuples_node *lptr = NULL;

  if (mat_t->tuples_head== NULL) return; //check for empty list
  do
  {
    combine = 0;
    ptr1 = mat_t->tuples_head;
    while (ptr1->next != lptr){
      if (ptr1->row == ptr1->next->row && ptr1->col == ptr1->next->col){
        swap(ptr1, ptr1->next);
        sp_tuples_node * temp =NULL; 
        temp = ptr1->next;
        ptr1->next = temp->next;
        free(temp);
        mat_t->nz--;
        combine = 1;
      }

      ptr1 = ptr1->next;
    }
    lptr = ptr1;
  }
  while (combine);
}

void zero(sp_tuples * mat_t);

/*
 * load_tuples() should open a file with the name 'input_file', read the data from the file,
 * and return a matrix of the list of tuples type. If any coordinates repeat in the input
 * file, the newer coordinates ( a lower line closer to the end of  the text document) 
 * should overwrite the old line. If there is an entry with a value of 0 then the
 * corresponding node should be deleted if it exists. The elements in the input text 
 * file may be unordered (unlike the example text file above), but the list of tuples 
 * returned will need to be in order. You do not have to handle cases where the input_file 
 * doesn't match the specified format.
*/
sp_tuples * load_tuples(char* input_file)
{
  FILE * file = NULL;
  file = fopen(input_file, "r"); //open file
  
  sp_tuples * mat = malloc(sizeof(sp_tuples)); //allocate memory for sp_tuples
  //get rows and cols of matrix
  int row=NULL, col=NULL;
  fscanf(file, "%d %d  ", &row, &col);
  mat->m = row; //set rows of mat
  mat->n = col; //set cols of mat
  int count=NULL;
  mat->nz = count;
  char buff[22];

  sp_tuples_node * new = NULL;
  sp_tuples_node * current = NULL;
  mat->tuples_head = NULL;
  while (1){    
    fgets(buff, sizeof(buff), file); //read line
    if (feof(file)) break; //break if at end of the file

    if (buff[0] != '\n'){ //account for possible blank lines
      new = malloc(sizeof(sp_tuples_node));
      mat->nz ++;
      //assign vale from file
      sscanf(buff, "%d %d %lf ", &new->row, &new->col, &new->value);
      new->next = NULL;
      if(mat->tuples_head==NULL){
        mat->tuples_head = new;
        current = new;
        //mat->nz += 1;
      }
      else{
        current->next = new;
        current = new;
        //mat->nz += 1;
      }
    } 
  }
  //printf("nz=%d\n", mat->nz);
  fclose(file);
  sort(mat->tuples_head);
  combine(mat);
  zero(mat);
  return mat;
}


void zero(sp_tuples * mat_t)
{
  int zero;
  sp_tuples_node *ptr1 =NULL;
  sp_tuples_node *lptr =NULL;
  if (mat_t->tuples_head== NULL) return; //check for empty list
  do
  {
    zero = 0;
    ptr1 = mat_t->tuples_head;
    //int count = mat_t->nz;
  //printf("nz=%d\n", mat_t->nz);
    while (ptr1->next != lptr){
      if (ptr1 == NULL) return;
      if (ptr1->value == 0){
        sp_tuples_node * temp = NULL;
        temp = ptr1->next;
        ptr1->next = temp;
        //free(temp);
        mat_t->nz--;
        zero = 1;
      }
      ptr1 = ptr1->next;
      //count--;
    }
    lptr = ptr1;
  }
  while (zero);
  return;
}
/*
 * gv_tuples returns the value of the element at the given row and column within the matrix
*/
double gv_tuples(sp_tuples * mat_t,int row,int col)
{
  sp_tuples_node *ptr1 = NULL;
  ptr1 = mat_t->tuples_head;
  while (ptr1 != NULL){
    if (ptr1->row == row){
      if (ptr1->col == col){
        return ptr1->value;
      }
      ptr1 = ptr1->next;
    }
  }
  return 0;
}

//insert node at beginning of linked list
void insertNode(sp_tuples * mat_t, int r, int c, double v)
{
  sp_tuples_node * new = NULL;
  new = malloc(sizeof(sp_tuples_node));

  new->row = r;
  new->col = c;
  new->value = v;
  new->next = (mat_t->tuples_head);
  (mat_t->tuples_head) = new; 
  mat_t->nz += 1;
  return;
}

/*
 * set_tuples() sets the element at row and col to value. 
 * - The function deletes any node with a value of 0 and frees the node from memory
 * - The function finds the correct location for the node within the sorted linked list.
 *   If the entry already exists, the function should replace the old value. If the entry
 *   doesnt exist, a node should be created and insrted into the linked list.
 * - Update the nz variable
 *
 * plan
 * - create new node for the input
 * - sort new node to correct location (so it can delete the prev node if it exists)
 * - run combine function to delete old value
 * - if value == 0 run zero
 */
void set_tuples(sp_tuples * mat_t, int row, int col, double value)
{
  
  insertNode(mat_t, row, col, value); //insert node
  sort(mat_t->tuples_head);
  combine(mat_t);
  
  if (value==0){
    zero(mat_t);
    return;
  }

  return;
}
/*
 * save_tuples writes the data in a sparse matrix structure to a text file in the format
 * specified above. Because of the way the linked lists are ordered, writing the entires
 * of the matrix as you traverse the list will give an output in row major order. Your text
 * file output must be in this order even though load_tuples() should be able to hndle 
 * reading un-ordered text files. 
*/
void save_tuples(char * file_name, sp_tuples * mat_t)
{ 
  sp_tuples_node *ptr1 = NULL;
  int save;
  int count = mat_t->nz;
  if (mat_t->tuples_head==NULL) return;
  do 
  {
    save = 0;
    ptr1 = mat_t->tuples_head;
    while (count > 0){
      printf("%d %d %lf \n", ptr1->row, ptr1->col, ptr1->value);
      count -=1;
      save = 1;
      ptr1 = ptr1->next;
    }
  }
  while(save);
	return;
}

void add(sp_tuples * mat_t)
{
  int add;
  sp_tuples_node *ptr1 =NULL;
  sp_tuples_node *lptr = NULL;

  if (mat_t->tuples_head== NULL) return; //check for empty list
  do
  {
    add = 0;
    ptr1 = mat_t->tuples_head;
    while (ptr1->next != lptr){
      if (ptr1->row == ptr1->next->row && ptr1->col == ptr1->next->col){
        ptr1->value += ptr1->next->value;
        sp_tuples_node * temp =NULL; 
        temp = ptr1->next;
        ptr1->next = temp->next;
        free(temp);
        mat_t->nz -= 1;
        add = 1;
      }

      ptr1 = ptr1->next;
    }
    lptr = ptr1;
  }
  while (add);
}


sp_tuples * add_tuples(sp_tuples * matA, sp_tuples * matB){
  if (matA->m != matB->m || matA->n != matB->n) return NULL; //different sizes, addition not possible
  sp_tuples * matC = malloc(sizeof(sp_tuples)); 
  matC->m = matA->m;
  matC->n = matA->n;
  matC->nz = 0;
  matC->tuples_head = NULL;

  sp_tuples_node *ptr1 = NULL;
  ptr1 = matA->tuples_head;
  while (ptr1 != NULL){ //adds a to c
    insertNode(matC, ptr1->row, ptr1->col, ptr1->value);
    ptr1 = ptr1->next;
  }  
  sp_tuples_node *ptr2 = NULL;
  ptr2 = matB->tuples_head;
  while (ptr2 != NULL) {
    insertNode(matC, ptr2->row, ptr2->col, ptr2->value);
    ptr2 = ptr2->next;
  }
  
  sort(matC->tuples_head);
  add(matC);
  return matC;
}

sp_tuples * mult_tuples(sp_tuples * matA, sp_tuples * matB){ 
   return NULL;
}

	
void destroy_tuples(sp_tuples * mat_t){
  sp_tuples_node * n = NULL;
  n = mat_t->tuples_head;
  if (mat_t->tuples_head == NULL){
    free(mat_t);
    mat_t=NULL;
    return;
  }

  int count = mat_t->nz;
  while (count > 0){
    sp_tuples_node * victim = NULL;
    victim = n;
    n = n->next;
    free(victim);
    victim = NULL;
    count--;
  }
  free(n);
  free(mat_t);
  mat_t=NULL;
  return;
}  

