/*
 *  graphe.c
 *  tabucol
 *
 *  Created by Laurent Moalic on 14/02/11.
 *  Copyright 2011 UTBM. All rights reserved.
 *
 */

#include <stdlib.h>
#include "graphe.h"
#include "util/gfile.h"



clock_t  time1=0;
clock_t  time2=0;
clock_t  time3=0;


float lambda=0.6;
int L=10;


void loadGraphe(char* filename){	
	openfile(filename);

	
	char* buf;
	char* tok;
	
	int nbArretesAjoutee=0;
	
	while (buf=readUncommentedLine()) {
		tok=getNextToken();
		
		if(*tok=='p'){ // lecture de la taille du graphe
			tok=getNextToken(); //pas interessant
			tok=getNextToken();
			nbSommets=atoi(tok);
			tok=getNextToken();
			nbArretes=atoi(tok);
			
			// creation du graphe
			tConnect=malloc(sizeof(char*) * nbSommets);
			for(int i=0; i<nbSommets; i++){
				tConnect[i]=malloc(sizeof(char) * nbSommets);
				for (int j=0; j<nbSommets; j++)
					tConnect[i][j]=0;
			}
			
			// creation de la couleur associée à chaque sommet pour tous les individus de la population
			tPopulationColor=malloc(sizeof(int*) * populationSize);
			for (int i=0; i<populationSize; i++) {
				tPopulationColor[i] = malloc(sizeof(int) * nbSommets);
			}
			tColor = tPopulationColor[0];
			
			// creation de la meilleure solution trouvee
			tBestSol = malloc(sizeof(int) * nbSommets);
	
			
			// creation du nombre de conflits de chaque sommet
			tConflicts = malloc(sizeof(int) * nbSommets);
			
			// creation de l'impact pour chaque sommet d'une transition vers chaque couleur
			tNewConflitsWithColor = malloc(sizeof(int*) * nbSommets );
			for(int i=0; i<nbSommets; i++){
				tNewConflitsWithColor[i]=malloc(sizeof(int) * (nbColor+2));
			}			
			
			// creation des periodes tabou des sommets
			tTabou = malloc(sizeof(int*) * nbSommets);
			for(int i=0; i<nbSommets; i++){
				tTabou[i]=malloc(sizeof(int) * (nbColor));
			}
			
			// creation de la liste des meilleures ameliorations
			tBestImprove = malloc(sizeof(int) * nbSommets);  // valeur
			tNbBestImprove = malloc(sizeof(int) * nbSommets); // couleur
			ttBestImproveColor = malloc(sizeof(int*) * nbSommets); // couleur
			for (int i=0; i<nbSommets; i++) {
				ttBestImproveColor[i]=malloc(sizeof(int) * nbColor);
			}
			
			// creation de la liste des noeuds avec conflit
			tNodeWithConflict = malloc(sizeof(int) * nbSommets);
			tNodeAdded = malloc(sizeof(int) * nbSommets);
			
			
			
			// MACOL - creation de la distance entre chaque couple d'individus
			tDistSolSol = malloc(sizeof(int*) * populationSize);
			for (int i=0; i<populationSize; i++) {
				tDistSolSol[i]=malloc(sizeof(int) * populationSize);
			}

			// MACOL - creation de la fitness de chaque individu (nombre d'arretes en conflit)
			tFitness = malloc(sizeof(int) * populationSize);
			
			// MACOL - creation de la couleur associe a chaque noeud du fils
			tChild = malloc(sizeof(int) * nbSommets);
			
			// MACOL - tableau des indices des parents pour creation du fils
			tParentsIndices = malloc(sizeof(int) * MAX_NB_PARENTS);
			
			
			printf("Sommets ajoutes: %d\n", nbSommets);
		}
		
		if(*tok=='e'){ // lecture d'une arrete
			tok=getNextToken();
			int v1=atoi(tok);
			tok=getNextToken();
			int v2=atoi(tok);
			tConnect[v1-1][v2-1]=1;
			tConnect[v2-1][v1-1]=1;
			nbArretesAjoutee++;
		}
	}
	
	printf("Arretes ajoutees: %d / %d\n",nbArretesAjoutee ,nbArretes);
	closefile();


	buildVoisins();
}


