#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define tailleMax 37000
#define sommetSuppSizeMax 200
int  sommetSupp[sommetSuppSizeMax],sommetSuppSize=0,temp,temp2,S,R,i=0,j=0,cpt,size,cptNbLien=0,y, size_moins,sizeApresModif,nblienTotal,nblienTotalApresModif, nbLien,nbLienFinal, nbColonne=0, colonneNotProba=5,indice=0, *listeLigne = NULL, *listeLigne_Modif = NULL; //Si colonneNotProba = 1 --> c'est une colonne sinon c'est une proba
int *vecteur_FT, *vecteur_FT_Modif;
char  *caractere;
double somme=0.0,proba, alpha = 0.85, *vecteur_n1, *vecteur_n2, *vecteur_n1_modif,*vecteur_n2_modif, sommeProbaSommetDisparu,tempDouble;
double **matrice_H = NULL, **matrice_H_Modif = NULL;
int verif = 0, iteration=0, t,tt;
double sommevec=0.0;
FILE* fichier=NULL;
char sentence[100000] ;
char *secondword, *word;
int col=0,ligne=0;
int main()
{
    //Stanford || wb-cs-stanford || in-2004v2 || wikipedia-20051105V2 || wb-edu --> OK
    //Stanford_BerkeleyV2 || --> NON
    word = strtok(sentence," ");
    cpt=0;
    char chaine[tailleMax] = "";
    fichier = fopen("wb-cs-stanford.txt","r");
    fgets(chaine, tailleMax, fichier); // recup taille
    size = 1 + (int)atof(chaine);
    size_moins = size - 1;
    fgets(chaine, tailleMax, fichier); // recup nblien
    nblienTotal = (int)atof(chaine);
    lectureMatrice();
    initDebutVecteurN1N2();
    verifConvergence();
    // DEBUT PROJET
    //printf("ff %lf %lf %lf\nsize moins = %d \n", matrice_H[1][0],matrice_H[2][0],matrice_H[0][0],size_moins);
    // Initialisation du tableau des sommets supprimé a -1 partout
    for(i=0;i<sommetSuppSizeMax;i++)
    {
        sommetSupp[i] = -1;
    }
    //ALGO 1
    /*algoAddAndSuppSommet(3,5); // puisque les nouveau sommets ne pointent pas sur des pages 'proba 0 donc multiplication deja a nulle' donc le nombre d'iteration varie en fonction du nombre de sommets supprimer si on supprime un petit nombre il devra recalculer beaucoup pour converger et plus on supprime plus il met moins de temps a converger
    for(i=0;i<sommetSuppSizeMax;i++)
    {
        if(sommetSupp[i] >=0)
        {
            sommetSuppSize++;
        }
    }
    verifConvergenceAddSuppSommet();*/

    // ALGO 2
    //algoAddSommetSeulement(5,5);
    //free(matrice_H);
    //verifConvergenceAddSommetSeulement(); // l'algo boucle a l'infini je ne sais pas pourquoi, pourtant la nouvelle matrice est stoquastique et on normalise le vecteur a chaque fois

    //ALGO 3
    // Remarque quand on supprime des sommets il faut en rajouter sinon ça boucle a l'infini et ce car le vecteur n'est pas en norme 1 "Car la formule de l'ennoncé n'est pas appliqué"
    // Remarque 2 : on ne peut pas supprimer plus de sommets que ceux de l'etat initial ex: 8 pour web1.txt
    // Remarque 3 : si on ajoute des sommets seulement on converge en 1 iteration car les valeurs des nouveau sommets dans le vecteur sera de 0 "Car pas de sommets disparu" donc en une muliplication on reconverge "Vu que des 0 et le reste c'est les valeur avant le modification"
    // Remarque 4 : Si vraiment on ajoute un nombre de liens trés superieur au nombre de sommets ajouter (genre 10 000 liens pour 500) la convergence va etre plus lente que si on fait un nombre de lien proche du nombre de sommets ajoutés (il aura plus du mal a converger rapidement car les valeurs du vecteur x et des liens serons de plus en plus petite
    /*for(i=0;i<size_moins;i++)
    {
        printf("Vecteur[%d] = %lf - %lf\n",i,vecteur_n2[i],vecteur_n1[i]);
    }*/
    // Remarque 5 : Si on supprime tout les sommets et on cherche a rajouter des liens on est obligé d'ajouter de nouveau sommets car sinon BUG (le lien d'autra pas de source ni de cible)
    algoAddAndSuppSommetAvecLiensProbaUnSurNblienDuSommet(0,0,0);
    //Plus on supprime de lien plus ça converge vite "Qlq soit le nombre de sommet ou de liens ajouter car la priorité est l'initialisation du vecteur <Formule enoncé>"
    //Aprés si on ajoute des liens seulement --> plus on ajoute de lien plus il converge plus vite car les proba serons encore plus petite --> multiplication sera plus petite
    //Quand on supprime pas de sommet le nombre de sommets ajouter ne va pas vraiment influencer la convergence car les nouveau sommet auront une valeur de 0 dans le vecteur X et donc seulement la petite proba des liens va influencer la convergence
    //initDebutVecteurN1N2();
    verifConvergenceAddSuppSommet();
    //verifConvergence();
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

void algoAddAndSuppSommetAvecLiensProbaUnSurNblienDuSommet(int nombreSommetSupp, int nombreSommetAjoute, int nombreLiensAjoute)
{
    //On ajoute des liens pour les nouveau sommets et on modifie pas les lien de l'ancien graphe (mais on peut changer leurs valeur ca depend des nouveau lien --> car on remmet la somme de la ligne a 1) on supp suelement les liens des sommets disparu (lien depuis le sommet disparu ou bien vers le sommet disparu)
    // On change le vecteur x
    S = nombreSommetAjoute;
    R = nombreSommetSupp;
    nblienTotalApresModif = nblienTotal + nombreLiensAjoute ;
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
    vecteur_FT_Modif = (int *) malloc(sizeof(int)*sizeApresModif);
    for(i=0; i<size_moins ; i++)
    {
        vecteur_n1_modif[i] = vecteur_n1[i];
        vecteur_n2_modif[i] = vecteur_n2[i];
        vecteur_FT_Modif[i] = vecteur_FT[i];
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
        vecteur_FT_Modif[j] = 1;
    }
    // Modification du vecteur et supp des ancien sommets et ajout des nouveau avec init
    // Je decide aleatoirement quels sommets vont etre supprimer
    i=0;
    sommeProbaSommetDisparu = 0.0;
    while(i<R)
    {
        temp = randNumber(1,size_moins)-1;
        if(vecteur_n1_modif[temp] != -1)
        {
            //Si le sommet supprimé est B et on a A qui pointe sur B --> On verifie que le sommet A n'a pas que ce lien "si c'est le cas alors vecteurFT_Modif[A] = 1";
            sommeProbaSommetDisparu = sommeProbaSommetDisparu + vecteur_n1_modif[temp];
            vecteur_n1_modif[temp] = -1;
            vecteur_n2_modif[temp] = -1;
            vecteur_FT_Modif[temp] = -1; // On notifi dans le vecteur que le sommet a etait supp
            // On supprime tout les liens qui ont temp comme source ou destination
            //printf("I = %d\n",i);
            //printf("Sommet supp = %d\n", temp);
            //sommetSupp[i] = temp; // on stock tout les sommets supprimer dans un tableau
            i++;
            //printf("Sommet supp  = %d\n", temp);
        }
    }
    t=0;
    // Init de la proba des nouveau sommets pour le vecteur
    for(j=size_moins; j<sizeApresModif; j++)
    {
        //initialisation du nouveau vecteur
        vecteur_n1_modif[j] = sommeProbaSommetDisparu/nombreSommetAjoute;
        vecteur_n2_modif[j] = 0.0;
    }

    //En d'autres terme on supprime des liens des les sommets qui ont disparu (On supprime le lien si ce lien la pointe sur un sommet disparu, ou si ce lien la a pour sommet de depart un lien disparu)
    for(i=0;i<size_moins;i++)
    {
        if(vecteur_n1_modif[i]<0)
        {
            for(j=0;j<nblienTotal;j++)
            {
                if((matrice_H_Modif[1][j] == i+1) || (matrice_H_Modif[2][j] == i+1))
                {
                    matrice_H_Modif[0][j] = -1;
                    matrice_H_Modif[2][j] = -1;
                    matrice_H_Modif[1][j] = -1;
                }
            }
        }
    }
    i=nblienTotal;
    cpt=0;
    while(i<nblienTotalApresModif)
    {
        temp = randNumber(1,sizeApresModif)-1;
        temp2 = randNumber(1,sizeApresModif)-1;
        if((vecteur_n1_modif[temp] != -1) && (vecteur_n1_modif[temp2] != -1))
        {
            vecteur_FT_Modif[temp] = 0;
            printf("temp1 = %d \t temp2= %d\n",temp,temp2);
            cpt=0;
            for(j=0;j<nblienTotalApresModif;j++)
            {
                if((matrice_H_Modif[1][j] == temp))
                {
                    cpt++;
                }
            }
            for(j=0;j<nblienTotalApresModif;j++)
            {
                if(matrice_H_Modif[1][j] == temp)
                {
                    tempDouble = (double)matrice_H_Modif[0][j]/cpt;
                    matrice_H_Modif[0][j] =(double) matrice_H_Modif[0][j]-tempDouble;
                }
            }
            matrice_H_Modif[1][i] = temp;
            matrice_H_Modif[2][i] = temp2;
            if(cpt != 0)
            {
                matrice_H_Modif[0][i] = (double)1/cpt;
            }
            else
            {
                matrice_H_Modif[0][i] = 1;
            }
            cpt=0;
            i++;
        }
    }
    // FAUT VERIFIER PENDANT LA MULTI DE LA CONVERGENCE QUE matrice_modif[0][i] != 0; car y'a des elements nul ici ;
    /*for(i=0; i<sizeApresModif; i++ )
    {
        printf("Vecteur_n1_modif[%d] = %lf || vecteur_FT[%d] = %d \n",i,vecteur_n1_modif[i],i,vecteur_FT_Modif[i]);
    }*/
    /*for(i=0; i<nblienTotalApresModif; i++ )
    {
        printf("MatriceModfi[1][%d] = %lf || MatriceModfi[2][%d] = %lf || MatriceModfi[0][%d] = %lf\n",i,matrice_H_Modif[1][i],i,matrice_H_Modif[2][i],i,matrice_H_Modif[0][i]);
    }*/

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
    vecteur_FT = (int *) malloc(sizeof(int)*size_moins);
    int nbLigne = 0;
    while(fgets(sentence,tailleMax, fichier) != NULL) // parcours fichier ligne par ligne
        {
            do
            {
                if(cpt == 0) //si on est en debut de ligne
                    {
                    word = strtok(sentence," ");
                    nbLigne = (int) atof(word);

                    secondword = strtok(NULL," ");
                    nbLien = (int) atof(secondword);
                    //printf("Ligne  = %d \t nbLien = %d \n", nbLigne, nbLien);
                    if(nbLien != 0)
                    {
                        //listeLigne[j] = indice;
                        vecteur_FT[nbLigne - 1] = (int)0;
                    }
                    else
                    {
                        //listeLigne[j] = -1;
                        vecteur_FT[nbLigne - 1] = (int)1;
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
    /*for(i=0 ; i<size_moins; i++)
    {
        printf("Vecteur_FT[%d] = %d \n", i, vecteur_FT[i]);
    }*/
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
                vecteur_n2[col] = (vecteur_n2[col] + (matrice_H[0][j] * vecteur_n1[ligne]));
                //printf("ligne = %d \t matriceModif[1][j] = %lf\n", ligne, matrice_H[1][j]);
            }
        /*for(i=0;i<size_moins;i++)
        {
            printf("vecteur_2[%d] = %lf \n",i,vecteur_n2[i]);
            vecteur_n1[i] = vecteur_n2[i];
            vecteur_n2[i] = 0;
        }*/
        //printf("\n");
        //verif++;
        // Cas a faire sinon
        /*for(j=0;j<size_moins;j++)
        {
            vecteur_n2[j] = vecteur_n2[j] * 0.99;
        }*/
        // On Calcul BETA
        for(i=0;i<size_moins; i++)
        {
            if(vecteur_FT[i] == 1)
            {
                somme = somme + vecteur_n1[i];
            }
        }
        //Construction matrice G
        for(j=0;j<size_moins;j++)
        {
            vecteur_n2[j] = vecteur_n2[j] * alpha + ((1-alpha)/size_moins) + (alpha*(somme/size_moins));
        }
        somme = 0.0;
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
    if(sommevec<pow(10,-9))
    {
        verif = 1;
        /*for(t=0;t<size_moins;t++)
        {
            printf("VECTEUR_n1_FINAL[%d] = %0.10lf\n",t,vecteur_n2[t]);
        }*/
        printf("SOMME VEC FIN = %0.10lf\n", sommevec);
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
    /*for(i=0;i<size_moins;i++)
    {
        printf("vecteur[%d] = %lf\n",i,vecteur_n2[i]);
    }*/
    printf("Nb iteration = %d\n", iteration);
}
void verifConvergenceAddSuppSommet()
{


    printf("\n");
    sommevec=0;
    iteration = 0;
    somme = 0.0;
    verif = 0;
    /*for(i=0;i<sizeApresModif;i++)
    {
        /*if(vecteur_n1_modif[i]<0)
        {
            vecteur_n2_modif[i] = vecteur_n1_modif[i];
        }
        printf("Vecteur_n1[%d] = %0.10lf\t vecteur_n2[%d] = %0.10lf\n", i,vecteur_n1[i],i,vecteur_n2[i]);
    }*/
    printf("\n");
    somme = 0.0;
    for(i=0;i<sizeApresModif;i++)
    {
        printf("vect 1 = %0.10lf ||vect 2 = %0.10lf\n", vecteur_n1_modif[i], vecteur_n2[i]);
        if(vecteur_n1_modif[i]<0)
        {
            vecteur_n2_modif[i] = vecteur_n1_modif[i];
        }
        else
        {
            vecteur_n2_modif[i] = 0;
        }

    }
    for(i=0;i<sizeApresModif;i++)
    {
        printf("vect 2 = %0.10lf\t vecteurFT[%d] = %d\n", vecteur_n2_modif[i],i,vecteur_FT_Modif[i]);
    }
    for(i=0; i<nblienTotalApresModif; i++ )
    {
        printf("MatriceModfi[1][%d] = %lf || MatriceModfi[2][%d] = %lf || MatriceModfi[0][%d] = %lf\n",i,matrice_H_Modif[1][i],i,matrice_H_Modif[2][i],i,matrice_H_Modif[0][i]);
    }
    while(verif == 0)
    {
        for(j=0;j<nblienTotalApresModif;j++)
            {
                if(matrice_H_Modif[0][j]>=0)
                {
                    ligne = (int)matrice_H_Modif[1][j]-1;
                    col = (int)matrice_H_Modif[2][j]-1;
                    vecteur_n2_modif[col] = (vecteur_n2_modif[col] + (matrice_H_Modif[0][j] * vecteur_n1_modif[ligne]));
                }

                //printf("ligne = %d \t matriceModif[1][j] = %lf\n", ligne, matrice_H[1][j]);
            }
        /*for(i=0;i<size_moins;i++)
        {
            printf("vecteur_2[%d] = %lf \n",i,vecteur_n2[i]);
            vecteur_n1[i] = vecteur_n2[i];
            vecteur_n2[i] = 0;
        }*/
        //printf("\n");
        //verif++;
        // Cas a faire sinon
        /*for(j=0;j<size_moins;j++)
        {
            vecteur_n2[j] = vecteur_n2[j] * 0.99;
        }*/
        // On Calcul BETA
        for(i=0;i<sizeApresModif; i++)
        {
            if(vecteur_FT_Modif[i] == 1)
            {
                somme = somme + vecteur_n1_modif[i];
            }
        }
        //Construction matrice G
        for(j=0;j<sizeApresModif;j++)
        {
            vecteur_n2_modif[j] = vecteur_n2_modif[j] * alpha + ((1-alpha)/sizeApresModif) + (alpha*(somme/sizeApresModif));
        }
        somme = 0.0;
    for(i=0; i<sizeApresModif ; i++)
    {
        somme = vecteur_n2_modif[i] - vecteur_n1_modif[i];
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
    if(sommevec<pow(10,-9))
    {
        verif = 1;
        /*for(t=0;t<size_moins;t++)
        {
            printf("VECTEUR_n1_FINAL[%d] = %0.10lf\n",t,vecteur_n2[t]);
        }*/
        printf("SOMME VEC FIN = %0.10lf\n", sommevec);
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
    sommevec = 0.0;
    iteration++;
    somme = 0.0;
    }
    /*for(i=0; i<size_moins; i++)
    {
        printf("vecteur_2[%d]=%lf \n",i,vecteur_n2[i]);
    }*/
    /*for(i=0;i<size_moins;i++)
    {
        printf("vecteur[%d] = %lf\n",i,vecteur_n2[i]);
    }*/
    printf("Nb iteration = %d\n", iteration);
}
