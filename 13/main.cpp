#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <array>
#include <functional>
#include <iterator>
#include <stack>
#include <sstream>
#include <valarray>
#include <list>
#include <queue>

struct ButtonConfiguration
{
    int x = 0;
    int y = 0;
};

struct Configuration
{
    ButtonConfiguration button_a;
    ButtonConfiguration button_b;
    std::pair<int, int> prizeLocation;
};

std::vector<Configuration> load_configurations()
{
    std::ifstream input;
    input.open("./input.txt");
    if (!input.is_open()) throw std::runtime_error("Error opening file");

    std::vector<Configuration> configurations;
    Configuration currentConfiguration;
    while (input)
    {
        std::string line;
        std::getline(input, line);
        if (line.empty())
        {
            configurations.push_back(currentConfiguration);
            continue;
        }

        if (line.find("Button A") != std::string::npos)
        {
            auto x_start = line.find("X+");
            auto x_end = line.find(",", x_start);
            auto x = line.substr(x_start + 2, x_end - x_start - 2);

            auto y_start = line.find("Y+");
            auto y = line.substr(y_start + 2);

            currentConfiguration.button_a.x = stoi(x);
            currentConfiguration.button_a.y = stoi(y);
        }
        else if (line.find("Button B") != std::string::npos)
        {
            auto x_start = line.find("X+");
            auto x_end = line.find(",", x_start);
            auto x = line.substr(x_start + 2, x_end - x_start - 2);

            auto y_start = line.find("Y+");
            auto y = line.substr(y_start + 2);

            currentConfiguration.button_b.x = stoi(x);
            currentConfiguration.button_b.y = stoi(y);
        }
        else if (line.find("Prize") != std::string::npos)
        {
            auto x_start = line.find("X=");
            auto x_end = line.find(",", x_start);
            auto x = line.substr(x_start + 2, x_end - x_start - 2);

            auto start = line.find("Y=");
            auto y = line.substr(start + 2);

            currentConfiguration.prizeLocation.first = stoi(x);
            currentConfiguration.prizeLocation.second = stoi(y);
        }
    }
    return configurations;
}

int minimum_tokens(Configuration& configuration)
{
    std::vector<std::vector<int>> dp(configuration.prizeLocation.first + 1, std::vector<int>(configuration.prizeLocation.second + 1, 0));

    int prev_x = 0;
    int prev_y = 0;
    int counter = 1;
    while (true)
    {
        const int x = counter * configuration.button_a.x;
        const int y = counter * configuration.button_a.y;
        if (x >= dp.size() || y >= dp[0].size()) break;

        dp[x][y] = dp[prev_x][prev_y] + 3;
        prev_x = x;
        prev_y = y;
        ++counter;
    }

    for (int i = 0; i < dp.size(); ++i)
    {
        for (int j = 0; j < dp[0].size(); ++j)
        {
            if (dp[i][j] != 0)
            {
                int initCost = dp[i][j];
                counter = 1;
                while (true)
                {
                    const int x = i + counter * configuration.button_b.x;
                    const int y = j + counter * configuration.button_b.y;
                    const int cost = initCost + counter;
                    if (x >= dp.size() || y >= dp[0].size()) break;

                    dp[x][y] = dp[x][y] == 0 ? cost : std::min(dp[x][y], cost);
                    ++counter;
                }
            }
        }
    }

    int ret = dp[configuration.prizeLocation.first][configuration.prizeLocation.second];
    return ret;
}

int minimum_tokens(std::vector<Configuration>& configurations)
{
    int sum = 0;
    for (auto& config : configurations)
    {
        sum += minimum_tokens(config);
        std::cout << "sum: " << sum << std::endl;
    }
    return sum;
}

int main()
{
    auto configurations = load_configurations();
    std::cout << minimum_tokens(configurations) << '\n';
    return 0;
}