void cleanMatrix(){
	for(int i=0; i<nbSommets; i++){
		free(tConnect[i]);
	}
	free(tConnect);
	
	for (int i=0; i<nbSommets; i++) {
		free(tVoisins[i]);
	}
	free(tVoisins);
	free(tNbVoisins);
	
	
	for (int i=0; i<populationSize; i++) {
		free(tPopulationColor[i]);
	}
	free(tPopulationColor);
	
	free(tBestSol);

	free(tConflicts);
	
	for(int i=0; i<nbSommets; i++){
		free(tNewConflitsWithColor[i]);
	}
	free(tNewConflitsWithColor);
				
	
	for(int i=0; i<nbSommets; i++){
		free(tTabou[i]);
	}
	free(tTabou);
	
	free(tBestImprove);
	free(tNbBestImprove);
	for(int i=0; i<nbSommets; i++){
		free(ttBestImproveColor[i]);
	}
	free(ttBestImproveColor);
	
	free(tNodeWithConflict);
	
	free(tNodeAdded);
	

	for (int i=0; i<populationSize; i++) {
		free(tDistSolSol[i]);
	}
	free(tDistSolSol);
	
	
	free(tFitness);	
	
	free(tChild);
	
	free(tParentsIndices);
	
}

void buildVoisins(){
	tNbVoisins=malloc(sizeof(int)*nbSommets);
	tVoisins=malloc(sizeof(int*)*nbSommets);
	for (int i=0; i<nbSommets; i++) {
		int nbVois=0;
		for (int j=0; j<nbSommets; j++) {
			if(tConnect[i][j])
				nbVois++;
		}
		tNbVoisins[i]=nbVois;
		tVoisins[i]=malloc(sizeof(int)*nbVois);
		
		int pos=0;
		for (int j=0; j<nbSommets; j++) {
			if(tConnect[i][j])
				tVoisins[i][pos++]=j;
		}
	}
}


void initRandomColor(){
	for (int i=0; i<populationSize; i++) {
		for (int j=0; j<nbSommets; j++) {
			int val=(rand()/(float)RAND_MAX) * nbColor ;
			tPopulationColor[i][j]=val;
		}
	}
	bestIterMean=0;
	nbIterations=0;
	nbIterationsCross=0;
	nbIterationsSinceRestart=0;
	nbBestEdgesConflict=999999;
}


void improvInitPop(){
	for (int i=0; i<populationSize; i++) {
		tColor=tPopulationColor[i];
		initConflict();
		
		for (int j=0; j<nbLocalSearch; j++) {
			determineBestImprove();
		}
		
		tFitness[i]=nbEdgesConflict;		
		
		updateDistTable(i);
	}
}



