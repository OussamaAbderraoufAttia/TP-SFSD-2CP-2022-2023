#ifndef BIB_H_INCLUDED
#define BIB_H_INCLUDED
#define Max 1024  /// le nombre max de caractère que peut contenir le bloc
#define MAX_TOF 128
#define MAX_INDEX_DENSE 3000
#define MAX_INDEX_NONDENSE 100
typedef int bool;



/*
+----------------------------+-------------+---------------------------------------------------------+---------------+---------------+--------+----------------------------+
| Taille de l'enregistrement | Identifiant | ID_Classe                                               | Nom           | Prénom        | Genre  | Tableau de notes           |
+----------------------------+-------------+---------------------------------------------------------+---------------+---------------+--------+----------------------------+
| 2 Bytes                    | 4 Bytes     | 1 Byte pour l'anée et 2 Bytes pour le numero de la sale | Max 255 Bytes | Max 255 Bytes | 1 Byte | If (annee!=0){4*16 Bytes}  |
+----------------------------+-------------+---------------------------------------------------------+---------------+---------------+--------+----------------------------+
*/

/*
+----------------------------------------+
| STRUCTURES ET VARIABLES GLOBALES TOVnC |
+----------------------------------------+
*/

/*
╔═══════════════════════════╗
║ Structure d'entete TOVC   ║
╚═══════════════════════════╝
*/
typedef struct Entete_TOVC
{
    int nbBloc;      /// Numéro de Blocs exsistants dans le fichier <=> Numeéo du dèrnier Bloc.
    int id;          ///Numéro de maximum id dans le fichier
}   Entete_TOVC ;


/*
╔═══════════════════════════╗
║ Structure d'un Bloc TOVC  ║
╚═══════════════════════════╝
*/
typedef struct TBloc_TOVC
{
    char Tab[Max];  /// Comme la structure est variable donc le bloc est vu comme un ensemble de caracteèes.
    int nb;  ///Nombre de caracteres inseres dans le Bloc.
}   TBloc_TOVC;

typedef struct TBloc_TOVC Buffer_TOVC;// definition du type Buffer TOVC(un alias du type TBloc)



/*
╔═════════════════════════════╗
║ Structure d'un Fcihier TOVC ║
╚═════════════════════════════╝
*/
typedef struct Fichier_TOVC
{
    FILE * fichier ;
    Entete_TOVC entete ;
}   Fichier_TOVC ;


/*
╔═════════════════════════════╗
║ Structure d'un eleve        ║
╚═════════════════════════════╝
*/
typedef struct eleve
{
    int taille;
    int eff;
    int id;
    int annee;
    int classe;
    char *nom;
    char *prenom;
    char genre;
    int notes[8];
}eleve;


/*
+------------------------------------+
| Définition de la machine abstraite |
+------------------------------------+
*/
///
void Ouvrir_TOVC(Fichier_TOVC *F, char *nomFichier, char mode);
void Fermer_TOVC(Fichier_TOVC *fich);
int entete_TOVC(Fichier_TOVC * F, int i);
void Aff_Entete_TOVC(Fichier_TOVC * F, int i, int val);
void LireDir_TOVC(Fichier_TOVC *fich, int i, Buffer_TOVC *buf);
void EcrireDir_TOVC(Fichier_TOVC *fich, int i, Buffer_TOVC buf);
void affichage_entete_TOVnC(Fichier_TOVC *fich);
void affichage_TOVC(Fichier_TOVC *F);
void recuperer_chaine(Fichier_TOVC *fichier,int n , int *i, int *j, char chaine[],Buffer_TOVC *buf);
void ecrire_chaine(Fichier_TOVC *fichier,unsigned  int n , int *i, int *j, char chaine[],Buffer_TOVC *buf);

///Des fonctions supplementaires pour mieux manipuler un fichier TOVC: (Les fonctions outils)
void NumtoS(unsigned int Num,int max,char* S);
void Decalage(Fichier_TOVC *fichier,char chaine[],int i, int j);









/*
+----------------------------------------+
| STRUCTURES ET VARIABLES GLOBALES TOF   |
+----------------------------------------+
*/


/*
╔═══════════════════════════╗
║ Structure d'entete TOF    ║
╚═══════════════════════════╝
*/
typedef struct Entete_TOF
{
    int nbBloc;      /// Numéro de Blocs exsistants dans le fichier <=> Numeéo du dèrnier Bloc.
    int nbEnreg;    ///Numero d'enregistrement existants dans le fichier

}   Entete_TOF ;


/*
╔═══════════════════════════════════╗
║ structure de l'enregistrement TOF ║
╚═══════════════════════════════════╝
*/
typedef struct eleve_TOF {
    int id;
    int annee;
    char nom[255];
    char prenom[255];
    char genre;
    float moyenne;

}eleve_TOF;


