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

struct Input
{
  unsigned long long int target;
  std::vector<int> nums;
};

std::vector<Input> load_input()
{
    std::ifstream input;
    input.open("./input.txt");
    if (!input.is_open()) throw std::runtime_error("Error opening file");

    std::vector<Input> inputs;
    while (input)
    {
        std::string line;
        std::getline(input, line);
        if (line.empty()) continue;

        if (auto pos = line.find(':'); pos != std::string::npos)
        {
            inputs.emplace_back();
            inputs.back().target = std::strtoull(line.substr(0, pos).c_str(), nullptr, 10);

            std::istringstream buffer(line.substr(pos + 1));
            for (auto it = std::istream_iterator<std::string>(buffer); it != std::istream_iterator<std::string>(); ++it)
            {
                int value = std::stoi(*it);
                inputs.back().nums.push_back(value);
            }
        }
    }
    return inputs;
}

unsigned long long int concatenate(unsigned long long int num1, unsigned long long int num2)
{
    const std::string ret = std::to_string(num1) + std::to_string(num2);
    return std::stoull(ret);
}

bool backtrack(std::vector<int>& nums, int index, unsigned long long int currentSum, unsigned long long int target)
{
    if (index == nums.size())
    {
        if (currentSum == target) return true;
        return false;
    }

    return backtrack(nums, index + 1, currentSum + nums[index], target)
        || backtrack(nums, index + 1, currentSum * nums[index], target)
        || backtrack(nums, index + 1, concatenate(currentSum, nums[index]), target);
}

bool isEquationTrue(Input& input)
{
    auto target = input.target;
    return backtrack(input.nums, 0, 0, target);
}

unsigned long long int calibration_result(std::vector<Input>& input)
{
    unsigned long long int ret = 0;
    for (auto in : input)
    {
        if (isEquationTrue(in)) ret += in.target;
    }
    return ret;
}

int main()
{
    auto input = load_input();
    std::cout << calibration_result(input) << std::endl;
    return 0;
}