void initConflict(){	
	/// determine les conflits entre les noeuds
	nbNodesConflict=0;
	nbEdgesConflict=0;
	for (int i=0; i<nbSommets; i++)
		tConflicts[i]=0;
	
	for (int i=0; i<nbSommets; i++) {
		for (int j=i; j<nbSommets; j++) {
			if (tConnect[i][j] && tColor[i]==tColor[j]) {
				tConflicts[i]++;
				tConflicts[j]++;
				nbEdgesConflict++;
				if (tConflicts[i]==1)nbNodesConflict++;
				if (tConflicts[j]==1)nbNodesConflict++;
			}
		}
	}
	
	
	/// determine les delta-conflits pour chaque transition de couleur
	for (int i=0; i<nbSommets; i++) {
		int nbCurrentConflict=tConflicts[i];
		for (int j=0; j<nbColor; j++) {
			tNewConflitsWithColor[i][j]=-nbCurrentConflict;
		}
		
		int nbVoisins=tNbVoisins[i];
		for (int j=0; j<nbVoisins; j++) {
			tNewConflitsWithColor[i][tColor[tVoisins[i][j]]]++;		
		}
	}
	
	
	
	/// initialise les durées tabou
	for (int i=0; i<nbSommets; i++) {
		for (int j=0; j<nbColor; j++) {
			tTabou[i][j]=-1;
		}
	}
	
	
	/// Determine les meilleures transition de chaque sommet
	for (int i=0; i<nbSommets; i++) {
		int bestVal=99999;
		int nbBestVal=0;
		for (int j=0; j<nbColor; j++) {
			int val=tNewConflitsWithColor[i][j];
			if (val<bestVal){
				nbBestVal=0;
				bestVal = val;
			}

			if(val<=bestVal){
				ttBestImproveColor[i][nbBestVal++]=j;
			}

		}
		tBestImprove[i] = bestVal;
		tNbBestImprove[i] = nbBestVal;
		
		//printf("%d/%d/%d  ", bestVal, bestColor, nbBestVal);
	}
	
	
	/// Determination des noeuds avec conflict (par défaut tous les noeuds)
	resetNodeWithConflict();
	
	//printf("\n\n\n\n");
}






void determineBestImprove(){
	/// choisit aléatoirement parmis les meilleurs noeuds
	
	nbIterations++;
	nbIterationsSinceRestart++;
	
	int bestVal=99999;
	int bestNode=-1;
	int bestColor=-1;
	int nbBestVal=0;
	
	//time1-=clock();
	
	
//	for(int i=0; i<nbSommets; i++){
//		if(tConflicts[i]>0){
//			int color = tColor[i];
//			
//			for (int j=0; j<nbColor; j++) {
//				int currentImprove=tNewConflitsWithColor[i][j]; 
//				if(currentImprove<bestVal && j!=color && (tTabou[i][j]<nbIterations || (currentImprove+nbEdgesConflict) < nbBestEdgesConflict)){
//					bestVal=currentImprove;
//					bestNode=i;
//					bestColor=j;
//					nbBestVal=1;
//				}
//				else if(currentImprove==bestVal && j!=color && (tTabou[i][j]<nbIterations || (currentImprove+nbEdgesConflict) < nbBestEdgesConflict)){ // on tire aleatoirement 1 des 2 
//					nbBestVal++;
//					int val=(rand()/(float)RAND_MAX) * nbBestVal ;
//					if(val==0){
//						bestVal=currentImprove;
//						bestNode=i;
//						bestColor=j;
//					}
//				}
//
//			}
//		}
//	}
	
//	for(int i=0; i<nbSommets; i++){
	for(int ind=0; ind<nbNodeWithConflict; ind++){
		int i=tNodeWithConflict[ind];
		if(tConflicts[i]>0 && tBestImprove[i]<=bestVal){
			int color = tColor[i];
			int currentBestImprove = tBestImprove[i];
			int currentNbBestImprove = tNbBestImprove[i];
			int added=0; /// permet de savoir si on a reussi à ajouter une valeure (1=true)
			
			for (int j=0; j<currentNbBestImprove; j++) {
				int currentCol=ttBestImproveColor[i][j];
				if (currentCol!=color && (tTabou[i][currentCol]<nbIterations || (currentBestImprove+nbEdgesConflict) < nbBestEdgesConflict)) {
					added=1;
					if (currentBestImprove < bestVal) {
						bestVal=currentBestImprove;
						bestNode=i;
						bestColor=currentCol;
						nbBestVal=1;						
					}
					else { // cas où la valeur est égale à la meilleure : on tire aleatoirement 1 des 2 
						nbBestVal++;
						int val=(rand()/(float)RAND_MAX) * nbBestVal ;
						if(val==0){
							bestNode=i;
							bestColor=currentCol;
						}
					}
				}
			}
			
			if (currentBestImprove < bestVal && added == 0) { /// on doit tout vérifier
				for (int j=0; j<nbColor; j++) {
					int currentImprove=tNewConflitsWithColor[i][j]; 
					if(currentImprove<bestVal && j!=color && (tTabou[i][j]<nbIterations || (currentImprove+nbEdgesConflict) < nbBestEdgesConflict)){
						bestVal=currentImprove;
						bestNode=i;
						bestColor=j;
						nbBestVal=1;
					}
					else if(currentImprove==bestVal && j!=color && (tTabou[i][j]<nbIterations || (currentImprove+nbEdgesConflict) < nbBestEdgesConflict)){ // on tire aleatoirement 1 des 2 
						nbBestVal++;
						int val=(rand()/(float)RAND_MAX) * nbBestVal ;
						if(val==0){
							bestVal=currentImprove;
							bestNode=i;
							bestColor=j;
						}
					}
				}
			}
			
		}
	}
	
	//time1+=clock();
	///
	
	//printf("%d/%d/%d  ", bestNode, tBestImproveColor[bestNode], tNbBestImprove[bestNode]);
	
	/// modifie la couleur du meilleur node trouve
	if(bestNode>-1){
		//time2-=clock();
		updateTables(bestNode, bestColor);
		//time2+=clock();
	}
	
	
	if (nbEdgesConflict < nbBestEdgesConflict) {
		nbBestEdgesConflict = nbEdgesConflict;
	}
}




