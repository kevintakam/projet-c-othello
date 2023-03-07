#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<stdbool.h>

#include "final_help_functions"

#define infini 1000000000
#define WIN 2000
#define LOSS -2000

int depth_hard=5;

long int g_l_BOARDS=0; //le nombre de board examiner durant la recherche minmax

int nexttoplay (int board[9][9], int i_player)
{
  int p,p_moves=0,player_moves=0;
  p = ennemi(i_player);

  p_moves=can_move(p,board);
  re_init(board);
  if (p_moves)
  {
      return p;
  }
  //si l'adv has to skip his turn
  player_moves=can_move(i_player,board);
  re_init(board);
  if (player_moves)
  {
    return i_player;
  }
  return 0;  // if no one can move
}

void copyboard (int i_dest[9][9],int i_source[9][9]) {
  int i,j;
  for (i=0; i<9; i++)
  {
      for (j=0; j<9; j++)
      {
          i_dest[i][j] = i_source[i][j];
      }
  }
}

//we save all the possible moves in a pointer in case the ai is playing
int * legalmoves ( int board[9][9])
{
  int j, i,k, * moves;
  moves = (int *)malloc(65 * sizeof(int));
  moves[0] = 0;
  k = 0;
  for (i=1;i<9;i++)
  {
      for(j=1;j<9;j++)
      {
          if(board[i][j]==vide)
          {
              k++;
              moves[k]=i*10+j;
          }
      }
  }
  moves[0]=k;
  return moves;
}

int evaluation (int i_player, int board[9][9])
{                                       /* utility is measured */
  int i,j, p, i_nbl=0, i_nb2 = 0;                      /* number of pieces */
  p = ennemi(i_player);
  for (i=1; i<9; i++)
  {
    for (j=1; j<9; j++)
    {
        if (board[i][j]==i_player)
        {
            i_nbl++;
        }
        if (board[i][j]==p)
        {
            i_nb2++;
        }
    }
  }
  return i_nbl-i_nb2;
}

int weightedevaluation (int player, int board[9][9])
{
  int i,j, i_nb1=0, i_nb2=0, p;
  const int weights[9][9]={{0,  0,  0,  0,  0,  0,  0,  0,  0},
                           {0,120,-20, 20,  5,  5, 20,-20,120},
                           {0,-20,-40, -5, -5, -5, -5,-40,-20},
                           {0, 20, -5, 15,  3,  3, 15, -5, 20},
                           {0,  5, -5,  3,  3,  3,  3, -5,  5},
                           {0,  5, -5,  3,  3,  3,  3, -5,  5},
                           {0, 20, -5, 15,  3,  3, 15, -5, 20},
                           {0,-20,-40, -5, -5, -5, -5,-40,-20},
                           {0,120,-20, 20,  5,  5, 20,-20,120}};

  p = ennemi(player);
  for (i=1; i<9; i++)
  {
     for (j=1; j<9; j++)
     {
         if (board[i][j]==player)
         {
             i_nb1=i_nb1+weights[i][j];
         }
         if (board[i][j]==p)
         {
             i_nb2=i_nb2+weights[i][j];
         }
     }
  }
  return (i_nb1-i_nb2);
}


//board deja marquée avec les vides
int newboardmax[9][9];


int maxchoice (int player, int board[9][9], int ply,int weight)
{
  int i, i_max, next_player, newscore, * moves;
  int x=0,y=0;

  //int minchoice (int pl, int table[9)[9], int depth, int _weight);
  //elle sera défini après

  if (ply == 0)
  {
      return weight;
  }

  can_move(player,board);
  moves = legalmoves( board);
  i_max = LOSS - 1;

  for (i=1; i <= moves[0]; i++)
  {
    copyboard(newboardmax,board);
    can_move(player, newboardmax);
    g_l_BOARDS = g_l_BOARDS + 1;
    x=moves[i]/10;
    y=moves[i]-10*x;
    flip(player,x,y, newboardmax);
    re_init(newboardmax);

    next_player = nexttoplay(newboardmax, player);
    if (next_player == 0)
    {
         newscore = evaluation(player, newboardmax);
         if (newscore > 0) newscore = WIN;
         if (newscore < 0) newscore = LOSS;
    }

    else if (next_player == player)
    {
        newscore = maxchoice(player, newboardmax, ply-1, weight);
    }

    else if (next_player == ennemi(player))
    {
        newscore = minchoice(player, newboardmax, ply-1, weight);
    }

    if (newscore > i_max)
    {
        i_max = newscore;
    }
  }
  free(moves);
  return i_max;
}

int newboardmin[9][9];
int minchoice (int player, int board[9][9], int ply,int weight)
{
    int i, i_min, next_player, newscore, * moves;
    int x=0,y=0;

    if (ply == 0)
    {
      return weight;
    }

    can_move(player,board);
    moves = legalmoves( board);
    i_min = WIN+1;

    for (i=1; i <= moves[0]; i++)
    {
        copyboard(newboardmin,board);
        can_move(player,newboardmin);
        g_l_BOARDS = g_l_BOARDS + 1;
        x=moves[i]/10;
        y=moves[i]-x*10;
        flip(ennemi(player),x,y, newboardmin);
        re_init(newboardmin);

        next_player = nexttoplay(newboardmin, ennemi(player));
        if (next_player == 0) {
             newscore = evaluation(player, newboardmin);
             if (newscore > 0) newscore = WIN;
             if (newscore < 0) newscore = LOSS;
        }

        else if (next_player == player)
        {
           newscore = maxchoice(player, newboardmin, ply-1, weight);
        }

        else if (next_player == ennemi(player))
        {
            newscore = minchoice(player, newboardmin, ply-1, weight);
        }

        if (newscore < i_min)
        {
            i_min = newscore;
        }
    }
    free(moves);
    return i_min;
}

// board avec les cases vides marquées
int newboard[9][9];
int minmax1 (int player, int board[9][9], int ply, int weight)
{
    int i, i_max, next_player, newscore, bestmove, * moves;
    int x,y;
    can_move(player,board);
    moves = legalmoves( board); /* get all legal moves for player */
    i_max = LOSS - 1;  /* any legal move will exceed this score */

    for (i=1; i <= moves[0]; i++)
    {
        copyboard(newboard,board);
        g_l_BOARDS = g_l_BOARDS + 1;
        x= (int) (moves[i]/10);
        y= moves[i]%10;
        flip(player,x,y, newboard);
        re_init(newboard);

        next_player = nexttoplay(newboard, player);
        if (next_player == 0) {  /* game over, so determine winner */
             newscore = evaluation(player, newboard);
             if (newscore > 0) newscore = WIN; /* a win for player */
             if (newscore < 0) newscore = LOSS; /* a win for opp */
        }

        else if (next_player == player)   /* opponent cannot move */
        {
            newscore = maxchoice(player, newboard, ply-1, weight);
        }

        else if (next_player == ennemi(player))
        {
            newscore = minchoice(player, newboard, ply-1, weight);
        }

        if (newscore > i_max)
        {
            i_max = newscore;
            bestmove = moves[i];  /* a better move found */
        }
    }
    free(moves);
    return bestmove;
}



int maxweighted(int player, int board[9][9])
{
   int weight = weightedevaluation(player,board);
   int best_move=minmax1(player, board, 6, weight);
   return best_move;
}



