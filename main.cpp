// https://www.hackerrank.com/challenges/two-pluses/problem?utm_campaign=challenge-recommendation&utm_medium=email&utm_source=24-hour-campaign

#include <iostream>
#include <vector>

using namespace std;

/*
 * Complete the 'twoPluses' function below.
 *
 * The function is expected to return an INTEGER.
 * The function accepts STRING_ARRAY grid as parameter.
 */


static int findRadiusCross(const vector<string>& grid, const unsigned long long idxLine, const unsigned long long idxRow, const int maxRadius) {
    int radius(-1);

    if (grid[idxLine][idxRow] == 'G') {
        radius = 0;

        while (++radius <= maxRadius) {
            if (grid[idxLine - radius][idxRow] != 'G') break;
            if (grid[idxLine][idxRow + radius] != 'G') break;
            if (grid[idxLine + radius][idxRow] != 'G') break;
            if (grid[idxLine][idxRow - radius] != 'G') break;
        }

        while (radius > maxRadius) {
            long long up(idxLine - radius);
            if (up >= 0) {
                if (grid[up][idxRow] == 'O') {
                    --radius;
                    continue;
                }
            }

            unsigned long long down(idxLine + radius);
            if (down < grid.size()) {
                if (grid[down][idxRow] == 'O') {
                    --radius;
                    continue;
                }
            }

            unsigned long long right(idxRow + radius);
            if (right < grid[0].size()) {
                if (grid[idxLine][right] == 'O') {
                    --radius;
                    continue;
                }
            }

            long long left(idxRow - radius);
            if (left >= 0) {
                if (grid[idxLine][left] == 'O') {
                    --radius;
                    continue;
                }
            }

            break;
        }

        --radius;
    }

    return radius;
}

struct CrossPoint {
    int line;
    int row;
    int radius;
};

static void crossFill(vector<string>& grid, const CrossPoint cp) {
    int radius(cp.radius);
    while (radius) {
        grid[cp.line - radius][cp.row] = 'O';
        grid[cp.line][cp.row + radius] = 'O';
        grid[cp.line + radius][cp.row] = 'O';
        grid[cp.line][cp.row - radius] = 'O';
        --radius;
    }
    grid[cp.line][cp.row] = 'O';
}

static int twoPluses(vector<string> grid) {
    int sizeRow(static_cast<int>(grid[0].size()));
    int sizeLine(static_cast<int>(grid.size()));

    int minSize(min(sizeRow, sizeLine));
    int maxCrossing(minSize % 2 ? minSize : minSize - 1);

    int numPoint(0);
    CrossPoint cp[2]{ {-1,-1,-1}, {-1,-1,-1} };
    int radius(maxCrossing / 2);
    int tempRad(0);

    while (radius >= 0) {
        if (cp[numPoint].radius >= radius) {
            if (numPoint) numPoint = 2;
            else {
                crossFill(grid, cp[numPoint]);
                ++numPoint;
            }

            if (numPoint == 2) break;
        }

        int lMin = radius;
        int rMin = radius;
        int lMax = sizeLine - lMin - 1;
        int rMax = sizeRow - rMin - 1;

        // horizontal
        for (int r(rMin); r <= rMax; ++r) {
            tempRad = findRadiusCross(grid, lMin, r, radius);
            if (tempRad >= 0) {
                if (tempRad == radius) {
                    cp[numPoint] = { lMin, r, tempRad };
                    crossFill(grid, cp[numPoint]);
                    ++numPoint;
                }
                else if (tempRad > cp[numPoint].radius) cp[numPoint] = { lMin, r, tempRad };
                if (numPoint == 2) break;
            }

            if (lMin != lMax) {
                tempRad = findRadiusCross(grid, lMax, r, radius);
                if (tempRad >= 0) {
                    if (tempRad == radius) {
                        cp[numPoint] = { lMax, r, tempRad };
                        crossFill(grid, cp[numPoint]);
                        ++numPoint;
                    }
                    else if (tempRad > cp[numPoint].radius) cp[numPoint] = { lMax, r, tempRad };
                    if (numPoint == 2) break;
                }
            }
        }
        if (numPoint == 2) break;

        // vertikal
        ++lMin;
        --lMax;
        for (int l(lMin); l <= lMax; ++l) {
            tempRad = findRadiusCross(grid, l, rMin, radius);
            if (tempRad >= 0) {
                if (tempRad == radius) {
                    cp[numPoint] = { l, rMin, tempRad };
                    crossFill(grid, cp[numPoint]);
                    ++numPoint;
                }
                else if (tempRad > cp[numPoint].radius) cp[numPoint] = { l, rMin, tempRad };
                if (numPoint == 2) break;
            }

            if (rMin != rMax) {
                tempRad = findRadiusCross(grid, l, rMax, radius);
                if (tempRad >= 0) {
                    if (tempRad == radius) {
                        cp[numPoint] = { l, rMax, tempRad };
                        crossFill(grid, cp[numPoint]);
                        ++numPoint;
                    }
                    else if (tempRad > cp[numPoint].radius) cp[numPoint] = { l, rMax, tempRad };
                    if (numPoint == 2) break;
                }
            }
        }
        if (numPoint == 2) break;

        --radius;
    }



    //size = findPlus(grid, maxCrossing/2, maxCrossing/2, maxCrossing);
    int square1 = cp[0].radius ? cp[0].radius * 4 + 1 : 0;
    int square2 = cp[1].radius ? cp[1].radius * 4 + 1 : 0;

    cout << square1 << " * " << square2 << " = ";

    return square1 * square2;
}


int main()
{
    setlocale(LC_ALL, "Russian");   // задаём русский текст
    system("chcp 1251");            // настраиваем кодировку консоли
    std::system("cls");


    vector<string> grid{
        "GGGGGG",
        "GBBBGB",
        "GGGGGG",
        "GGBBGB",
        "GGGGGG"
    };
    /*
    vector<string> grid{
        "BGBBGB",
        "GGGGGG",
        "BGBBGB",
        "GGGGGG",
        "BGBBGB",
        "BGBBGB"
    };
    */

    int result = twoPluses(grid);

    cout << result << "\n";

    return 0;
}
