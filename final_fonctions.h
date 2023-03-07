#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <math.h>

#include "final_ai_mode_hard.h"


#define vide 0  //cases que peut choisir le joueur
#define noir 1
#define blanc 2
#define interdit 3 //cases interdites au joueur
#define Unfilled 60  //nombre de cases à remlplir
#define True 1
#define False 0


char g_i_erreur = '1';

struct _couple {
	char x[6];
	char y[6];
}cple;

char g_s_nom1[20]="Player 1";
char g_s_nom2[20]="Player 2";
char g_s_ai[20] = "Fael2.0";

void recommencer(int i_board[9][9])
{
    g_i_tour = 0;
	init_board(i_board);
	int i;
	for (i = 0; i < 61; i++)
	{
		g_i_BLACK[i][0] = 0; g_i_WHITE[i][0] = 0;
		g_i_BLACK[i][1] = 0; g_i_WHITE[i][1] = 0;
	}
}
//////////////////////////////////////////////////////////////

void ajouterScore(int i_score, char s_nom[20])
{
	FILE* pf;
	pf = fopen("fichierdesscores.txt", "a");
	fprintf(pf, "%s\t\t%d\n", s_nom, i_score);
	fclose(pf);
}

int signUp()
{
	char s_name[20];
	char s_varname[20];
	char s_varpassword[50];
	char s_password[50];
	int i_ver = 0;
	printf("saisir un nom.\n");
	scanf("%s", s_name);
	FILE* pf;
	pf = fopen("fichierlogin.txt", "a+");

	if (pf == NULL)
	{
		printf("\nPROBLEME D'OUVERTURE!\n");
		exit(-1);
	}

	while (!feof(pf) && i_ver == 0)                             //on verifie si le nom existe deja
	{
		fscanf(pf, "%s", s_varname);
		fscanf(pf, "%s", s_varpassword);
		if (strcmp(s_name, s_varname) == 0) {
			i_ver = 1;
		}
	}
	if (i_ver == 0) {
		printf("saisir un mot de passe.\n");
		scanf("%s", s_password);
		fprintf(pf, "%s\n%s\n", s_name, s_password);

		fclose(pf);
		return 1; //sign up complete
	}
	else
	{
		printf("\nCe compte existe d\212j\205. Veuillez saisir un autre nom ou vous connecter au compte en question.\n");

		return 0; //sign up failed
	}

}

int login()
{
	char s_name[20];
	char s_password[50];
	char s_varname[20];
	char s_varpassword[50];
	printf("Saisir le nom.\n");
	scanf("%s", s_name);
	printf("saisir le mot de passe.\n");
	scanf("%s", s_password);
	FILE* pf;
	pf = fopen("fichierlogin.txt", "r");

	if (pf == NULL)
	{
		printf("\nPROBLEME D'OUVERTURE!\n");
		return 0;
	}

	while (!feof(pf))   //on verifie si les infos saisis sont correctes
	{
		fscanf(pf, "%s", s_varname);
		fscanf(pf, "%s", s_varpassword);
		if (strcmp(s_name, s_varname) == 0 && strcmp(s_password, s_varpassword) == 0)
		{
			return 1;
		}
	}
	return 0;
	fclose(pf);
}

int check_in_file(char s_name[20], FILE*pf) //vérifier l'unicité d'un nom
{
	char s_varname[20];
	if (pf == NULL)
	{
		printf("\nPROBLEME D'OUVERTURE! Le fichier n'existe pas.\n");
		getchar();
		exit(-1);
	}

	while (!feof(pf))
	{
		fscanf(pf, "%s", s_varname);
		if (strcmp(s_varname, s_name) == 0)
		{
			return 1;
		}
	}
	return 0; // name not found in the file
}

void create_file(FILE* pf, char* FICHIER)
{
    pf = fopen(FICHIER, "a");
	if (pf == NULL)
	{
		printf("\nPROBLEME D'OUVERTURE!\n");
		getchar();
		exit(-1);
	}
	fclose(pf);
}
/*La fonction create_file c'est juste por créer le fichier au cas où il n'existe pas vue
que qlq modes de lecture (par r+) nécessite que le fichier en question existe déjà*/