void updateTables(int node, int color){
	int prevColor=tColor[node];
	tColor[node]=color;	
	
	int rd=(rand()/(float)RAND_MAX) * L;
	
	tTabou[node][prevColor]=nbIterations + rd + lambda*nbNodesConflict;
	

	int nbVoisins=tNbVoisins[node];
	for (int i=0; i<nbVoisins; i++) {
		int indiceSommet=tVoisins[node][i];
		
		/// répercution sur les voisins
		if (tColor[indiceSommet] == prevColor) {
			tConflicts[indiceSommet]--;
			tConflicts[node]--;
			nbEdgesConflict--;
			if (tConflicts[indiceSommet]==0) nbNodesConflict--;
			if (tConflicts[node]==0) nbNodesConflict--;
			
			for(int j=0; j<nbColor; j++){
				tNewConflitsWithColor[indiceSommet][j]++;
				tNewConflitsWithColor[node][j]++;
			}
			tBestImprove[indiceSommet]++;
			tBestImprove[node]++;

		}
		else if (tColor[indiceSommet] == color) {
			tConflicts[indiceSommet]++;
			tConflicts[node]++;
			nbEdgesConflict++;
			if (tConflicts[indiceSommet]==1){
				nbNodesConflict++;
				if (tNodeAdded[indiceSommet]!=1) {
					tNodeAdded[indiceSommet]=1;
					tNodeWithConflict[nbNodeWithConflict++]=indiceSommet;
				}
			}
			if (tConflicts[node]==1){
				nbNodesConflict++;
				if (tNodeAdded[node]!=1) {
					tNodeAdded[node]=1;
					tNodeWithConflict[nbNodeWithConflict++]=node;
				}
			}
			
			for(int j=0; j<nbColor; j++){
				tNewConflitsWithColor[indiceSommet][j]--;
				tNewConflitsWithColor[node][j]--;
			}
			tBestImprove[indiceSommet]--;
			tBestImprove[node]--;
		}
		
		tNewConflitsWithColor[indiceSommet][prevColor]--;
		tNewConflitsWithColor[indiceSommet][color]++;
			
		/////////////////////
		//// ajout pour garder la meilleur transition
		int bestImprove=tBestImprove[indiceSommet];
		
		if (tNewConflitsWithColor[indiceSommet][prevColor] < bestImprove) {
			tBestImprove[indiceSommet]--;
			ttBestImproveColor[indiceSommet][0] = prevColor;
			tNbBestImprove[indiceSommet] = 1;
		}
		else if(tNewConflitsWithColor[indiceSommet][prevColor] == bestImprove){
			ttBestImproveColor[indiceSommet][tNbBestImprove[indiceSommet]] = prevColor;
			tNbBestImprove[indiceSommet]++;
		}

		
		if ( (tNewConflitsWithColor[indiceSommet][color]-1) == bestImprove) {// si c'était le meilleur
//			int nbBestImprove=tNbBestImprove[indiceSommet];
//			
//			if (nbBestImprove>1) {
//				
//				tNbBestImprove[indiceSommet]--;
//				int pos, found=0;
//				for (pos=0; found!=1; pos++) {
//					if (ttBestImproveColor[indiceSommet][pos] == color) 
//						found=1;
//				}
//				
//				for (pos; pos<nbBestImprove; pos++) {
//					ttBestImproveColor[indiceSommet][pos-1] = ttBestImproveColor[indiceSommet][pos];
//				}
//				
//			}
//			else {
				int nbBestVal=0;
				int bestVal=999999;
				int bestColor;
				
				for (int j=0; j<nbColor; j++) {
					int val=tNewConflitsWithColor[indiceSommet][j];
					if (val<bestVal) {
						bestVal=val;
						ttBestImproveColor[indiceSommet][0] = j;
						nbBestVal=1;
					}
					else if(val==bestVal){
						ttBestImproveColor[indiceSommet][nbBestVal] = j;
						nbBestVal++;
					}
				}
				tBestImprove[indiceSommet] = bestVal;
				tNbBestImprove[indiceSommet] = nbBestVal;
//			}
		}
		//printf("%d/%d/%d  ", tBestImprove[indiceSommet], ttBestImproveColor[indiceSommet][0], tNbBestImprove[indiceSommet]);
	}
	
	
	if (nbIterations % 100 == 0) { // arbitrairement tous les 100 on recalcule les noeuds avec conflit
		resetNodeWithConflict();
	}

}


