#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "final_fonctions.h"
#include "final_menus.h"


int logged_in(int c_choix1, int i_board[9][9])
{
    char c_choix3 = '1';
    int i_player = noir;
    do {
			switch (c_choix1)
			{
				case '0': //revenir au log in
				{
					do
					{
						printf("\nEtes-vous s\226r de vouloir vous d\202connecter?\n");
						printf("0.Oui			1.Non\n");
						getchar();
						c_choix3 = getchar();
                    } while (c_choix3 != '1' && c_choix3 != '0');

					if(c_choix3=='0')
					{
						return 1;
					}

					break;
                }

				case '1': //lancer a saved game
				{
					printf("RAPPEL!!!\n\nVous pouvez toujours:\n\n");
					menu33();  //menu pause
					printf("en milieu de partie en saisissant \"PAUSE\".\n");
					getchar();
					getchar();

					g_i_save = lancerPartiesauv(i_board);
					if (g_i_save)
					{
						if (g_c_choix_jeu == '1')
						{
							if(g_i_tour%2==1)
							{
								i_player= blanc;
							}
							startGame(i_player, i_board, g_i_BLACK, g_i_WHITE, g_s_nom1, g_s_nom2);
							getchar();
						}
						else if(g_c_choix_jeu == '2')
						{
							if(g_i_tour%2==0)
							{
								i_player= blanc;
							}
							startGame(i_player, i_board, g_i_BLACK, g_i_WHITE, g_s_nom1, g_s_ai);
						}

					}

					else
					{
						getchar();
						getchar();
					}

					g_i_save=0;
					break;
				}
				case '2': // NEW GAME
				{
					g_i_tour=0;
					if (c_choix1 == '2')
					{
						printf("RAPPEL!!!\n\nVous pouvez toujours:\n\n");
						menu33();  	//menu pause
						printf("en milieu de partie en saisissant \"PAUSE\".\n");
						getchar();
						getchar();
						init_board(i_board);
						do {
							menu2();
							scanf("%c", &g_c_choix_jeu);
						} while (g_c_choix_jeu != '1' && g_c_choix_jeu != '2');

						if (g_c_choix_jeu == '1')
						{
							printf("\nDonner le nickname du 1er joueur:   ");
							scanf("%s", g_s_nom1);

							printf("\nDonner le nickname du 2\212me joueur:   ");
							scanf("%s", g_s_nom2);

							startGame(i_player, i_board, g_i_BLACK, g_i_WHITE, g_s_nom1, g_s_nom2);
						}

						else if (g_c_choix_jeu == '2')
						{
							do {
								menu22();
								scanf("%c", &g_c_mode);
							} while (g_c_mode!= '1' && g_c_mode != '2'&& g_c_mode != '3');

							printf("\nSaisissez un nickname:   ");
							scanf("%s", g_s_nom1);

							startGame(i_player, i_board, g_i_BLACK, g_i_WHITE, g_s_nom1, g_s_ai);
						}
					}

					printf("\n>>>>>>>>>> GAME OVER <<<<<<<<<<\n");
					getchar();
					break;
				}

				case '3':
				{
					listeMeilleurScores();
					getchar();
					getchar();
					break;
				}


				default:
				{
					break;
				}
			}

			menu1(); //menu principal
			c_choix1 = getchar();
		} while (1);
}

