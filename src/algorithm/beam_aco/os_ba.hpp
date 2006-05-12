/*
*  os_ba.h
*  beamaco
*
*  Created by Per Willenius on 03.04.06.
*  Copyright 2006 __MyCompanyName__. All rights reserved.
*
*/


#ifndef _os_ba_h
#define _os_ba_h

#include "../../scheduling/os_sched.hpp"
#include "ba_env.hpp"

#include <iostream>



/// beam ACO implementation for open shop
/** @author Per Willenius
@version 0.1
*/



class ScheduleValuePair : public Lisa_Object {

public:

	ScheduleValuePair() {
		Schedule=NULL;
		Value=0;
		Hash=-1;
		sortbyValue=true;
		indexInAllSchedules=0;
	}

	ScheduleValuePair(Lisa_OsSchedule * currentSchedule,double rating, int scheduleIndex) {
		Schedule=currentSchedule;
		Value=rating;
		Hash=-1;
		sortbyValue=true;
		indexInAllSchedules=scheduleIndex;
	} 
	Lisa_OsSchedule *Schedule;

	double Value;

	long Hash;

	/// Index dieses Planes in der Menge allSchedules
	int	indexInAllSchedules;


	/// True, wenn Value bei dem Operator < verwendet werden soll (sonst Hash). Das hat Einfluss auf die 
	/// Sortierfunktion.
	bool sortbyValue;


	/// Zum Loeschen des Schedules per Hand
	void delete_schedule() {
		if (Schedule!=NULL) {
			delete Schedule;
			Schedule=NULL;
		}
	}

	/// Erzeugt aus dem Schedule einen eindeutigen Hashwert.
	long get_hash() {
		if (Hash==-1) {
			int n=Schedule->OSP->n;
			int m=Schedule->OSP->m;

			Lisa_Matrix<int> * LR=new Lisa_Matrix<int>(n,m);
			Schedule->write_LR(LR);
			Hash=0;
			for (int i=1;i<=n;i++) {
				for (int j=1;j<=m;j++) {
					Hash+=(*LR)[i-1][j-1]*(Hash+3)*7+551*i+456*j;
					Hash=Hash&234252478;
				}
			}
				delete LR;
		}
		return Hash;
	}

	bool operator<(const ScheduleValuePair &other ) const {
		if (sortbyValue)
			return (Value< other.Value);
		else
			return Hash < other.Hash;
	}

	bool operator<=(const ScheduleValuePair &other ) const {
		if (sortbyValue)
			return (Value<= other.Value);
		else
			return Hash <= other.Hash;
	}

	bool operator>(const ScheduleValuePair &other ) const {
		if (sortbyValue)
			return (Value> other.Value);
		else
			return Hash > other.Hash;
	}

	bool operator>=(const ScheduleValuePair &other ) const {
		if (sortbyValue)
			return (Value>= other.Value);
		else
			return Hash >= other.Hash;
	}

	/*
	inline std::ostream& operator << (const ScheduleValuePair & GenericObject){
	// GenericObject.write(strm);

	return std::cout;
	}*/

	/*																

	/// stream operator for writing Lisa_GenericObject objects
	inline std::ostream& operator << (std::ostream& strm, 
	const ScheduleValuePair & GenericObject){
	// GenericObject.write(strm);
	return strm;
	}
	*/

	bool operator==(ScheduleValuePair const & other)const {
		return (&other==this);
	}

	bool operator!=(ScheduleValuePair const & other)const {
		return (&other!=this);
	}




	void write(std::ostream & strm) const {
		strm << "ScheduleValuePair" << std::endl;
	}


	void read(std::istream& strm) 
	{

	}

};

 
class OS_BA: public Lisa_GenericBeamACO{
private:


	/// Wendet lokale Suche an, zurueckgeliefert wird genau dann true wenn der Plan verbessert werden konnte. 
	bool  apply_local_seach(Lisa_OsSchedule * schedule);


	/// ACO Parameter:

	/// Trial Parameter: t_ij bezeichnet die Wahrscheinlichkeit, dass Operation i von Operation j ausgefuehrt wird.
	double ** TIJ;

	/// Liefert den Zugriff auf TIJ mit Lisa-Operationen.
	double get_tij(int i1,int j1, int i2, int j2) {
		int i=P->m*i1+j1;
		int j=P->m*i2+j2;
		return TIJ[i][j];
	}

