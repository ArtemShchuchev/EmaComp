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
};

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
    vector<int> answer;
    multimap<int, pair<int, int>> allCross;
    vector<vector<pair<int, int>>> coordsOnLevel{ findCoord(grid) };
    int level(static_cast<int>(coordsOnLevel.size() - 1));
    while (level >= 0) {
        for (const auto& [l, r] : coordsOnLevel[level]) {
            int tempRad = findRadiusCross(grid, l, r, level);
            if (tempRad >= 0) allCross.insert({ tempRad, {l, r} });
        }
        --level;
    }

    auto lastIter(prev(allCross.crend()));
    for (auto it1(allCross.crbegin()); it1 != lastIter; ++it1) {
        auto& [rad1, coord1] = *it1;
        auto& [line1, row1] = coord1;
        CrossPoint cp1(line1, row1, rad1);
        //cout << line1 << ", " << row1 << " - " << rad1 << '\n';
        //cout << "---------\n";
        for (auto it2(allCross.crbegin()); it2 != allCross.crend(); ++it2) {
            auto& [rad2, coord2] = *it2;
            auto& [line2, row2] = coord2;
            CrossPoint cp2(line2, row2, rad2);

            if (it1 == it2) continue;
            else if (*it1 > *it2) cp1.radius = cp2.radius;

            if (!cp1.crossing(cp2)) {
                int square1 = cp1.radius * 4 + 1;
                int square2 = cp2.radius * 4 + 1;
                answer.push_back(square1 * square2);
                //cout << "---------\n";
                //cout << line1 << ", " << row1 << " - " << rad1 << '\n';
                //cout << line2 << ", " << row2 << " - " << rad2 << '\n';
                //cout << "---------\n";
                break;
            }
            //cout << line2 << ", " << row2 << " - " << rad2 << '\n';
        }
        //cout << "##################\n";
    }

    return *max_element(answer.begin(), answer.end());
}


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
