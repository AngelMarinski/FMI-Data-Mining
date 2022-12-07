#include <iostream>
#include <vector>

using namespace std;

int maximizer(int a, int b, int depth);
int minimizer(int a, int b, int depth);

vector<vector<char>> board(3);
char chosenSymbol = ' ', computerSymbol = ' ';
bool humansTurn = false;
int maxScore = 10;
int minScore = -10;

void init_board() {
	for (size_t i = 0; i < board.size(); i++)
	{
		board[i].resize(3);
		for (size_t j = 0; j < board[i].size(); j++)
		{
			board[i][j] = '-';
		}
	}
}

int is_won(int depth) {
	if (board[0][0] == board[0][1] && board[0][1] == board[0][2] && board[0][0] != '-') {
		if (board[0][0] == computerSymbol) {

			return maxScore - depth;
		}
		else {
			return minScore + depth;
		}
	}

	if (board[1][0] == board[1][1] && board[1][1] == board[1][2] && board[1][0] != '-') {
		if (board[1][0] == computerSymbol) {

			return maxScore - depth;
		}
		else {
			return minScore + depth;
		}
	}

	if(board[2][0] == board[2][1] && board[2][1] == board[2][2] && board[2][0] != '-') {
		
		if (board[2][0] == computerSymbol) {
			
			return maxScore - depth;
		}
		else {
			return minScore + depth;
		}
	}

	if (board[0][0] == board[1][0] && board[1][0] == board[2][0] && board[0][0] != '-') {
		if (board[0][0] == computerSymbol) {

			return maxScore - depth;
		}
		else {
			return minScore + depth;
		}
	}

	if (board[0][1] == board[1][1] && board[1][1] == board[2][1] && board[0][1] != '-') {
		if (board[0][1] == computerSymbol) {

			return maxScore - depth;
		}
		else {
			return minScore + depth;
		}
	}

	if(board[0][2] == board[1][2] && board[1][2] == board[2][2] && board[0][2] != '-') {

		if (board[0][2] == computerSymbol) {

			return maxScore - depth;
		}
		else {
			return minScore + depth;
		}
	}

	if ((board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != '-')
		|| (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != '-')) {

		if (board[1][1] == computerSymbol) {

			return maxScore - depth;
		}
		else {
			return minScore + depth;
		}
	}

	return 0;
}

bool are_moves_left()
{
	for (int i = 0; i < board.size(); i++)
	{
		for (int j = 0; j < board[i].size(); j++)
		{
			if (board[i][j] == '-')
			{
				return true;
			}
		}
	}
	return false;
}

void make_turn() {
	int row = 0, col = 0;

	do {
		printf("Enter a row [1,3] and column [1,3] where you want to place your symbol: ");
		cin >> row >> col;
	} while ((row < 1 || row > 3 || col < 1 || col > 3)
		 || (row > 1 && row < 3 && col > 1 && col < 3 && board[row - 1][col - 1] != '-'));

	board[row - 1][col -1] = chosenSymbol;
}

int minimizer(int a, int b, int depth)
{
	int curScore = is_won(depth);

	if (curScore != 0)
	{
		return curScore;
	}

	if (!are_moves_left())
	{
		return 0;
	}

	int bestScore = INT_MAX;

	for (int i = 0; i < board.size(); i++)
	{
		for (int j = 0; j < board[i].size(); j++)
		{
			if (board[i][j] == '-')
			{
				board[i][j] = chosenSymbol;

				bestScore = min(bestScore, maximizer(a, b, depth + 1));

				//undo so minimizer can try next possible moves
				board[i][j] = '-';

				if (bestScore <= a)
				{
					return bestScore;
				}
				b = min(b, bestScore);
			}
		}
	}
	return bestScore;
}

int maximizer(int a, int b, int depth)
{
	int curScore = is_won(depth);

	if (curScore != 0)
	{
		return curScore;
	}

	if (!are_moves_left())
	{
		return 0;
	}
	int bestScore = INT_MIN;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (board[i][j] == '-')
			{
				board[i][j] = computerSymbol;

				bestScore = max(bestScore, minimizer(a, b, depth + 1));

				//undo so maximizer can try next possible moves
				board[i][j] = '-';

				if (bestScore >= b)
				{
					return bestScore;
				}
				a = max(a, bestScore);
			}
		}
	}
	return bestScore;
}

void find_best_move(int& row, int& col) {
	int bestVal = INT_MIN;

	for (size_t i = 0; i < board.size(); i++)
	{
		for (size_t j = 0; j < board[i].size(); j++)
		{
			if (board[i][j] == '-') {
				board[i][j] = computerSymbol;

				int currVal = minimizer(INT_MIN, INT_MAX, 0);
				//undo 
				board[i][j] = '-';

				if (currVal > bestVal)
				{
					bestVal = currVal;
					row = i;
					col = j;
				}
			}
		}
	}
}

void print() {
	printf("\n-------------\n");

	for (int i = 0; i < board.size(); i++) {
		for (int j = 0; j < board[i].size(); j++) {
			printf("%c ", board[i][j]);
			cout << " | ";
		}
		printf("\n");
	}
	printf("\n-------------\n");
}

void play() {
	while (are_moves_left()) {
		if (humansTurn) {
			humansTurn = false;
			make_turn();
		}
		else {
			humansTurn = true;
			int row = -1, col = -1;
			find_best_move(row, col);
			board[row][col] = computerSymbol;
			print();
		}

		int result = is_won(0);

		if (result == 0 && !are_moves_left()) {
			print();
			printf("\n DRAW!!! \n");
			return;
		}
		else if (result > 0) {
			print();
			printf("\n You lose!!! \n");
			return;
		}
		else if (result < 0){
			print();
			printf("\n You win, surprisingly!!! \n");
			return;
		}
	}
}

int main() {
	init_board();

	printf("Choose your symbol by typing 'O' or 'X'.\n(X are first):");
	cin >> chosenSymbol;

	if (chosenSymbol >= 65 && chosenSymbol <= 90) {
		chosenSymbol += 22;
	}

	if (chosenSymbol == 'x') {
		humansTurn = true;
		computerSymbol = 'o';
	}
	else {
		computerSymbol = 'x';
	}

	play();
}