void save_last_game(int i_board[9][9]) //to keep the data of the last saved game
{
	int i, j;
	FILE*pflast=NULL;

	create_file(pflast,"fichierlastgame.txt"); //keep the last saved game's data here

	pflast = fopen("fichierlastgame.txt", "w");

	if (pflast==NULL)
	{
		printf("\nPROBLEME D'OUVERTURE!\n");
		getchar();
		exit(-1);
	}

	for (i = 1; i < 9; i++)
	{
		for (j = 1; j < 9; j++)
		{
			fprintf(pflast, "%d ", i_board[i][j]);
		}
	}
	fprintf(pflast, " %d ", g_i_tour-1);
	fprintf(pflast, "%d ", g_c_choix_jeu);
	fprintf(pflast, "%d  ", g_c_mode);

	for (i = 0; i < Unfilled; i++)
	{
		fprintf(pflast, "%d ", g_i_BLACK[i][0]);
		fprintf(pflast, "%d ", g_i_BLACK[i][1]);
	}
	for (i = 0; i < Unfilled; i++)
	{
		fprintf(pflast, "%d ", g_i_WHITE[i][0]);
		fprintf(pflast, "%d ", g_i_WHITE[i][1]);
	}
	fprintf(pflast, "%s ", g_s_nom1);
	fprintf(pflast, "%s ", g_s_nom2);
	fprintf(pflast, "\n\n");
	fclose(pflast);
}

void sauvegarder(int i_board[9][9])
{
	char s_namepartie[20];
	int i, j, i_check;
	FILE* pf=NULL;

	save_last_game(i_board);

	create_file(pf,"fichiersaving.txt"); //save all games
	pf = fopen("fichiersaving.txt", "r+");

	if (pf == NULL)
	{
		printf("\nPROBLEME D'OUVERTURE!\n");
		getchar();
		exit(-1);
	}

	do{
		printf("\nNommer cette partie pour la sauvegarder:   ");
		scanf("%s", s_namepartie);
		i_check=check_in_file(s_namepartie,pf);
		if(i_check)
		{
			printf("Le nom exite d\212j\205. Veuillez changer de nom.\n");
		}
	}while(i_check);  // to make sure that every saved game has a unique name

	fprintf(pf, "%s\n", s_namepartie);
	for (i = 1; i < 9; i++)
	{
		for (j = 1; j < 9; j++)
		{
			fprintf(pf, "%d ", i_board[i][j]);
		}
	}
	fprintf(pf, " %d ", g_i_tour-1);
	fprintf(pf, "%d ", g_c_choix_jeu);
	fprintf(pf, "%d  ", g_c_mode);

	for (i = 0; i < Unfilled; i++)
	{
		fprintf(pf, "%d ", g_i_BLACK[i][0]);
		fprintf(pf, "%d ", g_i_BLACK[i][1]);
	}
	for (i = 0; i < Unfilled; i++)
	{
		fprintf(pf, "%d ", g_i_WHITE[i][0]);
		fprintf(pf, "%d ", g_i_WHITE[i][1]);
	}
	fprintf(pf, "%s ", g_s_nom1);
	fprintf(pf, "%s ", g_s_nom2);
	fprintf(pf, "\n\n");
	fclose(pf);
}


