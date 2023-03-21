#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bib.h"









/*
+----------------------------------------+
|           FICHIER TYPE TOVC            |
+----------------------------------------+
*/
/*************************************************************|
|                                                             |
| Ouvrir fichier nom_fichier avec le mode correspondant TOVC  |
|                                                             |
|*************************************************************/
void Ouvrir_TOVC(Fichier_TOVC *F, char *nomFichier, char mode) {
    char s[4];
    if ((mode == 'a') || (mode == 'A'))
        sprintf(s, "rb+");
    else
        sprintf(s, "wb+");
    if ((mode == 'a') || (mode == 'A')) {
        F->fichier = fopen(nomFichier, s);
        rewind(F->fichier);
        fread(&(F->entete), sizeof(Entete_TOVC), 1, F->fichier);
    } else if ((mode == 'n') || (mode == 'N')) {
        F->fichier = fopen(nomFichier, s);
        F->entete.nbBloc = 0;
        F->entete.id = 0;
        fwrite(&(F->entete), sizeof(Entete_TOVC), 1, F->fichier);
    } else {
        printf(">>Le fichier n'existe pas");
    }
}





/*************************************************************|
|                                                             |
| Fermer fichier nom_fichier avec le mode correspondant TOVC  |
|                                                             |
|*************************************************************/
void Fermer_TOVC(Fichier_TOVC *fich)
{
    rewind(fich->fichier);                                          // positionnement au debut de fichier.
    fwrite(&(fich->entete), sizeof(Entete_TOVC), 1, fich->fichier); // mise a jour de l'entete.
    fclose(fich->fichier); // fermer le fichier
}





/**********************************************|
|                                              |
|  Retoure la i ème valeur del'entete  TOVC    |
|                                              |
|**********************************************/
int entete_TOVC(Fichier_TOVC * F, int i)
{
    switch (i) {
        case 1:
            return F->entete.nbBloc;
            break;
        case 2:
            return F->entete.id;
            break;
        default:
            printf("%s", ">>Indice entrant a entete_TOVC est faut..");
            return -1;
            break;
    }
}




/***********************************************|
|                                               |
|  modifie la i ème valeur de l'entete  TOVC    |
|                                               |
|***********************************************/
void Aff_Entete_TOVC(Fichier_TOVC * F, int i, int val)
{
    switch (i) {
        case 1:
            F->entete.nbBloc=val;
            break;
        case 2:
            F->entete.id= val;
            break;
        default:
            printf("%s", ">>Indice entrant a Aff_entete_TOVC est faut..");
            break;
    }
}





/********************************************|
|                                            |
|    Lire le i eme bloc dans buf TOVC        |
|                                            |
|********************************************/
void LireDir_TOVC(Fichier_TOVC *fich, int i, Buffer_TOVC *buf)
{
    rewind(fich->fichier);
    fseek(fich->fichier, sizeof(Entete_TOVC) + (i - 1) * sizeof(Buffer_TOVC), SEEK_SET); // positionnement a la place correspendante
    fread(buf, sizeof(Buffer_TOVC), 1, fich->fichier);                                   // lecture du bloc
};






/*********************************************|
|                                             |
|    Ecrire buf dans le i eme bloc TOVnC      |
|                                             |
|*********************************************/
void EcrireDir_TOVC(Fichier_TOVC *fich, int i, Buffer_TOVC buf)
{
    rewind(fich->fichier);
    fseek(fich->fichier, sizeof(Entete_TOVC) + (i - 1) * sizeof(Buffer_TOVC), SEEK_SET); // positionnement a la place correspendante meme si si c'etait au milieu il y aura lieu un ecrasement.
    fwrite(&buf, sizeof(Buffer_TOVC), 1, fich->fichier);                                 // ecriture en binaire

};





/**************************************************|
|                                                  |
|    afficher les caracteristqiues (entete) d'un   |
|                 fichier TOVC                     |
|                                                  |
|**************************************************/
void affichage_entete_TOVnC(Fichier_TOVC *fich)
{
    printf("\n\n\n*************************************************\n");
    printf("*                                               *\n");
    printf("*       caracteristiques du fichier             *\n");
    printf("*                                               *\n");
    printf("*************************************************\n");
    printf("*   -> Nombre de Blocs : %d\n", entete_TOVC(fich, 1));
    printf("*   -> Nombre de ID courrant : %d\n", entete_TOVC(fich, 2));
    printf("*************************************************\n");
}




/**********************************************|
|                                              |
|       affichier le contenu d'un fichier      |
|                de type TOVC                  |
|                                              |
|**********************************************/
void affichage_TOVC(Fichier_TOVC *F){
    Buffer_TOVC Buf;
    int i=1;

    while (i<= entete_TOVC(F,1)){
        LireDir_TOVC(F, i, &Buf);
        printf("i: %d buf: %d\n",i, Buf.nb);
        for (int k=0; k<Buf.nb; k++){
            printf("%c", Buf.Tab[k]);
        }
        printf("\n");
        i++;
    }
}






/**********************************************|
|                                              |
|       récupèrer un enregistrement du fichier |
|  et de le placer dans la variable chaine     |
|                                              |
|**********************************************/
void recuperer_chaine(Fichier_TOVC *fichier,int n , int *i, int *j, char chaine[],Buffer_TOVC *buf) //
{

    int k=0;
    sprintf(chaine,"%s","");
    for(k=0;k<n;k++)                    // boucle de n itérations correspondant a la longueur de la chaine
    {                                   // indice de parcours de la chaine resultata et j celui du tableau
        if((*j)<=1023)                    // si le kièmem caractère de la chaine correspondant au j ime car du buffer  est toujour dans le bloc i
        {                               // le caractère 1023 correspond a la fin de la chaine dans le tableau
            chaine[k]=buf->Tab[(*j)];   // recuperation du caractère dans la position k de la chaine
            (*j)++;                      // deplacement d'une position dans le buffer
        }
        else                             // si le k ièeme caractère a recuperer sort du buffer
        {
            (*i)= (*i) +1;       //  On passe au Bloc suivant
            LireDir_TOVC(fichier,(*i),buf);   // lecture
            chaine[k]=buf->Tab[0];       // recuperation du premier caractère du tableau de carractère correspondant au ka ième car de la chaine
            (*j)=1;                      //positionnement eu second caractère dans le nouveau buffer
        }
    }
    chaine[k]='\0';                        // fin de la chaine obtenue

}









/**********************************************|
|                                              |
|      Ecrire une chaine de taille n a partir  |
|  de la position j dans le bloc i dans Buf    |
|                                              |
|**********************************************/
void ecrire_chaine(Fichier_TOVC *fichier,unsigned  int n , int *i, int *j, char chaine[],Buffer_TOVC *buf)
{
    int k=0;
    for(k=0;k<n;k++)   // k pourn le deplacement dans la chaine
    {
        if((*j)<=1023)  //si je suis toujours dans le bloc conserné
        {
            buf->Tab[*j]=chaine[k]; // affectation des caractères de la chaine dans le buffer un a un
            (*j)++;                  // deplacement da,s le buffer
        }
        else                          // si la chaine a inserer depasse le buffer
        {
            buf->nb= 1024;
            EcrireDir_TOVC(fichier,*i,*buf);  // ecriture du precedent buffer dans le fichier
            memset(buf->Tab , ' ', 1024);
            buf->Tab[0]=chaine[k];      // ecrtiture du kiem caractère de la chaine dans la position 0
            (*j)=1;                     // passage a la position 1
            Aff_Entete_TOVC(fichier, 1, *i);
            (*i)= (*i)+1;
        }
    }
    buf->Tab[*j]='\0';                  // fin de la chaine
    buf->nb= *j;
}







/**********************************************|
|                                              |
|   transformation d'un entier en une char     |
|  ajout de zéros                              |
|                                              |
|**********************************************/
void NumtoS(unsigned int Num,int max,char* S)
{
    char inter[max];
    sprintf(S,"");
    sprintf(inter,"%d",Num);
    int j = max - strlen(inter);
    while (j>0)
    {
        sprintf(S,"%s0",S);
        j--;
    }
    sprintf(S,"%s%s",S,inter);
}











