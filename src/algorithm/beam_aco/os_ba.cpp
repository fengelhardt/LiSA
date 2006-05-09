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

#include "os_ba.h"


using namespace std;

// Zur Nutzung von pgraph
class Lisa_Values G_Values;

/// Ein Schedule mit einer zugeh√∂rigen Bewertungsfunktion.
/*
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

};*/

template class Lisa_List<ScheduleValuePair>;



//**************************************************************************

///Bewertungsfunktion.
double OS_BA::rating(Lisa_OsSchedule* schedule) {

	double lowerBound=0;
	double pCount=0;
	double maxP=0;
	int noOperations=0;
	for (int i=1;i<=schedule->P->n;i++) {
		double ci=0;
		for (int j=1;j<=schedule->P->m;j++) {
			if (!schedule->exists(i,j)) {
				double pij=(*(P->time))[i][j];
				noOperations++;
				ci+=pij;
				pCount+=pij;
				if (maxP<pij)
					maxP=pij;
			}
		}
		lowerBound+=ci;
	}


	schedule->SetValue(SUM_CI);

	/*
	if (noOperations>P->n*P->m/2.0)
	return schedule->GetValue()-lowerBound;
	else
	return schedule->GetValue();
	*/

	//return (schedule->GetValue()-lowerBound);
	//return schedule->GetValue()+((double)lisa_random(0,100,&seed))/100.0;
	return schedule->GetValue();

	// Zielfunktion soll erst gegen Ende der 
}


OS_BA::OS_BA()
{
	exclude_reverse=false;
	maxBeamWidth=10;
	maxExtensions=10;
	currentBeamWidth=0;
	currentExtensions=0;
	bestSchedule=NULL;
	
		/*
	 <integer name="BEAM_WIDTH" caption="Beamweite" default="100"/>
    <real name="LOWER_BOUND" caption="Untere Schranke" default="0"/>
    <real name="UPPER_BOUND" caption="Obere Schranke" default="1e+06"/>
    <choice name="EXTENSION_STRATEGIE" caption="Bestimmung der Beam Extensions">
      <item name="MED"/>
      <item name="LDS"/>
    </choice>
    <integer name="STEPS" caption="Anzahl Durchlaeufe" default="50"/>
    <real name="CONVERGENCE_FACTOR" caption="Konvergenzfaktor fuer Neustart" default="0.9"/>
    <real name="EVAPORATION_RATE" caption="evaporationRate" default="0.7" />
*/
	para_BEAM_WIDTH=100;
	para_LOWER_BOUND=0;
	para_UPPER_BOUND=1e+06;
	para_EXTENSION_STRATEGY=1;
	para_STEPS=50;
	para_CONVERGENCE_FACTOR=0.9;
	para_EVAPORATION_RATE=0.7;
} 


void OS_BA::clear_schedule_list() {

	int maxNoOfSchedules=(k_bw+1)*(k_ext+1);
	for (int i=0;i<maxNoOfSchedules;i++) {
		nextFreeSchedule[i]=i+1;
		lastFreeSchedule[i]=i-1;
		nextSchedule[i]=-1; // noch kein freier Plan verfügbar.
	}

}

int OS_BA::get_new_schedule() {

	int retVal=nextFreeSchedule[0];

	if (retVal==0) {
		cout << "Error: Max Number of available Schedules reached" << endl;

	}
	numberOfUsedSchedules++;
	if (retVal>0) {
	 // Lisa_OsSchedule * newSchedule=allSchedules[retVal];
		//*newSchedule=*cleanSchedule; // (oder folgendes)
		allSchedules[retVal]->clear();
		//*(allSchedules[retVal])=*cleanSchedule;
		int next=nextFreeSchedule[retVal];
		nextFreeSchedule[0]=nextFreeSchedule[retVal];
		if (next>0) 
			lastFreeSchedule[next]=0;
	}
	//cout << "new Schedules: no=" << numberOfUsedSchedules << " of " << maxNumberOfSchedules << endl;

	return retVal;
}


