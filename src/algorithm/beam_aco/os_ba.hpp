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

//const int MAX_HASH = 234252478;
const long MAX_HASH = 2524713;

/// beam ACO implementation for open shop
/** @author Per Willenius
@version 3.0
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


	/// Zum L√∂schen des Schedules per Hand
	void delete_schedule() {
		if (Schedule!=NULL) {
			delete Schedule;
			Schedule=NULL;
		}
	}

	 static long get_hash(Lisa_OsSchedule * schedule) {
			int n=schedule->OSP->n;
			int m=schedule->OSP->m;

			//
			long retVal=0;
			
			for (int i=1;i<=n;i++) {
				for (int j=1;j<=m;j++) {
					retVal+=(long)schedule->GetJOpred(i,j)*(retVal-MAX_HASH/2)*91+127*i+467*j;
					retVal=retVal%MAX_HASH;
					retVal+=(long)schedule->GetJOsucc(i,j)*(retVal-MAX_HASH/2)*73+274*i+456*j;
					retVal=retVal%MAX_HASH;
					retVal+=(long)schedule->GetMOpred(i,j)*(retVal-MAX_HASH/2)*62+742*i+375*j;
					retVal=retVal%MAX_HASH;
					retVal+=(long)schedule->GetMOsucc(i,j)*(retVal-MAX_HASH/2)*53+741*i+573*j;
					//Hash=Hash&234252478;
					retVal=retVal%MAX_HASH;
					if (retVal<0)
						retVal=-retVal;
					//Hash=Hash&MAX_HASH;
				}
			}
				return retVal;
	}


	/// Erzeugt aus dem Schedule einen eindeutigen Hashwert.
	/// Der Hashwert wird dabei nur einmal ermittelt. D.h. werden 
	/// weitere Operationen angefuegt, dann �ndert sich der Hashwert nicht.
	long get_hash() {
		if (Hash==-1) {
			Hash=get_hash(Schedule);
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

	/// Enthaelt alle Plaene, deren Nachbarschaft vollstaendig untersucht wurde. Wenn 
	/// tabuList[Hasch(Schedulule)]==true ist braucht die Nachbarschaft von Schedulule nicht weiter 
	/// untersucht zu werden.
	bool *tabuList;

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

	///Bewertungsfunktion fuer partielle Loesungen (zunaechst nur Zielfunktionswert, spaeter ACO-Parameter).
	double rating(Lisa_OsSchedule* schedule);

	///Einzelschritt bei der Beam-Suche: jedes Element des Beam wird erweitert.
	void beam_step();

	/// Zur Initialisierung und Weiterbenutzung der Zufallsfunktion.
	long seed;
	
	/// Anfuegen von einer Operation, maximal k_ext oft.
	void beam_append(Lisa_OsSchedule * solutionPart,Lisa_List<int> *sList);

/// Anfuegen von einer Operation, max k_ext oft. (Hier wird die Bedeutung der Einzelbwertungen gewichtet 
	/// und vorher sortiert).
	void beam_append_sorted(Lisa_OsSchedule * solutionPart,Lisa_List<int> *sList);

	/// Das benoetigte LR wird bur einmal allokiert und dann staendig benutzt, um
	/// Zit fuer das new und die Speicherfreigabe zu sparen.
	Lisa_Matrix<int> * tempLR;

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
	///Bestimmung der Beam Extensions (1==MED, 2==LDS, 3==FIXED)
	int para_EXTENSION_STRATEGY;
	///Bewerung der einzufaegenden Operationen bei Append (1==Original, 2==SORTED)
	int para_APPEND_STRATEGY;
	/// Anzahl Durchlaeufe
	int para_STEPS;
	///Konvergenzfaktor fuer Neustart
	double para_CONVERGENCE_FACTOR;
	/// evaporationRate
	double para_EVAPORATION_RATE;
	/// Anzahl der Beam-Extensions bei  para_EXTENSION_STRATEGY==FIXED.
	int para_FIXED_KEXT;
	/// Beeinflusst, welchen Einfluss, die Earliest Starting time auf den
	/// Einfuegemechnaismus hat.
		double para_WEIGHT_EST;
	/// Beeinflusst, welchen Einfluss, die Pheromone Werte auf den
	/// Einfuegemechnaismus haben.
	double para_WEIGHT_TIJ;
	///  Beeinflusst, welchen Einfluss, der Zufall auf den Einfuegemechnaismus hat.
double para_WEIGHT_RAND;
/// Gibt an welcher pre-Selection Typ angewendet werden soll (0-keins, 1 - active)
long para_PRE_SELECT; 


	/// startet aco beam search
	void run_aco_beam_search(Lisa_OsProblem * Pi, int zfn, Lisa_List<Lisa_Matrix<int>*> *results);
};           

#endif


