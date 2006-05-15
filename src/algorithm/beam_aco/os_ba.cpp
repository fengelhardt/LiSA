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

template class Lisa_List<ScheduleValuePair>;


//**************************************************************************

///Bewertungsfunktion.
double OS_BA::rating(Lisa_OsSchedule* schedule) {

	double minTimeLeft=0;
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
		minTimeLeft+=ci;
	}


	double rand=lisa_random(1,10000,&seed);		
	double dRand=rand/10000.0;
	dRand*=dRand;
	dRand*=0.1; // 

	//schedule->SetValue(zfn_type);
	/*
	if (noOperations<schedule->P->n*schedule->P->m)
		return dRand;
	else
	*/
		return schedule->GetValue()+minTimeLeft+dRand;

	//return schedule->GetValue();


	//should be sophisticated for objectoves other than C_MAX
	/*
	TIMETYP x_bound = 0;
	TIMETYP y_bound = 0;
	TIMETYP r = 0;
	for (int i = 0; i< schedule->P->n;i++){
	for (int j = 0; j < schedule->P->m;j++){
	r += (*(schedule->P->time))[i+1][j+1];
	}
	x_bound = MAX(r,x_bound);
	r=0;
	}
	for (int j = 0; j< schedule->P->m;j++){
	for (int i = 0; i < schedule->P->n;i++){
	r += (*(schedule->P->time))[i+1][j+1];
	}
	y_bound = MAX(r,y_bound);
	r=0;
	}

	return std::max<TIMETYP>((x_bound+y_bound)*schedule->P->m,(x_bound+y_bound)*schedule->P->n);
	*/
}


OS_BA::OS_BA()
{
	exclude_reverse=false;
	maxBeamWidth=10;
	maxExtensions=10;
	currentBeamWidth=0;
	currentExtensions=0;
	//	bestSchedule=NULL;
	tabuList=new bool[MAX_HASH];
	for (int i=0;i<MAX_HASH;i++) {
		tabuList[i]=false;
	}

	para_BEAM_WIDTH=32;
	para_LOWER_BOUND=0;
	para_UPPER_BOUND=1e+06;
	//para_EXTENSION_STRATEGY=2;
	para_EXTENSION_STRATEGY=3;
	para_FIXED_KEXT=1;
	para_STEPS=2500;
	para_CONVERGENCE_FACTOR=0.98;
	para_EVAPORATION_RATE=0.3;
	para_APPEND_STRATEGY=2;

	para_WEIGHT_EST=1.8;
 para_WEIGHT_TIJ=2;
 para_WEIGHT_RAND=1;
para_PRE_SELECT=1;
} 


void OS_BA::clear_schedule_list() {
	// int maxNoOfSchedules=(k_bw+1)*(k_ext+1);
	for (int i=0;i<maxNumberOfSchedules;i++) {
		nextFreeSchedule[i]=i+1;
		lastFreeSchedule[i]=i-1;
		nextSchedule[i]=-1; // noch kein freier Plan verfügbar.
	}
	nextFreeSchedule[maxNumberOfSchedules-1]=0;
	lastFreeSchedule[0]=maxNumberOfSchedules-1;

}


int OS_BA::get_new_schedule() {
	int retVal=nextFreeSchedule[0];
	if (retVal==0) {
		cout << "Error: Max Number of available Schedules reached" << endl;
	}
	numberOfUsedSchedules++;
	if (retVal>0) {
		allSchedules[retVal]->clear();
		int next=nextFreeSchedule[retVal];
		nextFreeSchedule[0]=nextFreeSchedule[retVal];
		if (next>0) 
			lastFreeSchedule[next]=0;
	}
	return retVal;
}


