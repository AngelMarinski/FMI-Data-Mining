#include <iostream>
#include <vector>
#include <string>

using namespace std;

vector<int> tiles;
vector<string> solution;
int boardSize, currZeroPosition, goalZeroPosition;
clock_t calculationTime;

const int FOUND = -1;

int manhattan()
{
    int distance = 0;
    int currentX, currentY, goalX, goalY;
    for (int i = 0; i < tiles.size(); i++)
    {
        int currTile = tiles[i];
        if (currTile != 0) {

            if (tiles[i] <= goalZeroPosition) {
                currTile -= 1;
            }
            currentX = i % boardSize;
            goalX = currTile % boardSize;
            currentY = i / boardSize;
            goalY = currTile / boardSize;
            
            distance += abs(currentX - goalX) + abs(currentY - goalY);
        }
    }
    return distance;
}

bool isSolvable() {
    int inversions = 0;
    int blankRow = currZeroPosition / boardSize;

    for (size_t i = 0; i < tiles.size(); i++)
    {
        for (size_t j = i + 1; j < tiles.size(); j++)
        {
            if (tiles[i] != 0 && tiles[j] != 0
                && tiles[i] > tiles[j]) {
                inversions++;
            }
        }
    }

    if (inversions % 2 == 1 && boardSize % 2 == 1) {
        return false;
    }
    else if (boardSize % 2 == 0 && (inversions + blankRow) % 2 == 0) {
        return false;
    }

    return true;
}

void printSolution() {
    calculationTime = clock() - calculationTime;

    cout << "Estimated time: " << ((float)calculationTime) / CLOCKS_PER_SEC << endl;
    cout << solution.size() - 1 << endl;
    for (int i = 1; i < solution.size(); i++) {
        cout << solution[i] << endl;
    }

    //Uncomment to see the final arrangement of the algorithm
    //for (size_t i = 0; i < tiles.size(); i++)
    //{
    //    cout << tiles[i] << " ";
    //    if ((i + 1) % boardSize == 0 && i != 0) {
    //        cout << endl;
    //    }
    //}
}

bool moveZeroUp() {
    if (currZeroPosition / boardSize == 0) {
        return false;
    }
    
    swap(tiles[currZeroPosition], tiles[currZeroPosition - boardSize]);
    currZeroPosition -= boardSize;

    return true;
}

bool moveZeroDown() {
    if (currZeroPosition / boardSize == boardSize - 1)
    {
        return false;
    }

    swap(tiles[currZeroPosition], tiles[currZeroPosition + boardSize]);
    currZeroPosition += boardSize;
    return true;
}

bool moveZeroLeft(){
    if (currZeroPosition % boardSize == 0)
    {
        return false;
    }
    swap(tiles[currZeroPosition], tiles[currZeroPosition - 1]);
    currZeroPosition--;
    return true;
}

bool moveZeroRight(){
    if (currZeroPosition % boardSize == boardSize - 1)
    {
        return false;
    }
    swap(tiles[currZeroPosition], tiles[currZeroPosition + 1]);
    currZeroPosition++;
    return true;
}

bool isSolutionFound(int temp, int& min) {
    if (temp == FOUND) {
        return true;
    }

    if (temp < min) {
        min = temp;
    }

    solution.pop_back();
    return false;
}

int search(int g, int estimatedCost) {
    //f(n) = g(n) + h(n)
    int f = g + manhattan();

    if (f > estimatedCost) {
        return f;
    }

    if (manhattan() == 0) {
        return FOUND;
    }

    int min = INT_MAX, temp;

    if (solution.back() != "up" && moveZeroUp()) {
        solution.push_back("down");
        temp = search(g + 1, estimatedCost);

        if (isSolutionFound(temp, min)) {
            return FOUND;
        }

        moveZeroDown();
    }
    if (solution.back() != "left" && moveZeroLeft()) {
        solution.push_back("right");
        temp = search(g + 1, estimatedCost);

        if (isSolutionFound(temp, min)) {
            return FOUND;
        }

        moveZeroRight();
    }
    if (solution.back() != "right" && moveZeroRight()) {
        solution.push_back("left");
        temp = search(g + 1, estimatedCost);

        if (isSolutionFound(temp, min)) {
            return FOUND;
        }

        moveZeroLeft();
    }
    if (solution.back() != "down" && moveZeroDown()) {
        solution.push_back("up");
        temp = search(g + 1, estimatedCost);

        if (isSolutionFound(temp, min)) {
            return FOUND;
        }

        moveZeroUp();
    }

    return min;
}

void idastar() {
    int estimatedCost = manhattan();
    solution.push_back("start");
    int temp;

    while (true)
    {
        temp = search(0, estimatedCost);
        if (temp == FOUND)
        {
            return;
        }
        estimatedCost = temp;
    }
}

int main() {
    int n = 0, zeroIdx = 0;

    cin >> n >> zeroIdx;

    for (size_t i = 0; i <= n ; i++)
    {
        int tile = 0;
        cin >> tile;
        if (tile == 0) {
            currZeroPosition = i;
        }
        tiles.push_back(tile);
    }

    boardSize = sqrt(n + 1);

    if (!isSolvable()) {
        cout << "\nUnsolvability determined!\n";
    }

    if (zeroIdx == -1) {
        goalZeroPosition = boardSize * boardSize - 1;
    }
    else {
        goalZeroPosition = zeroIdx;
    }

    
    calculationTime = clock();

    idastar();

    printSolution();

    return 0;
}