#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define tailleMax 37000
int  S,R,i=0,j=0,cpt,size,cptNbLien=0, size_moins,sizeApresModif,nblienTotal,nblienTotalApresModif, nbLien,nbLienFinal, nbColonne=0, colonneNotProba=5,indice=0, *listeLigne = NULL, *listeLigne_Modif = NULL; //Si colonneNotProba = 1 --> c'est une colonne sinon c'est une proba
char  *caractere;
double somme=0.0,proba, alpha = 1, *vecteur_n1, *vecteur_n2, *vecteur_n1_modif,*vecteur_n2_modif;
double **matrice_H = NULL, **matrice_H_Modif = NULL;
int verif = 0, iteration=0, t,tt;
double sommevec=0.0;
FILE* fichier=NULL;
char sentence[10000] ;
char *secondword, *word;
int col=0,ligne=0;
int main()
{
    word = strtok(sentence," ");
    cpt=0;
    char chaine[tailleMax] = "";
    fichier = fopen("web1.txt","r");
    fgets(chaine, tailleMax, fichier); // recup taille
    size = 1 + (int)atof(chaine);
    size_moins = size - 1;
    fgets(chaine, tailleMax, fichier); // recup nblien
    nblienTotal = (int)atof(chaine);
    //printf("Nb noeud = %d \nnblien = %d \n",size_moins, nblienTotal);
    lectureMatrice();
    initDebutVecteurN1N2();
    verifConvergence();
    // DEBUT PROJET

    algoAddSommetSeulement(5,5);
    //printf("size aprés modif = %d", nblienTotalApresModif);
    free(matrice_H);
    verifConvergenceApresModif(); // l'algo boucle a l'infini je ne sais pas pourquoi, pourtant la nouvelle matrice est stoquastique et on normalise le vecteur a chaque fois


    return 0;
}

int randNumber(int min, int max)
{
    int distance ,anyRandom,x;
    distance = max - min  +1;
    anyRandom = rand();
    x = anyRandom % distance;
    return(x+min);
}

void algoAddSommetSeulement(int nombreSommetAjoute, int nombreLiensAjoute)
{
    //On ajoute des liens seulement pour les nouveau sommets et les liens peuvent etre sur tout les sommets du graphe
    S = nombreSommetAjoute;
    nblienTotalApresModif = nblienTotal + nombreLiensAjoute;
    //Allocation nouvelle stucture augmenté avec 50 lien en plus
    matrice_H_Modif = malloc(nblienTotalApresModif * sizeof(double*));
    matrice_H_Modif[0] = malloc(nblienTotalApresModif * sizeof(double)); //vecteur proba
    matrice_H_Modif[1] = malloc(nblienTotalApresModif * sizeof(double)); //vecteur numero ligne
    matrice_H_Modif[2] = malloc(nblienTotalApresModif * sizeof(double)); //vecteur numero colonne
    for(i=0; i<nblienTotal ; i++)
    {
        matrice_H_Modif[0][i] = matrice_H[0][i];
        matrice_H_Modif[1][i] = matrice_H[1][i];
        matrice_H_Modif[2][i] = matrice_H[2][i];
    }
    //free(matrice_H);
    for(i=nblienTotal; i<nblienTotalApresModif; i++)
    {
        //initialiser les nouvelles cases a 0
        matrice_H_Modif[0][i] = 0;
        matrice_H_Modif[1][i] = 0;
        matrice_H_Modif[2][i] = 0;
    }
    //Allocation nouveau vecteur augmenter de 5 nouvelles pages
    sizeApresModif = size_moins + nombreSommetAjoute;
    vecteur_n1_modif = (double *) malloc(sizeof(double)*sizeApresModif);
    vecteur_n2_modif = (double *) malloc(sizeof(double)*sizeApresModif);
    for(i=0; i<size_moins ; i++)
    {
        vecteur_n1_modif[i] = vecteur_n1[i];
        vecteur_n2_modif[i] = 0;
    }
    //printf("size = %d\n", size);
    //printf("vecteur[%d] = %lf \n", size_moins-1, vecteur_n1[size_moins-1]);
    //printf("vecteur_modif[%d] = %lf \n", size_moins+5, vecteur_n1_modif[size_moins-5]);
    for(j=size_moins; j<sizeApresModif; j++)
    {
        //initialisation du nouveau vecteur
        //ici modif aprés
        vecteur_n1_modif[j] = 0.0;
        vecteur_n2_modif[j] = 0.0;
    }
    // Modification stucture de donnée et donc MODIFICATION DU GRAPHE DU WEB
    t = nblienTotalApresModif-nblienTotal;
    //printf("size moins = %d ttttt", size_moins);
    cpt = 0;
    for(i=nblienTotal; i<nblienTotalApresModif; i++)
    {
        ligne = randNumber(size_moins,sizeApresModif);

        col = randNumber(1, sizeApresModif);
        //printf("Ligne = %d \n",ligne);
        /*for(j=0; j<nblienTotalApresModif; j++)
        {
            if(matrice_H_Modif[1][j] == ligne)
            {
                //printf("Test %lf \n",matrice_H_Modif[1][j]);
                cpt++;
                //printf("cpt = %d \n",cpt);
            }
        }
        if(cpt == 0) //Pour eviter le problm de IND0.0
        {
            proba = 0.0;
        }
        else
        {
            proba = (double)1/cpt;
        }
        */
        //printf("proba = %lf \n",proba);
        //matrice_H_Modif[0][i] = proba;
        matrice_H_Modif[1][i] = ligne;
        matrice_H_Modif[2][i] = col;
        //printf("Ligne = %d || cpt= %lf\n",ligne, cpt);
        //cpt=0;
        //printf("matricemodfi[%d] = %lf \n",i, matrice_H_Modif[0][i]);
    }
    for(i=nblienTotal; i<nblienTotalApresModif; i++)
    {
        for(j=0; j<nblienTotalApresModif; j++)
        {
            if(matrice_H_Modif[1][j] == ligne)
            {
                //printf("Test %lf \n",matrice_H_Modif[1][j]);
                cpt++;
                //printf("cpt = %d \n",cpt);
            }
        }
        if(cpt == 0) //Pour eviter le problm de IND0.0
        {
            proba = 0.0;
        }
        else
        {
            proba = (double)1/cpt;
        }
        matrice_H_Modif[0][i] = proba;
        cpt = 0;
    }
}