/// Der Plan mit der angegebenen id wird wieder zur Neubenutzung freigegeben.
void OS_BA::delete_schedule(int schedule_id) {
	int nextFree=nextFreeSchedule[0]; 
	nextFreeSchedule[0]=schedule_id;
	nextFreeSchedule[schedule_id]=nextFree;
	lastFreeSchedule[nextFree]=schedule_id;
	lastFreeSchedule[schedule_id]=0;
	numberOfUsedSchedules--;
	//cout << "del Schedules: no=" << numberOfUsedSchedules << " of " << maxNumberOfSchedules << endl;
}


void OS_BA::resetPheromoneValues() {
	for (int i1=0;i1<P->n;i1++) {
		for (int j1=0;j1<P->m;j1++) {
			for (int i2=0;i2<P->n;i2++) {
				for (int j2=0;j2<P->m;j2++) {
					int indexo1=P->m*i1+j1;
					int indexo2=P->m*i2+j2;
					TIJ[indexo1][indexo2]=0.5;
				}
			}
		}
	}
}


double OS_BA::computeConvergenceFactor() {
	double retVal=0;
	double t_min=0.001;
	double t_max=0.999;
	int count=0;
	double sum=0;
	for (int i1=1;i1<P->n;i1++) {
		for (int j1=1;j1<P->m;j1++) {
			for (int i2=1;i2<P->n;i2++) {
				for (int j2=1;j2<P->m;j2++) {
					int indexo1=P->m*i1+j1;
					int indexo2=P->m*i2+j2;
					if (i1==i2 || j1==j2) {
						count++;
						double tij=TIJ[indexo1][indexo2];
						sum+=MAX(t_max-tij,tij-t_min);
					}
				}
			}
		}
	}
	retVal=2*(sum/((double)count*(t_max-t_min))-0.5);
	return retVal;
}


void  OS_BA::applyPheromoneUpdate(Lisa_OsSchedule * schedule) {
	Lisa_Matrix<int> * LR=new Lisa_Matrix<int>(P->n,P->m);
	schedule->write_LR(LR);
	// Zur Vermeidung zu schneller Konvergenz
	double t_min=0.001;
	double t_max=0.999;
	for (int i1=1;i1<P->n;i1++) {
		for (int j1=1;j1<P->m;j1++) {
			for (int i2=1;i2<P->n;i2++) {
				for (int j2=1;j2<P->m;j2++) {
					int indexo1=P->m*i1+j1;
					int indexo2=P->m*i2+j2;
					if (i1==i2 || j1==j2) {
						double tij=TIJ[indexo1][indexo2];
						double d=0;
						if ((*LR)[i1][j1]<(*LR)[i2][j2])
							d=1;
						TIJ[indexo1][indexo2]=tij+para_EVAPORATION_RATE*(d-tij);
						if ( TIJ[indexo1][indexo2]<t_min)
							TIJ[indexo1][indexo2]=t_min;
						if ( TIJ[indexo1][indexo2]>t_max)
							TIJ[indexo1][indexo2]=t_max;
					}
				}
			}
		}
	}
	delete LR;
}