	/// Fester Parameter fuer die Updates der pheromone Werte.
	//double evaporationRate; // p el [0,1];

	/// Aktualisiert die Pheromone Werte entspreched der besten bislang gefundenen Loesung.
	void applyPheromoneUpdate(Lisa_OsSchedule *);

	/// cf=0 bei Start des Algo, cf=1 bei Konvergenz (verwendet TIJ)
	double computeConvergenceFactor();

	/// Setzt alle Eintraege der TIJ auf 0.5
	void resetPheromoneValues();

	/// Zaehlt alle benutzen Plaene (debug only zur Vermeidung von zu vielen new ohne delete
	int numberOfUsedSchedules;
	/// Anzahl der verfuegbaren Plaene (debug only zur Vermeidung von zu vielen new ohne delete
	int maxNumberOfSchedules;
	/// Feste Liste von Plaenen zur Vermeidung von Speicherproblemen. In den
	/// Algorithmen werden stets Verweise auf diese Plaene benutzt.
	Lisa_OsSchedule ** allSchedules;

	/// Leeres Schedule zur Simulation von clear()
	// Lisa_OsSchedule * cleanSchedule;

	/// Loescht den Zugriff auf alle Plaene.
	void clear_schedule_list();

	/// Zeiger auf naechsten freien Speicherplatz in allSchedules.
	int * nextFreeSchedule;

	/// Zeiger auf vorherigen freien Speicherplatz in allSchedules.
	int * lastFreeSchedule;

	/// Zeiger auf naechsten Schedule in allSchedules.
	int * nextSchedule;

	/// Liefert den naechsten freien Plan.
	int get_new_schedule();

	/// Loescht den Plan mit der angegebenen id.
	void delete_schedule(int schedule_id);

	/// Beam Width
	int k_bw;

	/// Max. number of extensions.
	int k_ext;

	/// TODO: Array von Plaenen, die immer wieder verwendet werden, erzeugen. Diese Plaene werden bei der
	/// Initialisierung erzeugt und dann immer wieder verwendet. 
	//Lisa_OsSchedule *bestSchedule;

	///Bewertungsfunktion fuer partielle Loesungen (zunaechst nur Zielfunktionswert, spaoter ACO-Parameter).
	double rating(Lisa_OsSchedule* schedule);

	///Einzelschritt bei der Beam-Suche: jedes Element des Beam wird erweitert.
	void beam_step();

	/// Zur Initialisierung und Weiterbenutzung der Zufallsfunktion.
	long seed;
	//void beam_append(Lisa_OsSchedule * solutionPart,Lisa_List<Lisa_OsSchedule*> *sList);
	// Anfuegen von einer Operation, max k_ext oft.
	void beam_append(Lisa_OsSchedule * solutionPart,Lisa_List<int> *sList);

	int currentStep;
	int lastStep;

	/// Enthaelt die Menge der aktuellen Teilloesungen.
	//	Lisa_List<Lisa_OsSchedule*> *Beam;
	Lisa_List<ScheduleValuePair> *Beam;

	int maxBeamWidth;
	int maxExtensions;
	// Parameter
	int currentBeamWidth;
	int currentExtensions;

	/// startet eine vollstaendige Beam-Suche mit den
	void run_beam_search_step();

	/// do we exclude reversed schedules ?
	bool exclude_reverse;
	/// the real recursion, stripped of unnecessary parameters
	void sub_run(int);
	/// the schedule to work on:
	Lisa_OsProblem * P;
	/// the second schedule used as a backup copy:
	Lisa_OsSchedule * Schedule;  

	/// startet einen beam search Durchlauf
	void run_beam_search();

public:  

	/// constructor
	OS_BA();
	
	// Parameter:
	
	/// Beamweite
	int para_BEAM_WIDTH;
	///Untere Schranke
	double para_LOWER_BOUND;
	/// Obere Schranke
	double para_UPPER_BOUND;
	///Bestimmung der Beam Extensions (1==MED, 2==LDS)
	int para_EXTENSION_STRATEGY;
	/// Anzahl Durchlaeufe
	int para_STEPS;
	///Konvergenzfaktor fuer Neustart
	double para_CONVERGENCE_FACTOR;
	/// evaporationRate
	double para_EVAPORATION_RATE;
	
	/// startet aco beam search
	void run_aco_beam_search(Lisa_OsProblem * Pi, int zfn, Lisa_List<Lisa_Matrix<int>*> *results);
};           

#endif


