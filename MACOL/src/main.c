#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "util/gfile.h"
#include "graphe.h"

int main (int argc, char * const argv[]) {

	printf("Procedure de calcul\n");
	
	solutionFileName = malloc(sizeof(char)*250);
	fitnessFileName = malloc(sizeof(char)*250);
	
	
	//srand(time(NULL));
	srand(123456);

	
	
	clock_t startTime=clock();
	clock_t currentStartTime;
	double elapsedTime;
	
	
	//// >>>>>>>>>  affichage de l'heure au debut
	time_t rawtime;
	struct tm * timeinfo;
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	printf ( "Lancement du calcul :  %s", asctime (timeinfo) );
	//// <<<<<<<<<  affichage de l'heure de debut

	
	
	for (int i=0; i<1; i++) {
		
		
///////// >>>>>> Lancement sur dsjc500.5
		// params 1000.5
		nbColor=83;
		populationSize=8;
		nbLocalSearch=30000;
		// nbIter 30
		
		// params 500.5
//		nbColor=48;
//		populationSize=8;
//		nbLocalSearch=8000;
		// nbIter 3000
		
		
		loadGraphe("data/DSJC1000.5.txt");
		fitnessFileName="fitness_dsjc500_5_boucle.csv";
		solutionFileName="solution_dsjc500_5_boucle.csv";
		
		currentStartTime=clock();
		
		initRandomColor();
		//initConflict();
		improvInitPop();
		
		
		while (nbEdgesConflict > 0 && nbIterationsCross<1000)
			runIteration();
		
		if (nbEdgesConflict==0)
			save();
		
		elapsedTime=(clock()-currentStartTime) / (double)CLOCKS_PER_SEC;
		printf("\n\n%d (boucle)  => Fin apres %d iteration et %d croisements (%f sec)\n",i,nbIterations, nbIterationsCross,elapsedTime);
		printf("\tNombre d'iterations pour le fils final avant solution : %d\n", nbIterations%nbLocalSearch);
		printf("\t\tRecherche meilleur : %fsec\n", time1 / (double)CLOCKS_PER_SEC);
		printf("\t\tMise a jour : %fsec\n", time2 / (double)CLOCKS_PER_SEC);
		
		
		//cleanMatrix();

	}	
	
	
	
    elapsedTime=(clock()-startTime) / (double)CLOCKS_PER_SEC;
	printf("<< Procedure de calcul apres: %f secondes\n",elapsedTime);
	printf("\ttemps d'initialisation des conflits : %f sec\n", time1 / (double)CLOCKS_PER_SEC);
	printf("\ttemps de croisement : %f sec\n", time2 / (double)CLOCKS_PER_SEC);
	printf("\ttemps d'intensification tabucol : %f sec\n", time3 / (double)CLOCKS_PER_SEC);
	
	
	
	initConflict();
	printf("JUST FOR CHEKING : %d   %d\n\n", nbEdgesConflict, nbNodesConflict);
	
	
	//// >>  affichage de l'heure a la fin
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	printf ( "Fin du calcul :  %s", asctime (timeinfo) );
	//// <<  affichage de l'heure a la fin
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
//	///////////////////////////////////////////////////////////////
//	/////// Ajout pour analyse de la façon dont on se rapproche de la solution finale
//	///////////////////////////////////////////////////////////////
//	
//	int* tOptimum;
//	tOptimum=malloc(sizeof(int) * nbSommets);
//	for (int i=0; i<nbSommets; i++) {
//		tOptimum[i]=tColor[i];
//	};
//	srand(123456);
//	initRandomColor();
//	initConflict();
//	improvInitPop();
//	
//	
//	startTime=clock();
//	
//	printf("\n\n\n2EME PASSE POUR ANALYSE\n");
//	printf("nb edges conflits au demarrage: %d\n", nbEdgesConflict);
//	printf("nb nodes conflits au demarrage: %d\n", nbNodesConflict);
//	
//	
//	
//	while (nbEdgesConflict > 0 /*&& nbIterations<600000*/) {
//		runIteration();
//		
//		printf("Proximite avec l'optimum : ");
//		for (int i=0; i<populationSize; i++) {
//			int proxi=getProximite(tOptimum, tPopulationColor[i]);
//			printf("\t %d", proxi);
//		}
//		printf("\n");
//	}
//	
//	
//
//	printf("Proximite avec l'optimum : ");
//	int proxi=getProximite(tOptimum, tColor);
//	printf("\t %d", proxi);
//	printf("\n");
	
    return 0;
}
