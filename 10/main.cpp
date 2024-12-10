#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <array>
#include <functional>
#include <iterator>
#include <stack>

struct pair_hash
{
    int operator()(const std::pair<int, int>& p) const
    {
        return std::hash<int>()(p.first) ^ std::hash<int>()(p.second);
    }
};

std::vector<std::vector<int>> load_map()
{
    std::ifstream input;
    input.open("./input.txt");
    if (!input.is_open()) throw std::runtime_error("Error opening file");

    std::vector<std::vector<int>> map;
    while (input)
    {
        std::string line;
        std::getline(input, line);
        if (line.empty()) continue;

        map.emplace_back();
        for (const int num : line)
          map.back().push_back(num - '0');
    }
    return map;
}

std::vector<std::pair<int, int>> find_trailheads(std::vector<std::vector<int>>& map)
{
    std::vector<std::pair<int, int>> trailheads;
    for (int i = 0; i < map.size(); i++)
    {
        for (int j = 0; j < map[i].size(); j++)
            if (map[i][j] == 0) trailheads.emplace_back(i, j);
    }
    return trailheads;
}

int trailhead_score(int from_x, int from_y, std::vector<std::vector<int>>& map)
{
    static constexpr std::array<std::pair<int, int>, 4> directions = {std::make_pair(0, 1), {1, 0}, {0, -1}, {-1, 0}};

    int score = 0;
    std::unordered_map<std::pair<int, int>, bool, pair_hash> summit_visited;
    std::stack<std::pair<int, int>> stack;
    stack.emplace(from_x, from_y);

    while (!stack.empty())
    {
        auto [x, y] = stack.top();
        stack.pop();

        const int current_height = map[x][y];
        if (current_height == 9)
        {
            if (summit_visited[{x, y}]) continue;

            score++;
            summit_visited[{x, y}] = true;
        }

        for (auto dir : directions)
        {
            int new_x = x + dir.first;
            int new_y = y + dir.second;
            if (new_x < 0 || new_x >= map.size() || new_y < 0 || new_y >= map[0].size()) continue;
            if (map[new_x][new_y] != current_height + 1) continue;

            stack.emplace(new_x, new_y);
        }
    }
    return score;
}

int trailhead_rating(int from_x, int from_y, std::vector<std::vector<int>>& map)
{
    static constexpr std::array<std::pair<int, int>, 4> directions = {std::make_pair(0, 1), {1, 0}, {0, -1}, {-1, 0}};

    int score = 0;
    std::stack<std::pair<int, int>> stack;
    stack.emplace(from_x, from_y);

    while (!stack.empty())
    {
        auto [x, y] = stack.top();
        stack.pop();

        const int current_height = map[x][y];
        if (current_height == 9)
            score++;

        for (auto dir : directions)
        {
            int new_x = x + dir.first;
            int new_y = y + dir.second;
            if (new_x < 0 || new_x >= map.size() || new_y < 0 || new_y >= map[0].size()) continue;
            if (map[new_x][new_y] != current_height + 1) continue;

            stack.emplace(new_x, new_y);
        }
    }
    return score;
}

int score(std::vector<std::vector<int>>& map)
{
    int score = 0;
    for (const auto trailhead : find_trailheads(map))
    {
        score += trailhead_score(trailhead.first, trailhead.second, map);
    }
    return score;
}

int rating(std::vector<std::vector<int>>& map)
{
    int rating = 0;
    for (const auto trailhead : find_trailheads(map))
    {
        rating += trailhead_rating(trailhead.first, trailhead.second, map);
    }
    return rating;
}

int main()
{
    auto map = load_map();
    std::cout << score(map) << '\n';
    std::cout << rating(map) << '\n';
    return 0;
}