/// Der Plan mit der angegebenen id wird wieder zur Neubenutzung freigegeben.
/// Delete wird dabei nicht ausgeführt.
void OS_BA::delete_schedule(int schedule_id) {
	int nextFree=nextFreeSchedule[0]; 
	nextFreeSchedule[0]=schedule_id;
	nextFreeSchedule[schedule_id]=nextFree;
	lastFreeSchedule[nextFree]=schedule_id;
	lastFreeSchedule[schedule_id]=0;
	numberOfUsedSchedules--;
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
	//Lisa_Matrix<int> * LR=new Lisa_Matrix<int>(P->n,P->m);
	schedule->write_LR(tempLR);
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
						if ((*tempLR)[i1][j1]<(*tempLR)[i2][j2])
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
	//	delete LR;
}


// ******************************* ab hier neu *************************
void OS_BA::run_aco_beam_search(Lisa_OsProblem * Pi, int zfn, Lisa_List<Lisa_Matrix<int>*> * results)  { 	
	run_start();
	my_list=results;
	zfn_type=zfn;
	P=Pi;

	/// Beam Weite:
	//k_bw=MAX(10,(Pi->n)*(Pi->m)/10);
	k_bw= para_BEAM_WIDTH;


	/// Max. number of extensions.
	//k_ext=(Pi->n+1)*(Pi->m+1)/2;
	k_ext=(Pi->n+1)*(Pi->m+1);

	/// Initialisierung der Zufallsfunktion.
	seed=123456L; 

	/// Initialisierung der Pheromon-Werte
	//evaporationRate=0.2;
	tempLR=new Lisa_Matrix<int>(P->n,P->m);

	int maxNoElements=(Pi->n+1)*(P->m+1);
	TIJ=new double*[maxNoElements];
	for (int j=0;j<maxNoElements;j++) {
		TIJ[j]=new double[maxNoElements];
	}

	resetPheromoneValues();

	maxNumberOfSchedules=(k_bw+1)*(k_ext+1);

	cout << "Allocate Memory for " << maxNumberOfSchedules << " Schedules" << endl;
	numberOfUsedSchedules=0;
	allSchedules=new Lisa_OsSchedule*[maxNumberOfSchedules];
	//cleanSchedule=new Lisa_OsSchedule(Pi);

	/// Zeiger auf nächsten freien Speicherplatz in allSchedules.
	nextFreeSchedule=new int[maxNumberOfSchedules];

	/// Zeiger auf nächsten Schedule in allSchedules.
	nextSchedule=new int[maxNumberOfSchedules];

	lastFreeSchedule=new int[maxNumberOfSchedules];

	// bestSchedule=new Lisa_OsSchedule(Pi);
	for (int i=0;i<maxNumberOfSchedules;i++) {
		allSchedules[i]=new Lisa_OsSchedule(Pi);
		allSchedules[i]->ComputeHeadsTails(true, true);
		nextFreeSchedule[i]=i+1;
		lastFreeSchedule[i]=i-1;
		nextSchedule[i]=-1; // noch kein freier Plan verfügbar.
	}
	nextFreeSchedule[maxNumberOfSchedules-1]=0;
	lastFreeSchedule[0]=maxNumberOfSchedules-1;

	cout << "Memory for " << maxNumberOfSchedules << " Schedules Allocated " << endl;

	lastStep=(P->n)*(P->m);	

	// Ende der Initialisierung

	// Bester Plan
	// in Paper als S_bs bezeichnet
	Lisa_OsSchedule* bestSchedule=new Lisa_OsSchedule(Pi);
	bool bestScheduleIsEmpty=true; 
	double bestSumCi=-1;

	cout << "ConvergenceFactor=" << computeConvergenceFactor() << endl;

	// Bester Plan seit dem letzten Reset der Pheromone Values
	// im Paper als s_rb bezeichnet
	Lisa_OsSchedule* bestTempSchedule=new Lisa_OsSchedule(Pi);
	bool bestTempScheduleIsEmpty=true; 

	bool bs_update=false;

	for (int i=1; i<=para_STEPS;i++) {
		//	for (int i=1; i<2;i++) {
		cout << "********* step "<< i << " ****** OBJECTIVE " << bestSumCi << "********" << endl;
		run_beam_search();
		if(abort_algorithm)
			break;
		Beam->reset(); 
		Lisa_OsSchedule* bestScheduleInBeam=NULL;
		double bestObjectiveInBeam=-1; 

		if (Beam->length()>0) 
			do {
				Lisa_OsSchedule* currentSchedule=Beam->get().Schedule;
				currentSchedule->SetValue(zfn_type);
				// Nötig: Test, ob bestSchedule wirklich alle Elemente enthält.
				int operationCount=0;	for (int i=1;i<=P->n;i++) for (int j=1;j<=P->m;j++) {if (currentSchedule->exists(i,j)) operationCount++; }
				// ende Anzahl der Operationen zählen:
				if (operationCount==P->n*P->m) {
					/*
					// debug only:
					long hash=ScheduleValuePair::get_hash(currentSchedule);
					cout << endl << endl << endl;
					cout << "currentSchedule.Hash= " << hash << endl;
					// ende debug only
					*/
					while (apply_local_seach(currentSchedule)) {
						// debug only
						/*
						hash=ScheduleValuePair::get_hash(currentSchedule);
						cout << "sub:" << hash << endl;
						*/
						// ende debug only
						//cout << " : "<< currentSchedule->GetValue() << " ";
					}

					if(bestScheduleInBeam==NULL) {
						bestScheduleInBeam=currentSchedule;
						bestObjectiveInBeam=bestScheduleInBeam->GetValue();
					} else {
						if (bestObjectiveInBeam>currentSchedule->GetValue()) {
							bestScheduleInBeam=currentSchedule;
							bestObjectiveInBeam=currentSchedule->GetValue();
						}
					}
				}
			}  while(Beam->next());

		cout<< "bestObjectiveInBeam=" << bestObjectiveInBeam << endl;
		if (bestScheduleIsEmpty) {
			cout << "new bestSchedule: objective="<< bestObjectiveInBeam << endl;
			bestSumCi=bestObjectiveInBeam;
			*bestSchedule=*bestScheduleInBeam;
			bestScheduleIsEmpty=false;
		} else {
			if(bestSumCi>bestObjectiveInBeam) {
				cout << "new bestSchedule: objective="<< bestObjectiveInBeam << endl;
				bestSumCi=bestObjectiveInBeam;
				*bestSchedule=*bestScheduleInBeam;
			}
		}

		if (bestTempScheduleIsEmpty) {
			*bestTempSchedule=*bestScheduleInBeam;
			bestTempScheduleIsEmpty=false;
			bestTempSchedule->SetValue(zfn_type);
			cout << "new bestTempSchedule: objective="<< bestTempSchedule->GetValue() << endl;
		} else {
			if(bestTempSchedule->GetValue()>bestObjectiveInBeam) {
				cout << "new bestTempSchedule: objective=" << bestObjectiveInBeam << endl;
				*bestTempSchedule=*bestScheduleInBeam;

			}
		}

		if (bs_update) 
			applyPheromoneUpdate(bestTempSchedule);
		else
			applyPheromoneUpdate(bestSchedule);

		double cf=computeConvergenceFactor();	
		cout << "ConvergenceFactor=" << cf << endl;
		if (cf>	para_CONVERGENCE_FACTOR)  {
			cout << "cf>	para_CONVERGENCE_FACTOR bs_update="<< bs_update << endl;
			if (bs_update) {
				cout << "resetPheromoneValues()" << endl;
				resetPheromoneValues();			
				bestTempScheduleIsEmpty=true;
				bs_update=false;
			} else {
				bs_update=true;
			}
		}

		cout << "OBJECTIVE= " << bestSumCi << endl;
		if(abort_algorithm)
			break;
	}


	// Schreiben der Resultate:
	// es wird nur ein Ergebnis zurückgeliefert.
	if (bestSchedule!=NULL) {
		Lisa_Matrix<int> * retLR=new Lisa_Matrix<int>(P->n,P->m);
		bestSchedule->write_LR(retLR);
		cout << "Return: " << * retLR << endl;
		bestSchedule->SetValue(zfn_type);
		results->append(retLR);
	}

	delete Beam;
	delete tempLR;
	delete[] TIJ;

	delete	nextFreeSchedule;
	delete nextSchedule;
	delete lastFreeSchedule;
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
	if(abort_algorithm)
		return;
	currentStep++;
	//	cout << "OS_BA::beam_step currentStep=" << currentStep << endl;
	Lisa_List<int>  *sList=new Lisa_List<int>();
	Beam->reset();
	if (Beam->length()==0) {
		cout << "ERROR in OS_BA::beam_step(): Beam is empty" << endl;
		return;
	}
	do {
		if (para_APPEND_STRATEGY==1)
		  beam_append(Beam->get().Schedule,sList);
		else
		  beam_append_sorted(Beam->get().Schedule,sList);
	}	while(Beam->next());

	// nach aufsteigenden Zielfunktionswert sortierte Pläne
	// TODO: Bessere Bewertungsfunktion
	Lisa_List< ScheduleValuePair> *extensions=new Lisa_List<ScheduleValuePair>;

	sList->reset();
	if (sList->length()>0) {
		do {
			int scheduleIndex=sList->get();
			Lisa_OsSchedule*currentSchedule=allSchedules[scheduleIndex];		
			ScheduleValuePair newPair(currentSchedule, rating(currentSchedule),scheduleIndex);
			extensions->append(newPair);
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

			delete extensionsAsNormals;
		}

	} else {
		cout << "ERROR: sList is empty" << endl; 
		abort_algorithm=true;
		return;
	}
	delete extensions;
	delete sList;

	if (currentStep<lastStep) 
		beam_step();

}

