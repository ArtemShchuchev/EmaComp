// https://www.hackerrank.com/challenges/two-pluses/problem?utm_campaign=challenge-recommendation&utm_medium=email&utm_source=24-hour-campaign

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

/*
 * Complete the 'twoPluses' function below.
 *
 * The function is expected to return an INTEGER.
 * The function accepts STRING_ARRAY grid as parameter.
 */

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

    int findMaxSquare(const CrossPoint& cp) const {
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

static vector<CrossPoint> findCross(const vector<string>& grid) {
    vector<CrossPoint> cross;
    auto findRadiusCross = [&grid, &cross](const int l, const int r, const int rad) {
            int radius(-1);

            if (grid[l][r] == 'G') {
                ++radius;

                while (++radius <= rad) {
                    if (grid[l - radius][r] != 'G') break;
                    if (grid[l][r + radius] != 'G') break;
                    if (grid[l + radius][r] != 'G') break;
                    if (grid[l][r - radius] != 'G') break;
                }

                --radius;
            }

            if (radius >= 0)
                if (radius > 0 || cross.size() < 2)
                    cross.push_back({ l, r, radius });
        };

    int sizeRow(static_cast<int>(grid[0].size()));
    int sizeLine(static_cast<int>(grid.size()));
    int minSize(min(sizeRow, sizeLine));
    int maxCrossing(minSize % 2 ? minSize : minSize - 1);
    int level(maxCrossing / 2);

    while (level >= 0) {
        int lMin = level;
        int rMin = level;
        int lMax = sizeLine - lMin - 1;
        int rMax = sizeRow - rMin - 1;

        // horizontal
        for (int r(rMin); r <= rMax; ++r) {
            findRadiusCross(lMin, r, level);
            if (lMin != lMax) findRadiusCross(lMax, r, level);
        }
        // vertikal
        ++lMin;
        --lMax;
        for (int l(lMin); l <= lMax; ++l) {
            findRadiusCross(l, rMin, level);
            if (rMin != rMax) findRadiusCross(l, rMax, level);
        }

        --level;
    }

    return cross;
}

static int twoPluses(const vector<string>& grid) {
    int answer(0);
    vector<CrossPoint> crossing{ findCross(grid) };

    auto lastIt(prev(crossing.cend()));
    for (auto it1(crossing.cbegin()); it1 != lastIt; ++it1) {
        for (auto it2(next(it1)); it2 != crossing.cend(); ++it2) {
            int square(it1->findMaxSquare(*it2));
            if (answer < square) answer = square;
        }
    }

    return answer;
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