void resetNodeWithConflict(){
	nbNodeWithConflict=0;
	for (int i=0; i<nbSommets; i++)
		tNodeAdded[i]=0;
	
	for (int i=0; i<nbSommets; i++){
		if(tConflicts[i]>0){
			tNodeWithConflict[nbNodeWithConflict++]=i;
			tNodeAdded[i]=1;
		}
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////
/////////////// EA PART////////////////
///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// Choisit n parents, cree un enfant, l'intensifie et met a jour la population
void runIteration(){
	// Selection des parents
	int nbParents=2;
	
	selectParents(nbParents);
	
	buildChildN(nbParents);


	tColor=tChild;
	
	initConflict();

	
	bestFitnessValue=99999;
	for (int i=0; i<nbLocalSearch && nbEdgesConflict > 0 ; i++) {
		determineBestImprove();
		if (nbEdgesConflict<bestFitnessValue) {
			bestFitnessValue=nbEdgesConflict;
			bestIter=i;
			for (int j=0; j<nbSommets; j++) {
				tBestSol[j]=tColor[j];
			}
		}
	}
	
	bestIterMean=((nbIterationsCross-1)*bestIterMean + bestIter)/nbIterationsCross;
	
	printNewChild(nbParents);
	saveFitnessValue();
	
	// remplacement d'un parent si la solution n'est pas trouvee
	if (nbEdgesConflict>0) {
		updatePopulation();		
	}
}



void selectParents(int nbParents){
	char* tIsUsed=malloc(sizeof(char) * populationSize);
	for (int i=0; i<populationSize; i++)
		tIsUsed[i]=0;
	
	for (int i=0; i<nbParents; i++) {
		int pos=(int)((rand()/(float)RAND_MAX) * populationSize);
		while ( tIsUsed[pos] ) {
			pos=(pos+1)%populationSize;
		}
		tParentsIndices[i] = pos;
		tIsUsed[pos]=1;
	}
	free(tIsUsed);
}


/// Effectue le croisement entre les n parents
/// les parents sont les tColor (couleur de chaque sommet) passés dans **p
void buildChildN(int n){
	nbIterationsCross++;
	int** tSizeOfColors = malloc((sizeof(int*) * n));
	
	for (int i=0; i<n; i++) {
		tSizeOfColors[i] = malloc(sizeof(int) * nbColor);
		
		int* currentParent=tPopulationColor[tParentsIndices[i]];
		
		for (int j=0; j<nbColor; j++){
			tSizeOfColors[i][j]=0;
		}
		
		
		for (int j=0; j<nbSommets; j++) {
			tSizeOfColors[i][currentParent[j]]++; /// compte le nombre de couleurs
		}

		
	}

	
	// initialisation du fils qu'on va construire
	for (int i=0; i<nbSommets; i++)
		tChild[i]=-1;
	
	int* currentParent;
	int* currentSizeOfColors;
	
	double valMax;
	int colorMax;
	
	
	for (int i=0; i<nbColor; i++) {
		int indice=i%n;
		currentParent = tPopulationColor[tParentsIndices[indice]];
		currentSizeOfColors = tSizeOfColors[indice];
		valMax=-1;
		colorMax=-1;
		
		//determine le max des restants
		for (int j=0; j<nbColor; j++) {
			double currentVal = (double)currentSizeOfColors[j];

			if (currentVal>valMax) {
				valMax=currentVal;
				colorMax=j;
			}
		}
		

		
		//affecte la couleur aux noeuds du fils et met a jour les noeuds restant à attribuer au fils
		for (int j=0; j<nbSommets; j++) {
			if (currentParent[j]==colorMax && tChild[j]<0) {
				tChild[j]=i;
				
				for (int k=0; k<n; k++) {
					tSizeOfColors[k][tPopulationColor[tParentsIndices[k]][j]]--;
				}
			}
		}
	}
	
	
	/// complete les noeuds n'ayant pas recu de couleur des parents
	for (int i=0; i<nbSommets; i++) {
		if (tChild[i]<0) {
			tChild[i]=(rand()/(float)RAND_MAX) * nbColor ;
		}
	}
	
	
	for (int i=0; i<n; i++) {
		free(tSizeOfColors[i]);
	}
	free(tSizeOfColors);
	
	
}


// met a jour la population avec l'individu nouvellement cree 
void updatePopulation(){
	int indice;
	//// >>>> Choix de la plus vieille solution pour remplacement
	indice=nbIterationsCross%populationSize; // ici on prend le parent le plus vieux
	//// <<<< Choix de la plus vieille solution pour remplacement
	
//	//// >>>> Choix de la moins bonne solution pour remplacement
//	int maxFitness = 0;
//	int maxProxi = 0;
//	for (int i=0; i<populationSize; i++) {
//		if(tFitness[i]>maxFitness){
//			maxFitness = tFitness[i];
//			maxProxi = getMaxProxi(i);
//			indice = i;
//		}
//		else if(tFitness[i] == maxFitness){
//			int proxi = getMaxProxi(i);
//			if (proxi > maxProxi) {
//				maxFitness = tFitness[i];
//				maxProxi = proxi;
//				indice = i;
//			}
//		}
//	}
//	//// <<<< Choix de la moins bonne solution pour remplacement	
	
	
	
	// echange pour conserver la meilleure solution
	int* tmp=tPopulationColor[indice];
	tPopulationColor[indice]=tBestSol;
	tBestSol=tmp;
	tFitness[indice]=bestFitnessValue;
	updateDistTable(indice);

}


//prend en parametre la position de l'individu qui a ete modifie pour mettre a jour la table des proximite pour cet individu
void updateDistTable(int indPos){
	// met a jour la proximite
	for (int i=0; i<populationSize; i++) {
		int res=getDist(tPopulationColor[indPos], tPopulationColor[i]);
		tDistSolSol[indPos][i]=res;
		tDistSolSol[i][indPos]=res;
	}
}


// determine la proximité de 2 individus (on identifier les meilleurs associations de couleur entre les 2 individus). 
int getDist(int* ind1, int* ind2){
	int proxi=0;
	int **ttNbSameColor; // pour identifier les meilleurs correspondance de couleurs
	ttNbSameColor=malloc(sizeof(int*) * nbColor);
	for (int i=0; i<nbColor; i++) {
		ttNbSameColor[i]=malloc(sizeof(int) * nbColor);
		for (int j=0; j<nbColor; j++) {
			ttNbSameColor[i][j]=0;
		}
	}
	
	for (int i=0; i<nbSommets; i++) {
		ttNbSameColor[ind1[i]][ind2[i]]++;
	}
	
	
	for (int c=0; c<nbColor; c++) {
		int maxVal=-1,maxI,maxJ; 
		for (int i=0; i<nbColor; i++) {
			for (int j=0; j<nbColor; j++) {
				if( ttNbSameColor[i][j]>maxVal ){
					maxVal=ttNbSameColor[i][j];
					maxI=i;
					maxJ=j;
				}
			}
		}
		
		proxi+=maxVal;
		
		for (int i=0; i<nbColor; i++) {
			ttNbSameColor[maxI][i]=-1;
			ttNbSameColor[i][maxJ]=-1;
		}
	}
	
	
	for (int i=0; i<nbColor; i++)
		free(ttNbSameColor[i]);
	free(ttNbSameColor);

	return proxi;
}


int getMaxProxi(int indiv){
	int max=0;
	for (int i=0; i<populationSize; i++) {
		if ( i!=indiv && tDistSolSol[indiv][i] > max) {
			max=tDistSolSol[indiv][i];
		}
	}
	return max;
}


int getMinProxi(int indiv){
	int min=999999;
	for (int i=0; i<populationSize; i++) {
		if (tDistSolSol[indiv][i] < min) {
			min=tDistSolSol[indiv][i];
		}
	}
	return min;
}

int getMaxProxiOfAll(){
	int max=0;
	int tmp;
	for (int i=0; i<populationSize; i++) {
		if ( (tmp=getMaxProxi(i)) > max) {
			max=tmp;
		}
	}
	return max;
}

int getMinProxiOfAll(){
	int min=999999;
	int tmp;
	for (int i=0; i<populationSize; i++) {
		if ( (tmp=getMinProxi(i)) < min) {
			min=tmp;
		}
	}
	return min;
}



//////////////////////////////
/////////////  SAVE  //////////
////////////////////////////////
void printNewChild(int nbParents){
	printf("\nIteration %4d : cross(", nbIterationsCross);
	for (int i=0; i<nbParents; i++) {
		printf("%2d;",tParentsIndices[i]);
	}
	
	printf(") => \tproxMin=%d \tproxMax=%d \tf=(%3d , %4d, %4d)",getMinProxiOfAll(), getMaxProxiOfAll(), bestFitnessValue, bestIter, bestIterMean);
	printf("\t tFitness =");
	for (int i=0; i<populationSize; i++) {
		printf(" %2d",tFitness[i]);
	}
	printf("  $ %d", nbNodeWithConflict);
}


void printDist(){
	for (int i=0; i<populationSize; i++) {
		for (int j=0; j<populationSize; j++) {
			printf("%5d \t", tDistSolSol[i][j]);
		}
		printf("\n");
	}
}


void save(){
	FILE *f;
	f = fopen(solutionFileName, "a");
	
	for (int i=0; i<nbSommets; i++) {
		fprintf(f, "%d\t", tColor[i]);
	}
	fprintf(f, "\n\n\n");
	
	fclose(f);
}


void saveFitnessValue(){
	FILE *f;
	f = fopen(fitnessFileName, "a");
	
	
	if (nbIterationsCross==1) { /// on a commence une nouvelle recherche (construction du premier enfant)
		fprintf(f, "\n");
	}
	
	
	fprintf(f, "%d;", nbEdgesConflict);
	
	fclose(f);
}



