/* ************************************************************************* */
/*                              Filename: Sort.c                             */
/*                                                                           */
/*  EXPORT: Procedure Sort: Sorting of an array into a non-decreasing order  */
//   @version 3.0pre1
/* ************************************************************************* */




/* ************************************************************************* */
/*                             Procedure Sort()                              */
/*                                                                           */
/*  INPUT:  key_array:  Array with keys                                      */
/*          sort_array: Array to be sorted according to the keys             */
/*          last:       last position of the array                           */
/*                                                                           */
/*  FUNCTION: When this function is called, the integers to be sorted        */
/*            are stored in sort_array[1] ... sort_array[last].              */
/*            The index with smallest key_array-entry is assigned to         */
/*            sort_array[1], and so on.                                      */ 
/* ************************************************************************* */

void Sort(int key_array[], int sort_array[], int last)
{

  register int i, j, k, x;
  
  for (i = 1; i < last; ++i) {
     k = i;
     x = key_array[sort_array[i]];
     for (j = i+1; j <= last; ++j) 
        if (key_array[sort_array[j]] < x) {
           k = j;
           x = key_array[sort_array[j]];
        }
     j = sort_array[k];
     sort_array[k] = sort_array[i];
     sort_array[i] = j;
  }
}