/**********************************************|
|                                              |
|   Insertion avec decalge de la chaine chaine |
|   a partir de la position j dans le bloc i   |
|                                              |
|**********************************************/
void Decalage(Fichier_TOVC *fichier,char chaine[],int i, int j)  ///decale les char du fichier TOVC de (taille de chaine) char a partir de (i,j)
{
    char temp[strlen(chaine)];
    int i1,k,d;
    Buffer_TOVC buffer,buffer2;
    int taille=strlen(chaine);
    while (i!=entete_TOVC(fichier,1))  //i<Entete2(fichier,1)
    {
        LireDir_TOVC(fichier,i,&buffer);
        if((j+taille)<= Max) ///toute la chaine //pas de c
        {
            for(k=0;k<taille;k++)
            {
                temp[taille-1-k] = buffer.Tab[Max-1-k];
            }
            temp[k]='\0';
            for(k=0;k<(Max-j-taille);k++)
            {       //t-1-taille-k=j     k=t-1-j-taille
                buffer.Tab[Max-1-k] = buffer.Tab[Max-1-taille-k]; //dec interne
            }
            for(k=0;k<taille;k++)
            {
                buffer.Tab[j+k]=chaine[k]; //on place chaine à sa place
            }
            strcpy(chaine,temp);
            EcrireDir_TOVC(fichier,i,buffer);
            i++;
        }
        else // if ((j<t)&&((j+taille)=>t)) CUZ j HA TKUN > 0 AND < t
        {
            for(k=0;k<Max-j;k++)    //k=t+t-j-taille-1
            {
                //t+j+taille
                temp[taille-1-k] = buffer.Tab[Max-1-k]; //dec b1-> b2
            }
            temp[taille]='\0';
            for (k=0;k<taille;k++)
            {
                if ((j+k)<Max)
                {
                    buffer.Tab[j+k]=chaine[k]; //on place chaine à sa place
                }
                else   // j+taille-1-t
                {
                    temp[j+k-Max] = chaine[k];
                }
            }
            strcpy(chaine,temp);
            EcrireDir_TOVC(fichier,i,buffer);
            i++;
        }
        j=0;
    }
    if (i==entete_TOVC(fichier,1))
    {
        d=entete_TOVC(fichier,3);
        LireDir_TOVC(fichier,i,&buffer);
        if(Max-d>=taille)
        {
            for(k=0;k<d-j;k++)
            {            //j=d-1-k     k=d-j-1
                buffer.Tab[d-1+taille-k]=buffer.Tab[d-1-k];
            }
            for(k=0;k<taille;k++)
            {
                buffer.Tab[j+k]=chaine[k];
            }
            EcrireDir_TOVC(fichier,i,buffer);
        }
        else
        {

            for (k=0;k<d-j;k++)
            {
                if(d-1-k+taille>=Max)
                {
                    buffer2.Tab[d-1-k+taille-Max]=buffer.Tab[d-1-k];
                }
                else
                {    //j=d-1-k+taille    k=d-1+taille-j
                    buffer.Tab[d-1-k+taille]=buffer.Tab[d-1-k];
                }
            }
            for (k=0;k<taille;k++)
            {
                if ((j+k)<Max)
                {
                    buffer.Tab[j+k]=chaine[k]; //on place chaine à sa place
                }
                else
                {
                    buffer2.Tab[j+k-Max] = chaine[k];
                }
            }
            EcrireDir_TOVC(fichier,i,buffer);
            EcrireDir_TOVC(fichier,entete_TOVC(fichier,1),buffer2);

        }
    }
}









/*
+----------------------------------------+
|           FICHIER TYPE TOF             |
+----------------------------------------+
*/
/*************************************************************|
|                                                             |
| Ouvrir fichier nom_fichier avec le mode correspondant TOF   |
|                                                             |
|*************************************************************/
void Ouvrir_TOF(Fichier_TOF *F, char *nomFichier, char mode) {
    char s[4];
    if ((mode == 'a') || (mode == 'A'))
        sprintf(s, "rb+");
    else
        sprintf(s, "wb+");
    if ((mode == 'a') || (mode == 'A')) {
        F->fichier = fopen(nomFichier, s);
        rewind(F->fichier);
        fread(&(F->entete), sizeof(Entete_TOF), 1, F->fichier);
    } else if ((mode == 'n') || (mode == 'N')) {
        F->fichier = fopen(nomFichier, s);
        F->entete.nbBloc = 0;
        F->entete.nbEnreg = 0;
        fwrite(&(F->entete), sizeof(Entete_TOF), 1, F->fichier);
    } else {
        printf(">>Le fichier n'existe pas");
    }
}












/*************************************************************|
|                                                             |
| Fermer fichier nom_fichier avec le mode correspondant TOF   |
|                                                             |
|*************************************************************/
void Fermer_TOF(Fichier_TOF *fich)
{
    rewind(fich->fichier);                                          // positionnement au debut de fichier.
    fwrite(&(fich->entete), sizeof(Entete_TOF), 1, fich->fichier); // mise a jour de l'entete.
    fclose(fich->fichier); // fermer le fichier
}








/**********************************************|
|                                              |
|  Retoure la i ème valeur del'entete  TOF     |
|                                              |
|**********************************************/
int entete_TOF(Fichier_TOF * F, int i)
{
    switch (i) {
        case 1:
            return F->entete.nbBloc;
            break;
        case 2:
            return F->entete.nbEnreg;
            break;
        default:
            printf("%s", ">>Indice entrant a entete est faut..");
            break;
    }
}









/***********************************************|
|                                               |
|  modifie la i ème valeur de l'entete  TOF     |
|                                               |
|***********************************************/
void Aff_Entete_TOF(Fichier_TOF * F, int i, int val)
{
    switch (i) {
        case 1:
            F->entete.nbBloc=val;
            break;
        case 2:
            F->entete.nbEnreg= val;
            break;
        default:
            printf("%s", ">>Indice entrant a Aff_entete est faut..");
            break;
    }
}











/********************************************|
|                                            |
|    Lire le i eme bloc dans buf TOF         |
|                                            |
|********************************************/
void LireDir_TOF(Fichier_TOF *fich, int i, Buffer_TOF *buf)
{
    fseek(fich->fichier, sizeof(Entete_TOF) + (i - 1) * sizeof(Buffer_TOF), SEEK_SET); // positionnement a la place correspendante
    fread(buf, sizeof(Buffer_TOF), 1, fich->fichier);                                   // lecture du bloc
};











/*********************************************|
|                                             |
|    Ecrire buf dans le i eme bloc TOF        |
|                                             |
|*********************************************/
void EcrireDir_TOF(Fichier_TOF *fich, int i, Buffer_TOF buf)
{
    fseek(fich->fichier, sizeof(Entete_TOF) + (i - 1) * sizeof(Buffer_TOF), SEEK_SET); // positionnement a la place correspendante meme si si c'etait au milieu il y aura lieu un ecrasement.
    fwrite(&buf, sizeof(Buffer_TOF), 1, fich->fichier);                                 // ecriture en binaire

};









/***********************************************|
|                                               |
|     FONCTIONS IMPLEMENTES POUR PARTIE 01      |
|                                               |
|***********************************************/



/*************************************************************|
|                                                             |
|                 1- Les fonction demandees                   |
|                                                             |
|*************************************************************/



