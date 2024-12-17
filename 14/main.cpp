#include <vector>
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <array>
#include <iterator>
#include <queue>
#include <set>

struct Robot
{
    int id;
    std::pair<int, int> position;
    std::pair<int, int> velocity;
};

std::vector<Robot> load_robots()
{
    std::ifstream input;
    input.open("./input.txt");
    if (!input.is_open()) throw std::runtime_error("Error opening file");

    int id = 1;
    std::vector<Robot> robots;
    while (input)
    {
        std::string line;
        std::getline(input, line);
        if (line.empty()) continue;

        Robot robot;
        robot.id = id;
        {
            auto pos_start = line.find("p=");
            auto pos_comma = line.find(',', pos_start + 1);
            auto pos_x = line.substr(pos_start + 2, pos_comma - (pos_start + 2));
            auto pos_y = line.substr(pos_comma + 1, line.find(' ', pos_start) - (pos_comma + 1));
            robot.position = {std::stoi(pos_y), std::stoi(pos_x)};
        }
        {
            auto v_start = line.find("v=");
            auto v_comma = line.find(',', v_start + 1);
            auto v_x = line.substr(v_start + 2, v_comma - (v_start + 2));
            auto v_y = line.substr(v_comma + 1, line.find(' ', v_start) - (v_comma + 1));
            robot.velocity = {std::stoi(v_y), std::stoi(v_x)};
        }
        robots.push_back(robot);
        id++;
    }
    return robots;
}

void place_robots(std::vector<std::vector<int>>& map, std::vector<Robot>& robots)
{
    for (auto& robot : robots)
    {
        map[robot.position.first][robot.position.second] += 1;
    }
}

void move_robot(Robot& robot, std::vector<std::vector<int>>& map)
{
    map[robot.position.first][robot.position.second] -= 1;

    const int m = map.size();
    const int n = map[0].size();

    int new_x = robot.position.first + robot.velocity.first;
    if (new_x >= m)     new_x = new_x % m;
    else if (new_x < 0) new_x = m + new_x % m;

    int new_y = robot.position.second + robot.velocity.second;
    if (new_y >= n)     new_y = new_y % n;
    else if (new_y < 0) new_y = n + new_y % n;

    robot.position.first = new_x;
    robot.position.second = new_y;
    map[robot.position.first][robot.position.second] += 1;
}

int count_robots_in_quadrant1(std::vector<std::vector<int>>& map)
{
    int count = 0;
    for (int i = 0; i < map.size() / 2; i++)
    {
        for (int j = 0; j < map[0].size() / 2; j++)
            count += map[i][j];
    }
    return count;
}

int count_robots_in_quadrant2(std::vector<std::vector<int>>& map)
{
    int count = 0;
    for (int i = map.size() / 2 + 1; i < map.size(); i++)
    {
        for (int j = 0; j < map[0].size() / 2; j++)
            count += map[i][j];
    }
    return count;
}

int count_robots_in_quadrant3(std::vector<std::vector<int>>& map)
{
    int count = 0;
    for (int i = 0; i < map.size() / 2; i++)
    {
        for (int j = map[0].size() / 2 + 1; j < map[0].size(); j++)
            count += map[i][j];
    }
    return count;
}

int count_robots_in_quadrant4(std::vector<std::vector<int>>& map)
{
    int count = 0;
    for (int i = map.size() / 2 + 1; i < map.size(); i++)
    {
        for (int j = map[0].size() / 2 + 1; j < map[0].size(); j++)
            count += map[i][j];
    }
    return count;
}

void dump(std::vector<std::vector<int>>& map, int sec)
{
    std::string filename = "map.dump." + std::to_string(sec) + ".txt";
    std::ofstream o(filename);

    for (int i = 0; i < map.size(); i++)
    {
        for (int j = 0; j < map[0].size(); j++)
        {
            if (map[i][j] > 0) o << '*';
            else o << '.';
        }
        o << '\n';
    }
    o << std::endl;
    o.close();
}

int main()
{
    // PART 1
    {
        std::vector<std::vector<int>> map(103, std::vector<int>(101, 0));
        auto robots = load_robots();
        place_robots(map, robots);

        for (int i = 0; i < 100; ++i)
        {
            for (auto& r : robots)
            {
                move_robot(r, map);
            }
        }
        int q1 = count_robots_in_quadrant1(map);
        int q2 = count_robots_in_quadrant2(map);
        int q3 = count_robots_in_quadrant3(map);
        int q4 = count_robots_in_quadrant4(map);
        std::cout << (q1 * q2 * q3 * q4) << std::endl;
    }

    // PART 2
    {
        std::vector<std::vector<int>> map(103, std::vector<int>(101, 0));
        auto robots = load_robots();
        place_robots(map, robots);

        for (int i = 0; i < 100000; ++i) // Place a big number here as an upper bound.
        {
            // Check if most of the robots are close to each other after move.
            std::set<std::pair<int, int>> nearby;
            std::vector<Robot> matching;
            for (auto& r : robots)
            {
                move_robot(r, map);

                if (nearby.contains(std::make_pair(r.position.first, r.position.second)))
                {
                    matching.push_back(r);
                }
                for (auto dx : {-1, 0, 1})
                {
                    for (auto dy : {-1, 0, 1})
                    {
                        nearby.insert(std::make_pair(r.position.first + dx, r.position.second + dy));
                    }
                }
            }

            if (matching.size() > 255)
            {
                // More than a half are close to each other.
                dump(map, i + 1); // Check the file if there's a christmas tree inside.
            }
        }
    }
    return 0;
}