// best: 3660, 3525, 3143, 3087


void OS_BA::beam_append_sorted(Lisa_OsSchedule * solutionPart,Lisa_List<int> *sList) {

	if (solutionPart==NULL) {
		cout << "ERROR in OS_BA::beam_append: solutionPart==NULL" << endl;
		return;
	} 

double max_value_in_order=100000; // Pläne deren Wichtung größer als max_value_in_order ist werden nicht berücksichtigt.

	Lisa_Order * order = new Lisa_Order(P->n, P->m);

	int k_ext_temp=10;
	if( para_EXTENSION_STRATEGY==1) {
		//Straegie MED	
		k_ext_temp=MAX(4,(P->n*P->m-currentStep)/2);
		if (k_ext_temp>k_ext)
			k_ext_temp=k_ext;
	} 
	if( para_EXTENSION_STRATEGY==2) {
		// stategie LDS:
		k_ext_temp=MAX(1,(P->n*P->m)-currentStep);
		if (currentStep>(P->n*P->m)/20)
			k_ext_temp=2;
	}
 if( para_EXTENSION_STRATEGY==3) {
		// stategie LDS:
		k_ext_temp=para_FIXED_KEXT;
	}


	//Lisa_Matrix<int> * LR=new Lisa_Matrix<int>(P->n,P->m);
	solutionPart->write_LR(tempLR);

	int *predi=new int[2*solutionPart->P->n];
	int *predj=new int[2*solutionPart->P->m];

	// direkte Vorgänger der Einfügeposition bestimmen. Diese Bestimmung ist notwendig, da
	// Operationen stets angefügt werden.
	for (int j=1;j<=solutionPart->P->m;j++) {
		int woi=0;
		int maxVal=0;
		predj[j-1]=0;
		for(int k=1;k<=solutionPart->P->n;k++) {
			int newVal=(*tempLR)[k-1][j-1];
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
			int newVal=(*tempLR)[i-1][l-1];
			if (newVal>maxVal) {
				maxVal=newVal;
				predi[i-1]=l;
			}
		}
	}	 

	double* EarliestStartingTimesI=new double[P->n];
	double* EarliestStartingTimesJ=new double[P->m];

	double minEarliestStartingTime=-1;
	double maxEarliestStartingTime=-1;

	for (int i=1;i<=solutionPart->P->n;i++) {
		double maxi=0;
		for (int j=1;j<=solutionPart->P->m;j++) {
			double tes=solutionPart->GetHead(i,j);
			if (tes>0)
				tes+=(*(P->time))[i][j];
				
			if (tes>maxi)
				maxi=tes;
				
		}
		EarliestStartingTimesI[i-1]=maxi;
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
	
	
	for (int i=1;i<=solutionPart->P->n;i++) {
		for (int j=1;j<=solutionPart->P->m;j++) {
			if (!solutionPart->exists(i,j)) { 
			  double tesMax=MAX(EarliestStartingTimesI[i-1],EarliestStartingTimesJ[j-1]);
				if (minEarliestStartingTime<0)
					minEarliestStartingTime=tesMax;
				else 
					if (minEarliestStartingTime>tesMax)
						minEarliestStartingTime=tesMax;
					if (maxEarliestStartingTime<tesMax)
						maxEarliestStartingTime=tesMax;
				}
			}
	}


	double diffEarliestStartingTime=maxEarliestStartingTime-minEarliestStartingTime;
	double tijGlobalMin=-1;
	double tijGlobalMax=-1;

	// Pheromone Parameter
	double alpha=2; // Resultiert in tijMIn*tijMIn.
	double sum_min_tij=0;
	for (int i=1;i<=solutionPart->P->n;i++) {
		for (int j=1;j<=solutionPart->P->m;j++) {
			if (!solutionPart->exists(i,j)) { 
				//double tijMIn=0.5;
				double tijMin=1.5;
				for (int k=1;k<=solutionPart->P->n;k++) {
					if (!solutionPart->exists(k,j) && k!=i) {
						tijMin=MIN(tijMin, get_tij(i-1,j-1,k-1,j-1));
						tijMin*=tijMin; //alpha==2
						if(tijMin<=1) {
						if (tijGlobalMin<0)
							tijGlobalMin=tijMin;
						else
							if (tijGlobalMin>tijMin)
								tijGlobalMin=tijMin;
						if (tijGlobalMax<tijMin)
							tijGlobalMax=tijMin;
}
					}
				}
				for (int l=1;l<=solutionPart->P->m;l++) {
					if (!solutionPart->exists(i,l) && j!=j) {
						tijMin=MIN(tijMin, get_tij(i-1,j-1,i-1,l-1));
						tijMin*=tijMin; 
						if(tijMin<=1) {
						if (tijGlobalMin<0)
							tijGlobalMin=tijMin;
						else
							if (tijGlobalMin>tijMin)
								tijGlobalMin=tijMin;
						if (tijGlobalMax<tijMin)
							tijGlobalMax=tijMin;
}
					}
				}
				// sum_min_tij+=tijMIn*tijMIn*1/(MAX(EarliestStartingTimesI[i-1],EarliestStartingTimesJ[j-1])+1)/sum_n_oi;
			}
		}
	}

	double diffTij=tijGlobalMax-tijGlobalMin;

long elementsToInsert=0; // Anzahl der einzufügenden Operationen
// debug only
long debugFreeOperations=0;


	// T_ec earliest completion time:
	int newScheduleCount=1; // Zählt alle neu hinzugefügten Pläne
	for (int i=1;i<=solutionPart->P->n;i++) {
		for (int j=1;j<=solutionPart->P->m;j++) {
			if (!solutionPart->exists(i,j)) {
				debugFreeOperations++;
				// Zufallsanteil
				double rand=((double)lisa_random(1,100000,&seed))/100000.0;			
				rand*=rand;
				rand*=rand;
				rand*=rand; // Zufallseinfluss nur in seltenen Fällen

				// Pheromonewertanteil
				double tijMin=1.5;
				for (int k=1;k<=solutionPart->P->n;k++) {
					if (!solutionPart->exists(k,j) && k!=i) {
						tijMin=MIN(tijMin, get_tij(i-1,j-1,k-1,j-1));
					}
				}
				for (int l=1;l<=solutionPart->P->m;l++) {
					if (!solutionPart->exists(i,l) && j!=j) {
						tijMin=MIN(tijMin, get_tij(i-1,j-1,i-1,l-1));
					}
				}

				tijMin*=tijMin;
				if(tijMin>1) 
						tijMin=0.5;

				// Anteil der erliest Starting times
				//double p=tijMIn*tijMIn*(1.0/(MAX(EarliestStartingTimesI[i-1],EarliestStartingTimesJ[j-1])+1.0));
				double p=(tijMin-tijGlobalMin)/diffTij*para_WEIGHT_TIJ+
					((MAX(EarliestStartingTimesI[i-1],EarliestStartingTimesJ[j-1]))-minEarliestStartingTime)/diffEarliestStartingTime*para_WEIGHT_EST;
				p+=para_WEIGHT_RAND*rand;
				if (para_PRE_SELECT==0) {
					order->read_one_key(i-1,j-1,p);
					elementsToInsert++;
				}	
				if (para_PRE_SELECT==1) { // Nur Pläne mit minimalen earliest Due date sind zugelassen
					if (MAX(EarliestStartingTimesI[i-1],EarliestStartingTimesJ[j-1])> minEarliestStartingTime+0.001)
						order->read_one_key(i-1,j-1,max_value_in_order+1);
					else {
						order->read_one_key(i-1,j-1,p);
						elementsToInsert++;
					}
				}
			} else
				order->read_one_key(i-1,j-1,max_value_in_order+1);
		}
	}

	order->sort();
//cout << "debugFreeOperations=" << debugFreeOperations << "elementsToInsert=" << elementsToInsert << endl;
	for (int l=0;l<P->m*P->n;l++) {
		int i=order->row(l)+1;
		int j=order->col(l)+1;
		if (!solutionPart->exists(i,j)) { // Test ist eigentlich nicht erforderlich
			int newScheduleIndex=get_new_schedule();
			if(newScheduleIndex==0) // Schedule-Speicher ist voll
				return;
			Lisa_OsSchedule * newShedule=allSchedules[newScheduleIndex];
			*newShedule=*solutionPart;
			if (newShedule->insert(i,j, predj[j-1], predi[i-1])!=-1) {
				newScheduleCount++;
				sList->append(newScheduleIndex);
				}
			else
			{
				cout << "ERROR CYCLE in beam_append" << endl;
				delete_schedule(newScheduleIndex);
			}
			if (newScheduleCount>k_ext_temp |newScheduleCount>elementsToInsert) {
				l=P->m*P->n;
				break;
			}
		}
		}

//cout << "debugFreeOperations=" << debugFreeOperations << "elementsToInsert=" << elementsToInsert << " added:" << newScheduleCount <<endl;

	delete[]EarliestStartingTimesI;
	delete[]EarliestStartingTimesJ;

	delete[] predi;
	delete[] predj;
}



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
	} 
	if( para_EXTENSION_STRATEGY==2) {
		// stategie LDS:
		k_ext_temp=MAX(1,(P->n*P->m)-currentStep);
		if (currentStep>(P->n*P->m)/20)
			k_ext_temp=2;
	}
	if( para_EXTENSION_STRATEGY==3) {
		// stategie LDS:
		k_ext_temp=para_FIXED_KEXT;
	}


	//	Lisa_Matrix<int> * LR=new Lisa_Matrix<int>(P->n,P->m);

	solutionPart->write_LR(tempLR);

	int *predi=new int[2*solutionPart->P->n];
	int *predj=new int[2*solutionPart->P->m];

	// direkte Vorgänger der Einfügeposition bestimmen. Diese Bestimmung ist notwendig, da
	// Operationen stets angefügt werden.
	for (int j=1;j<=solutionPart->P->m;j++) {
		int woi=0;
		int maxVal=0;
		predj[j-1]=0;
		for(int k=1;k<=solutionPart->P->n;k++) {
			int newVal=(*tempLR)[k-1][j-1];
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
			int newVal=(*tempLR)[i-1][l-1];
			if (newVal>maxVal) {
				maxVal=newVal;
				predi[i-1]=l;
			}
		}
	}	 

	double* EarliestStartingTimesI=new double[P->n];
	double* EarliestStartingTimesJ=new double[P->m];

	for (int i=1;i<=solutionPart->P->n;i++) {
		double maxi=0;
		for (int j=1;j<=solutionPart->P->m;j++) {
			double tes=solutionPart->GetHead(i,j);
			if (tes>0)
				tes+=(*(P->time))[i][j];
			if (tes>maxi)
				maxi=tes;
		}
		EarliestStartingTimesI[i-1]=maxi;
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

	double sum_n_oi=0; // Zur Normalisierung der Einträge 
	for (int i=1;i<=solutionPart->P->n;i++) {
		for (int j=1;j<=solutionPart->P->m;j++) {
			if (!solutionPart->exists(i,j)) { 
				sum_n_oi+=1/(MAX(EarliestStartingTimesI[i-1],EarliestStartingTimesJ[j-1])+1);
			}
		}
	}

	// Pheromone Parameter
	double alpha=2; // Resultiert in tijMIn*tijMIn.
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

	// T_ec earliest completion time:
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

				double p=tijMIn*tijMIn*(1.0/(MAX(EarliestStartingTimesI[i-1],EarliestStartingTimesJ[j-1])+1.0)/sum_n_oi)/sum_min_tij;
				double pTest=p*10000;
				if ( (pTest*k_ext_temp >= rand && newScheduleCount<= k_ext_temp) || 
					(solutionPart->P->m*solutionPart->P->n-currentStep<=k_ext_temp)) {
						newScheduleCount++;
						int newScheduleIndex=get_new_schedule();
						if(newScheduleIndex==0) // Schedule-Speicher ist voll
							return;
						Lisa_OsSchedule * newShedule=allSchedules[newScheduleIndex];
						*newShedule=*solutionPart;
						if (newShedule->insert(i,j, predj[j-1], predi[i-1])!=-1)
							sList->append(newScheduleIndex);
						else
						{
							cout << "ERROR CYCLE in beam_append" << endl;
							delete_schedule(newScheduleIndex);
						}
					}		
			}
		}
	}

	if (	newScheduleCount==1) {
		// auf jeden Fall eine Erweiterung hinzufügen:
		for (int i=1;i<=solutionPart->P->n;i++) {
			for (int j=1;j<=solutionPart->P->m;j++) {
				if (!solutionPart->exists(i,j)) {
					int newScheduleIndex=get_new_schedule();
					if(newScheduleIndex==0) // Schedule-Speicher ist voll
						return;
					Lisa_OsSchedule * newShedule=allSchedules[newScheduleIndex];
					*newShedule=*solutionPart;
					if (newShedule->insert(i,j, predj[j-1], predi[i-1])!=-1) {
						sList->append(newScheduleIndex);
						i=P->n+1;
						j=P->m+1;
						break;
					}
					else
					{
						cout << "ERROR CYCLE in beam_append" << endl;
						delete_schedule(newScheduleIndex);
					}


				}
			}
		}


	} 	

	delete[]EarliestStartingTimesI;
	delete[]EarliestStartingTimesJ;

	//	delete LR;
	delete[] predi;
	delete[] predj;

}


