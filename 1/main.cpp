#include <fstream>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>

std::pair<std::vector<int>, std::vector<int>> read_input()
{
    std::ifstream input;
    input.open("./input.txt");
    if (!input.is_open()) throw std::runtime_error("Error opening file");

    std::vector<int> list1;
    std::vector<int> list2;
    while (input)
    {
        std::string line;
        std::getline(input, line);
        if (line.empty()) continue;

        const auto pos = line.find(' ');
        const auto val1 = std::stoi(line.substr(0, pos));
        const auto val2 = std::stoi(line.substr(pos + 1));
        list1.push_back(val1);
        list2.push_back(val2);
    }
    return {list1, list2};
}

int distance(std::vector<int>& lhs, std::vector<int>& rhs)
{
    std::priority_queue<int, std::vector<int>, std::greater<>> left_pq;
    std::priority_queue<int, std::vector<int>, std::greater<>> right_pq;

    for (auto num : lhs) left_pq.push(num);
    for (auto num : rhs) right_pq.push(num);

    int sum = 0;
    while (!left_pq.empty() && !right_pq.empty())
    {
        const int v1 = left_pq.top();
        const int v2 = right_pq.top();
        left_pq.pop();
        right_pq.pop();
        sum += std::abs(v1 - v2);
    }
    return sum;
}

int similiarity(std::vector<int>& lhs, std::vector<int>& rhs)
{
    std::unordered_map<int, int> rhs_histogram;
    for (auto num : rhs) rhs_histogram[num]++;

    int similarity = 0;
    for (auto num : lhs)
    {
        similarity += num * rhs_histogram[num];
    }
    return similarity;
}

int main()
{
    auto [list1, list2] = read_input();
    std::cout << distance(list1, list2) << std::endl;
    std::cout << similiarity(list1, list2) << std::endl;
    return 0;
}
