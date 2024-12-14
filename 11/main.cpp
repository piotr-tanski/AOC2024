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

std::vector<unsigned long long int> load_stones()
{
    std::ifstream input;
    input.open("./input.txt");
    if (!input.is_open()) throw std::runtime_error("Error opening file");

    std::vector<unsigned long long int> stones;
    while (input)
    {
        std::string line;
        std::getline(input, line);
        if (line.empty()) continue;

        std::istringstream buffer(line);
        for (auto it = std::istream_iterator<std::string>(buffer); it != std::istream_iterator<std::string>(); ++it)
        {
            stones.push_back(std::stoull(*it));
        }
    }
    return stones;
}

struct NumberSplit
{
    bool hasEvenNumberOfDigits = false;
    unsigned long long int left = 0;
    unsigned long long int right = 0;
};

NumberSplit split(unsigned long long int value)
{
    int digitsCount = 0;
    unsigned long long int tmp = value;
    while (tmp != 0)
    {
        tmp /= 10;
        digitsCount++;
    }

    const bool is_even = digitsCount % 2 == 0;
    if (!is_even) return {false, 0, 0};

    tmp = value;
    const int half = digitsCount / 2;

    unsigned long long int right = 0;
    for (int i = 0; i < half; i++)
    {
        right += (tmp % 10) * std::pow(10, i);
        tmp = tmp / 10;
    }

    unsigned long long int left = 0;
    for (int i = 0; i < half; i++)
    {
        left += (tmp % 10) * std::pow(10, i);
        tmp = tmp / 10;
    }
    return {is_even, left, right};
}

std::vector<unsigned long long int> blink(unsigned long long int stone)
{
    std::vector<unsigned long long int> newStones;
    if (stone == 0)
    {
        newStones.push_back(1);
    }
    else if (const auto [has_even_no_digits, left, right] = split(stone); has_even_no_digits)
    {
        newStones.push_back(left);
        newStones.push_back(right);
    }
    else
    {
        newStones.push_back(stone * 2024);
    }
    return newStones;
}

unsigned long long int dfs(std::vector<unsigned long long int>& stones, int current, int n)
{
    if (current == n + 1)
    {
        return 1;
    }

    unsigned long long int count = 0;
    for (auto num : stones)
    {
        auto vec = blink(num);
        count += dfs(vec, current + 1, n);
    }
    return count;
}

int main()
{
    auto stones = load_stones();
    unsigned long long int size = 0;
    for (auto stone : stones)
    {
        std::vector single{{stone}};
        auto partial_size = dfs(single, 0, 25);
        size += partial_size;
    }
    std::cout << size << std::endl;
    return 0;
}