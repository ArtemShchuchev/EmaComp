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
        ++radius;

        while (++radius <= maxRadius) {
            if (grid[idxLine - radius][idxRow] != 'G') break;
            if (grid[idxLine][idxRow + radius] != 'G') break;
            if (grid[idxLine + radius][idxRow] != 'G') break;
            if (grid[idxLine][idxRow - radius] != 'G') break;
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

        vector<pair<int, int>> coords;

        // horizontal
        for (int r(rMin); r <= rMax; ++r) {
            coords.push_back({ lMin, r });
            if (lMin != lMax) coords.push_back({ lMax, r });
        }
        // vertikal
        ++lMin;
        --lMax;
        for (int l(lMin); l <= lMax; ++l) {
            coords.push_back({ l, rMin });
            if (rMin != rMax) coords.push_back({ l, rMax });
        }
        // find cross
        for (const auto& [l, r] : coords) {
            tempRad = findRadiusCross(grid, l, r, radius);
            if (tempRad >= 0) {
                if (tempRad == radius) {
                    cp[numPoint] = { l, r, tempRad };
                    crossFill(grid, cp[numPoint]);
                    ++numPoint;
                }
                else if (tempRad > cp[numPoint].radius) cp[numPoint] = { l, r, tempRad };
                if (numPoint == 2) break;
            }
        }

        if (numPoint == 2) break;

        --radius;
    }


    int square1 = cp[0].radius * 4 + 1;
    int square2 = cp[1].radius * 4 + 1;

    std::cout << square1 << " * " << square2 << " = ";

    return square1 * square2;
}


int main()
{
    setlocale(LC_ALL, "Russian");   // задаём русский текст
    system("chcp 1251");            // настраиваем кодировку консоли
    std::system("cls");


    /*
    vector<string> grid{
        "BGBBGB",
        "GGGGGG",
        "BGBBGB",
        "GGGGGG",
        "BGBBGB",
        "BGBBGB"
    };
    vector<string> grid{
        "GGGGGG",
        "GBBBGB",
        "GGGGGG",
        "GGBBGB",
        "GGGGGG"
    };
    */
    vector<string> grid{ //81
        "GGGGGGGG",
        "GBGBGGBG",
        "GBGBGGBG",
        "GGGGGGGG",
        "GBGBGGBG",
        "GGGGGGGG",
        "GBGBGGBG",
        "GGGGGGGG"
    };

    int result = twoPluses(grid);

    std::cout << result << "\n";

    return 0;
}