/*
╔═══════════════════════════╗
║ Structure d'un Bloc TOF   ║
╚═══════════════════════════╝
*/
typedef struct TBloc_TOF
{
    eleve_TOF Tab[MAX_TOF];
    int nb;

}TBloc_TOF;
typedef TBloc_TOF Buffer_TOF; // definition du type Buffer TOF(un alias du type TBloc)

/*
╔═════════════════════════════╗
║ Structure d'un Fcihier TOF  ║
╚═════════════════════════════╝
*/
typedef struct Fichier_TOF
{
    FILE * fichier ;
    Entete_TOF entete ;

}   Fichier_TOF ;


/*
+------------------------------------+
| Définition de la machine abstraite |
+------------------------------------+
*/
void Ouvrir_TOF(Fichier_TOF *F, char *nomFichier, char mode);
void Fermer_TOF(Fichier_TOF *fich);
int entete_TOF(Fichier_TOF * F, int i);
void Aff_Entete_TOF(Fichier_TOF * F, int i, int val);
void LireDir_TOF(Fichier_TOF *fich, int i, Buffer_TOF *buf);
void EcrireDir_TOF(Fichier_TOF *fich, int i, Buffer_TOF buf);
















/*
+-----------------------------------------------------+
| STRUCTURES ET VARIABLES GLOBALES des tables d'index |
+-----------------------------------------------------+
*/

/*
╔═════════════════════════════╗
║ Structure d'un Index dense  ║
╚═════════════════════════════╝
*/
typedef struct Tenreg_INDEX_dense
{
    float moyenne;
    int NumBloc;                              // 1ere coordonnee (numero du bloc) de l'adresse de l'identifiant dans le fichier TOF
    int Deplacement;                          // 2eme coordonnee(deplacement dans loe bloc) de l'adresse de l'identifiant dans TOF

} Tenreg_INDEX_dense;


/*
╔══════════════════════════════════╗
║ Structure d'un Index non dense   ║
╚══════════════════════════════════╝
*/
typedef struct Tenreg_INDEX_nondense
{
    float moyenneMax;
    float moyenneMin;
    int NumBloc;                              // 1ere coordonnee (numero du bloc) de l'adresse de l'identifiant dans le fichier TOVnC

} Tenreg_INDEX_nondense;
















/***********************************************|
|                                               |
|     FONCTIONS IMPLEMENTES POUR PARTIE 01      |
|                                               |
|***********************************************/
///1- Les fonction demandees:
void creer_fichier_etudiants(Fichier_TOVC *F);
void insertion_eleve(Fichier_TOVC *F);
void mise_a_jour_eleve(Fichier_TOVC *F);
void Supprimer_eleve_logiquement(Fichier_TOVC *F);

///2- Les fonctions de recherches:
void Recherche_eleve_id(Fichier_TOVC *F,int id, int *i, int *j,bool *trouv, char chaine[]);
void Recherche_classe_fichier(Fichier_TOVC *F, int *imin, int *jmin, int *imax, int *jmax, bool *trouv, int classe);
void Recherche_eleve_nom(Fichier_TOVC *F, int *i, int *j,bool *trouv, char nom[], char chaine[], Buffer_TOVC *Buf);

///3- Les fonctions d'affichages:
void affichage_eleves(Fichier_TOVC F);
void affichage_classe(Fichier_TOVC F, int classe);
void affichage_eleve_id(Fichier_TOVC F);
void affichage_eleve_nom(Fichier_TOVC F);

///4- Les fonctions outils:
void enreg_vers_chaine(eleve eleve, char chaine[]);
void chaine_vers_enreg(eleve *eleve, char *chaine);


















/***********************************************|
|                                               |
|     FONCTIONS IMPLEMENTES POUR PARTIE 02      |
|                                               |
|***********************************************/
///1- Les fonction demandees:
void creer_fichier_des_dossiers_scolaires(Fichier_TOF *F, Fichier_TOVC *Fcour, Tenreg_INDEX_dense Index[], int *nbInd, int *cpt_EcrireDire_TOF);
void creer_Index_nondense(Fichier_TOF archive, Tenreg_INDEX_nondense  Indexnondense[], int *NbInd);
void creer_fichier_5annee(Fichier_TOF *F, Tenreg_INDEX_dense Index[], int *nbInd, int *cpt_EcrireDire_TOF, int *cpt_LireDire_TOF);
void Transfert(Fichier_TOF *archive, Fichier_TOVC *F, Tenreg_INDEX_dense Index[], int *nbInd, int *cpt_EcrireDire_TOF);

///2-Les fonctions d'affichage:
void affichage_elevesTof(Fichier_TOF F);
void affichage_eleve_moy12(Fichier_TOF F);




#endif // BIB_H_INCLUDED
