/*
*  os_ba.cpp
*  XcodebeamACO
*
*  Created by Per Willenius on 03.04.06.
*  Copyright 2006 __MyCompanyName__. All rights reserved.
*
*/


//#include "stdafx.h"

#include "../../main/global.hpp"
#include "../../basics/order.hpp"

#include "os_ba.hpp"


using namespace std;

// Zur Nutzung von pgraph
class Lisa_Values G_Values;

/// Ein Schedule mit einer zugehoerigen Bewertungsfunktion.
class ScheduleValuePair : public Lisa_Object {

public:

	ScheduleValuePair() {
		Schedule=NULL;
		Value=0;
		Hash=-1;
		sortbyValue=true;
	}

	ScheduleValuePair(Lisa_OsSchedule * currentSchedule,double rating) {
		Schedule=currentSchedule;
		Value=rating;
		Hash=-1;
		sortbyValue=true;
	} 
	Lisa_OsSchedule *Schedule;

	double Value;

	long Hash;


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

			Lisa_Matrix<int> * retLR=new Lisa_Matrix<int>(n,m);
			Schedule->write_LR(retLR);
			Hash=0;
			for (int i=1;i<=n;i++) {
				for (int j=1;j<=m;j++) {
					Hash+=(*retLR)[i-1][j-1]*(Hash+3)*7+551*i+456*j;
					Hash=Hash&234252478;
				}
			}
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

template class Lisa_List<ScheduleValuePair>;

//**************************************************************************

///Bewertungsfunktion.
double OS_BA::rating(Lisa_OsSchedule* schedule) {
	schedule->SetValue(SUM_CI);       
	return(schedule->GetValue());
}


OS_BA::OS_BA()
{
	exclude_reverse=false;
	maxBeamWidth=10;
	maxExtensions=10;
	currentBeamWidth=0;
	currentExtensions=0;
	bestSchedule=NULL;
} 



// ******************************* ab hier neu *************************
void OS_BA::run_beam_search(Lisa_OsProblem * Pi, int zfn, Lisa_List<Lisa_Matrix<int>*> * results) 
{ 

	my_list=results;
	zfn_type=zfn;
	P=Pi;

	currentStep=0;
	lastStep=(P->n)*(P->m);

	// Der Beam enthaelt alle aktuellen temporaeren Plaene
	Beam=new Lisa_List<Lisa_OsSchedule*>();

	// StartPlan
	Lisa_OsSchedule *rootSchedule=new Lisa_OsSchedule(P);
	Beam->append(rootSchedule);
	// Beam enthaet den leeren Plan

	// Start der iterativen Beam-Suche:
	beam_step();

	// Schreiben der Resultate:
	// es wird nur ein Ergebnis zurueckgeliefert.
	if (bestSchedule!=NULL) {
		Lisa_Matrix<int> * retLR=new Lisa_Matrix<int>(P->n,P->m);
		bestSchedule->write_LR(retLR);
		cout << "Return: " << * retLR << endl;
		results->append(retLR);
	}

	delete Beam;
}


/// Erweiterung des Beams.
void OS_BA::beam_step() {

	int beamWidth=2;
	int noOfExtensions=90;

	currentStep++;
	cout << "currentStep= " << currentStep << endl;
	Lisa_List<Lisa_OsSchedule*>  *sList=new Lisa_List<Lisa_OsSchedule*>();
	Beam->reset();
	do {
		beam_append(Beam->get(),sList);

	}	while(Beam->next());

	// nach aufsteigenden Zielfunktionswert sortierte Plaene
	Lisa_List< ScheduleValuePair> *extensions=new Lisa_List<ScheduleValuePair>;

	sList->reset();
	if (sList->length()>0) {
		do {
			Lisa_OsSchedule*currentSchedule=sList->get();		
			ScheduleValuePair * newPair=new ScheduleValuePair(currentSchedule, rating(currentSchedule));
			extensions->append(*newPair);
		} while(sList->next());

		if (extensions->length()>0) {

			// Vorher doppelte Eintraege entfernen:
			extensions->reset();
			do{
				extensions->get().sortbyValue=false;
				extensions->get().get_hash();
			} 
			while(extensions->next());

			// um doppelte Eintraege bereinige extensions:
			Lisa_List< ScheduleValuePair> *extensionsAsNormals=new Lisa_List<ScheduleValuePair>;

			extensions->sort();
			extensions->reset();
			long currentHash=-1;
			do{
				extensions->get().sortbyValue=true;
				if (currentHash!=extensions->get().get_hash()) {
					extensionsAsNormals->append(extensions->get());
					currentHash=extensions->get().get_hash();
				} 
			} 
			while(extensions->next());
			// extensionsAsNormals ist nun korrekt befuellt

			if (extensionsAsNormals->length()>0) {

				// Beam aufraeumen
				if (Beam->length()>0) {
				Beam->reset();
				do {
					Lisa_OsSchedule *schedule=Beam->get();
					delete schedule;
				} while(Beam->next());
				}
				Beam->clear();

				extensionsAsNormals->sort();
				extensionsAsNormals->reset();
				int count=0;

				bestSchedule=extensionsAsNormals->get().Schedule;
				cout << "OBJECTIVE= " << bestSchedule->GetValue() << endl;
				do{
					count++;
					Beam->append(extensionsAsNormals->get().Schedule);
				} 
				while(extensionsAsNormals->next()&&count<noOfExtensions);

				// Nur zum Aufraeumen:
				
				count=0;
				do{
					count++;
					if (count>=noOfExtensions) {
							ScheduleValuePair svp=extensionsAsNormals->get();
							svp.delete_schedule();
					}
				} 
				while(extensionsAsNormals->next());
			}

			delete extensions;
			delete extensionsAsNormals;

			//	if (currentStep<=1) 
			//	beam_step();

			if (currentStep<=lastStep) 
				beam_step();

		}
		delete sList;
	}
}


/// sList erweitern
void OS_BA::beam_append(Lisa_OsSchedule * solutionPart,Lisa_List<Lisa_OsSchedule*> *sList) {
	int Bext=5; // max no of extensions (wird noch nicht verwendet).
			// LR wird leider benoetigt um Operation anzuhaengen.
			
			
//				Lisa_Matrix<int> * retLR=new Lisa_Matrix<int>(P->n,P->m);
			//	solutionPart->write_LR(retLR);
				
				/*
		int *predi=new int[P->n];
		int *predj=new int[P->m];
		
		cout << *retLR << endl; 
	for (int i=1;i<=solutionPart->P->n;i++) {
		int woj=0;
		predi[i]=0;
				int maxVal=0;
				for(int l=1;l<solutionPart->P->n;l++) {
					int newVal=(*retLR)[i-1][l-1];
					if (newVal>maxVal) {
						maxVal=newVal;
						woj=l;
						predi[i]=l;
					}
				}
				cout << "i " << i << "predi[i]=" << predi[i] << "  ";
}		
cout << endl;

		for (int j=1;j<=solutionPart->P->m;j++) {
				int woi=0;
				predj[j]=0;
				int maxVal=0;
				for(int k=1;k<solutionPart->P->n;k++) {
					int newVal=(*retLR)[k-1][j-1];
					if (newVal>maxVal) {
						maxVal=newVal;
						woi=k;
						predj[j]=k;
				//		cout << "j " << j << "predj[j]=" << predj[j] << "  ";
					}
				}
						cout << "j " << j << "predj[j]=" << predj[j] << "  ";
				
}
cout << endl;
*/

	for (int i=1;i<=solutionPart->P->n;i++) {
		for (int j=1;j<=solutionPart->P->m;j++) {
			if (!solutionPart->exists(i,j)) { 
				Lisa_OsSchedule * newShedule=new Lisa_OsSchedule(solutionPart->OSP);
				*newShedule=*solutionPart;
				Lisa_Matrix<int> * retLR=new Lisa_Matrix<int>(P->n,P->m);

					newShedule->write_LR(retLR);
				
				
				//cout << "(" << i<< "," <<j <<")" << * retLR << endl;
				// ende debug only:

				//
				// newShedule->append exisitert leider nicht, deshalb werden hier
				// die passenden woi und woj bestimmt
				// muss spaeter auf jeden Fall optimiert werden

		
				

				// newShedule->insert(i,j, SOURCE, SOURCE);
				// 

				//cout << "(" << i<< "," <<j <<"): woi="<< woi <<" woj=" << woj << * retLR << endl;


// test only:
int woj=0;
				int maxVal=0;
				for(int l=1;l<solutionPart->P->n;l++) {
					int newVal=(*retLR)[i-1][l-1];
					if (newVal>maxVal) {
						maxVal=newVal;
						woj=l;
					}
				}
				
				int woi=0;
				maxVal=0;
				for(int k=1;k<solutionPart->P->n;k++) {
					int newVal=(*retLR)[k-1][j-1];
					if (newVal>maxVal) {
						maxVal=newVal;
						woi=k;
					}
				}
				
				// ende test only

				newShedule->insert(i,j,  woj,woi);
				//newShedule->insert(i,j,  predj[j],predi[i]);
				// newShedule->insert(i,j, SOURCE, SOURCE);

				// debug only:
				/*
				retLR=new Lisa_Matrix<int>(P->n,P->m);
				newShedule->write_LR(retLR);
				cout << "nach insert (" << i<< "," <<j <<")" << * retLR << endl;
				delete retLR;
				*/
				// ende debug only:
				sList->append(newShedule);
			} 
		}
	}
			//		delete retLR;


}



// ******************************* ab hier alt *************************

//**************************************************************************


/*
void OS_BA::run_beam_search(Lisa_OsProblem * Pi, int zfn, 
Lisa_List<Lisa_Matrix<int>*> * results)
{ 
// copy parameters:
my_list=results;
zfn_type=zfn;
P=Pi;

// set up insertion order:
int i, j; 
long seed=123456L;
order = new Lisa_Order(P->n, P->m);
for (i=0; i<P->n ; i++ ) 
for (j=0; j<P->m ; j++ )
switch (ins_order_type)
{
case LPT_ORDER: 
order->read_one_key(i, j, - (*(P->time))[i+1][j+1]);
break;
default:  
order->read_one_key(i, j, lisa_random(1,1000,&seed));
}
order->sort();
// put non-existing operations to the end:
number_ops=0;
for (i=0; i<P->n ; i++ ) 
for (j=0; j<P->m ; j++ )
if ((*(P->sij))[i+1][j+1])
{
order->read_one_key(i, j, 0);
number_ops++;
}
else 
order->read_one_key(i, j, 1);
order->sort();     


// start algorithm:
Schedule =new Lisa_OsSchedule(P);
run_start();
Schedule->insert(order->row(0)+1, order->col(0)+1, SOURCE, SOURCE); 
sub_run(1);
run_stop();
delete Schedule;
delete order;
}
*/

// Rekursion
void OS_BA::sub_run(int op_nr)
{
	Lisa_OsSchedule *Schedule2;
	Lisa_Matrix<int> * erg;

	int posi, posj, i, j;

	// get backup copy of Schedule:
	Schedule2 = new Lisa_OsSchedule(P);

	// Hier Beamweite und MAX-Extensions einfuegen
	// wird hier erst einmal hart gesetzt:

	int myExtension=0;
	// find positions: 

	// TODO: ueber (i,j) iterieren, posi=posj=SINK festlassen

	i=order->row(op_nr)+1; 
	j=order->col(op_nr)+1;
	// Wird nun am Ende angefuegt?
	// posi=SOURCE;   
	posi=SINK;   
	do 
	{
		// posj=SOURCE;
		posj=SINK;
		do 
		{
			//myExtension++;
			if (myExtension<=maxExtensions) 
			{
				(*Schedule2)=(*Schedule);
				count_steps();
				if (Schedule->insert(i,j,posi,posj)>0)
				{
					steps_ok++;
					Schedule->SetValue(zfn_type);
					if ((Schedule->GetValue() < upper_bound) || 
						((Schedule->GetValue() == upper_bound) && 
						(my_list->length()<output_bound)))
					{ 
						if (op_nr+1 < number_ops) 
						{ 
							// Hier Aufruf der Rekursion
							if (!abort_algorithm) sub_run(op_nr+1);
						}
						else 
						{     // Aha, bis zum Ende durchgerechnet                  
							if (upper_bound>Schedule->GetValue())
							{
								upper_bound=Schedule->GetValue();
								my_list->clear();
							} 

							erg = new Lisa_Matrix<int>(P->n, P->m);
							Schedule->write_LR(erg);
							my_list->append(erg);
							if ((Schedule->GetValue()<=given_lower_bound) &&
								(my_list->length()>=output_bound))
								abort_algorithm=true;

						}   // if schedule complete
					}       // if value <= upper_bound       
				}           // if insertion ok
				(*Schedule)=(*Schedule2);
				posj=Schedule->GetMOsucc(i,posj);      
			} // Ende Test auf Extensions
		}
		while (posj!=SINK);
		posi=Schedule->GetJOsucc(posi,j);
	} 
	while (posi!=SINK);     
	delete Schedule2;
}

//**************************************************************************



