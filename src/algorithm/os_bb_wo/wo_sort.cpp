/* ************************************************************************* */
/*                            Filename: Sort.c                               */
/*                                                                           */
/*  EXPORT: Procedure Sort:                                                  */
/*                    Sortieren eines Arrays in nicht-fallender Reihenfolge  */
/* ************************************************************************* */




/* ************************************************************************* */
/*                             Procedure Sort()                              */
/*                                                                           */
/*  INPUT:  key_array:  Array mit 'keys'                                     */
/*          sort_array: Array, der gemaess der keys sortiert wird            */
/*          last:       letzte Position des Arrays                           */
/*                                                                           */
/*  FUNCTION: Beim Aufruf dieser Funktion, werden die sortierten Integer in  */
/*            sort_array[1] ... sort_array[last] gespeichert.                */
/*            Der Index mit kleinstem key_array-entry is sort_array[1] zuge- */
/*	      ordnet, usw.						     */
/* ************************************************************************* */

void Sort(key_array, sort_array, last)	
     
     int key_array[];  
     int sort_array[];  
     int last;
{

  register int i, 
	       j, 
	       k, 
	       x;
  

  for (i = 1; i < last; ++i) 
  {
     k = i;
     x = key_array[sort_array[i]];
     for ( j = i+1; j <= last; ++j ) 
	if (key_array[sort_array[j]] < x) 
	{
	   k = j;
	   x = key_array[sort_array[j]];
	}
     j = sort_array[k];
     sort_array[k] = sort_array[i];
     sort_array[i] = j;
  }
}
