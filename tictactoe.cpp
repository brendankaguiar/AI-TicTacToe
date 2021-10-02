#include <iostream>
#include <fstream>
#include <iomanip>
/*
Tic Tac Toe with AI
X = 1
O = -1
|_| = 0
*/
struct Game {
	int player;
	int transition[3][3];
	void to_move(int state[][3])//Check which turn it is assuming X goes first
	{
		player = 0;
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				player += state[i][j]; //if 0 MAX's move, else if 1, MIN's Move
		if (player == 1)
			player = -player;//if max just went, player = -1 to indicate min's move
		else
			player = 1;
	}
	bool is_terminal(int state[][3])//check if state is terminal 
	{
		for (int i = 0; i < 3; i++)
		{
			if (state[i][0] == state[i][1] && state[i][1] == state[i][2] && state[i][0] != 0)//check horizontals
				return true;
			if (state[0][i] == state[1][i] && state[2][i] == state[1][i] && state[0][i] != 0)//check verticals
				return true;

		}
		if (state[0][0] == state[1][1] && state[1][1] == state[2][2] && state[2][2] != 0)
			return true;//check 1st diagonal
		if (state[0][2] == state[1][1] && state[1][1] == state[2][0] && state[2][0] != 0)
			return true;//check 2nd diagonal
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if (state[i][j] == 0)//move available, not terminal
				{
					return false;
				}
			}
		}
		return true;
	}
	int utility(int state[][3]) //get utility value of terminal state
	{
		for (int i = 0; i < 3; i++)
		{
			if (state[i][0] == state[i][1] && state[i][1] == state[i][2])//check horizontals
			{
				return state[i][0];//will return -1 if O wins, else returns 1 if X wins
			}
			if (state[0][i] == state[1][i] && state[1][i] == state[2][i])//check verticals
			{
				return state[0][i];//return utility value
			}
		}
		if (state[0][0] == state[1][1] && state[1][1] == state[2][2])//check diagonals
			return state[0][0];
		if (state[0][2] == state[1][1] && state[1][1] == state[2][0])
			return state[0][2];
		return 0;//returns 0 if no winner was decided
	}
	void actions(int a[][2], int state[][3]) //get possible actions
	{
		int k = 0;
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				// find an empty square
				if (state[i][j] == 0)
				{
					a[k][0] = i;//assign new possible action
					a[k][1] = j;
					k++;//iterate for next action, else action array filled with -2
				}
	}
	void result(int state[][3], int a[])
	{
		to_move(state);
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if (a[0] == i && a[1] == j)
					transition[i][j] = player;//assign next transition
				else
					transition[i][j] = state[i][j];//assign current state
			}
		}
		to_move(transition);//assign player for next transition
	}
};
Game game;
void minimax_search(Game game, int state[][3], int move[]);
void max_value(Game game, int state[][3], int move[]);
void min_value(Game game, int state[][3], int move[]);
void make_move(int state[][3], int move[]);
void display_board(int state[][3]);
int load_board(std::string filename, int board[][3]);
int save_board(std::string filename, int board[][3]);
int main()
{
	int state[3][3], move[3];//[0] = i, [1] = j, [2] = value
	// load in current board state
	load_board("test.board", state);
	std::cout << "Original Board... " << std::endl;
	// make move
	display_board(state);
	minimax_search(game, state, move);
	make_move(state, move);
	std::cout << "New Board... " << std::endl;
	display_board(state);
	std::cout << "Utility Value (should be 1): " << move[2] << std::endl;
	// save board state
	save_board("test.board", state);
	return 0;
}
void display_board(int state[][3])
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
			std::cout << state[i][j] << " ";
		std::cout << std::endl;
	}
}
void make_move(int state[][3], int move[])
{
	state[move[0]][move[1]] = 1;//Make move for X
}
int load_board(std::string filename, int board[][3])
{
	std::ifstream bfile(filename.c_str());
	if (bfile.is_open())
	{

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				bfile >> board[i][j];
				//printf ("%d ", board[i][j] );
			}
			//printf ("\n");
		}

		bfile.close();
	}
	else
	{
		printf("unable to open file: [%s]\n", filename.c_str());
		return -1;
	}

	return 0;
}
int save_board(std::string filename, int board[][3])
{
	std::ofstream bfile(filename.c_str());
	if (bfile.is_open())
	{

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				bfile << std::setw(5);
				bfile << board[i][j];
				//bfile << " ";
			}
			bfile << std::endl;
		}

		bfile.close();
	}
	else
	{
		printf("unable to open file: [%s]\n", filename.c_str());
		return -1;
	}

	return 0;
}
/**
	make_move: takes a board state and makes a legal
	(hopefully optimal) move

	args:
		int [][3] board: 3x3 array of ints representing the
		board state. The values of board are altered based
		on the move
			0: empty
			1: x
		 -1: o

	returns (int):
		the number of steps it took to choose the best move
		(current implementation returns 1 by default, 0 if no move made)
**/
void minimax_search(Game game, int state[][3], int move[])
{
	// figure out what move it is
	//game.to_move(state);
	max_value(game, state, move); //Initially X's move
}
void max_value(Game game, int state[][3], int move[])
{
	if (game.is_terminal(state))
	{
		move[0] = NULL;
		move[1] = NULL;
		move[2] = game.utility(state);
	}
	else
	{
		move[2] = -100000;
		int a[9][2];//possible moves or actions
		for (int i = 0; i < 9; i++)
			a[i][0] = 3;//initialize actions array to 3
		game.actions(a, state);
		int i = 0;
		while (a[i][0] != 3)
		{
			int move2[3];
			game.result(state, a[i]);//assign next transition state
			min_value(game, game.transition, move2);
			if (move2[2] > move[2]) //return next action and it's utility value
			{
				move[0] = a[i][0];
				move[1] = a[i][1];
				move[2] = move2[2];
			}
			i++;
		}
	}
}
void min_value(Game game, int state[][3], int move[])
{
	if (game.is_terminal(state))
	{
		move[0] = NULL;
		move[1] = NULL;
		move[2] = game.utility(state);
	}
	else
	{
		move[2] = 100000;
		int a[9][2];//possible moves or actions
		for (int i = 0; i < 9; i++)
			a[i][0] = 3;//initialize actions array to 3
		game.actions(a, state);
		int i = 0;
		while (a[i][0] != 3)
		{
			int move2[3];
			game.result(state, a[i]);//assign next transition state/player to move
			max_value(game, game.transition, move2);
			if (move2[2] < move[2]) //return next action and it's utility value
			{
				move[0] = a[i][0];
				move[1] = a[i][1];
				move[2] = move2[2];
			}
			i++;
		}
	}
}