/*
+-------------------------------------------------------+
|           Le chargement initial de fichier            |
+-------------------------------------------------------+
*/
void creer_fichier_etudiants(Fichier_TOVC *F) {
    srand(time(0)); const char  *separators = ","; const char *prepa= "P"; const char eff[2]={'0', '\0'};
    Buffer_TOVC Buf;
    char nom[255]; char prenom_genre[255];
    char *prenom; char *genre;
    unsigned int id=1000; char idCh[5];
    unsigned int classe=1; char classeCh[3];
    unsigned int annee=0; char anneeCh[2];
    char anneeClasseCh[3];
    unsigned int taille; char tailleCh[3];

    int i=1; int j=0; int k=0;
    unsigned int notes; char notesCh[17]; char note[3];
    int cpt=1; int cpt2=1;
    FILE *noms= fopen("noms.csv", "r+");
    FILE *prenoms= fopen("prenoms_genre.csv", "r+");
    while((!feof(noms))|| (!feof(prenoms))){
        fgets(nom, 255, noms); nom[strlen(nom)-1]= '\0';
        fgets(prenom_genre, 255, prenoms);
        prenom = strtok(prenom_genre, separators );
        prenom[strlen(prenom)]= '\0';
        genre =strtok ( NULL, separators );
        genre[strlen(genre)-1]= '\0';

        if (cpt2==4){
            annee++;
            cpt2=1;
        }

        NumtoS(id, 4, idCh);
        if (cpt==24){
            classe++;
            NumtoS(classe,2,classeCh);
            cpt=0;
            if ((classe % 3==1)|| (classe!= 1)){
                cpt2++;
            }
        }
        NumtoS(classe,2,classeCh);
        if (annee==0){
            //Cas de classe preparatoire
            strcpy(anneeClasseCh, prepa);
            strcat(anneeClasseCh, classeCh);

            taille=2+ 4 + 3 +strlen(nom)  + strlen(prenom)  +8+1;
            NumtoS(taille,2, tailleCh);
        }else{
            NumtoS(annee, 1, anneeCh);
            NumtoS(classe,2,classeCh);
            strcpy(anneeClasseCh, anneeCh);
            strcat(anneeClasseCh, classeCh);

            taille= 2+ 4 + 3 +strlen(nom) + strlen(prenom) + 8 +16+2;
            NumtoS(taille,2, tailleCh);
            k=0;
            while(k<8){
                notes= rand() %21;
                NumtoS(notes, 2,note);
                if(k==0){
                    strcpy(notesCh, note);
                }else{
                    strcat(notesCh, note);
                }
                k++;
            }
        }
        char chaine[taille+5];
        strcpy(chaine,tailleCh);
        strcat(chaine,",");
        strcat(chaine,eff);
        strcat(chaine,",");
        strcat(chaine, idCh);
        strcat(chaine,",");
        strcat(chaine,anneeClasseCh);
        strcat(chaine,",");
        strcat(chaine, nom);
        strcat(chaine,",");
        strcat(chaine, prenom);
        strcat(chaine,",");
        strcat(chaine,genre);
        if (annee!=0){
            strcat(chaine,",");
            strcat(chaine, notesCh);
        }
        strcat(chaine,";");
        ecrire_chaine(F, strlen(chaine), &i,&j, chaine, &Buf);
        cpt++; id++;
    }
    Aff_Entete_TOVC(F, 1, i);
    Aff_Entete_TOVC(F, 2, id-1);
    if (j<1023){
        Buf.Tab[j]='\0';
        EcrireDir_TOVC(F,i, Buf);
    }
}

















/*
+----------------------------------------------------------+
|           Inserer un eleve dans un fichier TOVC          |
+----------------------------------------------------------+
*/
//->Remarque: Un eleve dans un fichier TOVC:
/*
+----------------------------+-------------+---------------------------------------------------------+---------------+---------------+--------+----------------------------+
| Taille de l'enregistrement | Identifiant | ID_Classe                                               | Nom           | Prénom        | Genre  | Tableau de notes           |
+----------------------------+-------------+---------------------------------------------------------+---------------+---------------+--------+----------------------------+
| 2 Bytes                    | 4 Bytes     | 1 Byte pour l'anée et 2 Bytes pour le numero de la sale | Max 255 Bytes | Max 255 Bytes | 1 Byte | If (annee!=0){4*16 Bytes}  |
+----------------------------+-------------+---------------------------------------------------------+---------------+---------------+--------+----------------------------+
*/
void insertion_eleve(Fichier_TOVC *F)
{
    Buffer_TOVC Buf; eleve eleve;
    char nom[255];
    char prenom[255]; char genre;
    unsigned int taille;

    Aff_Entete_TOVC(F, 2, entete_TOVC(F,2)+1);
    eleve.id= entete_TOVC(F,2);
    eleve.eff=0;
    printf(">>Entrez le nom: ");
    fgets(nom, 255, stdin);
    nom[strlen(nom)-1]='\0';
    eleve.nom= nom;
    printf("\n>>Entrez le prenom: ");
    fgets(prenom, 255, stdin);
    prenom[strlen(prenom)-1]='\0';
    eleve.prenom= prenom;
    printf("\n>>Entrez le genre de cet eleve: ");
    scanf("%c", &genre);
    eleve.genre= genre;
    printf("\n>>Entrez l'annee'de cet eleve. NB:Pour les classes preparatoires, veuillez entrer 0: ");
    scanf("%d", &eleve.annee);
    printf("\n>>Entrez la classe de cet eleve: ");
    scanf("%d", &eleve.classe);
    if (eleve.annee!=0){
        taille= 2+ 4 + 3 +strlen(nom) + strlen(prenom) + 8 +16+3;
        printf(">>Voulez vous introduite les notes de: %s %s\n", eleve.prenom, eleve.nom);
        printf(">>Repondez par 1 si oui et par = si non!\n");
        printf(">>Reponse: \n");
        int reponse; scanf("%d", &reponse);
        if(reponse==1){
            printf("\n>>Donner la note d'Arabe: ");
            scanf("%d", &eleve.notes[0]);
            printf("\n>>Donner la note de Maths: ");
            scanf("%d", &eleve.notes[1]);
            printf("\n>>Donner la note de Sciences: ");
            scanf("%d", &eleve.notes[2]);
            printf("\n>>Donner la note de Histoire/Geographique: ");
            scanf("%d", &eleve.notes[3]);
            printf("\n>>Donner la note de Sciences Islamiques: ");
            scanf("%d", &eleve.notes[4]);
            printf("\n>>Donner la note de Francais: ");
            scanf("%d", &eleve.notes[5]);
            printf("\n>>Donner la note d'Anglais: ");
            scanf("%d", &eleve.notes[6]);
            printf("\n>>Donner la note de sport: ");
            scanf("%d", &eleve.notes[7]);
        }else{
            for (int note=0; note<=7; note++){
                eleve.notes[note]=0;
            }
        }

    }else{
        taille=2+ 4 + 3 +strlen(nom)  + strlen(prenom)  +8+2;
    }
    eleve.taille= taille;
    char chaine[255];
    enreg_vers_chaine(eleve, chaine);
    LireDir_TOVC(F, entete_TOVC(F,1), &Buf);
    int i, j; bool trouv=0;
    Recherche_eleve_classe(F,  &i,&j,&trouv, eleve.classe, eleve.nom,  chaine);
    if(trouv==1){
        printf("\n>>%s %s existe deja dans le fichier!", eleve.prenom, eleve.nom);
    }else{
        char chaine2[taille];
        enreg_vers_chaine(eleve, chaine2);
        Decalage(F,chaine, i,j);
    }
}