// ******************************* ab hier neu *************************
void OS_BA::run_aco_beam_search(Lisa_OsProblem * Pi, int zfn, Lisa_List<Lisa_Matrix<int>*> * results) 
{ 

	my_list=results;
	zfn_type=zfn;
	P=Pi;

	/// Beam Weite:
	//k_bw=MAX(10,(Pi->n)*(Pi->m)/10);
	
	k_bw= para_BEAM_WIDTH;

	

	/// Max. number of extensions.
	//k_ext=(Pi->n+1)*(Pi->m+1)/2;
	k_ext=(Pi->n+1)*(Pi->m+1);

	cout << "k_bw=" << k_bw << endl;

	/// Initialisierung der Zufallsfunktion.
	seed=123456L; 

	/// Initialisierung der Pheromon-Werte
	evaporationRate=0.2;

	int maxNoElements=(Pi->n+1)*(P->m+1);
	TIJ=new double*[maxNoElements];
	for (int j=0;j<maxNoElements;j++) {
		TIJ[j]=new double[maxNoElements];
	}

	resetPheromoneValues();

	int maxNoOfSchedules=(k_bw+1)*(k_ext+1);
	maxNumberOfSchedules=maxNoOfSchedules;
	numberOfUsedSchedules=0;
	allSchedules=new Lisa_OsSchedule*[maxNoOfSchedules];
cleanSchedule=new Lisa_OsSchedule(Pi);
	
	/// Zeiger auf nächsten freien Speicherplatz in allSchedules.
	nextFreeSchedule=new int[maxNoOfSchedules];

	/// Zeiger auf nächsten Schedule in allSchedules.
	nextSchedule=new int[maxNoOfSchedules];

	lastFreeSchedule=new int[maxNoOfSchedules];

	bestSchedule=new Lisa_OsSchedule(Pi);
	for (int i=0;i<maxNoOfSchedules;i++) {
		allSchedules[i]=new Lisa_OsSchedule(Pi);
		allSchedules[i]->ComputeHeadsTails(true, true);
		nextFreeSchedule[i]=i+1;
		lastFreeSchedule[i]=i-1;
		nextSchedule[i]=-1; // noch kein freier Plan verfügbar.
	}
	nextFreeSchedule[maxNoOfSchedules-1]=0;
	lastFreeSchedule[0]=maxNoOfSchedules-1;


	lastStep=(P->n)*(P->m);	
	
	// Ende der Initialisierung
	
	Lisa_OsSchedule* currentBestSchedule=new Lisa_OsSchedule(Pi);

	double bestSumCi=-1;
	
	cout << "ConvergenceFactor=" << computeConvergenceFactor() << endl;

  for (int i=1; i<=para_STEPS;i++) {
	//	for (int i=1; i<2;i++) {
	  cout << "********* step "<< i << " ****** OBJECTIVE " << bestSumCi << "********" << endl;
		run_beam_search();
		Beam->reset(); 
		if (Beam->length()>0) 
		 do {
		 		bool needApply=false;
		  Lisa_OsSchedule* currentSchedule=Beam->get().Schedule;
		  currentSchedule->SetValue(SUM_CI);
		  // Nötig: Test, ob bestSchedule wirklich alle Elemente enthält.
		  int operationCount=0;	for (int i=1;i<=P->n;i++) for (int j=1;j<=P->m;j++) {if (currentSchedule->exists(i,j)) operationCount++; }
			// ende Anzahl der Operationen zählen:
			if (operationCount==P->n*P->m) {
			  //cout << "currentValue=";
				while (apply_local_seach(currentSchedule)) {
					//cout << " : "<< currentSchedule->GetValue() << " ";
				}
				//cout << currentSchedule->GetValue() << endl;

				if (bestSumCi<0) {
					bestSumCi=currentSchedule->GetValue();
					needApply=true;	
				}
				if (bestSumCi>currentSchedule->GetValue()) {
					bestSumCi=currentSchedule->GetValue();
					needApply=true;	
				}
				cout << "OBJECTIVE= " << bestSumCi << endl;
				if (needApply) {
				cout << "OBJECTIVE= " << bestSumCi << endl;
				  //cout << "new " << bestSumCi << endl;
					*currentBestSchedule=*currentSchedule;
					
					applyPheromoneUpdate(currentSchedule);
					double cf=computeConvergenceFactor();
					cout << "ConvergenceFactor=" << cf << endl;
					if (cf>	para_CONVERGENCE_FACTOR)  {//0.7 ist zu klein 
					  	cout << "resetPheromoneValues()" << endl;
  					resetPheromoneValues();						
						}
	      }	
			} 
		} while(Beam->next());
		cout << "OBJECTIVE= " << bestSumCi << endl;
	delete Beam;
 }


	// Schreiben der Resultate:
	// es wird nur ein Ergebnis zurückgeliefert.
	if (currentBestSchedule!=NULL) {
		Lisa_Matrix<int> * retLR=new Lisa_Matrix<int>(P->n,P->m);
		currentBestSchedule->write_LR(retLR);
		cout << "Return: " << * retLR << endl;
		currentBestSchedule->SetValue(SUM_CI);
		//cout << "Return Objective= " << currentBestSchedule->GetValue();
		results->append(retLR);
	}
}