int lancerPartiesauv(int i_board[9][9])
{
	int i, j, verify=1;
	char s_n1[20], s_n2[20], c_choix='0';
	FILE* pf=NULL;
	char s_namepartie[20]="1";
	char s_varname[20]="";

	do{
            printf("Voulez-vous:\n\n");
            printf("1. Relancer la derni\212re partie sauvegard\202e?\n");
            printf("2. Relancer une partie sp\202cifique?\n");
            scanf("%c", &c_choix);
	}while(c_choix!='1' && c_choix!='2');

	if(c_choix=='1')
    {
        pf = fopen("fichierlastgame.txt", "r");
        if (pf == NULL)
        {
            printf("\nPROBLEME D'OUVERTURE! Le fichier n'existe pas.\n");
            return 0;
        }
    }

    else
    {
        pf = fopen("fichiersaving.txt", "r");
        if (pf == NULL)
        {
            printf("\nPROBLEME D'OUVERTURE! Le fichier n'existe pas.\n");
            return 0;
        }

         printf("\nEntrez le nom de votre partie:   ");
         scanf("%s", s_namepartie);
    }


	while (!feof(pf))
	{
		if(c_choix=='2')
        {
            fscanf(pf, "%s", s_varname);
            verify=strcmp(s_varname, s_namepartie);
        }

		if ( verify== 0 || c_choix=='1')
		{
			for (i = 1; i < 9; i++)
			{
				for (j = 1; j < 9; j++)
				{
					fscanf(pf, "%d", &(i_board[i][j]));
				}
			}

			fscanf(pf, "%d", &g_i_tour);
			fscanf(pf, "%d", &g_c_choix_jeu);
			fscanf(pf, "%d", &g_c_mode);

			for (i = 0; i < Unfilled; i++)
			{
				fscanf(pf, "%d", &(g_i_BLACK[i][0]));
				fscanf(pf, "%d", &(g_i_BLACK[i][1]));
			}
			for (i = 0; i < Unfilled; i++)
			{
				fscanf(pf, "%d", &(g_i_WHITE[i][0]));
				fscanf(pf, "%d", &(g_i_WHITE[i][1]));
			}
			fscanf(pf, "%s", s_n1);
			fscanf(pf, "%s", s_n2);
			strcpy(g_s_nom1,s_n1);
			strcpy(g_s_nom2,s_n2);

			if(verify == 0)
            {
                fclose(pf);
                printf("\nVotre partie a \202t\202 charg\202 correctement.\n");
                getchar();
                return 1;
            }
		}
	}
	if(feof(pf) && c_choix=='1')
    {
        fclose(pf);
        printf("\nVotre partie a \202t\202 charg\202 correctement.\n");
        getchar();
        return 1;
    }

	fclose(pf);
	printf("\nLe nom que vous avez saisit n'existe pas.\n");
	printf("Veuillez ressaisir le nom correctement ou commencer une nouvelle partie.\n");
	return 0;
}


void afficherhistorique() {
	int i;
	int *i_turn=&g_i_tour;

	printf("l'historique des mouvement est :\n");
	printf("Noir\t\tBlanc\n");
	printf("(x,y)\t\t(x,y)\n");
	for (i = 1; i < *i_turn; i++)
	{
		printf("(%d,%d)\t\t(%d,%d)\n", g_i_BLACK[i][0], g_i_BLACK[i][1], g_i_WHITE[i][0], g_i_WHITE[i][1]);
	}
}

typedef struct {
	char name[20];
	int score;
}couple;

int listeMeilleurScores()
{
	couple tab[100];

	FILE* pf;
	int verf = 0;
	int i = 0;

	pf = fopen("fichierdesscores.txt", "r");

	if (pf == NULL)
	{
		printf("\nProbl\212me d'ouverture. Aucun score n'a encore \202t\202 sauvegard\202.\n");
		return 0;
	}

	while (!feof(pf) &&i<100)      //on insert les valeurs du fichier dans notre tableau
	{
		if (verf == 0) {
			fscanf(pf, "%s", tab[i].name);
			verf = 1;
		}
		else {
			fscanf(pf, "%d", &(tab[i].score));
			verf = 0;
			i++;
		}
	}

	int j, k;
	couple c;
	for (j = 0; j < i - 1; j++)    //on tri notre tableau
	{
		for (k = j + 1; k < i; k++) {
			if ((tab[j].score) < (tab[k].score)) {
				c = tab[j];
				tab[j] = tab[k];
				tab[k] = c;
			}
		}
	}
	fclose(pf);
	int mmin=fmin(10,i);
	printf("\nLes dix meilleurs scores sont:\n");
	printf("\nName \t\t Score \n");
	printf("___________________________\n");
	for (j = 0; j < mmin; j++)
	{
		printf("%s \t\t %d\n", tab[j].name, tab[j].score);
	}
	return 1;
}


//////////////////////////////////////////////////////////////

void menu3()
{
	printf("Rec.  Recommencer\n");
	printf("Hist. Montrer l'historique des mouvements\n");
	printf("Save. Sauvegarder\n");
	printf("Cont. Continuer la partie\n");
	printf("Quit. Quitter\n\n");
}
char g_s_rec[] = "REC", g_s_hist[] = "HIST", g_s_quit[] = "QUIT", g_s_save[] = "SAVE", g_s_cont[]="CONT";
///////////////////////////////////////////////////////////////