/*
+--------------------------------------------------------------------------+
|           Misse a jour d'un nom a partir de son nom de famille           |
+--------------------------------------------------------------------------+
*/
void mise_a_jour_eleve(Fichier_TOVC *F) {
    int i = 1;
    int j = 0;
    bool trouv;
    char chaine[256];
    Buffer_TOVC Buf;
    char nom[255];
    eleve eleve;
    printf("\n>>Donner le nom de famille de l'eleve a mettre a jour: ");
    fgets(nom, 255, stdin);
    nom[strlen(nom) - 1] = '\0';

    char nomMod[255]; char prenomMod[255];

    int choix;
    Recherche_eleve_nom(F, &i, &j, &trouv, nom, chaine, &Buf);
    if (trouv==1) {
        chaine_vers_enreg(&eleve, chaine);
        printf("\n-------------------------------");
        printf("\n>>Chosir le champ a modifier: ");
        printf("\n    1-Classe\n    2-Nom\n    3-prenom\n    4-Une note");
        printf("\n>>choix: ");
        scanf("%d", &choix);
        printf("\n-------------------------------");
        switch (choix) {
            case 1:
                printf("\n    1>>Donnez la nouvelle classe: ");
                scanf("%d", &eleve.classe);
                enreg_vers_chaine(eleve, chaine);
                Decalage(F, chaine, i,j);
                break;
            case 2:
                printf("%s","\n   2>>Donnez le nom modifie: ");
                scanf("%s", &nomMod);
                strcpy(eleve.nom, nomMod);
                enreg_vers_chaine(eleve, chaine);
                Decalage(F, chaine, i,j);
                break;
            case 3:
                printf("%s", "\n   2>>Donnez le prenom modifie: ");
                scanf("%s", &prenomMod);
                strcpy(eleve.prenom, prenomMod);
                enreg_vers_chaine(eleve, chaine);
                Decalage(F, chaine, i,j);
                break;
            case 4:
                if(eleve.annee==0){
                    printf("%s %s est un eleve en prepa!", eleve.prenom, eleve.nom);
                }else{
                    printf("\n   2>>Donnez la matiere a modifier: ");
                    printf("\n\t1-Arab\n\t2-Maths\n\t3-Sciences\n\t4-Histoire/Geographique\n\t5-Sciences Islamiques\n\t6-Francais\n\t7-Anglais\n\t8-Sport\n");
                    printf("\n\tchoix: ");
                    scanf("%d", &choix);
                    printf("\n\tDonner la nouvelle note (La note c'est un entier sur 20): ");
                    scanf("%d", &eleve.notes[choix-1]);
                    enreg_vers_chaine(eleve, chaine);
                    Decalage(F, chaine, i,j);
                }

                break;
            default:
                printf("\n>>Error");
        }
    } else {
        printf("\n>>Cet eleve avec le nom { %s } n'existe pas!", nom);
    }

}















/*
+---------------------------------------------------------------------+
|           Supprimer un eleve logiquement a partir de son ID         |
+---------------------------------------------------------------------+
*/
void Supprimer_eleve_logiquement(Fichier_TOVC *F)
{
    Buffer_TOVC Buf; int id; int i=1; int j=0; bool trouv;
    char chaine[255];
    printf(">>Entrez le ID de l'eleve que vous voulez supprimer logiquement: ");
    scanf("%d", &id);
    Recherche_eleve_id(F,id, &i, &j, &trouv, chaine);
    if (j+3<=1023){
        LireDir_TOVC(F,i,&Buf);
        Buf.Tab[j+3]= '1';
        EcrireDir_TOVC(F, i, Buf);
    }

}






/*************************************************************|
|                                                             |
|               2- Les fonctions de recherches                |
|                                                             |
|*************************************************************/


/*
+---------------------------------------------------------------------+
|             Recherche d'un eleve a partir de son ID                 |
+---------------------------------------------------------------------+
*/
void Recherche_eleve_id(Fichier_TOVC *F,int id, int *i, int *j,bool *trouv, char chaine[])
{
    Buffer_TOVC Buf;
    *trouv=0;
    char *idCh;
    //int id;
    *i=1; const char  *separators = ",;";
    unsigned int taille; char tailleCh[5];
    int k=1; int l=0;
    *i=1; *j=0; int stop=0;
    LireDir_TOVC(F,k,&Buf);
    while ((*trouv==0) && (stop==0)){
        recuperer_chaine(F,2,&k,&l,tailleCh, &Buf);
        taille= atoi(tailleCh);
        char string[taille];
        char string2[taille];
        recuperer_chaine(F,taille-2,&k,&l,string, &Buf);
        strcpy(string2, string);

        idCh= strtok(string, separators);
        idCh= strtok(NULL, separators);
        if(atoi(idCh)==id){
            *trouv=1;
            strcpy(chaine, tailleCh);
            strcat(chaine, string2);
            *j= l- taille;
            *i=k;
            if (*j<0){
                ///Cas de cehvauchement:
                *i= k-1;
                *j = 1023+*j;
            }
        }else{
            if ((k> entete_TOVC(F,1)) || ( (k==entete_TOVC(F,1)) && (l>Buf.nb))){
                stop=1;

            }
        }
        memset(string,' ' , taille*sizeof(char));
    }

}









/*
+---------------------------------------------------------------------+
|             Recherche d'un eleve a partir de son nom                |
+---------------------------------------------------------------------+
*/
void Recherche_eleve_nom(Fichier_TOVC *F, int *i, int *j,bool *trouv, char nom[], char chaine[], Buffer_TOVC *Buf)
{
    *trouv=0;
    char *nomPrenom;
    *i=1; const char  *separators = ",;";
    unsigned int taille; char tailleCh[5];
    int k=1; int l=0;
    *i=1; *j=0; int stop=0;
    LireDir_TOVC(F,k,Buf);
    while ((*trouv==0) && (stop==0)){
        recuperer_chaine(F,2,&k,&l,tailleCh, Buf);
        taille= atoi(tailleCh);
        char string[taille];
        recuperer_chaine(F,taille-2,&k,&l,string, Buf);
        nomPrenom= strtok(string, separators);
        nomPrenom= strtok(NULL, separators);
        nomPrenom= strtok(NULL, separators);
        nomPrenom= strtok(NULL, separators);
        if(strncmp(nomPrenom, nom, strlen(nom))==0){
            *trouv=1;
        }else{
            if ((k> entete_TOVC(F,1)) || ( (k==entete_TOVC(F,1)) && (l>Buf->nb))) {
                stop=1;
            }
        }
        memset(string,' ' , taille*sizeof(char));
    }
    if(*trouv){
        *j= l- taille;
        *i=k;
        if (*j<0){
            ///Cas de cehvauchement:
            *i= k-1;
            *j = 1023+*j;
        }

        k= *i; l= *j;
        recuperer_chaine(F,taille,&k,&l,chaine, Buf);
    }
}











/*
+-----------------------------------------------------------------------------------------------------------------------+
|             Recherche d'une classe a partir de son numero et retourner son intervalle dans le fichier                 |
+-----------------------------------------------------------------------------------------------------------------------+
*/
void Recherche_classe_fichier(Fichier_TOVC *F,  int *imin, int *jmin,int *imax, int *jmax, bool *trouv, int classe){
    const char  *separators = ",;";
    Buffer_TOVC Buf;
    int k=1; int l=0;
    LireDir_TOVC(F, k, &Buf);
    int x; int y;
    int stop=0;
    *trouv=0;
    char *anneeClasseCh;
    char classCh[2];
    int c; int condition=0;
    unsigned int taille; char tailleCh[5];
    while (stop==0) {
        recuperer_chaine(F,2,&k,&l,tailleCh, &Buf);
        taille= atoi(tailleCh);
        char string[taille];
        recuperer_chaine(F,taille-2,&k,&l,string, &Buf);

        anneeClasseCh= strtok(string, separators);
        anneeClasseCh= strtok(NULL, separators);
        anneeClasseCh= strtok(NULL, separators);
        classCh[0]=anneeClasseCh[1];
        classCh[1]=anneeClasseCh[2];
        c= atoi(classCh);
        if((c==classe) && (condition==0)){
            y= l- taille;
            x=k;
            if (y<0){
                ///Cas de cehvauchement:
                x= k-1;
                y = 1023+y;
            }
            *trouv=1;
            *imin=x;
            *jmin=y;
            condition++;
        }
        if(c==classe){
            y= l- taille;
            x=k;
            if (y<0){
                ///Cas de cehvauchement:
                x= k-1;
                y = 1023+y;
            }
            *imax= x;
            *jmax= y;
        }
        if ( (k==entete_TOVC(F,1)) && (l==Buf.nb)){
            stop=1;
        }
        memset(string,' ' , taille*sizeof(char));
    }

}





