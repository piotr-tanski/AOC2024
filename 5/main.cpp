#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <set>
#include <fstream>
#include <iostream>
#include <queue>

struct Input
{
    std::unordered_map<int, std::set<int>> rules;
    std::vector<std::vector<int>> lists;
};

Input load_input()
{
    std::ifstream input;
    input.open("./input.txt");
    if (!input.is_open()) throw std::runtime_error("Error opening file");

    Input in;
    while (input)
    {
        std::string line;
        std::getline(input, line);
        if (line.empty()) continue;

        if (int pos = line.find('|'); pos != std::string::npos) // Rule
        {
            int dependency = std::stoi(line.substr(0, pos));
            int page = std::stoi(line.substr(pos + 1));
            in.rules[page].insert(dependency);
        }
        else // List
        {
            in.lists.emplace_back();
            int commaPos = line.find(',');
            std::string num = line.substr(0, commaPos);
            in.lists.back().push_back(std::stoi(num));

            while (commaPos != std::string::npos)
            {
                auto num = line.substr(commaPos + 1, 2);
                in.lists.back().push_back(std::stoi(num));
                commaPos = line.find(',', commaPos + 1);
            }
        }
    }
    return in;
}

bool rules_fulfilled(std::vector<int>& ordered_pages, std::unordered_map<int, std::set<int>>& rules)
{
    std::unordered_map<int, bool> pages;
    for (auto item : ordered_pages)
        pages[item] = false;

    for (auto& item : ordered_pages)
    {
        for (auto& prerequisite : rules[item])
        {
            if (pages.contains(prerequisite) && pages[prerequisite] == false)
            {
                return false;
            }
        }
        pages[item] = true;
    }
    return true;
}

struct FilteringResults
{
    std::vector<std::vector<int>> ok;
    std::vector<std::vector<int>> filtered_out;
};

FilteringResults filter_by_rules(Input& input)
{
    FilteringResults results;
    for (auto& list : input.lists)
    {
        if (rules_fulfilled(list, input.rules))
        {
            results.ok.push_back(list);
        }
        else
        {
            results.filtered_out.push_back(list);
        }
    }
    return results;
}

int sum_midpoints(std::vector<std::vector<int>>& lists)
{
    int result = 0;
    for (auto& list : lists)
    {
        result += list[list.size() / 2];
    }
    return result;
}

std::pair<bool, std::vector<int>> correct_list(std::vector<int>& list, std::unordered_map<int, std::set<int>>& rules)
{
    // topological sort
    std::unordered_map<int, int> in_degrees;
    for (auto item : list)
        in_degrees[item] = 0;

    std::unordered_map<int, std::vector<int>> adjacencyList;
    for (const auto& [page, prerequisites] : rules)
    {
        if (!in_degrees.contains(page)) continue;

        for (int prerequisite : prerequisites)
        {
            if (!in_degrees.contains(prerequisite)) continue;

            adjacencyList[prerequisite].push_back(page);
            in_degrees[page]++;
        }
    }

    std::unordered_map<int, bool> visited;
    auto find_zero = [&in_degrees, &visited]()
    {
        for (auto [page, num] : in_degrees)
        {
            if (visited[page]) continue;
            if (num == 0) return page;
        }
        return -1;
    };

    std::vector<int> ret;
    std::queue<int> q;
    q.push(find_zero());
    while (!q.empty())
    {
        auto item = q.front();
        q.pop();
        if (item == -1) continue;

        ret.push_back(item);
        visited[item] = true;
        for (auto neighbours : adjacencyList[item])
        {
            in_degrees[neighbours]--;
        }
        q.push(find_zero());
    }

    for (auto [page, num] : in_degrees)
    {
        if (num != 0) return {false, {}};
    }
    return {true, ret};
}

std::vector<std::vector<int>> correct_lists(std::vector<std::vector<int>>& lists, std::unordered_map<int, std::set<int>>& rules)
{
    std::vector<std::vector<int>> ret;
    for (auto& list : lists)
    {
        if (auto [success, corrected_list] = correct_list(list, rules); success)
        {
            ret.push_back(corrected_list);
        }
    }
    return ret;
}

int main()
{
    Input input = load_input();
    auto filtered = filter_by_rules(input);
    std::cout << sum_midpoints(filtered.ok) << std::endl;

    auto corrected_lists = correct_lists(filtered.filtered_out, input.rules);
    std::cout << sum_midpoints(corrected_lists) << std::endl;
    return 0;
}