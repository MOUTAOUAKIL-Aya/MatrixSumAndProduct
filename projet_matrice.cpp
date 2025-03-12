
#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>

using namespace std;

const int Max1 = 6;

struct Cellule {
    int Info;
    int Ind_i, Ind_j;
    Cellule* Suiv_L;
    Cellule* Suiv_C;
};

struct TMat {
    Cellule* TV[Max1];  
    Cellule* TH[Max1];  
    Cellule* THQ[Max1];
    Cellule* TVQ[Max1];
    int Nb_L, Nb_C;     
};

void initialiserM(TMat* M)
{
    for (int i = 0; i <M->Nb_L; i++) 
	{
        M->TH[i] = NULL;
        M->THQ[i] = NULL;
    }
      for (int j = 0; j <M->Nb_C; j++) 
	{
        M->TV[j] = NULL;
        M->TVQ[j] = NULL;
    }
}

void creerMatriceM(TMat*& M, const string& Name)
{
    ifstream fichier(Name.c_str(), ios::in); // Correction ifstream et conversion c_str()
    if (!fichier.is_open()) 
	{
        cerr << "Erreur d'ouverture du fichier " << endl;
       
    }

    int i, j, Nb_L, Nb_C;
    fichier >> Nb_L >> Nb_C;
    M = new TMat;
    M->Nb_C = Nb_C;
    M->Nb_L = Nb_L;
    initialiserM(M);

    for (i = 0; i < Nb_L; i++) {
        for (j = 0; j < Nb_C; j++) {
            int valeur;
            fichier >> valeur;
            if (valeur != 0) {
                Cellule* P = new Cellule;
                P->Info = valeur;
                P->Ind_i = i;
                P->Ind_j = j;
                P->Suiv_L = NULL;
                P->Suiv_C = NULL;

                // Mise à jour des listes horizontales
                if (M->TH[i] == NULL) {
                    M->TH[i] = P;
                    M->THQ[i] = P;
                } else {
                    M->THQ[i]->Suiv_L = P;
                    M->THQ[i] = P;
                }

                // Mise à jour des listes verticales
                if (M->TV[j] == NULL) {
                    M->TV[j] = P;
                    M->TVQ[j] = P;
                } else {
                    M->TVQ[j]->Suiv_C =P;
                    M->TVQ[j] = P;
                }
            }
        }
    }

    fichier.close();
}

void afficherM(TMat* M) 
{
    Cellule* Q;
    if (M != NULL)
	{
        for (int i = 0; i < M->Nb_L; i++) {
            Q = M->TH[i];
            for (int j = 0; j < M->Nb_C; j++) {
                if (Q != NULL && Q->Ind_j == j) {
                    cout << Q->Info << "  |  ";
                    Q = Q->Suiv_L;
                } else {
                    cout << 0 << "  |  ";
                }
            }
            cout << endl;
        }
    }
}


void libererMatriceM(TMat* M)
{
	int i , j ; 
	Cellule* P;
	Cellule* Q;
	for(i=0;i<M->Nb_L;i++)
	{
		P=M->TH[i];
		while (P!=NULL)
		{
			Q=P;
			P=P->Suiv_L;
			delete Q ; 
			P=Q->Suiv_L;
		}
		M->TH[i]=NULL;
		M->THQ[i]=NULL;
	}
	for(j=0;j<M->Nb_C;j++)
	{
		M->TV[j]=NULL;
		M->TVQ[j]=NULL;
		}
}


void AjouterFin(TMat* M3, int elem, int i, int j) 
{
    Cellule* P = new Cellule;
    P->Info = elem;
    P->Ind_j = j;
    P->Ind_i = i;
    P->Suiv_L = NULL;
    P->Suiv_C = NULL;
     // Ajout dans la liste horizontale (ligne i)
    if (M3->TV[j] == NULL) {
        M3->TV[j] = P;
        M3->TVQ[j] = P;
    } else 
	{
        M3->TVQ[j]->Suiv_C = P;
        M3->TVQ[j] = P;
    }

    if (M3->TH[i] == NULL) {
        M3->TH[i] = P;
        M3->THQ[i] = P;
    } else 
	{
        M3->THQ[i]->Suiv_L = P;
        M3->THQ[i] = P;
    }
}
void copier(TMat* M1, TMat*& M2) 
{
	
    M2 = new TMat;
    M2->Nb_C = M1->Nb_C;
    M2->Nb_L = M1->Nb_L;
    initialiserM(M2);

    Cellule* P;
    for (int i = 0; i < M1->Nb_L; i++)
	{
        P = M1->TH[i];
        for (int j = 0; j < M1->Nb_C; j++) 
		{
            if (P != NULL && P->Ind_j == j) {
                AjouterFin(M2, P->Info, P->Ind_i, P->Ind_j);
                P = P->Suiv_L;
            }
        }
    }
}