/// Wendet lokale Suche an. Zurückgeliefert wird genau dann true wenn der Plan verbessert werden konnte. 
bool  OS_BA::apply_local_seach(Lisa_OsSchedule * schedule) {
	//return false;
	long hash=ScheduleValuePair::get_hash(schedule);

	//cout << "apply_local_seach Hash: " << hash << endl;
	if (tabuList[hash]==true) { // Aha wurde bereits untersucht
		//cout << "eigentlich Abbruch bei Hash: " << hash << endl;
		return false;
	}

	bool retVal=false;
	schedule->SetValue(zfn_type);
	double objective=schedule->GetValue();
	Lisa_OsSchedule * tempSchedule=new Lisa_OsSchedule(P);
	//tempSchedule->ComputeHeads=false;
	Lisa_OsSchedule * bestTempSchedule=NULL;

	//	Lisa_Matrix<int> * LR=new Lisa_Matrix<int>(P->n,P->m);
	schedule->write_LR(tempLR);

	//cout << *LR << endl;

	// Als Quelle einfügen
	for (int i=1;i<=P->n;i++) {
		for (int j=1;j<=P->m;j++) {
			if (schedule->exists(i,j)) { 
				if ((*tempLR)[i-1][j-1]!=1) {
					*tempSchedule=*schedule;
					if (!schedule->exists(i,j))
						cout << "Error 1" << endl;
					tempSchedule->exclude(i,j);
					tempSchedule->insert(i,j,SOURCE,SOURCE);
					tempSchedule->SetValue(zfn_type);
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

	// Als Senke einfügen
	int *predi=new int[2*P->n];
	int *predj=new int[2*P->m];


	for (int j=1;j<=P->m;j++) {
		int woi=0;
		int maxVal=0;
		predj[j-1]=0;
		for(int k=1;k<=P->n;k++) {
			int newVal=(*tempLR)[k-1][j-1];
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
			int newVal=(*tempLR)[i-1][l-1];
			if (newVal>maxVal) {
				maxVal=newVal;
				predi[i-1]=l;
			}
		}
	}	 


	for (int i=1;i<=P->n;i++) {
		for (int j=1;j<=P->m;j++) {
			if (schedule->exists(i,j)) { 
				if (predj[j-1]>0 && predi[i-1]>0 && j != predi[i-1] && i!= predj[j-1]) {
					*tempSchedule=*schedule;				
					if (!tempSchedule->exists(i,j))
						cout << "Error 1" << endl;
					tempSchedule->exclude(i,j);

					tempSchedule->insert(i,j, predj[j-1], predi[i-1]);
					tempSchedule->SetValue(zfn_type);
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

	for (int i=1;i<=P->n;i++) {
		for (int j=1;j<=P->m;j++) {
			if (schedule->exists(i,j)) { 	

				int jopred=tempSchedule->GetJOpred(i,j);
				int mopred=tempSchedule->GetMOpred(i,j);
				if (jopred>0) {
					*tempSchedule=*schedule;			
					int joprepred=tempSchedule->GetJOpred(jopred,j);
					if (joprepred>0) {
						tempSchedule->exclude(i,j);
						int test=tempSchedule->insert(i,j, joprepred, mopred);
						if (test!=ERROR && test!=CYCLE) {
							tempSchedule->SetValue(zfn_type);
							if (tempSchedule->GetValue()<objective) {
								objective=tempSchedule->GetValue();
								if (bestTempSchedule==NULL)
									bestTempSchedule=new Lisa_OsSchedule(P);
								*bestTempSchedule=*tempSchedule;
							}
						}
					}
				}



				if (mopred>0) {
					*tempSchedule=*schedule;			
					int moprepred=tempSchedule->GetMOpred(i,mopred);
					if (moprepred>0) {
						tempSchedule->exclude(i,j);
						int test=tempSchedule->insert(i,j, jopred, moprepred);
						if (test!=ERROR && test!=CYCLE) {
							tempSchedule->SetValue(zfn_type);
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
		}
	}


	//cout << "hash" << hash << " " ; 
	tabuList[hash]=true; // Wird als bereits untersucht gekennzeichnet.
	//tabuList[ScheduleValuePair::get_hash(schedule)]=true; // Aha wurde bereits untersucht

	if (bestTempSchedule!=NULL) {
		*schedule=*bestTempSchedule;
		schedule->SetValue(zfn_type);
		retVal=true;
	}

	// cout << "retVal=" << retVal<<endl;
	//delete LR;
	delete tempSchedule;
	delete[] predi;
	delete[] predj;
	if (bestTempSchedule!=NULL)
		delete bestTempSchedule;
	return retVal;

}


//**************************************************************************