/*
+------------------------------------------------------------------------------------+
|             Recherche d'un eleve a partir de son nom dans une classe               |
+------------------------------------------------------------------------------------+
*/
void Recherche_eleve_classe(Fichier_TOVC *F,  int *i, int *j, bool *trouv, int classe, char nom[], char chaine[]){
    Buffer_TOVC Buf; eleve eleve;
    int imin, jmin, imax, jmax; bool trouvClasse; bool stop=0;
    int x; int y;
    unsigned int taille; char tailleCh[5];
    Recherche_classe_fichier(F,  &imin, &jmin, &imax, &jmax, &trouvClasse, classe);
    LireDir_TOVC(F, imin, &Buf);
    if(trouvClasse==1){
        while(stop==0) {
            recuperer_chaine(F,2,&imin,&jmin,tailleCh, &Buf);
            taille= atoi(tailleCh);
            char chaine1[taille];
            char chaine2[taille];
            recuperer_chaine(F,taille-2,&imin,&jmin,chaine1, &Buf);
            strcpy(chaine2, tailleCh);
            strcat(chaine2, chaine1);
            chaine_vers_enreg(&eleve, chaine2);
            if(strncmp(eleve.nom, nom, strlen(nom))==0){
                *trouv=1;
                strcpy(chaine, tailleCh);
                strcat(chaine, chaine1);
                y= jmin- taille;
                x= imin;
                if (y<0){
                    ///Cas de cehvauchement:
                    x= imin-1;
                    y = 1023+y;
                }
                *i=x; *j=y;
                stop=1;
            }
            if(((imin==imax)  && (jmin== jmax)) || (strncmp(eleve.nom, nom, strlen(nom))>0)){
                stop=1;
            }
        }
    }
    if (*trouv==0){
        y= jmin- taille;
        x= imin;
        if (y<0){
            ///Cas de cehvauchement:
            x= imin-1;
            y = 1023+y;
        }
        *i=x; *j=y;
    }

}














/*************************************************************|
|                                                             |
|                 1- Les fonction d'affichage                 |
|                                                             |
|*************************************************************/




/*
+---------------------------------------------------------------------+
|             Affichage de tous les eleves de l'annee courrante       |
+---------------------------------------------------------------------+
*/
void affichage_eleves(Fichier_TOVC F){
    eleve eleve; bool trouv;
    char chaine[255];
    int i=1; int j=0;
    for (int k=1000; k<=entete_TOVC(&F,2); k++){
        Recherche_eleve_id(&F,k, &i, &j, &trouv, chaine);
        chaine_vers_enreg(&eleve, chaine);
        printf("--------------------------------------%s  %s----------------------------------------------\n",eleve.prenom, eleve.nom);
        printf("|    -> L'identifiant : %d\n", eleve.id);           // afficher la cle
        if (eleve.annee==0){
            printf("|    -> L'annee scolaire' : Preparatoire\n");           // afficher l'annee si prepa
        }else{
            printf("|    -> L'annee scolaire' : %d\n", eleve.annee);           // afficher l'annee si non prepa
        }
        printf("|    -> La classe : %d \n", eleve.classe); // afficher la classe
        printf("|    -> Le genre : %c\n", eleve.genre);  // afficher le genrre de cet eleve
        printf("|    -> La taille de l'enregistrement : %d\n", eleve.taille);          // afficher la taille de l'enregistrement ou les , et ; sont incluses
        if (eleve.annee!=0){
            //Afficher les noets si non prepa:
            printf("|                        |-----------------------Les notes-----------------------|\n");
            printf("|                          * Arabe : %d \n", eleve.notes[0]);
            printf("|                          * Maths : %d \n", eleve.notes[1]);
            printf("|                          * Science : %d \n", eleve.notes[2]);
            printf("|                          * Histoire/Geographique : %d \n", eleve.notes[3]);
            printf("|                          * Sciences Islamiques : %d \n", eleve.notes[4]);
            printf("|                          * Francais : %d \n", eleve.notes[5]);
            printf("|                          * Anglais : %d \n", eleve.notes[6]);
            printf("|                          * sport: %d \n", eleve.notes[7]);
            printf("|                        |-------------------------------------------------------|\n");        }
        printf("----------------------------------------------------------------------------------------------------------\n\n");

    }
}













/*
+-------------------------------------------------------------------------+
|            Affichage d'une classe a partir de son numero                |
+-------------------------------------------------------------------------+
*/
void affichage_classe(Fichier_TOVC F, int classe){
    Buffer_TOVC Buf; eleve eleve;
    int imin, jmin, imax, jmax; bool trouv=0; bool stop=0;
    unsigned int taille; char tailleCh[5];
    Recherche_classe_fichier(&F,  &imin, &jmin, &imax, &jmax, &trouv, classe);
    if (trouv==1){
        while(stop==0){
            LireDir_TOVC(&F, imin, &Buf);
            recuperer_chaine(&F,2,&imin,&jmin,tailleCh, &Buf);
            taille= atoi(tailleCh);
            char chaine[taille];
            char chaine2[taille];
            recuperer_chaine(&F,taille-2,&imin,&jmin,chaine, &Buf);
            strcpy(chaine2, tailleCh);
            strcat(chaine2, chaine);
            chaine_vers_enreg(&eleve, chaine2);
            printf("----------------------------------------------------------------------------------------------------------\n\n");
            printf("|    -> Prenom : %s\n",eleve.prenom);           // afficher le prenom dabord
            printf("|    -> Nom : %s\n",eleve.nom);           // afficher le nom
            printf("|    -> L'identifiant : %d\n", eleve.id);           // afficher la cle
            if (eleve.annee==0){
                printf("|    -> L'annee scolaire' : Preparatoire\n");           // afficher l'annee si prepa
            }else{
                printf("|    -> L'annee scolaire' : %d\n", eleve.annee);           // afficher l'annee si non prepa
            }
            printf("|    -> La classe : %d \n", eleve.classe); // afficher la classe
            printf("|    -> Le genre : %c\n", eleve.genre);  // afficher le genrre de cet eleve
            printf("----------------------------------------------------------------------------------------------------------\n\n");
            if((imin==imax)  && (jmin== jmax)){
                stop=1;
            }
        }
    }
}










/*
+---------------------------------------------------------------------+
|             Affichage d'un eleve a partir de son ID                 |
+---------------------------------------------------------------------+
*/
void affichage_eleve_id(Fichier_TOVC F) {
    eleve eleve;
    int id; int i=1; int j=0;  bool trouv; char chaine[255];
    printf("\nDonnez le ID de l'eleve a afficher: ");
    scanf("%d", &id);
    Recherche_eleve_id(&F,id,&i,&j,&trouv, chaine);
    if(trouv==1){
        chaine_vers_enreg(&eleve, chaine);
        printf("----------------------------------------------------------------------------------------------------------\n");
        printf("|    -> Prenom : %s\n", eleve.prenom);           // afficher le prenom
        printf("|    -> nom : %s\n", eleve.nom);           // afficher le nom
        printf("|    -> L'identifiant : %d\n", eleve.id);           // afficher la cle
        if (eleve.annee==0){
            printf("|    -> L'annee scolaire' : Preparatoire\n");           // afficher l'annee si prepa
        }else{
            printf("|    -> L'annee scolaire' : %d\n", eleve.annee);           // afficher l'annee si non prepa
        }
        printf("|    -> La classe : %d \n", eleve.classe); // afficher la classe
        printf("|    -> Le genre : %c\n", eleve.genre);  // afficher le genrre de cet eleve
        printf("|    -> La taille de l'enregistrement : %d\n", eleve.taille);          // afficher la taille de l'enregistrement ou les , et ; sont incluses
        if (eleve.annee!=0){
            //Afficher les noets si non prepa:
            printf("|                        |-----------------------Les notes-----------------------|\n");
            printf("|                          * Arabe : %d \n", eleve.notes[0]);
            printf("|                          * Maths : %d \n", eleve.notes[1]);
            printf("|                          * Science : %d \n", eleve.notes[2]);
            printf("|                          * Histoire/Geographique : %d \n", eleve.notes[3]);
            printf("|                          * Sciences Islamiques : %d \n", eleve.notes[4]);
            printf("|                          * Francais : %d \n", eleve.notes[5]);
            printf("|                          * Anglais : %d \n", eleve.notes[6]);
            printf("|                          * sport: %d \n", eleve.notes[7]);
            printf("|                        |-------------------------------------------------------|\n");        }
        printf("----------------------------------------------------------------------------------------------------------\n\n");
    }else{
        printf("\n>>Cet eleve n'existe pas dans le fichier!");
    }
}











