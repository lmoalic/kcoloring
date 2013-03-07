#ifndef MY_GRAPHE_H
#define MY_GRAPHE_H
/*
 *  graphe.h
 *  tabucol
 *
 *  Created by Laurent Moalic on 14/02/11.
 *  Copyright 2011 UTBM. All rights reserved.
 *
 */

//#include "node.h"

#include<time.h>


#define MAX_NB_PARENTS 10



/////////////////////////////////
int nbColor;
int populationSize;
int nbLocalSearch; // nb d'étapes tabucol sans amélioration
double alpha;
/////////////////////////////////


int bestIterMean;
int nbIterations;
int nbIterationsCross;
int nbIterationsSinceRestart;
int nbEdgesConflict;
int nbNodesConflict;
int nbBestEdgesConflict; // pour le critere d'aspiration

/// caraterisation du graphe
char filename[255];
int nbSommets;
int nbArretes;
char** tConnect; // tableau carre des connections noeud à noeud
int** tVoisins; // tableau qui pour chaque noeud contient la liste des voisins
int* tNbVoisins; // tableau qui pour chaque noeud contient le nombre de voisins

/// caracterisation d'une instance de solution
int* tColor; // tableau contenant pour chaque noeud sa couleur. tColor pointe sur l'instance a intensifier (pas de malloc !)
int* tBestSol; // contient pour une intensification la meilleure solution
int bestFitnessValue; // contient la meilleure valeure
int bestIter;
int* tConflicts; // tableau contenant le nombre de conflits de chaque noeud
int** tNewConflitsWithColor; // contient pour chaque noeud ne nb de conflis en plus ou moins si changement vers chaque couleur, suivi de la meilleure couleur et meilleur gain associé 
int** tTabou; // contient pour chaque noeud et chaque couleur la fin de la période taboue

// optimisation tabucol
int* tBestImprove; // contient pour chaque sommet la meilleure valeur de transition
int** ttBestImproveColor; // contient pour chaque sommet la meilleure transition de couleur (-1 si plusieurs equivalentes => stratégie classique)
int* tNbBestImprove; // contient le nombre pour chaque sommet de couleurs fournissant la meilleure amélioration
int* tNodeWithConflict; // contient les noeuds avec conflit pour ne pas tout parcourir
int nbNodeWithConflict;
int* tNodeAdded; // permet de ne pas ajouter 2 fois le meme noeud

void loadGraphe();
void cleanMatrix();
void buildVoisins();
void initRandomColor();
void improvInitPop();
void initConflict();
void determineBestImprove();
void updateTables(int node, int color);
void resetNodeWithConflict();

///////////////////////////////
/////////// EA ////////////////
///////////////////////////////
int** tPopulationColor; // tableau d'individus, chaque individu etant un tColor
int* tParentsIndices; /// vecteur pour operation de croisement
int* tChild; /// contient le resultat du croisement de 2 parents
int** tDistSolSol; /// contient pour chaque couple d'individus le niveau de proximite
int* tFitness; // valeur de la fitness pour chaque individu de la polulation

void runIteration();
void selectParents(int nbParents);
void buildChildN(int n); /// permet de croiser n parents
void updatePopulation();
void updateDistTable(int indPos);
int getDist(int* ind1, int* ind2);
int getMaxProxi(int indiv);
int getMinProxi(int indiv);
int getMaxProxiOfAll();
int getMinProxiOfAll();


///////////////////////////////
/////////// FILE ////////////////
///////////////////////////////
void printNewChild(int nbParents);
void printDist();
void save();
void saveFitnessValue();

char* solutionFileName;
char* fitnessFileName;



clock_t  time1;
clock_t  time2;
clock_t  time3;
int nbMinutes1;
int nbMinutes2;
int nbMinutes3;

#endif
