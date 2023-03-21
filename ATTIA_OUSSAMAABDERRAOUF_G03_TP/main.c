#include <stdio.h>
#include <stdlib.h>
#include "bib.h"
#include <string.h>
#include <time.h>


int main() {
    //printf("%s\n", " ______  _____  __\n|  ____|/ ____||__|  Ecole nationale\n| |__  | (___   __\n|  __|  \\___ \\ |  |  Superieure\n| |____ ____) ||  |\n|______|_____/ |__|  d'Informatique");
    Fichier_TOVC F;
    Fichier_TOF archive;
    bool trouv=0; int i=1; int j=0;
    Buffer_TOVC Buf; char chaine[255];
    int cpt_EcrireDire_TOF=0;
    int cpt_LireDire_TOF=0;
    char nom[255];
    Tenreg_INDEX_dense *Indexdense= malloc(MAX_INDEX_DENSE*sizeof(Tenreg_INDEX_dense));
    for(int ind=0; ind<MAX_INDEX_DENSE; ind++){
        Indexdense[ind].moyenne=0;
        Indexdense[ind].Deplacement=0;
        Indexdense[ind].NumBloc=0;
    }
    Tenreg_INDEX_nondense  *Indexnondense= malloc(MAX_INDEX_NONDENSE*sizeof(Tenreg_INDEX_nondense));
    for(int ind=0; ind<MAX_INDEX_NONDENSE; ind++){
        Indexnondense[ind].moyenneMin=0;
        Indexnondense[ind].moyenneMax=0;
        Indexnondense[ind].NumBloc=0;
    }
    Ouvrir_TOVC(&F,"Fichier_Courrant", 'n');
    Ouvrir_TOF(&archive,"archive", 'n');
    int id;
    int classe;
    memset(Buf.Tab , ' ', 1024);

    int NbE=0;
    int n;




      printf("%s\n", " ______  _____  __");
  printf("%s\n", "|  ____|/ ____||__|  Ecole nationale");
  printf("%s\n", "| |__  | (___   __");
  printf("%s\n", "|  __|  \\___ \\ |  |  Superieure");
  printf("%s\n", "| |____ ____) ||  |");
  printf("%s\n\n\n", "|______|_____/ |__|  d'Informatique");
  printf("\t\t\t\t%s\n", "+------------------------------------------------------+");
  printf("\t\t\t\t%s\n", "| TP de Structure de Fichiers et Structures de Donnees |");
  printf("\t\t\t\t%s\n", "+------------------------------------------------------+");
  printf("%s\n", "________________________________________________________________________________________________________________________");
  //printf("\t\t\t\t\t\t%s\n", "Gestion de la scolarite");
  printf("\t\t%s\n", "    ___ _____ ___ ______ ______   __               __   ____  ");
  printf("\t\t%s\n", "   /    |    /       |      |    /   \\  |\\   |    |  \\  |        |        /\\ ");
  printf("\t\t%s\n", "  / ___ |____\\___    |      |   /     \\ | \\  |    |   | |____    |       /__\\");
  printf("\t\t%s\n", "  \\    ||        \\   |      |   \\     / |  \\ |    |   | |        |      /    \\");
  printf("\t\t%s\n", "   \\___||____ ___/   |    __|___ \\___/  |   \\|    |__/  |____    |____ /      \\");
  printf("\t\t\t%s\n", "     ___    ___  __                   __   _____ _____ _____ ");
  printf("\t\t\t%s\n", "    /      /    /   \\ |        /\\    |   \\   |     |   |     ");
  printf("\t\t\t%s\n", "    \\___  |    /     \\|       /__\\   |__ /   |     |   |____");
  printf("\t\t\t%s\n", "        \\ |    \\     /|      /    \\  |  \\    |     |   | ");
  printf("\t\t\t%s\n", "     ___/  \\___ \\___/ |____ /      \\ |   \\ __|__   |   |____");
  // printf("%s\n", );
  // printf("%s\n", );

  printf("%s\n", "________________________________________________________________________________________________________________________");
  printf("\t\t\t\t\t\t%s\n", "      REALISE PAR:");
  printf("\n\t\t\t\t\t\t%s\n", "Oussama Abderraouf ATTIA");
  printf("\n\t\t\t\t\t\t%s\n", " SECTION A  |  GROUPE 03");
  printf("\n\t\t\t\t\t%s\n", "     ENCADRE PAR: Mme.ARTABAZ Saliha");
  printf("\n\t\t\t\t\t\t\t\t\t\t\t%s\n\n\n\n", "ANNEE UNIVERSITAIRE : 2022/2023");
  int reponse=1;
    printf("%s\n", "+--------------------------------------------------------------------------+");
    printf("%s\n", "| Bienvenue dans mon TP qui sert a repertorier les informations scolaires  |");
    printf("%s\n", "| des eleves d’une ecole primaire dans un fichier binaire de type TOVC   |");
    printf("%s\n", "| avec acces accelere par index                                            |");
    printf("%s\n\n\n", "+--------------------------------------------------------------------------+");
  sleep(2);
  while(reponse){

    printf("%s\n", "+----------------------------------------------------------------------------------------------------------+");
    printf("%s\n", "|                                                 PARTIE 1                                                 |");
    printf("%s\n", "+----------------------------+------+----------------------------------------------------------------------+");
    printf("%s\n", "|    Le fonctions demandes   |  [1] | Creer le fichier de l'annee courante                                 |");
    printf("%s\n", "|                            +------+----------------------------------------------------------------------+");
    printf("%s\n", "|                            |  [2] | Insertion d'un nouveau eleve                                         |");
    printf("%s\n", "|                            +------+----------------------------------------------------------------------+");
    printf("%s\n", "|                            |  [3] | Mise a jour d'un eleve a partir de son nom de famille                |");
    printf("%s\n", "|                            +------+----------------------------------------------------------------------+");
    printf("%s\n", "|                            |  [4] | Suppresion d'un eleve a partir de son ID (id>=1000)                  |");
    printf("%s\n", "+----------------------------+------+----------------------------------------------------------------------+");
    printf("%s\n", "| Le fonctions de recherche: |  [5] | Recherche d'un eleve a partir de son ID  (id>=1000)                  |");
    printf("%s\n", "|                            +------+----------------------------------------------------------------------+");
    printf("%s\n", "|                            |  [6] | Recherche d'un eleve a partir de son nom de famille                  |");
    printf("%s\n", "+----------------------------+------+----------------------------------------------------------------------+");
    printf("%s\n", "|  Les fonctions d'affichage |  [7] | Affichage de tous les eleves de l'annee courante                     |");
    printf("%s\n", "|                            +------+----------------------------------------------------------------------+");
    printf("%s\n", "|                            |  [8] | Affichage de fichier avec Bloc                                       |");
    printf("%s\n", "|                            +------+----------------------------------------------------------------------+");
    printf("%s\n", "|                            |  [9] | Affichage des eleve de son classe                                    |");
    printf("%s\n", "|                            +------+----------------------------------------------------------------------+");
    printf("%s\n", "|                            |  [10] | Affichage d'eleve a partir de son ID (id>=1000)                      |");
    printf("%s\n", "|                            +------+----------------------------------------------------------------------+");
    printf("%s\n", "|                            | [11] | Affichage d'eleve a partir de son nom de famille                     |");
    printf("%s\n", "+----------------------------+------+----------------------------------------------------------------------+");
    printf("%s\n", "|                                                 PARTIE 2                                                 |");
    printf("%s\n", "+----------------------------+------+----------------------------------------------------------------------+");
    printf("%s\n", "|    Le fonctions demandes   | [12] | Creer fichier des dossiers scolaires (Creer le fichier archive)      |");
    printf("%s\n", "|                            +------+----------------------------------------------------------------------+");
    printf("%s\n", "|                            | [13] | Simulation des 5 annees (Creer le fichier de simulation)             |");
    printf("%s\n", "|                            +------+----------------------------------------------------------------------+");
    printf("%s\n", "|                            | [14] | Transfert d'un eleve                                                 |");
    printf("%s\n", "+----------------------------+------+----------------------------------------------------------------------+");
    printf("%s\n", "|  Les fonctions d'affichage | [15] | Affichage de tous les eleves de fichier d'archive                    |");
    printf("%s\n", "|                            +------+----------------------------------------------------------------------+");
    printf("%s\n", "|                            | [16] | Affichage de tous les eleves qui ont plus de 12 dans tous les annees |");
    printf("%s\n", "+----------------------------+------+----------------------------------------------------------------------+");


    printf("\n\n\t\t%s","|Reponse|: " );
    scanf("%d", &reponse);
    switch (reponse) {
       case 1:
              printf("%s\n", "+-----------------------------------------------------------------+");
              printf("%s\n", "| Creer le fichier de l'annee courante                            |");
              printf("%s\n", "+-----------------------------------------------------------------+");
              creer_fichier_etudiants(&F);
              printf("%s\n", "+-----------------------------------+");
              printf("%s\n", "| Creeation de fichier avec succes! |");
              printf("%s\n", "+-----------------------------------+");
              break;
      case 2: printf("%s\n", "+------------------------------+");
            printf("%s\n", "| Insertion d'un nouveau eleve |");
            printf("%s\n", "+------------------------------+");
            insertion_eleve(&F);

            break;
      case 3: printf("%s\n", "+--------------------------------------------------------+");
          printf("%s\n", "| Mise a jour d'une eleve a partir de son nom de famille |");
          printf("%s\n", "+--------------------------------------------------------+");
          mise_a_jour_eleve(&F);
          break;
      case 4:
              printf("%s\n", "+------------------------------------------------------+");
              printf("%s\n", "| Suppression d'un eleve a partir de son ID (ID>=1000) |");
              printf("%s\n", "+------------------------------------------------------+");
              Supprimer_eleve_logiquement(&F);
              break;
      case 5:
              printf("%s\n", "+----------------------------------------------------+");
              printf("%s\n", "| Recherche d'un eleve a partir de son ID (ID>=1000) |");
              printf("%s\n", "+----------------------------------------------------+");
              printf("%S\n",">>Donnez le ID a trouve: ");
              scanf("%d", &id);
              Recherche_eleve_id(&F, id, &i,&j,&trouv, chaine);
              if (trouv==1){
                printf("%s  %s\n", ">>Voila la chaine de cette eleve dans le fichier:", chaine);
              }else{
              printf("%s\n", ">>L'eleve est non trouve..");
              }
              break;
      case 6:
            printf("%s\n", "+-----------------------------------------------------+");
            printf("%s\n", "| Recherche d'un eleve a partir de son nom de famille |");
            printf("%s\n", "+-----------------------------------------------------+");
            Recherche_eleve_nom(&F,&i,&j,&trouv, nom, chaine, &Buf);
            if (trouv==1){
              printf("%s  %s\n", ">>Voila la chaine de cette eleve dans le fichier:", chaine);
            }else{
              printf("%s\n", ">>L'eleve est non trouve..");
            }
            break;
      case 7:
            printf("%s\n", "+--------------------------------------------------+");
            printf("%s\n", "| Affichage de tous les eleves de l'annee courante |");
            printf("%s\n", "+--------------------------------------------------+");
            affichage_eleves(F);
            break;
      case 8:
            printf("%s\n", "+--------------------------------------------------+");
            printf("%s\n", "| Affichage de fichier en Bloc                     |");
            printf("%s\n", "+--------------------------------------------------+");
            affichage_TOVC(&F);
            break;
      case 9:
            printf("%s\n", "+--------------------------------------------------+");
            printf("%s\n", "| Affichage des eleves d'une classe                |");
            printf("%s\n", "+--------------------------------------------------+");
            printf("%s\n", ">>Donnez la classe que vous voulez afficher: ");
            scanf("%s", &classe);
            affichage_classe(F, classe);
            break;
      case 10:
            printf("%s\n", "+--------------------------------------------------------------+");
            printf("%s\n", "| Affichage d'eleve a partir de son ID (id>=1000)              |");
            printf("%s\n", "+--------------------------------------------------------------+");
            affichage_eleve_id(F);
            break;
      case 11:
            printf("%s\n", "+--------------------------------------------------------------+");
            printf("%s\n", "| Affichage d'eleve a partir de son nom de fanille             |");
            printf("%s\n", "+--------------------------------------------------------------+");
            affichage_eleve_nom(F);
            break;
      case 12:
            printf("%s\n", "+-----------------------------------------------------------------+");
            printf("%s\n", "| Creer fichier des dossiers scolaires (Creer le fichier archive) |");
            printf("%s\n", "+-----------------------------------------------------------------+");
            creer_fichier_des_dossiers_scolaires(&archive, &F, Indexdense, &NbE, &cpt_EcrireDire_TOF);
            printf("%s\n", "Affichage de la table d'index:");
            creer_Index_nondense(archive, Indexnondense, &n); // Creation de l'Index
            printf("%s\n", "+-----------------------------------+");
            printf("%s\n", "| Creeation de fichier avec succes! |");
            printf("%s\n", "+-----------------------------------+");
            break;
      case 13:
            printf("%s\n", "+-----------------------------------------------------------------+");
            printf("%s\n", "| Simulation des 5 annees (Creer le fichier de simulation)        |");
            printf("%s\n", "+-----------------------------------------------------------------+");
            creer_fichier_5annee( &archive, Indexdense, &NbE, &cpt_EcrireDire_TOF, &cpt_LireDire_TOF);
            printf("%s\n", "Affichage de la table d'index:");
            creer_Index_nondense(archive, Indexnondense, &n); // Creation de l'Index
            printf("%s\n", "+-----------------------------------+");
            printf("%s\n", "| Creeation de fichier avec succes! |");
            printf("%s\n", "+-----------------------------------+");
            break;
      case 14:
            printf("%s\n", "+-----------------------------------+");
            printf("%s\n", "| Transfert d'une elve              |");
            printf("%s\n", "+-----------------------------------+");
            Transfert(&archive, &F, Indexdense,&NbE,&cpt_EcrireDire_TOF);
            break;
      case 15:
      printf("%s\n", "+-----------------------------------------------------------+");
      printf("%s\n", "| Affichage de tous les eleves de fichier d'archive         |");
      printf("%s\n", "+-----------------------------------------------------------+");
      affichage_elevesTof(archive);
      break;
       case 16:
       printf("%s\n", "+------------------------------------------------------------------------------+");
       printf("%s\n", "|Affichage de tous les eleves qui ont plus de 12 dans tous les annees          |");
       printf("%s\n", "+------------------------------------------------------------------------------+");
       affichage_eleve_moy12(archive);
      default:
      printf("%s\n", "+------------------------------------------------------------------------------+");
      printf("%s\n", "|Votre reponse est fausse veuillez essayes de nouveau                          |");
      printf("%s\n", "+------------------------------------------------------------------------------+");
      break;
     }
    sleep(1);

  }





    Fermer_TOVC(&F);
    Fermer_TOF(&archive);
}