void lectureMatrice()
{
    if(fichier != NULL)
    {
    size_moins = size - 1;
    //listeLigne = (int *) malloc(size_moins * sizeof(int));
    //printf("Nb lien first = %d \n", nblienTotal);
    matrice_H = malloc(nbLien * sizeof(double*));
    matrice_H[0] = malloc(nblienTotal * sizeof(double)); //vecteur proba
    matrice_H[1] = malloc(nblienTotal * sizeof(double)); //vecteur numero ligne
    matrice_H[2] = malloc(nblienTotal * sizeof(double)); //vecteur numero colonne
    vecteur_n1 = (double *) malloc(sizeof(double)*size_moins);
    vecteur_n2 = (double *) malloc(sizeof(double)*size_moins);
    int nbLigne = 0;
    while(fgets(sentence,tailleMax, fichier) != NULL) // parcours fichier ligne par ligne
        {
            do
            {
                if(cpt == 0) //si on est en debut de ligne
                    {
                    word = strtok(sentence," ");
                    nbLigne = (int) atof(word);
                    //printf("Ligne test bebe = %d\n", nbLigne);
                    secondword = strtok(NULL," ");
                    nbLien = (int) atof(secondword);
                    if(nbLien != 0)
                    {
                        //listeLigne[j] = indice;
                    }
                    else
                    {
                        //listeLigne[j] = -1;
                        j++;
                        //("\tTest ici ligne = %d \n",nbLigne);
                    }
                    cpt++;
                    }
                else
                    {
                    word = strtok(NULL," ");
                    secondword = strtok(NULL," ");
                    nbColonne = (int) atof(word);
                    proba = atof(secondword);
                    cptNbLien++;
                if((proba != 0.0) && (nbColonne != 0)) // ps condition nbcolone inutile
                    {
                    indice++;
                    matrice_H[0][i] = (double)proba;
                    matrice_H[1][i] = (int)nbLigne;
                    matrice_H[2][i] = (int)nbColonne;
                    i++; //i represente l'indice de parcout de notre double vecteur
                    }
                if(cptNbLien == nbLien)
                    {
                    j++;
                    }
                }

            }while(secondword != NULL);
            cpt=0;
            cptNbLien=0;
            }

    }
    else
    {
        printf("Impossible d'ouvrir le fichier");
    }
    fclose(fichier);
}
void initDebutVecteurN1N2()
{
    for(i=0 ; i<size_moins ; i++)
    {
        //printf("Liste[%d] = %d\n", i,listeLigne[i]);
        vecteur_n1[i] = (double)1/size_moins;
        vecteur_n2[i] = 0.0;
        //vecteur_FT[i] = 0;
        //printf("vecteur[%d] = %lf\n",i,vecteur_n1[i]);
    }
}
void verifConvergence()
{
    printf("\n");
    somme = 0.0;

    while(verif == 0)
    {
        for(j=0;j<nblienTotal;j++)
            {
                ligne = (int)matrice_H[1][j]-1;
                col = (int)matrice_H[2][j]-1;
                vecteur_n2[col] = vecteur_n2[col] + (matrice_H[0][j] * vecteur_n1[ligne]);
            }
        /*for(i=0;i<size_moins;i++)
        {
            printf("vecteur_2[%d] = %lf \n",i,vecteur_n2[i]);
            vecteur_n1[i] = vecteur_n2[i];
            vecteur_n2[i] = 0;
        }*/
        //printf("\n");
        //verif++;
    for(i=0; i<size_moins ; i++)
    {
        somme = vecteur_n2[i] - vecteur_n1[i];
        if(somme<0)
        {
            somme = -somme;
        }
        sommevec = sommevec + somme;
        //
        //printf("difference vecteur = %lf \n", sommevec);
    }
    //printf("SOMME VEC = %lf\n",sommevec);
    //printf("Somme vec = %lf \n",sommevec);
    if(sommevec<pow(10,-6))
    {
        verif = 1;
    }
    else
    {
        for(i=0; i<size_moins; i++)
        {
            vecteur_n1[i] = vecteur_n2[i];
            vecteur_n2[i] = 0;
            //printf("Iteration %d : vecteur1_modif[%d] = %lf \n",iteration, i, vecteur_n1[i]);
        }

    }
    sommevec = 0.0;
    iteration++;
    somme = 0.0;
    }
    /*for(i=0; i<size_moins; i++)
    {
        printf("vecteur_2[%d]=%lf \n",i,vecteur_n2[i]);
    }*/
    /*for(i=0; i<size_moins; i++)
    {
        printf("vecteur_2[%d]=%lf \n",i,vecteur_n2[i]);
    }*/

    /*for(i=0;i<size_moins;i++)
    {
        printf("vecteur[%d] = %lf\n",i,vecteur_n1[i]);
    }*/
    printf("Nb iteration = %d\n", iteration);
}
void verifConvergenceApresModif()
{
    verif=0;
    iteration = 0;
    printf("\n");
    somme = 0.0;
    sommevec=0;
    while(verif == 0)
    {

        for(j=0;j<nblienTotalApresModif;j++)
            {
                //printf("toto = %d \n", (int)matrice_H_Modif[1][j]-1);
                ligne = (int)matrice_H_Modif[1][j]-1;
                col = (int)matrice_H_Modif[2][j]-1;
                vecteur_n2_modif[col] = vecteur_n2_modif[col] + (matrice_H_Modif[0][j] * vecteur_n1_modif[ligne]);
            }
        //printf("\n");
        //verif++;
    for(i=0; i<sizeApresModif ; i++)
    {
        somme = vecteur_n2_modif[i] - vecteur_n1_modif[i];
        if(somme<0)
        {
            somme = -somme;
        }
        sommevec = sommevec + somme;
        //
        printf("%d \\ %lf - %lf = %lf \n",i,vecteur_n2_modif[i], vecteur_n1_modif[i], somme);
    }
    printf("Somme vec = %lf\n",sommevec);
    //printf("SOMME VEC modif = %0.50lf\n",sommevec);
    //printf("Somme vec = %lf \n",sommevec);
    if(sommevec<pow(10,-3))
    {
        verif = 1;
    }
    else
    {
        for(i=0; i<sizeApresModif; i++)
        {
            vecteur_n1_modif[i] = vecteur_n2_modif[i];
            vecteur_n2_modif[i] = 0;
            //printf("Iteration %d : vecteur1_modif[%d] = %lf \n",iteration, i, vecteur_n1[i]);
        }

    }
    somme=0.0;
    //renormalisation
    for(i=0; i<sizeApresModif; i++)
    {
        somme = somme + vecteur_n1_modif[i];
        if(somme > 1)
        {
            vecteur_n1_modif[i] = vecteur_n1_modif[i] / sizeApresModif;
        }
    }
    somme=0.0;
    /*for(i=0; i<sizeApresModif; i++)
    {
        printf("vecteur_n1_Norma[%d] = %lf\n",i,vecteur_n1_modif[i]);
    }
    printf("Normmmmaaa = %lf \n", somme);*/
    sommevec = 0.0;
    iteration++;
    somme = 0.0;
    }
    /*for(i=0; i<size_moins; i++)
    {
        printf("vecteur_2[%d]=%lf \n",i,vecteur_n2[i]);
    }*/
    /*for(i=0; i<size_moins; i++)
    {
        printf("vecteur_2[%d]=%lf \n",i,vecteur_n2[i]);
    }*/

    /*for(i=0;i<size_moins;i++)
    {
        printf("vecteur[%d] = %lf\n",i,vecteur_n1[i]);
    }*/
    printf("Nb iteration apres modif = %d\n", iteration);

    /*for(i=0;i<sizeApresModif;i++)
    {
        printf("vecteurModif[%d] = %lf\n",i,vecteur_n1_modif[i]);
    }
    printf("Nb iteration_Modifie = %d\n", iteration);*/
}