void upper(char tab[])
{
	int i;
	for (i = 0; i < strlen(tab); i++)
	{
		tab[i] = toupper(tab[i]);
	}
}

int menu_pause(int i_board[9][9], char c_tab[])
{
	if (strcmp(c_tab, "PAUSE") == 0) //c_tab is supposedly in capital
	{
		char s_pause[6];
		printf("\n");
		menu3();
		scanf("%s", s_pause);
		upper(s_pause);

		if (strcmp(s_pause, g_s_rec) == 0)
		{
			recommencer(i_board);
			return 2;
		}

		else if (strcmp(s_pause, g_s_hist) == 0)
		{
			printf("\n___________________________\n");
			afficherhistorique();
			printf("\n___________________________\n\n");
			getchar();
			return 1;
		}

		else if (strcmp(s_pause, g_s_save) == 0)
		{
			sauvegarder(i_board);
			getchar();
			printf("\n-------> Partie Sauvegard\202<-------\n");
			return 1;
		}

		else if (strcmp(s_pause, g_s_quit) == 0)
		{
			return 3;
		}
		else if (strcmp(s_pause, g_s_cont) == 0)
		{
			return 0;
		}
	}
	return -1;
}


int play_othello(int i_player, int i_board[9][9])
{

	srand(time(NULL));
	int i_legal = 1, i_vides, x=0, y=0, best;
	char c_choice2; int i_stopx = 0;

	i_vides = can_move(i_player, i_board);
	print_board(i_board);

	if (i_vides == 0)
	{
		if (i_player == noir)
		{
			g_i_BLACK[g_i_tour][0] = 0; g_i_BLACK[g_i_tour][1] = 0;
		}
		else if (i_player == blanc)
		{
			g_i_WHITE[g_i_tour][0] = 0; g_i_WHITE[g_i_tour][1] = 0;
		}
		printf("\nVous ne pouvez pas bouger.\n");
		return 0;
	}
	else
	{
		if (g_c_choix_jeu == '1'|| (g_c_choix_jeu == '2' && i_player==noir)) //H vs H OR Human vs ai but h's turn
		{
			do {
				int i_menux, i_menuy; //to know when menu pause is called
				printf("Saisir une position (x,y) puis valider.\n");
				scanf("%s", cple.x);
				upper(cple.x);
				i_menux = menu_pause(i_board, cple.x);
				switch (i_menux)
				{
					case 3: //quit
					{
						printf("Etes-vous s\226r de vouloir quitter?\n");
						printf("0.Oui			1.Non\n");
						getchar();
						c_choice2 = getchar();
						switch (c_choice2)
						{
							case '0':
							{
								printf("Voulez-vous sauvegarder la partie avant de quitter?\n");
								printf("0.Oui			1.Non, pas la peine\n");
								getchar();
                                char c_choice = getchar();
                                if(c_choice=='0')
                                {
                                    sauvegarder(i_board);
                                    getchar();
                                    printf("\n-------> Partie Sauvegard\202<-------\n\n");
                                }
								return -1;
							}
							case '1': //resume playing
							    {
							        printf("\nSaisissez votre couple:  ");
                                    scanf("%s", cple.x);
							    }
							default:
							{
								break;
							}
						}
						break;
					}

					case 2: //recommencer
					{
						return 2;
					}

					case 1: //saved ou historique
					{
						i_stopx = 1;
						break;
					}

					case 0:
					{
						printf("\nSaisissez votre couple:  ");
						scanf("%s", cple.x);
					}

					default:
					{
						break;
					}
				}

				if (!i_stopx) //si le menu pause n'a pas été appelé ç la 1ère validation
				{
					scanf("%s", cple.y);
					upper(cple.y);
					i_menuy = menu_pause(i_board, cple.y);
					switch (i_menuy)
					{
						case 3: //quit
						{
							printf("Etes-vous s\226r de vouloir quitter?\n");
							printf("0.Oui			1.Non\n");
							getchar();
							c_choice2 = getchar();
							switch (c_choice2)
							{
								case '0':
								{
									printf("Voulez-vous sauvegarder la partie avant de quitter?\n");
                                    printf("0.Oui			1.Non, pas la peine\n");
                                    getchar();
                                    char c_choice = getchar();
                                    if(c_choice=='0')
                                    {
                                        sauvegarder(i_board);
                                        getchar();
                                        printf("\n-------> Partie Sauvegard\202<-------\n\n");
                                    }
                                    return -1;
								}
								case '1':
								    {
								        printf("\nSaisir votre couple:  ");
                                        scanf("%s%s", cple.x,cple.y);
								    }
								default:
								{
									break;
								}
							}
							break;
						}

						case 2: //recommencer
						{
							return 2;
						}

						case 0: //continue
						{
							printf("\nSaisir votre couple:  ");
							scanf("%s%s", cple.x,cple.y);
						}

                        case 1: // saved or history
						default:
						{
							break;
						}
					}
				}
				x = atoi(cple.x); y = atoi(cple.y);
				i_legal = move_in(x, y, i_player) & move_to(x, y, i_board, i_player);
				i_stopx = 0;
			} while (!i_legal);
		}

		else if (g_c_choix_jeu == '2')  //human vs ai
		{
			if (i_player == blanc)
			{
				if (g_c_mode == '1') //easy
				{
					do {
						x = rand() % 8 + 1; y = rand() % 8 + 1;
						i_legal = move_in(x, y, i_player) & move_to(x, y, i_board, i_player);
					} while (!i_legal);
				}

				else if (g_c_mode == '2')  //hard
				{
					do
                    {
                        best=maxweighted(i_player, i_board);
                        //best=maxdiff(i_player, i_board);
                        x=(int) (best/10);
                        y= best-10*x;
                        i_legal = move_in(x,y,i_player) * move_to(x, y, i_board, i_player);
                    }while (!i_legal);
				}

			}
		}

		if (i_player == noir)
		{
			g_i_BLACK[g_i_tour][0] = x; g_i_BLACK[g_i_tour][1] = y;
		}
		else if (i_player == blanc)
		{
			g_i_WHITE[g_i_tour][0] = x; g_i_WHITE[g_i_tour][1] = y;
		}

		flip(i_player, x, y, i_board);
		//re_init(i_board);	 //to remove the hints for the current player
		return 1;
	}
}

