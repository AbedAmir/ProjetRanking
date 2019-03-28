#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define tailleMax 37000
int main()
{
    char sentence[1000000] ;
    char *secondword, *word = strtok(sentence," ");
    /*printf("%s \n", word);
    secondword = strtok(NULL," ");
    while(secondword != NULL)
    {
        printf("%s \n", secondword);
        secondword = strtok(NULL," ");

    }*/

    int  i=0,j=0,cpt,size,cptNbLien=0, size_moins,nblienTotal, nbLien, nbColonne=0, colonneNotProba=5,indice=0, *listeLigne = NULL, *listeLigne_Modif = NULL; //Si colonneNotProba = 1 --> c'est une colonne sinon c'est une proba
    char  *caractere;
    cpt=0;
    FILE* fichier=NULL;
    double somme=0.0,proba, alpha = 1, *vecteur_n1, *vecteur_n2;
    char chaine[tailleMax] = "";
    fichier = fopen("web1.txt","r");

    fgets(chaine, tailleMax, fichier); // recup taille
    size = 1 + (int)atof(chaine);
    size_moins = size - 1;
    fgets(chaine, tailleMax, fichier); // recup nblien
    nblienTotal = (int)atof(chaine);
    printf("Nb noeud = %d \nnblien = %d \n",size_moins, nblienTotal);
    double **matrice_H = NULL, **matrice_H_Modif = NULL;
    if(fichier != NULL)
    {
    size_moins = size - 1;
    listeLigne = (int *) malloc(size_moins * sizeof(int));
    matrice_H = malloc(nbLien * sizeof(double*));
    matrice_H[0] = malloc(nbLien * sizeof(double)); //vecteur proba
    matrice_H[1] = malloc(nbLien * sizeof(double)); //vecteur numero ligne
    matrice_H[2] = malloc(nbLien * sizeof(double)); //vecteur numero colonne
    vecteur_n1 = (double *) malloc(sizeof(double)*size);
    vecteur_n2 = (double *) malloc(sizeof(double)*size);
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
                        listeLigne[j] = indice;
                    }
                    else
                    {
                        listeLigne[j] = -1;
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
    for(i = 0 ; i<nblienTotal;i++)
    {
        printf("Matrice[0][%d] = %lf || Matrice[1][%d] = %d || Matrice[2][%d] = %d\n",i,matrice_H[0][i],i,(int)matrice_H[1][i],i,(int)matrice_H[2][i]);
        //printf("Matrice[0][%d] = %lf || Matrice[1][%d] = %d \n",i,matrice_H[0][i],i,(int)matrice_H[1][i]);

    }
    for(i=0 ; i<size_moins ; i++)
    {
        //printf("Liste[%d] = %d\n", i,listeLigne[i]);
        vecteur_n1[i] = (double)1/size_moins;
        vecteur_n2[i] = 0.0;
        //vecteur_FT[i] = 0;
        //printf("vecteur[%d] = %lf\n",i,vecteur_n1[i]);
    }
    for(i=0 ; i<size_moins ; i++)
    {
        vecteur_n1[i] = (double)1/size_moins;
        //printf("Vecteur[%d] = %lf \n",i,vecteur_n1[i]);
    }
    printf("\n");
    somme = 0.0;
    int verif = 0, iteration=0, t,tt,col=0;
    double sommevec=0.0;
    while(verif != 4)
    {
        for (i=0; i<size_moins;i++)
        {
            for(j=0;j<nblienTotal;j++)
            {
                if(matrice_H[1][j] == i+1)
                {
                    col = (int)matrice_H[2][j]-1;
                    vecteur_n2[col] = vecteur_n2[col] + (matrice_H[0][j] * vecteur_n1[i]);
                }
            }
        }
        for(i=0;i<size_moins;i++)
        {
            printf("vecteur_2[%d] = %lf \n",i,vecteur_n2[i]);
            vecteur_n1[i] = vecteur_n2[i];
            vecteur_n2[i] = 0;
        }
        if(verif == 0)
        {
        printf("ITERATION 2\n");
        }
        printf("\n");
        verif++;
    /*for(i=0; i<size_moins ; i++)
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

    if(sommevec<pow(10,-1))
    {
        verif = 1;
    }
    else
    {
        for(i=0; i<size_moins; i++)
        {
            vecteur_n1[i]= vecteur_n2[i];
            //printf("Iteration %d : vecteur1_modif[%d] = %lf \n",iteration, i, vecteur_n1[i]);
        }

    }
    sommevec = 0.0;
    iteration++;
    somme = 0.0;*/
    }
    /*for(i=0; i<size_moins; i++)
    {
        printf("vecteur_2[%d]=%lf \n",i,vecteur_n2[i]);
    }*/
    /*for(i=0; i<size_moins; i++)
    {
        printf("vecteur_2[%d]=%lf \n",i,vecteur_n2[i]);
    }*/
    return 0;
}
