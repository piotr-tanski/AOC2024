#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <set>
#include <fstream>
#include <iostream>
#include <queue>
#include <array>
#include <functional>

std::vector<std::vector<char>> load_map()
{
    std::ifstream input;
    input.open("./input.txt");
    if (!input.is_open()) throw std::runtime_error("Error opening file");

    std::vector<std::vector<char>> map;
    while (input)
    {
        std::string line;
        std::getline(input, line);
        if (line.empty()) continue;

        map.emplace_back();
        for (auto c : line) map.back().push_back(c);
    }
    return map;
}

std::pair<int, int> find_guard_location(std::vector<std::vector<char>>& map)
{
    for (int i = 0; i < map.size(); i++)
    {
        for (int j = 0; j < map[i].size(); j++)
        {
            if (map[i][j] == '^')
            {
                return {i, j};
            }
        }
    }
    return {-1, -1};
}

static constexpr std::array<std::pair<int, int>, 4> directions = {std::make_pair(-1, 0), {0, 1}, {1, 0}, {0, -1}};

struct TraversingInfo
{
    int x = -1;
    int y = -1;
    int directionIndex = -1;
};

bool move(std::pair<int, int>& position, int& direction, std::vector<std::vector<char>>& map)
{
    std::pair next = {position.first + directions[direction].first, position.second + directions[direction].second};
    if (next.first < 0 || next.first >= map.size() || next.second < 0 || next.second >= map[0].size())
    {
        return false;
    }

    int originalDirection = direction;
    while (map[next.first][next.second] == '#')
    {
        direction = (direction + 1) % directions.size();
        if (originalDirection == direction) return false;

        next = {position.first + directions[direction].first, position.second + directions[direction].second};
        if (next.first < 0 || next.first >= map.size() || next.second < 0 || next.second >= map[0].size())
        {
            return false;
        }
    }
    position = next;
    return true;
}

bool checkCycle(std::vector<std::vector<char>>& map, std::pair<int, int> origin, int direction)
{
    std::pair<int, int> slow = origin;
    std::pair<int, int> fast = origin;
    int slowDirection = direction;
    int fastDirection = direction;

    if (!move(slow, slowDirection, map)) return false;
    if (!move(fast, fastDirection, map)) return false;
    if (!move(fast, fastDirection, map)) return false;

    auto areEqual = [](std::pair<int, int>& position1, std::pair<int, int>& position2) { return position1.first == position2.first && position1.second == position2.second; };
    while (!areEqual(slow, fast))
    {
        if (!move(slow, slowDirection, map)) return false;
        if (!move(fast, fastDirection, map)) return false;
        if (!move(fast, fastDirection, map)) return false;
    }
    return true;
}

int predict_guard_path(std::vector<std::vector<char>>& map)
{
    auto guard = find_guard_location(map);

    std::vector<std::pair<int, int>> new_obstacles;
    std::queue<TraversingInfo> q;
    q.push(TraversingInfo{guard.first, guard.second, 0});
    while (!q.empty())
    {
        auto t = q.front();
        q.pop();

        map[t.x][t.y] = 'X';

        int nextX = t.x + directions[t.directionIndex].first;
        int nextY = t.y + directions[t.directionIndex].second;
        if (nextX < 0 || nextX >= map.size() || nextY < 0 || nextY >= map[0].size()) break;

        if (map[nextX][nextY] == '#')
        {
            int newDirection = (t.directionIndex + 1) % directions.size();
            q.push(TraversingInfo{t.x, t.y, newDirection});
        }
        else
        {
            if (!(nextX == guard.first && nextY == guard.second))
            {
                char tmp = map[nextX][nextY];
                map[nextX][nextY] = '#';
                if (checkCycle(map, {t.x, t.y}, (t.directionIndex + 1) % directions.size()))
                {
                    new_obstacles.push_back({nextX, nextY});
                }
                map[nextX][nextY] = tmp;
            }

            q.push(TraversingInfo{nextX, nextY, t.directionIndex});
        }
    }
    // for (auto o : new_obstacles)
    // {
    //     map[o.first][o.second] = 'O';
    // }
    // map[guard.first][guard.second] = 'G';
    return new_obstacles.size();
}

int count_visited_positions(std::vector<std::vector<char>>& map)
{
    int count = 0;
    for (int i = 0; i < map.size(); i++)
    {
        for (int j = 0; j < map[i].size(); j++)
        {
            if (map[i][j] == 'X') count++;
        }
    }
    return count;
}

int main()
{
    auto map = load_map();
    std::cout << predict_guard_path(map) << std::endl;
    std::cout << count_visited_positions(map) << std::endl;

    for (int i = 0; i < map.size(); i++)
    {
        for (int j = 0; j < map[i].size(); j++)
            std::cout << map[i][j];
        std::cout << std::endl;
    }
    return 0;
}