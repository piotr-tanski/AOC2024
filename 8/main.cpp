#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <set>
#include <fstream>
#include <iostream>
#include <queue>
#include <array>
#include <functional>
#include <sstream>
#include <iterator>

struct PairOfLocations
{
    std::pair<int, int> location1;
    std::pair<int, int> location2;

    bool operator==(const PairOfLocations& oth) const
    {
        return location1 == oth.location1 && location2 == oth.location2;
    }
};

struct PairOfLocationsHash
{
    int operator()(const PairOfLocations& other) const
    {
        return std::hash<int>()(other.location1.first) ^ std::hash<int>()(other.location1.second) ^
            std::hash<int>()(other.location2.first) ^ std::hash<int>()(other.location2.second);
    }
};

std::vector<std::vector<char>> load_antennas()
{
    std::ifstream input;
    input.open("./input.txt");
    if (!input.is_open()) throw std::runtime_error("Error opening file");

    std::vector<std::vector<char>> antennas;
    while (input)
    {
        std::string line;
        std::getline(input, line);
        if (line.empty()) continue;

        antennas.emplace_back();
        for (const char c : line)
          antennas.back().push_back(c);
    }
    return antennas;
}

std::vector<std::pair<int, int>> find_pairs(const std::vector<std::vector<char>>& antennas_map, std::pair<int, int> from, char frequency)
{
    std::vector<std::pair<int, int>> locations;
    for (int i = 0; i < antennas_map.size(); i++)
    {
        for (int j = 0; j < antennas_map[i].size(); j++)
        {
            if (antennas_map[i][j] == frequency)
            {
                if (from.first == i && from.second == j) continue; // skip if same antenna.
                locations.emplace_back(i, j);
            }
        }
    }
    return locations;
}

int place_antinodes(std::vector<std::vector<char>>& antennas_map, std::pair<int, int> location1, std::pair<int, int> location2)
{
    int noAntinodes = 0;

    int distance_x = location1.first - location2.first;
    int distance_y = location1.second - location2.second;
    int antinode_x = location1.first + distance_x;
    int antinode_y = location1.second + distance_y;

    if (antinode_x >= 0 && antinode_y >= 0 && antinode_x < antennas_map.size() && antinode_y < antennas_map[0].size())
    {
        if (antennas_map[antinode_x][antinode_y] != '#')
        {
            noAntinodes++;
            if (antennas_map[antinode_x][antinode_y] == '.')
                antennas_map[antinode_x][antinode_y] = '#';
        }
    }

    distance_x = location2.first - location1.first;
    distance_y = location2.second - location1.second;
    antinode_x = location2.first + distance_x;
    antinode_y = location2.second + distance_y;

    if (antinode_x >= 0 && antinode_y >= 0 && antinode_x < antennas_map.size() && antinode_y < antennas_map[0].size())
    {
        if (antennas_map[antinode_x][antinode_y] != '#')
        {
            noAntinodes++;
            if (antennas_map[antinode_x][antinode_y] == '.')
                antennas_map[antinode_x][antinode_y] = '#';
        }
    }

    return noAntinodes;
}

int count_antinodoes(std::vector<std::vector<char>>& antennas_map)
{
    std::unordered_map<PairOfLocations, bool, PairOfLocationsHash> visited_locations;
    int count = 0;
    for (int i = 0; i < antennas_map.size(); i++)
    {
        for (int j = 0; j < antennas_map[i].size(); j++)
        {
            if (const char freq = antennas_map[i][j]; freq != '.' && freq != '#')
            {
                auto curr_location = std::make_pair(i, j);
                for (auto location : find_pairs(antennas_map, curr_location, freq))
                {
                    if (visited_locations[PairOfLocations(curr_location, location)]) continue; // already visited.
                    if (visited_locations[PairOfLocations(location, curr_location)]) continue; // already visited.

                    count += place_antinodes(antennas_map, curr_location, location);
                    visited_locations[PairOfLocations(curr_location, location)] = true;
                    visited_locations[PairOfLocations(location, curr_location)] = true;
                }
            }
        }
    }
    return count;
}

int main()
{
    auto input = load_antennas();
    std::cout << count_antinodoes(input) << std::endl;

    for (auto row : input)
    {
        for (auto col : row)
            std::cout << col;
        std::cout << '\n';
    }
    return 0;
}