/*
+---------------------------------------------------------------------+
|             Affichage d'un eleve a partir de son nom                |
+---------------------------------------------------------------------+
*/
void affichage_eleve_nom(Fichier_TOVC F){
    eleve eleve; Buffer_TOVC  Buf;
    int i=1; int j=0;  bool trouv; char chaine[255]; char nom[50];
    printf("\nDonnez le nom de famille de l'eleve a afficher: ");
    fgets(nom, 255, stdin);
    nom[strlen(nom) - 1] = '\0';

    Recherche_eleve_nom(&F,&i,&j,&trouv,nom, chaine, &Buf);
    if(trouv==1){
        chaine_vers_enreg(&eleve, chaine);
        printf("----------------------------------------------------------------------------------------------------------\n");
        printf("|    -> Prenom : %s\n", eleve.prenom);           // afficher le prenom
        printf("|    -> nom : %s\n", eleve.nom);           // afficher le nom
        printf("|    -> L'identifiant : %d\n", eleve.id);           // afficher la cle
        if (eleve.annee==0){
            printf("|    -> L'annee scolaire' : Preparatoire\n");           // afficher l'annee si prepa
        }else{
            printf("|    -> L'annee scolaire' : %d\n", eleve.annee);           // afficher l'annee si non prepa
        }
        printf("|    -> La classe : %d \n", eleve.classe); // afficher la classe
        printf("|    -> Le genre : %c\n", eleve.genre);  // afficher le genrre de cet eleve
        printf("|    -> La taille de l'enregistrement : %d\n", eleve.taille);          // afficher la taille de l'enregistrement ou les , et ; sont incluses
        if (eleve.annee!=0){
            //Afficher les noets si non prepa:
            printf("|                        |-----------------------Les notes-----------------------|\n");
            printf("|                          * Arabe : %d \n", eleve.notes[0]);
            printf("|                          * Maths : %d \n", eleve.notes[1]);
            printf("|                          * Science : %d \n", eleve.notes[2]);
            printf("|                          * Histoire/Geographique : %d \n", eleve.notes[3]);
            printf("|                          * Sciences Islamiques : %d \n", eleve.notes[4]);
            printf("|                          * Francais : %d \n", eleve.notes[5]);
            printf("|                          * Anglais : %d \n", eleve.notes[6]);
            printf("|                          * sport: %d \n", eleve.notes[7]);
            printf("|                        |-------------------------------------------------------|\n");        }
        printf("----------------------------------------------------------------------------------------------------------\n\n");
    }else{
        printf("\n>>Cet eleve n'existe pas dans le fichier!");
    }
}




/*************************************************************|
|                                                             |
|                 1- Les fonction Outils                      |
|                                                             |
|*************************************************************/




/*
+--------------------------------------------------------------------------------+
|           Recupperer un enregistrement eleve a partir d'une chaine             |
+----------------------------------------  --------------------------------------+
*/

///Remarque: Une chaine est de forme:
// Taille,ID,Annee_Classe,nom,prenom,genere si annee=0
// Taille,ID,Annee_Classe,nom,prenom,genere,n1n2n3n4n5n6n7n8 si annee<>0

void chaine_vers_enreg(eleve *eleve, char *chaine)
{
    const char  *separators = ",;";
    char * sousChaine= strtok ( chaine, separators ); //On est dans le champ taille
    eleve->taille= atoi(sousChaine);
    sousChaine= strtok ( NULL, separators ); //On est dans le champ eff
    eleve->eff= atoi(sousChaine);
    sousChaine= strtok ( NULL, separators ); // On est dans le champ id
    eleve->id= atoi(sousChaine);
    sousChaine= strtok ( NULL, separators ); // On AnneeClasse (annee est represente dans le poid faible)
    if (sousChaine[0]=='P'){
        eleve->annee=0;
    }else{
        char anneeCh[2];
        strncpy(anneeCh, sousChaine,1);
        anneeCh[1]= '\0';
        eleve->annee= atoi(anneeCh);
    }
    char classeCh[3];
    classeCh[0]= sousChaine[1];
    classeCh[1]= sousChaine[2];
    eleve->classe= atoi(classeCh);
    sousChaine= strtok ( NULL, separators ); // On est dans le champ nom
    eleve->nom= sousChaine;
    sousChaine= strtok ( NULL, separators ); // On est dans le champ prenom
    eleve->prenom= sousChaine;
    sousChaine= strtok ( NULL, separators ); // On est dans le champ genre
    eleve->genre= sousChaine[0];
    if(eleve->annee!=0){
        sousChaine= strtok ( NULL, separators ); // On est dans le champ notes
        char notes[3];
        int i=0; int k=0;
        while(i<8){
            notes[0]= sousChaine[k];
            k++;
            notes[1]= sousChaine[k];
            k++;
            notes[2]= '\0';
            //eleve->notes = malloc(8*sizeof(int));
            eleve->notes[i]= atoi(notes);
            i++;
        }

    }
};







/*
+--------------------------------------------------------------------------------+
|           Recupperer une chaine eleve a partir d'un enregistrement             |
+----------------------------------------  --------------------------------------+
*/

///Remarque: Une chaine est de forme:
// Taille,ID,Annee_Classe,nom,prenom,genere si annee=0
// Taille,ID,Annee_Classe,nom,prenom,genere,n1n2n3n4n5n6n7n8 si annee<>0

void enreg_vers_chaine(eleve eleve, char chaine[]){
    const char *prepa= "P";
    char idCh[5]; char eff[3];
    char classeCh[3]; char anneeCh[2];  char anneeClasseCh[4];
    char notesCh[17]; char note[3];
    char genreCh[2];
    char tailleCh[3];
    int k=0;
    NumtoS(eleve.classe,2,classeCh);
    if (eleve.annee==0){
        //Cas de classe preparatoire
        strcpy(anneeClasseCh, prepa);
        strcat(anneeClasseCh, classeCh);
    }else{
        NumtoS(eleve.annee, 1, anneeCh);
        NumtoS(eleve.classe,2,classeCh);
        strcpy(anneeClasseCh, anneeCh);
        strcat(anneeClasseCh, classeCh);
        while(k<8){
            NumtoS(eleve.notes[k], 2,note);
            if(k==0){
                strcpy(notesCh, note);
            }else{
                strcat(notesCh, note);
            }
            k++;
        }
    }

    NumtoS(eleve.taille, 2, tailleCh);
    NumtoS(eleve.eff, 1, eff);
    NumtoS(eleve.id, 4, idCh);
    NumtoS(eleve.annee, 1, anneeCh);
    char chaineenreg[eleve.taille];
    strcpy(chaineenreg,tailleCh);
    strcat(chaineenreg,",");
    strcat(chaineenreg, eff);
    strcat(chaineenreg,",");
    strcat(chaineenreg, idCh);
    strcat(chaineenreg,",");
    strcat(chaineenreg, anneeClasseCh);
    strcat(chaineenreg,",");
    strcat(chaineenreg, eleve.nom);
    strcat(chaineenreg,",");
    strcat(chaineenreg, eleve.prenom);
    strcat(chaineenreg,",");
    genreCh[0]=eleve.genre;
    genreCh[1]= '\0';
    strcat(chaineenreg,genreCh);
    if (eleve.annee!=0){
        strcat(chaineenreg,",");
        strcat(chaineenreg, notesCh);
    }
    strcat(chaineenreg,";");
    strcpy(chaine, chaineenreg);
}






















/***********************************************|
|                                               |
|     FONCTIONS IMPLEMENTES POUR PARTIE 02      |
|                                               |
|***********************************************/






/***********************************************|
|                                               |
|         1- Les fonction demandees:            |
|                                               |
|***********************************************/






