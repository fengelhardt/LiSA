/* ************************************************************************ */
/*                            Filename: Read.c                              */
/*                                                                          */
/*     EXPORT: Procedure Read_Data:                                         */
/*                       Einlesen der Daten aus der Datei 'FileName'        */
/* ************************************************************************ */


#include <cstdio>
#include <cstdlib>
#include "wo_data.hpp"
#include "wo_table.hpp"
#include "wo_read.hpp"
#include "wo_list.hpp"

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

void Read_Data(char *FileName){

 int      i, 
	  j, 
	  num_of_op_for_job, 
	  sum = 1;
 FILE     *fp;


 if ((fp = fopen(FileName,"r")) == NIL)
    exit(1);   

 /* ermitteln des Problems (m x n - Problem);
    erfassen der Daten in den globalen Variablen NumOfMachines, NumOfJobs;    
 */
 if ( !fscanf(fp,"%d %d ", &NumOfMachines, &NumOfJobs))  
    exit(1);

 for (i = 1; i <= NumOfJobs; ++i) 
 {
    if (!fscanf(fp,"%d", &num_of_op_for_job)) 
      exit(1);

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
          exit(1);

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
  
}