void OS_BA::run_beam_search() {
	currentStep=0;
	clear_schedule_list();
	// Der Beam enthält alle aktuellen temporären Pläne
	//Beam=new Lisa_List<Lisa_OsSchedule*>();
	Beam=new Lisa_List<ScheduleValuePair>();

	// StartPlan
	int rootScheduleIndex=get_new_schedule();
	//Lisa_OsSchedule *rootSchedule=new Lisa_OsSchedule(P);
	Lisa_OsSchedule *rootSchedule=allSchedules[rootScheduleIndex];
	ScheduleValuePair newSPair(rootSchedule, -1,rootScheduleIndex);
	Beam->append(newSPair);
	// Beam enthält den leeren Plan

	// Start der iterativen Beam-Suche:
	beam_step();

	//delete Beam;
}


/// Erweiterung des Beams.
void OS_BA::beam_step() {
	currentStep++;
//	cout << "OS_BA::beam_step currentStep=" << currentStep << endl;
	Lisa_List<int>  *sList=new Lisa_List<int>();
	Beam->reset();
	if (Beam->length()==0) {
	cout << "ERROR in OS_BA::beam_step(): Beam is empty" << endl;
return;
	}
	do {
		beam_append(Beam->get().Schedule,sList);
	}	while(Beam->next());

	// nach aufsteigenden Zielfunktionswert sortierte Pläne
	// TODO: Bessere Bewertungsfunktion
	Lisa_List< ScheduleValuePair> *extensions=new Lisa_List<ScheduleValuePair>;

	sList->reset();
	if (sList->length()>0) {
		do {
			int scheduleIndex=sList->get();
			Lisa_OsSchedule*currentSchedule=allSchedules[scheduleIndex];		
			ScheduleValuePair * newPair=new ScheduleValuePair(currentSchedule, rating(currentSchedule),scheduleIndex);
			extensions->append(*newPair);
		} while(sList->next());

		if (extensions->length()>0) {
			// Vorher doppelte Einträge entfernen:
			extensions->reset();
			do{
				extensions->get().sortbyValue=false;
				extensions->get().get_hash();
			} 
			while(extensions->next());

			// um doppelte Einträge bereinige extensions:
			Lisa_List< ScheduleValuePair> *extensionsAsNormals=new Lisa_List<ScheduleValuePair>;

			extensions->sort();
			extensions->reset();
			long currentHash=-1;
			do{
				extensions->get().sortbyValue=true;
				if (currentHash!=extensions->get().get_hash()) {
					extensionsAsNormals->append(extensions->get());
					currentHash=extensions->get().get_hash();
				} else {
					//cout << " Hash: ";
					delete_schedule(extensions->get().indexInAllSchedules);
				}
			} 
			while(extensions->next());

			// extensionsAsNormals ist nun korrekt befüllt

			if (extensionsAsNormals->length()>0) {

				// Beam aufräumen
				if (Beam->length()>0) {
					Beam->reset();
					do {
						ScheduleValuePair svp=Beam->get();
						delete_schedule(svp.indexInAllSchedules);
					} while(Beam->next());
				}
				Beam->clear();

				extensionsAsNormals->sort();
				extensionsAsNormals->reset();
				int count=0;

				*bestSchedule=*(extensionsAsNormals->get().Schedule);

				// debug only: Anzahl der Operationen zählen:
				int operationCount=0;	
				for (int i=1;i<=P->n;i++) 
					for (int j=1;j<=P->m;j++) {
						if (bestSchedule->exists(i,j))
							operationCount++;
					}

					//if (currentStep!=operationCount)
					//bestSchedule->SetValue(SUM_CI);
					//int objective=(int)(bestSchedule->GetValue());
					//if (operationCount==30 && objective==1164)
					//	cout << " opDiff=" << currentStep- operationCount << endl;
					// ende debug only


					// cout << "OBJECTIVE= " << bestSchedule->GetValue() << "  " << (int)(((double)currentStep/lastStep)*100)<< "%"<< endl;
					do{
						count++;
						if(count<k_bw) {
							Beam->append(extensionsAsNormals->get());
			}
						else {
							ScheduleValuePair svp=extensionsAsNormals->get();
							delete_schedule(svp.indexInAllSchedules);
						}
					} 
					while(extensionsAsNormals->next());
			}

			delete extensions;
			delete extensionsAsNormals;

			//	if (currentStep<=1) 
			//	beam_step();


	//cout << " currentStep=" << currentStep << "  lastStep=" << lastStep << endl;

//			if (currentStep<=lastStep) 
//				beam_step();

		}
	} else {
	cout << "ERROR: sList is empty" << endl; 
	}
		delete sList;
	
	// cout << " currentStep=" << currentStep << "  lastStep=" << lastStep << endl;
			if (currentStep<lastStep) 
				beam_step();

}

