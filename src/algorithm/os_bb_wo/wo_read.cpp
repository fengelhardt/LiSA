/* ************************************************************************ */
/*                            Filename: Read.c                              */
/*                                                                          */
/*     EXPORT: Procedure Read_Data:                                         */
/*                       Einlesen der Daten aus der Datei 'FileName'        */
/* ************************************************************************ */


#include <stdio.h>
#include "data.h"
#include "table.h"
#include "read.h"
#include "list.h"

/* ************************************************************************ */
/*                           Procedure  Read_Data()                         */
/*                                                                          */
/*   INPUT:  FileName:    Datei, die die Problemdaten enthaelt              */
/*                                                                          */
/*   FUNCTION: Liest die Daten aus der Datei und speichert sie in globalen  */
/*             Variablen.						    */
/*             Wenn das Datei-lesen fehlerfrei verlaeuft, so gibt Read_Data */
/*             den Wert TRUE zurueck.                                       */
/* ************************************************************************ */

boolean Read_Data(FileName)	char *FileName;
{

 int      i, 
	  j, 
	  num_of_op_for_job, 
	  sum = 1;
 FILE     *fp;


 if ((fp = fopen(FileName,"r")) == NIL)
    return false;   

 /* ermitteln des Problems (m x n - Problem);
    erfassen der Daten in den globalen Variablen NumOfMachines, NumOfJobs;    
 */
 if ( !fscanf(fp,"%d %d ", &NumOfMachines, &NumOfJobs))  
    return(false);

 for (i = 1; i <= NumOfJobs; ++i) 
 {
    if (!fscanf(fp,"%d", &num_of_op_for_job)) 
	return(false);

    /* ermitteln der Bearbeitungszeit u. Maschinennummer einer Operation;
       erfassen der Daten in der globalen Variablen OpData[];
       Operationen, die auf derselben Maschine bearbeitet werden, kommen in
       eine gemeinsame globale Liste OpsOnMachine[];
       Operationen, die zum selben Job gehoeren, kommen in
       eine gemeinsame globale Liste OpsOfJob[] 
    */
    for (j = 1; j <= num_of_op_for_job; ++j)
    {
       if (!fscanf(fp,"%d %d ",&OpData[sum].process_time,
                               &OpData[sum].machine_nr) )
          return(false);

       OpData[sum].job_nr = i;

       OpsOnMachine[OpData[sum].machine_nr] =
			     Insert(OpsOnMachine[OpData[sum].machine_nr],sum);
       OpsOfJob[OpData[sum].job_nr] = 
			     Insert(OpsOfJob[OpData[sum].job_nr],sum);

       sum ++;
    
    }
 }
 fclose(fp); 
 NumOfOperations = --sum;
 return(true);    
}