//////////////////////////////////////////////////////////////


void startGame(int i_player, int i_board[9][9], int BLACK[Unfilled][2], int WHITE[Unfilled][2], char s_n1[20], char s_n2[20])
{
	int i_play, i_fill=0;
	int i_tour=0;

	if (g_c_choix_jeu != '1' && g_c_choix_jeu != '2')
	{
		printf("\n!!!!!!!!!! ERROR !!!!!!!!!!\n");
		getchar();
		getchar();
		return;
	}

	do
	{
		g_i_tour++;
		re_init(i_board);
		switch (i_player)
		{
			case noir:
			{
				printf("\n###############\n\nC'est le tour de %s (NOIR).\n\n", s_n1);
				break;
			}
			case blanc:
			{
				printf("\n###############\n\nC'est le tour de %s (BLANC).\n\n", s_n2);
				break;
			}
		}

		i_play = play_othello(i_player, i_board);
		if (i_play == 0)
		{
			printf("Turn skipped.\n\n");

			if(g_i_tour-i_tour==1) //2 skip d'affilée
            {
                break; //quitte la boucle et affiche le gagnant
            }
			i_tour=g_i_tour;

			i_player = ennemi(i_player);
			continue;
		}
		else if (i_play == -1) //quit
		{
			return ;
		}
		else if (i_play == 2) //le joueur a recommencer la partie
		{
			i_player = noir;
			continue;
		}

		i_fill = g_i_nbnoir+g_i_nbblanc;
		gagner(i_board);
		i_player = ennemi(i_player);


	} while (i_fill <64 && g_i_win_p1 == False && g_i_win_p2 == False && g_i_egalite == False && (i_player == noir || i_player == blanc));

	print_board(i_board);
	gagner(i_board);

	if (g_i_win_p1 == True)
	{
		printf("\nF\202licitation! %s a gagn\202! \n", s_n1);
		ajouterScore(g_i_nbnoir, s_n1);
	}

	else if (g_i_win_p2 == True)
	{
		printf("\nF\202licitation! %s a gagn\202! \n", s_n2);
		ajouterScore(g_i_nbblanc, s_n2);
	}

	else if (g_i_egalite == True)
	{
		printf("\nEgalit\202.\n");
	}
	getchar();
}