// best: 3660, 3525

/// sList erweitern
void OS_BA::beam_append(Lisa_OsSchedule * solutionPart,Lisa_List<int> *sList) {

if (solutionPart==NULL) {
cout << "ERROR in OS_BA::beam_append: solutionPart==NULL" << endl;
return;
} 

int k_ext_temp=10;
	if( para_EXTENSION_STRATEGY==1) {
	//Straegie MED	
	k_ext_temp=MAX(4,(P->n*P->m-currentStep)/2);
	if (k_ext_temp>k_ext)
		k_ext_temp=k_ext;
		} else {
	// stategie LDS:
	k_ext_temp=MAX(1,(P->n*P->m)-currentStep);
	if (currentStep>(P->n*P->m)/20)
	k_ext_temp=2;
	}
	
	Lisa_Matrix<int> * LR=new Lisa_Matrix<int>(P->n,P->m);
	solutionPart->write_LR(LR);

	int *predi=new int[2*solutionPart->P->n];
	int *predj=new int[2*solutionPart->P->m];

// direkte Vorgänger der Einfügeposition bestimmen.
	for (int j=1;j<=solutionPart->P->m;j++) {
		int woi=0;
		int maxVal=0;
		predj[j-1]=0;
		for(int k=1;k<=solutionPart->P->n;k++) {
			int newVal=(*LR)[k-1][j-1];
			if (newVal>maxVal) {
				maxVal=newVal;
				woi=k;
				predj[j-1]=k;
			}
		}
	}
	for (int i=1;i<=solutionPart->P->n;i++) {
		int maxVal=0;
		predi[i-1]=0;
		for(int l=1;l<=solutionPart->P->m;l++) {
			int newVal=(*LR)[i-1][l-1];
			if (newVal>maxVal) {
				maxVal=newVal;
				predi[i-1]=l;
			}
		}
	}	 

	double* EarliestStartingTimesI=new double[P->n];
	double* EarliestStartingTimesJ=new double[P->m];
	double* MinTIJ_I=new double[P->n];
	double* MinTIJ_J=new double[P->m];

	for (int i=1;i<=solutionPart->P->n;i++) {
		double maxi=0;
		double min_tij=-1;
		for (int j=1;j<=solutionPart->P->m;j++) {
			double tes=solutionPart->GetHead(i,j);
			if (tes>0)
				tes+=(*(P->time))[i][j];
			if (tes>maxi)
				maxi=tes;
		}
		EarliestStartingTimesI[i-1]=maxi;
		MinTIJ_I[i-1]=min_tij;
	}
	for (int j=1;j<=solutionPart->P->m;j++) {
		double maxj=0;
		for (int i=1;i<=solutionPart->P->n;i++) {
			double tes=solutionPart->GetHead(i,j);
			if (tes>0)
				tes+=(*(P->time))[i][j];
			if (tes>maxj)
				maxj=tes;
		}
		EarliestStartingTimesJ[j-1]=maxj;
	}
	double sum_n_oi=0;
	for (int i=1;i<=solutionPart->P->n;i++) {
		for (int j=1;j<=solutionPart->P->m;j++) {
			if (!solutionPart->exists(i,j)) { 
				sum_n_oi+=1/(MAX(EarliestStartingTimesI[i-1],EarliestStartingTimesJ[j-1])+1);
			}
		}
	}

	// Pheromone Parameter
	double alpha=2;
	double sum_min_tij=0;
	for (int i=1;i<=solutionPart->P->n;i++) {
		for (int j=1;j<=solutionPart->P->m;j++) {
			if (!solutionPart->exists(i,j)) { 
				double tijMIn=0.5;
				for (int k=1;k<=solutionPart->P->n;k++) {
				  if (!solutionPart->exists(k,j) && k!=i) {
				  	tijMIn=MIN(tijMIn, get_tij(i-1,j-1,k-1,j-1));
				}
				}
				for (int l=1;l<=solutionPart->P->m;l++) {
				if (!solutionPart->exists(i,l) && j!=j) {
				  	tijMIn=MIN(tijMIn, get_tij(i-1,j-1,i-1,l-1));
				}
				}
				sum_min_tij+=tijMIn*tijMIn*1/(MAX(EarliestStartingTimesI[i-1],EarliestStartingTimesJ[j-1])+1)/sum_n_oi;
			}
		}
	}


/*
	// es wird ein Faktor berechnet, der für die Konvergenzfaktoren garantiert, dass nicht zu viele Teilpläne
	// erzeugt werden
	for (int i=1;i<=solutionPart->P->n;i++) {
		for (int j=1;j<=solutionPart->P->m;j++) {
			// Es kommen nur zwei Vorgängeroperationen in Frage:
			// o(i,predj[j]) und o(predi[i],j)
			// 0: nicht, 0.5 weiss nicht, 1 ja
			double p1=0;
			if (predj[j-1]>0) {
				p1=get_tij(i-1,j-1,i-1,predj[j-1]-1)-0.5;
			}
			double p2=0;
			if (predi[i-1]>0){
				p2=get_tij(i-1,j-1,predi[i-1],j-1)-0.5;
			}
			double p=p1*p2*2.0+1; // p=0, oij wird hier eingefügt, p=1 oij wird später eingefügt
		}
	}
	*/

	// T_ec earliest completion time:

	// CIJ bestimmen:
	int newScheduleCount=1; // Zählt alle neu hinzugefügten Pläne
	for (int i=1;i<=solutionPart->P->n;i++) {
		for (int j=1;j<=solutionPart->P->m;j++) {
			if (!solutionPart->exists(i,j)) {
				long rand=lisa_random(1,10000,&seed);			
				double tijMIn=0.5;
				
				for (int k=1;k<=solutionPart->P->n;k++) {
				if (!solutionPart->exists(k,j) && k!=i) {
				  	tijMIn=MIN(tijMIn, get_tij(i-1,j-1,k-1,j-1));
				}
				}
				for (int l=1;l<=solutionPart->P->m;l++) {
				if (!solutionPart->exists(i,l) && j!=j) {
				  	tijMIn=MIN(tijMIn, get_tij(i-1,j-1,i-1,l-1));
				}
				}
				
				/*
				if (predi[i-1]>0)
					tijMIn=MIN(tijMIn, get_tij(i-1,j-1,predi[i-1]-1,j-1));
				if (predj[j-1]>0)
					tijMIn=MIN(tijMIn, get_tij(i-1,j-1,i-1, predj[j-1]-1));
					*/
					
				// alpha=2:
				//double p=tijMIn*tijMIn*1/(MAX(EarliestStartingTimesI[i-1],EarliestStartingTimesJ[j-1])+1)/sum_n_oi;
				// erst mal ohne tij-einfluss:
				double p=tijMIn*tijMIn*(1.0/(MAX(EarliestStartingTimesI[i-1],EarliestStartingTimesJ[j-1])+1.0)/sum_n_oi)/sum_min_tij;

				double pTest=p*10000;
				
				// cout << "xax tijMIn=" << tijMIn << " pTest*k_ext_temp=" << pTest*k_ext_temp << "rand=" << rand << " newScheduleCount=" << newScheduleCount << endl;
				
				//if (pTest*k_ext >= rand && newScheduleCount<= k_ext) {
				if ( (pTest*k_ext_temp >= rand && newScheduleCount<= k_ext_temp) || 
				  (solutionPart->P->m*solutionPart->P->n-currentStep<=k_ext_temp)) {
					newScheduleCount++;
					int newScheduleIndex=get_new_schedule();
					if(newScheduleIndex==0) // Schedule-Speicher ist voll
						return;
					Lisa_OsSchedule * newShedule=allSchedules[newScheduleIndex];
					*newShedule=*solutionPart;
					newShedule->insert(i,j, predj[j-1], predi[i-1]);
					//newShedule->insert(i,j, SOURCE, SOURCE);
					sList->append(newScheduleIndex);
				}		
			}
		}
	}

	delete[]EarliestStartingTimesI;
	delete[]EarliestStartingTimesJ;
	delete[]MinTIJ_I;
	delete[]MinTIJ_J;

	delete LR;
	delete[] predi;
	delete[] predj;
	
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
// long seed=123456L;
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

/*
void OS_BA::sub_run(int op_nr)
{
Lisa_OsSchedule *Schedule2;
Lisa_Matrix<int> * erg;

int posi, posj, i, j;

// get backup copy of Schedule:
Schedule2 = new Lisa_OsSchedule(P);

// Hier Beamweite und MAX-Extensions einfügen
// wird hier erst einmal hart gesetzt:

int myExtension=0;
// find positions: 

// TODO: über (i,j) iterieren, posi=posj=SINK festlassen

i=order->row(op_nr)+1; 
j=order->col(op_nr)+1;
// Wird nun am Ende angefügt?
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
}*/




/// Wendet lokale Suche an, zurückgeliefert wird genau dann true wenn der Plan verbessert werden konnte. 
bool  OS_BA::apply_local_seach(Lisa_OsSchedule * schedule) {



	bool retVal=false;
	schedule->SetValue(SUM_CI);
	double objective=schedule->GetValue();
	Lisa_OsSchedule * tempSchedule=new Lisa_OsSchedule(P);
	//tempSchedule->ComputeHeads=false;
	Lisa_OsSchedule * bestTempSchedule=NULL;

	Lisa_Matrix<int> * LR=new Lisa_Matrix<int>(P->n,P->m);
	schedule->write_LR(LR);

	// Als Quelle einfügen
	/*
	for (int i=1;i<=P->n;i++) {
		for (int j=1;j<=P->m;j++) {
			if (schedule->exists(i,j)) { 
				if ((*LR)[i-1][j-1]!=1) {
					*tempSchedule=*schedule;
					if (!schedule->exists(i,j))
						cout << "Error 1" << endl;
					tempSchedule->exclude(i,j);
					tempSchedule->insert(i,j,SOURCE,SOURCE);
					tempSchedule->SetValue(SUM_CI);
					if (tempSchedule->GetValue()<objective) {
						objective=tempSchedule->GetValue();					
						if (bestTempSchedule==NULL)
							bestTempSchedule=new Lisa_OsSchedule(P);
						*bestTempSchedule=*tempSchedule;
					}
				}
			}
		}
	}
	*/

	// Als Senke einfügen
	int *predi=new int[2*P->n];
	int *predj=new int[2*P->m];


	for (int j=1;j<=P->m;j++) {
		int woi=0;
		int maxVal=0;
		predj[j-1]=0;
		for(int k=1;k<=P->n;k++) {
			int newVal=(*LR)[k-1][j-1];
			if (newVal>maxVal) {
				maxVal=newVal;
				woi=k;
				predj[j-1]=k;
			}
		}
	}

	for (int i=1;i<=P->n;i++) {
		int maxVal=0;
		predi[i-1]=0;
		for(int l=1;l<=P->m;l++) {
			int newVal=(*LR)[i-1][l-1];
			if (newVal>maxVal) {
				maxVal=newVal;
				predi[i-1]=l;
			}
		}
	}	 



	for (int i=1;i<=P->n;i++) {
		for (int j=1;j<=P->m;j++) {
			if (schedule->exists(i,j)) { 
				*tempSchedule=*schedule;				
				if (!tempSchedule->exists(i,j))
					cout << "Error 1" << endl;
				tempSchedule->exclude(i,j);

				if (predj[j-1]>0 && predi[i-1]>0 && j != predi[i-1] && i!= predj[j-1]) {
					tempSchedule->insert(i,j, predj[j-1], predi[i-1]);
					tempSchedule->SetValue(SUM_CI);
					if (tempSchedule->GetValue()<objective) {
						objective=tempSchedule->GetValue();
						if (bestTempSchedule==NULL)
							bestTempSchedule=new Lisa_OsSchedule(P);
						*bestTempSchedule=*tempSchedule;
					}
				}
			}
		}
	}


	if (bestTempSchedule!=NULL) {
		*schedule=*bestTempSchedule;
		schedule->SetValue(SUM_CI);
		retVal=true;
	}

	delete LR;
	delete tempSchedule;
	if (bestTempSchedule!=NULL)
		delete bestTempSchedule;
	return retVal;

}


//**************************************************************************