/*
+--------------------------------------------------------------------------------+
|           construire un fichier de dossiers scolaires parélève comprenant les  |
|  années de scolarisation par élève et la moyenne de passage.                   |
+----------------------------------------  --------------------------------------+
*/
void creer_fichier_des_dossiers_scolaires(Fichier_TOF *F, Fichier_TOVC *Fcour, Tenreg_INDEX_dense Index[], int *nbInd, int *cpt_EcrireDire_TOF) {
    Buffer_TOF Buf;
    eleve eleve;
    bool trouv;
    const float matieres = 8;
    eleve_TOF eleveTof;
    char chaine[255];
    int i1 = 1;
    int j1 = 0;
    int i2 = 1;
    int j2 = 0;

    Tenreg_INDEX_dense e;
    int NbE = 0;
    Buf.nb = 0;
    for (int k=1000; k<=entete_TOVC(Fcour, 2) ; k++){

        Recherche_eleve_id(Fcour, k, &i1, &j1, &trouv, chaine);
        chaine_vers_enreg(&eleve, chaine);
        eleveTof.id= eleve.id;
        strcpy(eleveTof.nom, eleve.nom);
        eleveTof.nom[strlen(eleveTof.nom)]='\0';
        strcpy(eleveTof.prenom, eleve.prenom);
        eleveTof.prenom[strlen(eleveTof.prenom)]='\0';
        eleveTof.genre= eleve.genre;
        if (eleve.annee==0){
            for(int l=0;l<=5;l++){
                eleveTof.annee= l;
                eleveTof.moyenne=0;
                if(j2<128){
                    Buf.Tab[j2]= eleveTof;
                    Buf.nb++; j2++;
                }else{
                    EcrireDir_TOF(F, i2, Buf);
                    *cpt_EcrireDire_TOF++;
                    Buf.nb = 1;
                    Buf.Tab[0] = eleveTof;
                    j2 = 1;
                    i2++;
                }
            }
        }else{
            {
                for (int l = 0; l <= 5; l++)
                {

                    eleveTof.annee = l;
                    if (l == eleve.annee) {

                        int sum = 0;
                        for (int m = 0; m <= 7; m++) {
                            sum = sum + eleve.notes[m];
                        }
                        eleveTof.moyenne = sum / matieres;
                        NbE++;
                        if (j2 < 128) {
                            Buf.Tab[j2] = eleveTof;
                            ///Insertion dans l'index:
                            e.moyenne = eleveTof.moyenne;
                            e.NumBloc = i2;
                            e.Deplacement = j2;
                            insertion_Index(Index, e, NbE);
                            Buf.nb++;
                            j2++;
                        } else {
                            EcrireDir_TOF(F, i2, Buf);
                            *cpt_EcrireDire_TOF++;
                            Buf.nb = 1;
                            Buf.Tab[0] = eleveTof;
                            ///Insertion dans l'index:
                            e.moyenne = eleveTof.moyenne;
                            e.NumBloc = i2;
                            e.Deplacement = 0;
                            insertion_Index(Index, e, NbE);
                            j2 = 1;
                            i2++;

                        }
                    } else {
                        int sum = 0;
                        if ((l < eleve.annee) && (l != 0)) {
                            NbE++;
                            while (sum == 0) {
                                sum = rand() % 161;
                            }
                            eleveTof.moyenne = sum / matieres;
                        } else {
                            eleveTof.moyenne = 0;
                        }
                        if (j2 < 128) {
                            Buf.Tab[j2] = eleveTof;
                            ///Insertion dans l'index:
                            if ((l < eleve.annee) && (l != 0)) {
                                e.moyenne = eleveTof.moyenne;
                                e.NumBloc = i2;
                                e.Deplacement = j2;
                                insertion_Index(Index, e, NbE);
                            }
                            Buf.nb++;
                            j2++;
                        } else {
                            EcrireDir_TOF(F, i2, Buf);
                            *cpt_EcrireDire_TOF++;
                            Buf.nb = 1;
                            Buf.Tab[0] = eleveTof;
                            j2 = 1;
                            i2++;
                            ///Insertion dans l'index:
                            if ((l < eleve.annee) && (l != 0)) {
                                e.moyenne = eleveTof.moyenne;
                                e.NumBloc = i2;
                                e.Deplacement = 0;
                                insertion_Index(Index, e, NbE);
                            }
                        }
                    }
                }
            }
        }
    }
    Aff_Entete_TOF(F, 1, i2);
    Aff_Entete_TOF(F, 2, (entete_TOVC(Fcour, 2)-1000 +1)*6);
    if(j2<128){
        EcrireDir_TOF(F,i2, Buf);
    }
    *nbInd= NbE;
}




















/*
+--------------------------------------------------------------------------------+
|           Insertion dans un Index dense avec decalagaes                        |
+----------------------------------------  --------------------------------------+
*/
void insertion_Index(Tenreg_INDEX_dense Index[], Tenreg_INDEX_dense e, int NbE){
    int k=NbE; int l=0;
    if(k==0){
        Index[0]=e;
    }else{
        while((e.moyenne>Index[l].moyenne) && (l<NbE)){
            l++;
        }
        Index[k+1]= Index[k];
        while(k>l)
        {
            Index[k]= Index[k-1];
            k--;
        }
        Index[l]=e;
    }
}



/*
+--------------------------------------------------------------------------------+
|                           Creer Index nonDense                                 |
+--------------------------------------------------------------------------------+
*/
void creer_Index_nondense(Fichier_TOF archive, Tenreg_INDEX_nondense  Indexnondense[], int *NbInd){
    Buffer_TOF Buf; eleve_TOF eleveTof;
    printf("entete: %d", entete_TOF(&archive,1));
    int i=1;
    int condition=0;
    while (i<= entete_TOF(&archive,1)) {
        LireDir_TOF(&archive, i, &Buf);
        for (int k = 0; k < Buf.nb; k++) {
            if(condition==0){
                if(Buf.Tab[k].moyenne!=0){
                    Indexnondense[i].moyenneMin= Buf.Tab[k].moyenne;
                    Indexnondense[i].moyenneMax= Buf.Tab[k].moyenne;
                    condition++;
                }
            }
            if(condition==1){
                if((Buf.Tab[k].moyenne<Indexnondense[i].moyenneMin ) && (Buf.Tab[k].moyenne!=0)){
                    Indexnondense[i].moyenneMin= Buf.Tab[k].moyenne;
                }
                if((Buf.Tab[k].moyenne> Indexnondense[i].moyenneMax) && (Buf.Tab[k].moyenne!=0)){
                    Indexnondense[i].moyenneMax= Buf.Tab[k].moyenne;
                }
            }
        }
        i++; condition=0;
    }
    *NbInd=i-1;
    for (int j=1; j<= *NbInd; j++){
        printf("(%d) min: %.2f  max: %.2f \n", j, Indexnondense[j].moyenneMin, Indexnondense[j].moyenneMax );
    }
}








/*
+---------------------------------------------------------------------------------+
|           simuler 5 années de scolarisation ainsi que l’archivage               |
+---------------------------------------------------------------------------------+
*/
void creer_fichier_5annee(Fichier_TOF *F, Tenreg_INDEX_dense Index[], int *nbInd, int *cpt_EcrireDire_TOF, int *cpt_LireDire_TOF){
    Buffer_TOF Buf;
    const float matieres = 8;
    int i=1;
    int NbE= 336;
    Tenreg_INDEX_dense e;
    for(i=1; i<=entete_TOF(F,1); i++){
        LireDir_TOF(F,i,&Buf);
        *cpt_LireDire_TOF++;
        for (int j=0; j<Buf.nb; j++){
            if(Buf.Tab[j].moyenne==0){
                int sum = 0;
                NbE++;
                while (sum == 0) {
                    sum = rand() % 161;
                }
                Buf.Tab[j].moyenne= sum / matieres;
                ///Insertion dans l'Index
                e.moyenne = Buf.Tab[j].moyenne;
                e.NumBloc = i;
                e.Deplacement = j;
                insertion_Index(Index, e, NbE);
            }
        }
        EcrireDir_TOF(F, i, Buf);
        *cpt_EcrireDire_TOF++;
    }
    *nbInd= NbE;
}






