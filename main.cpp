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

    CrossPoint() : line(-1), row(-1), radius(-1) {}
    CrossPoint(const int l, const int r, const int rad) : line(l), row(r), radius(rad) {}

    CrossPoint* operator= (CrossPoint* cp) {
        this->line = cp->line;
        this->row = cp->row;
        this->radius = cp->radius;

        return this;
    }
};

static void crossFill(vector<string>& grid, const CrossPoint cp, const char ch) {
    int radius(cp.radius);
    while (radius) {
        grid[cp.line - radius][cp.row] = ch;
        grid[cp.line][cp.row + radius] = ch;
        grid[cp.line + radius][cp.row] = ch;
        grid[cp.line][cp.row - radius] = ch;
        --radius;
    }
    grid[cp.line][cp.row] = ch;
}

static vector<vector<pair<int, int>>> findCoord(const vector<string>& grid) {
    int sizeRow(static_cast<int>(grid[0].size()));
    int sizeLine(static_cast<int>(grid.size()));

    int minSize(min(sizeRow, sizeLine));
    int maxCrossing(minSize % 2 ? minSize : minSize - 1);

    int level(maxCrossing / 2);

    vector<vector<pair<int, int>>> coordsOnLevel(level+1);

    while (level >= 0) {
        int lMin = level;
        int rMin = level;
        int lMax = sizeLine - lMin - 1;
        int rMax = sizeRow - rMin - 1;

        // horizontal
        for (int r(rMin); r <= rMax; ++r) {
            coordsOnLevel[level].push_back({ lMin, r });
            if (lMin != lMax) coordsOnLevel[level].push_back({ lMax, r });
        }
        // vertikal
        ++lMin;
        --lMax;
        for (int l(lMin); l <= lMax; ++l) {
            coordsOnLevel[level].push_back({ l, rMin });
            if (rMin != rMax) coordsOnLevel[level].push_back({ l, rMax });
        }

        --level;
    }

    return coordsOnLevel;
}

static int twoPluses(vector<string> grid) {
    vector<vector<pair<int, int>>> coordsOnLevel{ findCoord(grid) };
    int testRad(static_cast<int>(coordsOnLevel.size() - 1));
    vector<int> square;

    while (testRad > 1) {
        int radius(testRad);
        --testRad;

        int numPoint(0);
        CrossPoint cp[2];

        int level(static_cast<int>(coordsOnLevel.size() - 1));
        while (radius >= 0) {
            // find cross
            for (const auto& [l, r] : coordsOnLevel[level]) {
                int tempRad = findRadiusCross(grid, l, r, radius);
                if (tempRad > cp[numPoint].radius) {
                    cp[numPoint] = { l, r, tempRad };
                
                    if (cp[numPoint].radius == radius) {
                        if (++numPoint == 1) crossFill(grid, cp[0], 'O');
                        else break;
                    }
                }
            }
            if (numPoint == 2) break;

            --level;
            if (level < radius) radius = level;

            if (cp[numPoint].radius >= radius) {
                if (numPoint >= 1) break;
                crossFill(grid, cp[numPoint], 'O');
                ++numPoint;
            }
        }
        crossFill(grid, cp[0], 'G');

        int square1 = cp[0].radius * 4 + 1;
        int square2 = cp[1].radius * 4 + 1;
        square.push_back(square1 * square2);
        std::cout << square1 << " * " << square2 << " = " << square.back() << endl;
    }

    return *max_element(square.begin(), square.end());
}


int main()
{
    setlocale(LC_ALL, "Russian");   // задаём русский текст
    system("chcp 1251");            // настраиваем кодировку консоли
    std::system("cls");


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
    vector<string> grid{ //81
        "GGGGGGGGGGGG",
        "GBGGBBBBBBBG",
        "GBGGBBBBBBBG",
        "GGGGGGGGGGGG",
        "GGGGGGGGGGGG",
        "GGGGGGGGGGGG",
        "GGGGGGGGGGGG",
        "GBGGBBBBBBBG",
        "GBGGBBBBBBBG",
        "GBGGBBBBBBBG",
        "GGGGGGGGGGGG",
        "GBGGBBBBBBBG"
    };
    */

    int result = twoPluses(grid);

    std::cout << result << "\n";

    return 0;
}
