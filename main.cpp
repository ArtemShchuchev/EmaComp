// https://www.hackerrank.com/challenges/two-pluses/problem?utm_campaign=challenge-recommendation&utm_medium=email&utm_source=24-hour-campaign

#include <iostream>
#include <fstream>
#include <vector>
#include <map>

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


///////////////////////////////////
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

    bool crossing(const CrossPoint& cp) const{
        bool answer(false);

        int rowDistance(abs(row - cp.row) - 1);
        int lineDistance(abs(line - cp.line) - 1);

        if (line == cp.line) {
            if (rowDistance < radius + cp.radius) answer = true;
        }
        else if (row == cp.row) {
            if (lineDistance < radius + cp.radius) answer = true;
        }
        else {
            int maxRad(max(radius, cp.radius));
            int minRad(min(radius, cp.radius));

            if (lineDistance < minRad) {
                if (rowDistance < maxRad) answer = true;
            }
            else if (rowDistance < minRad) {
                if (lineDistance < maxRad) answer = true;
            }
        }

        return answer;
    }

    int maxCrossing(const CrossPoint& cp) const {
        auto crossing = [this, &cp](int rad1, int rad2) {
                int rowDistance(abs(row - cp.row) - 1);
                int lineDistance(abs(line - cp.line) - 1);
                bool answer(false);
                if (line == cp.line) {
                    if (rowDistance < rad1 + rad2) answer = true;
                }
                else if (row == cp.row) {
                    if (lineDistance < rad1 + rad2) answer = true;
                }
                else {
                    int maxRad(max(rad1, rad2));
                    int minRad(min(rad1, rad2));

                    if (lineDistance < minRad) {
                        if (rowDistance < maxRad) answer = true;
                    }
                    else if (rowDistance < minRad) {
                        if (lineDistance < maxRad) answer = true;
                    }
                }
                return answer;
            };

        int square(0);
        for (int r1(0); r1 <= radius; ++r1) {
            for (int r2(0); r2 <= cp.radius; ++r2) {
                if (!crossing(r1, r2)) {
                    int temp((r1 * 4 + 1) * (r2 * 4 + 1));
                    if (square < temp) square = temp;
                }
            }
        }

        return square;
    }
};

static void findRadiusCross(const vector<string>& grid, CrossPoint& cp) {
    int radius(-1);

    if (grid[cp.line][cp.row] == 'G') {
        ++radius;

        while (++radius <= cp.radius) {
            if (grid[cp.line - radius][cp.row] != 'G') break;
            if (grid[cp.line][cp.row + radius] != 'G') break;
            if (grid[cp.line + radius][cp.row] != 'G') break;
            if (grid[cp.line][cp.row - radius] != 'G') break;
        }

        --radius;
    }

    cp.radius = radius;
}

static vector<CrossPoint> findCross(const vector<string>& grid) {
    int sizeRow(static_cast<int>(grid[0].size()));
    int sizeLine(static_cast<int>(grid.size()));

    int minSize(min(sizeRow, sizeLine));
    int maxCrossing(minSize % 2 ? minSize : minSize - 1);

    int level(maxCrossing / 2);

    vector<CrossPoint> cross;
    auto findTest = [&grid, &cross](int l, int r, int rad) {
        CrossPoint cp(l, r, rad);
        findRadiusCross(grid, cp);
        if (cp.radius >= 0)
            if (cp.radius > 0 || cross.size() < 2)
                cross.push_back(cp);
        };

    while (level >= 0) {
        int lMin = level;
        int rMin = level;
        int lMax = sizeLine - lMin - 1;
        int rMax = sizeRow - rMin - 1;

        // horizontal
        for (int r(rMin); r <= rMax; ++r) {
            findTest(lMin, r, level);
            if (lMin != lMax) findTest(lMax, r, level);
        }
        // vertikal
        ++lMin;
        --lMax;
        for (int l(lMin); l <= lMax; ++l) {
            findTest(l, rMin, level);
            if (rMin != rMax) findTest(l, rMax, level);
        }

        --level;
    }

    return cross;
}