/*
+--------------------------------------------+
|             Reception d'un eleve           |
+--------------------------------------------+
*/
void Transfert(Fichier_TOF *archive, Fichier_TOVC *F, Tenreg_INDEX_dense Index[], int *nbInd, int *cpt_EcrireDire_TOF) {
    Buffer_TOF Buf;
    eleve eleve;
    bool trouv;
    const float matieres = 8;
    eleve_TOF eleveTof;
    char chaine[255];
    int i1 = 1;
    int j1 = 0;
    int i2 = entete_TOF(archive, 1);
    LireDir_TOF(archive, i2, &Buf);
    int j2 = Buf.nb;
    Tenreg_INDEX_dense e;
    int NbE= entete_TOF(archive,2);
    insertion_eleve(F); //Pour l'annee en cours
    affichage_TOVC(F);
    ///Insertion dans le fichier de dossies scolaireesr:
    Recherche_eleve_id(F, entete_TOVC(F, 2), &i1, &j1, &trouv, chaine);
    chaine_vers_enreg(&eleve, chaine);
    eleveTof.id = eleve.id;
    strcpy(eleveTof.nom, eleve.nom);
    eleveTof.nom[strlen(eleveTof.nom)] = '\0';
    strcpy(eleveTof.prenom, eleve.prenom);
    eleveTof.prenom[strlen(eleveTof.prenom)] = '\0';
    eleveTof.genre = eleve.genre;

    if (eleve.annee == 0) {
        for (int l = 0; l <= 5; l++) {
            eleveTof.annee = l;
            eleveTof.moyenne = 0;
            if (j2 < 128) {
                Buf.Tab[j2] = eleveTof;
                Buf.nb++;
                j2++;
            } else {
                EcrireDir_TOF(archive, i2, Buf);
                *cpt_EcrireDire_TOF++;
                Buf.nb = 1;
                Buf.Tab[0] = eleveTof;
                j2 = 1;
                i2++;
            }
        }
    } else {
        for (int l = 0; l <= 5; l++)
        {
            eleveTof.annee = l;
            if (l == eleve.annee) {

                int sum = 0;
                for (int m = 0; m <= 7; m++) {
                    sum = sum + eleve.notes[m];
                }
                eleveTof.moyenne = sum / matieres;
                NbE++;
                if (j2 < 128) {
                    Buf.Tab[j2] = eleveTof;
                    ///Insertion dans l'index:
                    e.moyenne = eleveTof.moyenne;
                    e.NumBloc = i2;
                    e.Deplacement = j2;
                    insertion_Index(Index, e, NbE);
                    Buf.nb++;
                    j2++;
                } else {
                    EcrireDir_TOF(archive, i2, Buf);
                    *cpt_EcrireDire_TOF++;
                    Buf.nb = 1;
                    Buf.Tab[0] = eleveTof;
                    ///Insertion dans l'index:
                    e.moyenne = eleveTof.moyenne;
                    e.NumBloc = i2;
                    e.Deplacement = 0;
                    insertion_Index(Index, e, NbE);
                    j2 = 1;
                    i2++;

                }
            } else {
                if ((l < eleve.annee) && (l != 0)) {
                    NbE++;
                    printf(">>Donner la moyenne sur 20 de la forme nn.nn pour l'annee %d: ", l);
                    scanf("%f", &eleveTof.moyenne);
                } else {
                    eleveTof.moyenne = 0;
                }
                if (j2 < 128) {
                    Buf.Tab[j2] = eleveTof;
                    ///Insertion dans l'index:
                    if ((l < eleve.annee) && (l != 0)) {
                        e.moyenne = eleveTof.moyenne;
                        e.NumBloc = i2;
                        e.Deplacement = j2;
                        insertion_Index(Index, e, NbE);
                    }
                    Buf.nb++;
                    j2++;
                } else {
                    EcrireDir_TOF(archive, i2, Buf);
                    *cpt_EcrireDire_TOF++;
                    Buf.nb = 1;
                    Buf.Tab[0] = eleveTof;
                    j2 = 1;
                    i2++;
                    ///Insertion dans l'index:
                    if ((l < eleve.annee) && (l != 0)) {
                        e.moyenne = eleveTof.moyenne;
                        e.NumBloc = i2;
                        e.Deplacement = 0;
                        insertion_Index(Index, e, NbE);
                    }
                }
            }
        }
    }
    Aff_Entete_TOF(archive, 1, i2);
    Aff_Entete_TOF(archive, 2, entete_TOF(archive,2)+6);
    if(j2<128){
        EcrireDir_TOF(archive,i2, Buf);
    }
    *nbInd= NbE;
}





/***********************************************|
|                                               |
|        2  -Les fonctions d'affichage          |
|                                               |
|***********************************************/


/*
+--------------------------------------------+
|             Reception d'un eleve           |
+--------------------------------------------+
*/
void affichage_eleve_moy12(Fichier_TOF F){
    Buffer_TOF Buf; eleve_TOF eleveTof;
    int i=1; int j=0; bool sup12=1; bool stop=0;
    int cpt=0; int ok=0;
    LireDir_TOF(&F,i,&Buf);
    while(stop==0){
        while(cpt<6){
            if ((Buf.Tab[j].moyenne<12) && (Buf.Tab[j].annee!=0)){
                sup12=0;
            }
            ok=j;
            if(j==127){
                j=0;
                i++;
                LireDir_TOF(&F,i,&Buf);
            }else{
                j++;
            }
            cpt++;
        }
        if(sup12==1){
            eleveTof= Buf.Tab[ok];
            printf("--------------------------------------%s  %s----------------------------------------------\n",eleveTof.prenom, eleveTof.nom);
            printf("|    -> L'identifiant : %d\n", Buf.Tab[ok].id);           // afficher la cle
            printf("|    -> Le genre : %c\n", Buf.Tab[ok].genre);  // afficher la taille de la description
            printf("----------------------------------------------------------------------------------------------------------\n\n");
        }
        cpt=0;
        sup12=1;
        if((i== entete_TOF(&F,1))&&(j=Buf.nb)){
            stop=1;
        }
    }
    printf(">>Si aucun eleve n'est affiche alors la fonction rand() %% int n'a pas pu generer pour toutes les 5 annees une moyenne superieure a 12! \n");
}







/*
+----------------------------------------------------------------------------+
|             AFfichage de tous les eleves dans le fichier archive           |
+----------------------------------------------------------------------------+
*/
void affichage_elevesTof(Fichier_TOF F){
    Buffer_TOF Buf; eleve_TOF eleveTof;
    int i=1;
    printf("entete: %d\n\n", entete_TOF(&F, 2));
    while (i<= entete_TOF(&F,1)){
        LireDir_TOF(&F, i, &Buf);
        printf("( %d )----------------------------------------------------------------------------------------------------------------\n", i);
        for (int k=0; k<Buf.nb; k++){
            eleveTof= Buf.Tab[k];
            printf("--------------------------------------%s  %s----------------------------------------------\n",eleveTof.prenom, eleveTof.nom);
            printf("|    -> L'identifiant : %d\n", Buf.Tab[k].id);           // afficher la cle
            if (Buf.Tab[k].annee==0){
                printf("|    -> L'annee scolaire' : Preparatoire\n");           // afficher l'annee
            }else{
                printf("|    -> L'annee scolaire : %d\n", Buf.Tab[k].annee);           // afficher l'annee
            }
            printf("|    -> Le genre : %c\n", Buf.Tab[k].genre);  // afficher la taille de la description
            if (Buf.Tab[k].annee!=0){
                printf("|    -> La moyenne: %.2f\n", Buf.Tab[k].moyenne);  // afficher la taille de la description
            }
            printf("----------------------------------------------------------------------------------------------------------\n\n");
        }
        printf("\n");
        i++;
    }
}
