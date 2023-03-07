#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "final_logged_in.h"


/*
##########################    ATTENTION!    #####################################
->Le nom des variables commence par leur type (i:int, s:string:, c:char...).
->Pour les variables globales le nom commence d'abord par "g" suivi de leur type.
#################################################################################
*/


//char g_s_nom1[20]="Player 1";
//char g_s_nom2[20]="Player 2";
//char g_s_ai[20] = "Fael2.0";


int main(int argc, char* argv[])
{
	srand(time(NULL));
	int i_quit=0; //pour revenir au menu de log in/ sign up;
	int i_log=1;
	char c_choix = '1', c_choix1, c_choix2;
	int i_board[9][9];
	init_board(i_board);
	while (1)
	{
		menu();
		scanf("%c", &c_choix);

		switch (c_choix)
		{
			case '0': //quitter
			{
				printf("Etes-vous s\226r de vouloir quitter?\n");
				printf("0.Oui			1.Non\n");
				getchar();
				c_choix2 = getchar();
				switch (c_choix2)
				{
					case '0':
					{
						return 0;
					}
					case '1':
					default:
					{
						break;
					}
				}
				break;
			}

			case '1': //login
			{
				i_log = login();
				while (i_log==0)
				{
					printf("\nVous avez saisie le mauvais identifiant ou votre compte n'existe pas.\n");
					printf("\nSaisir '0' pour revenir au menu principal ou n'importe quelle autre touche pour corriger votre identifiant.\n");
					getchar();
					g_i_erreur = getchar();
					if (g_i_erreur == '0')
					{
						break;
					}
					i_log = login();
				}

				if (g_i_erreur == '0')
				{
					break;
				}

			}

			case '2': //sign in
			{
				if (c_choix == '2')
				{
				    int signed_up= signUp();
				    if(!signed_up) //sign up failed
                    {
                        getchar();
                        getchar();
                        break;
                    }

				}

				menu1();
				getchar();
				c_choix1 = getchar();

				i_quit=logged_in(c_choix1,i_board);
				if(i_quit!=0)
                {
                    break;
                }

				break;
			}

			default:
			{
				break;
			}
		}

	}
	return 0;
}