void Somme(TMat* M1, TMat* M2, TMat*& M3) 
{
    if (M1 == NULL && M2 == NULL) 
	{
        M3 = NULL;
      
    } 
    M3 = new TMat;
    M3->Nb_L = M1->Nb_L;
    M3->Nb_C = M1->Nb_C;
    initialiserM(M3);

    for (int i = 0; i < M3->Nb_C; i++) 
	{
        Cellule *P = M1->TV[i], *Q = M2->TV[i];
        while (P != NULL || Q != NULL) 
		{
            if ((Q == NULL) || (P != NULL && P->Ind_i < Q->Ind_i))
		   {
                AjouterFin(M3, P->Info, P->Ind_i, P->Ind_j);
                P = P->Suiv_C;
            } 
			else if ((P == NULL) || (Q != NULL && Q->Ind_i < P->Ind_i)) 
			{
                AjouterFin(M3, Q->Info, Q->Ind_i, Q->Ind_j);
                Q = Q->Suiv_C;
            } else 
			{
                int somme = P->Info + Q->Info;
                if (somme != 0) 
				{
                    AjouterFin(M3, somme, P->Ind_i, P->Ind_j);
               }
                P = P->Suiv_C;
                Q = Q->Suiv_C;
            }
        }
    }
}




//Produit 
///////////////////////////////////////////////////////////////////////////

void ProduitM(TMat *M1,TMat *M2,TMat *&M4)
{
	
	if ((M1 == NULL) || (M2 == NULL)) 
	{
        M4 = NULL;
       
    }
    else if (M1->Nb_C != M2->Nb_L)
    {
    	 M4 = NULL;
	}
	else 
	{
		  M4 = new TMat;
		  M4->Nb_C = M2->Nb_C;
          M4->Nb_L = M1->Nb_L;  
	    initialiserM(M4);
	    	Cellule *P , *Q;
          
	
	for(int i=0;i<M1->Nb_L;++i)
	{
		for(int j=0;j<M2->Nb_C;++j)
		{
				P = M1->TH[i]; 
                Q = M2->TV[j];
				int som = 0;
				
				if ((Q!=NULL) && (P != NULL))
				{
				while(Q != NULL || P != NULL)
				{
					if((P==NULL)  || ((Q!=NULL) && (Q->Ind_i < P->Ind_j)))
					{
					    Q= Q->Suiv_C;
					}
					else if((Q==NULL)  || ((P!=NULL) && (Q->Ind_i > P->Ind_j)))
					{
						P=P->Suiv_L;
					}
					else
					{
					    som =som + (P->Info * Q->Info);
						Q=Q->Suiv_C;
						P=P->Suiv_L;
					}
				}
		    	}
				if(som != 0)
				{
					AjouterFin(M4,som,i,j);
				}
				
			
		}
	
	}
	}		
	
}
	
	


void puissMatrice(TMat *M1,TMat *&res,int k )
{
	TMat *M2 = NULL;
	
	if(M1 == NULL || k<0 )
	{
		res= NULL;
	}
	else if (k == 0 && M1 != NULL)
	{
		int x =1;
		Cellule *P;
		res = new TMat();
	    initialiserM(res);
		for(int i=0;i<M1->Nb_L;++i)
		{		
			P = new Cellule();
			P->Info = x;
			P->Ind_i = i;
			P->Ind_j = i;
			P->Suiv_C = NULL;
			P->Suiv_L = NULL;
			res->TH[i] = P;
		    res->THQ[i] = P;
			res->TV[i] = P;
			res->TVQ[i] = P;
		}
		  	
	}
	else
	{
			res = new TMat();
			initialiserM(res);
			copier(M1,res);
			
			for(int i=2;i<=k;i++)
			{
				M2 = new TMat();
                initialiserM(M2);
				copier(res,M2);
				libererMatriceM(res);
				ProduitM(M1,M2,res);
				libererMatriceM(M2);
			
			}
	}
}




