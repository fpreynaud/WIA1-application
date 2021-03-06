#include "hashtable.h"
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Hashage par la méthode de la multiplication
 * @param cle : la clé à hasher
 * @return l'indice hashé 
 */
int hash(const char* cle)
{
	int i = 0  ;
	const double A = (sqrt(5) - 1) / 2 ;
	double hashedValue = 0 ;

	if(cle == NULL)
		return -1 ;

	for (i = 0 ; cle[i] != '\0' ; i++)
	{
		hashedValue += cle[i] * (5 - (i < 5 ? i : 4)) ;
	}

	return floor(TAILLE * (hashedValue*A - floor(hashedValue*A) ) ) ;
}

/*
 * Crée une HashTable vide, d'une taille donnée
 * @param tll : taille de la HashTable
 * @return une hash table vide de taille tll
 */
HashTable* creer_dico(int tll)
{
	HashTable* dico ;
	int i ;

	/* Initialisations préliminaires */
	dico = malloc(sizeof(HashTable)) ;

	if(dico == NULL)
	{
		perror("creer_dico : malloc de dico ") ;
		exit(errno) ;
	}
	
	dico->taille = tll ;
	dico->contenu = malloc(tll * sizeof(ListeMots*)) ;

	if(dico->contenu == NULL)
	{
		perror("creer_dico : malloc de dico->contenu ") ;
		exit(errno) ;
	}

	/* Affecter une liste de mots vide à chaque entrée */
	for(i = 0 ; i < tll ; i++)
	{
		dico->contenu[i] = init_listemots() ;
	}

	return dico ;

}

/*
 * Détermine si un mot se trouve dans la hash table
 * @param dico : la hash table dans laquelle chercher
 * @param mot : le mot à rechercher
 * @return 1 si mot se trouve dans dico, 0 sinon
 */
int contient(HashTable* dico, char* mot)
{
	int index = 0 ;

	if(mot == NULL || dico == NULL)
		return 0 ;

	if((index = hash(mot)) == -1)
		return 0 ;

	return in_list(mot, dico->contenu[index]) ;
}

/*
 * Insère un nouveau mot dans la hash table, si celui-ci ne s'y trouve pas encore
 * @param mot : le mot à insérer
 * @param dico : la hash table dans laquelle ajouter mot
 * @return : la hash table mise à jour
 */
HashTable* insere(Mot mot, HashTable* dico)
{
	int index, contains ;

	contains = contient(dico, mot.mot) ;

	/* Retourner la table inchangée si le mot s'y trouve déjà */
	if(contains)
		return dico ;

	index = hash(mot.mot) ;
	dico->contenu[index] = add(mot, dico->contenu[index]) ;

	return dico ;
}

/*
 * Affiche le contenu d'une hash table
 * @param dico : la hash table à afficher
 */
void print_dico(HashTable* dico)
{
	int i ;

	if(dico == NULL)
	{
		fprintf(stderr, "dictionnaire inexistant\n") ;
		exit(-1) ;
	}

	for(i = 0 ; i < dico->taille ; i++)
	{
		if(dico->contenu[i]->element.mot[0] != '\0')
		{
			printf("entrée %d : ", i) ; print_listemot(dico->contenu[i]) ; printf("\n") ;
		}
	}
}

/*
 * Récupère un mot de la hash table
 * @param mot : le mot qu'on veut récupérer
 * @param dico : la hash table dont on veut extraire le mot
 * @return le mot voulu s'il se trouve dans la table, le mot vide sinon
 */
Mot get(char* mot, HashTable* dico)
{
	int index = hash(mot) ;

	return list_get(mot, dico->contenu[index]) ;
}

/*
 * Met à jour un mot du dictionnaire
 * @param mot : le mot à jour
 * @param dico : le dictionnaire à mettre à jour
 */
void set(Mot mot, HashTable* dico)
{
	int index = hash(mot.mot) ;
	ListeMots* liste = dico->contenu[index] ;

	while(liste->suivant != NULL)
	{
		/* Mettre à jour le mot dans le dictionnaire une fois qu'on l'y a trouvé
		 */
		if(strcmp(liste->element.mot, mot.mot) == 0)
		{
			liste->element = copie_mot(&(liste->element), &mot) ; 
			break ;
		}
		liste = liste->suivant ;
	}
}
