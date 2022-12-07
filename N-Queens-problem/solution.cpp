#include <iostream>
#include <vector>

using namespace std;

vector<int> queensPositions;
vector<int> queensOnD1;
vector<int> queensOnD2;
vector<int> queensOnRow;
clock_t calculationTime;
bool is_solved = false;

void init() {
	int col = 1;
	const int Offset = queensPositions.size() - 1;
	
	// horse distance
	for (int row = 0; row < queensPositions.size(); row++)
	{
		queensPositions[col] = row;
		queensOnRow[row]++;
		queensOnD1[col - row + Offset]++;
		queensOnD2[col + row]++;

		col += 2;
		if (col >= queensPositions.size())
		{
			col = 0;
		}
	}
}


int col_with_max_conf() {
	int maxConflict = INT_MIN;
	const int Offset = queensPositions.size() - 1;

	vector<int> conflictCols;

	for (size_t col = 0; col < queensPositions.size(); col++)
	{
		int row = queensPositions[col];
		int conflict = queensOnRow[row] + queensOnD1[Offset - row + col]
			+ queensOnD2[row + col] - 3;

		if (conflict > maxConflict) {
			maxConflict = conflict;
			conflictCols.clear();
			conflictCols.push_back(col);
		}
		else if (conflict == maxConflict) {
			conflictCols.push_back(col);
		}
	}

	if (maxConflict == 0) {
		is_solved = true;
	}

	int randConflictIdx = rand() % conflictCols.size();

	return conflictCols[randConflictIdx];
}

int row_with_min_conf(int col) {
	const int Offset = queensPositions.size() - 1;
	int minConflict = INT_MAX;

	vector<int> conflictRow;

	for (size_t row = 0; row < queensPositions.size(); row++)
	{
		int conflict = queensOnRow[row] + queensOnD1[Offset - row + col]
			+ queensOnD2[row + col] - 3;

		if (conflict < minConflict) {
			minConflict = conflict;
			conflictRow.clear();
			conflictRow.push_back(row);
		}
		else if (conflict == minConflict) {
			conflictRow.push_back(row);
		}
	}

	int randConflictIdx = rand() % conflictRow.size();

	return conflictRow[randConflictIdx];
}

void update(int col, int row) {
	int prev = queensPositions[col];
	queensOnD1[col - prev + queensPositions.size() - 1]--;
	queensOnD2[col + prev]--;
	queensOnRow[prev]--;

	queensPositions[col] = row;
	queensOnD1[col - row + queensPositions.size() - 1]++;
	queensOnD2[col + row]++;
	queensOnRow[row]++;
}

void print() {
	calculationTime = clock() - calculationTime;

	for (size_t i = 0; i < queensPositions.size(); i++)
	{
		for (size_t j = 0; j < queensPositions.size(); j++)
		{
			printf("%c ",
				(i == queensPositions[j] ? '*' : '_')
			);
		}
		printf("\n");
	}

	printf("\nEstimated time: %f", ((double)calculationTime) / CLOCKS_PER_SEC);
}

void solve() {
	init();

	int k = queensPositions.size() - 2;
	int col, row, iter = 0;

	while (iter++ < k * queensPositions.size()) {

		col = col_with_max_conf();
		row = row_with_min_conf(col);
		if (is_solved) {
			return;
		}
		else {
			update(col, row);
		}
	}

	if (!is_solved) {
		solve();
	}
}



int main() {
	int size = 0;

	cin >> size;

	if (size <= 3) {
		printf("\nUnsolveable!\n");
	}

	calculationTime = clock();

	queensPositions.resize(size, 0);
	queensOnRow.resize(size, 0);
	queensOnD1.resize(2 * size - 1, 0);
	queensOnD2.resize(2 * size - 1, 0);

	solve();
	print();

	return 0;
}