//Partie 4
///////////////////////////////////////////////////////////////////////////////////////////
// Q/°8

void Sigma_Puissance(TMat* M1, int n, TMat*& S) 
{
    TMat* Z =  NULL;
	TMat* prod= NULL;  // Pour stocker les puissances successives de M1
    
    // Initialiser S comme une matrice nulle
    S = new TMat();
    S->Nb_L = M1->Nb_L;
    S->Nb_C = M1->Nb_C;
    initialiserM(S);
    
    // Copier M1 dans Z pour commencer le calcul de M1^1
    Z = new TMat();
    Z->Nb_L = M1->Nb_L;
    Z->Nb_C = M1->Nb_C;
    initialiserM(Z);
    copier(M1, Z);  // Copie de M1 dans Z pour commencer avec M1^1
    
    // Ajouter M1^1 à S
    Somme(S, Z, S);  // S = S + M1^1
    // Pour chaque k de 2 à n, calculer M1^k et l'ajouter à S
    for (int i = 2; i <= n; ++i)
	{
        prod = new TMat();	
        initialiserM(prod);
        // Calculer M1^k (produit de M1^(k-1) avec M1)
        ProduitM(M1, Z, prod);  // Z = M1^(k-1) * M1 -> Z = M1^k  
        
        
        copier(Z,prod);  // Copier M1^k dans Z pour la prochaine itération
        libererMatriceM(prod);
         
        // Ajouter M1^k à S
        Somme(S, Z, S);  // S = S + M1^k
        
    }
    
}

//////////////////////////////////////////////////////////////////////////////////////////

Menu()
{
	cout<<"\t\t\t\t\t*Matrice Creuse*\n\n";
	system("color f4");
	cout<<"*Menu\n\n 1-Afficher M\n 2-Afficher M3(La somme)\n 3-Afficher M4(Le produit)\n 4-Afficher res(la puissance)\n 5-Afficher S(La somme de puissance)\n\n |Saisir Votre choix  | :";
}	

int main() {
	
	
	
    int choix;
	TMat* M = NULL;
    TMat* M1 = NULL;
    TMat* M2 = NULL;
    TMat* M3 = NULL; 
	TMat* M4 = NULL; // Déclaration de M3 pour la somme
    TMat* res = NULL;
    TMat* S = NULL;
    
    creerMatriceM(M,"fichier.txt");
    //ibererMatriceM(M);
    creerMatriceM(M1,"fiche2.txt");
	creerMatriceM(M2,"fiche3.txt");
	Somme(M1,M2,M3);		
	creerMatriceM(M1,"fiche2.txt");
	creerMatriceM(M2,"fiche3.txt");
    ProduitM(M1,M2,M4);	
	creerMatriceM(M1,"fiche2.txt");     
    puissMatrice(M1,res,2 );
    creerMatriceM(M1,"fichier.txt");   
    Sigma_Puissance(M,3,S);
    
	do{
	
		Menu();
		cin>>choix;
		switch(choix)
		{
		
			case 1:     
			            cout << "\nAffichage de la matrice M :\n";
   					    afficherM(M);
   			break;
   			case 2:  	         
                      cout << "\nAfficher la matrice M3 (Somme de M1 et M2) :\n";  // Option pour afficher M3
                      afficherM(M3);
                     
            break;
            case 3: 
      			     cout << "\nAfficher la matrice M4 (Produit de M1 et M2) :\n";  // Option pour afficher M3
      			     afficherM(M4);
      		break;
      	    case 4 :   cout << "\nAfficher la matrice res (Puissance de M1) :\n";  
					   afficherM(res);		   
		   break;
		  case 5 :    cout << "\nAfficher la matrice S (La somme du Puissance de M1) :\n";  		   
			          afficherM(S);
					   
		   break;
        } 
        
		getch();
	    system("cls");
		 
	}while(choix!= 6);	
	
    return 0;
}




