/* ************************************************************************* */
/*                            Filename: Read.h                               */
/*                                                                           */
/*     EXPORT: Procedure Read_Data:                                          */
/*                       Einlesen der Daten aus der Datei 'FileName'         */
/* ************************************************************************* */


#ifndef _READ_H
#define _READ_H

#include "wo_data.hpp"

/* ************************************************************************* */
/*                           Procedure      Read_Data()                      */
/*                                                                           */
/*   INPUT:  FileName:  Datei, die die Problemdaten enthaelt                 */
/*                                                                           */
/*   FUNCTION: Liest die Daten aus der Datei und speichert sie in globalen   */
/*             Variablen.						     */
/*             Wenn das Datei-lesen fehlerfrei verlaeuft, so gibt Read_Data  */
/*             den Wert TRUE zurueck.                                        */
/* ************************************************************************* */

boolean Read_Data(char *FileName);


#endif