static int twoPluses(const vector<string>& grid) {
    int answer(0);
    vector<CrossPoint> crossing{ findCross(grid) };

    for (auto it1(crossing.begin()); it1 != crossing.cend(); ++it1) {
        for (auto it2(crossing.begin()); it2 != crossing.end(); ++it2) {
            if (it1 == it2) continue;

            int square(it1->maxCrossing(*it2));
            if (answer < square) answer = square;
        }
    }

    return answer;
}
///////////////////////////////////


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

/*
static int twoPluses(vector<string> grid) {
    multimap<int, pair<int, int>> allCross;
    vector<vector<pair<int, int>>> coordsOnLevel{ findCoord(grid) };
    int level(static_cast<int>(coordsOnLevel.size() - 1));
    while (level >= 0) {
        for (const auto& [l, r] : coordsOnLevel[level]) {
            int tempRad = findRadiusCross(grid, l, r, level);
            if (tempRad >= 0) {
                if (tempRad == 0 && allCross.count(0) > 0) continue;
                    
                allCross.insert({ tempRad, {l, r} });
            }
        }
        --level;
    }

    vector<pair<int, pair<int, int>>> crossing(allCross.rbegin(), allCross.rend());

    int answer(0);
    auto lastIter(prev(crossing.cend()));
    for (auto it1(crossing.cbegin()); it1 != lastIter; ++it1) {
        auto& [rad1, coord1] = *it1;
        auto& [line1, row1] = coord1;
        CrossPoint cp1(line1, row1, rad1);
        //cout << count1 << ": " << line1 << ", " << row1 << " - " << rad1 << '\n';
        //cout << "---------\n";

        int r1(cp1.radius);
        for (auto it2(crossing.cbegin()); it2 != crossing.cend(); ++it2) {
            auto& [rad2, coord2] = *it2;
            auto& [line2, row2] = coord2;
            CrossPoint cp2(line2, row2, rad2);

            if (it1 == it2) continue;
            else if (it1 > it2) {
                if (cp1.radius > 0) --cp1.radius;
                cp2.radius = cp1.radius;
            }

            if (!cp1.crossing(cp2)) {
                int square( (cp1.radius * 4 + 1) * (cp2.radius * 4 + 1) );
                if (answer < square) answer = square;
                //cout << "*********\n";
                //cout << count1 << ": " << line1 << ", " << row1 << " - " << rad1 << '\n';
                //cout << count2 << ": " << line2 << ", " << row2 << " - " << rad2 << '\n';
                //cout << "*********\n";
                break;
            }
            //cout << count2 << ": " << line2 << ", " << row2 << " - " << rad2 << '\n';
            cp1.radius = r1;
        }
        //cout << "##################\n";
    }

    return answer;
}
*/


int main()
{
    setlocale(LC_ALL, "Russian");   // задаём русский текст
    system("chcp 1251");            // настраиваем кодировку консоли
    std::system("cls");

    vector<pair<int, vector<string>>> tests;
    const std::string IN_FILE = "../tests.txt";

    cout << "Чтение файла: \"" << IN_FILE << "\".\n";
    std::ifstream file(IN_FILE);
    if (file.is_open()) {
        int line(0), answer(0);
        while (file >> line >> answer) {
            vector<string> test;
            while (line) {
                string str;
                if (file >> str) {
                    test.push_back(str);
                }
                else break;
                --line;
            }
            if (file.good()) tests.push_back({ answer, test });
            else break;
        }
        
        if (file.bad())
            std::cout << "Ошибка ввода-вывода при чтении файла!\n";
        else if (file.eof())
            std::cout << "Чтение успешно.\n";
        else if (file.fail())
            std::cout << "Неверный формат данных!\n";
        
        file.close();
    }
    else {
        cout << "Нет такого файла!\n";
    }

    for (const auto& [answer, data] : tests) {
        cout << "\nМатрица...\n";
        for (const auto& str : data) {
            cout << '\t' << str << '\n';
        }
        cout << "Правильно: " << answer << '\n';
        cout << "Решение  : ";

        int result = twoPluses(data);
        std::cout << result << "\n";
    }

    return 0;
}
