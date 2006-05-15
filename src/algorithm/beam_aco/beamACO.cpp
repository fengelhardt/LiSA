// beamACO.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"

//#include "mainapp.cpp"

int ba_main(int argc, char *argv[]) ;


int _tmain(int argc, _TCHAR* argv[])
{


	int i=0;
	/*
	scanf("%i", &i);

	printf("start ");

	scanf("%i", &i);
	*/
	if (ba_main(argc,argv)==-1)
		scanf("%i", &i);
	// int i=0;

	return 0;
}

