#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

std::vector<std::vector<int>> read_reports()
{
  std::ifstream input;
  input.open("./input.txt");
  if (!input.is_open()) throw std::runtime_error("Error opening file");

  std::vector<std::vector<int>> reports;
  while (input)
  {
    std::string line;
    std::getline(input, line);
    if (line.empty()) continue;

    std::istringstream buffer(line);
    std::vector<int> report;
    for (auto it = std::istream_iterator<std::string>(buffer); it != std::istream_iterator<std::string>(); ++it)
    {
      int value = std::stoi(*it);
      report.push_back(value);
    }
    reports.push_back(report);
  }
  return reports;
}

bool is_safe(std::vector<int>& report)
{
  std::vector dp(report.size() - 1, 0);
  for (int i = 1; i < report.size(); ++i)
  {
    const int diff = report[i] - report[i - 1];
    if (std::abs(diff) > 3) return false;
    dp[i - 1] = diff;
  }
  return std::all_of(dp.begin(), dp.end(), [](int i) { return i < 0; }) || std::all_of(dp.begin(), dp.end(), [](int i) { return i > 0; });;
}

int count_safe_reports(std::vector<std::vector<int>>& reports)
{
  int count = 0;
  for (auto& report : reports)
  {
    if (is_safe(report)) count++;
  }
  return count;
}

std::function<bool(int)> get_diff_comparator(std::vector<int>& report)
{
  int less_count = 0;
  int greater_count = 0;
  for (int i = 1; i < report.size() && i < 4; ++i)
  {
    if (report[i] < report[i - 1]) less_count++;
    else if (report[i] > report[i - 1]) greater_count++;
  }
  std::function comparator = greater_count > less_count
      ? [](int a) { return a > 0; }
      : [](int a) { return a < 0; };
  return comparator;
}

bool is_safe_with_problem_dampener(std::vector<int>& report, const std::function<bool(int)>& comparator, bool unsafe_removed = false)
{
  for (int i = 1; i < report.size(); ++i)
  {
    const int diff = report[i] - report[i - 1];
    const int abs_diff = std::abs(diff);
    if (!comparator(diff) || abs_diff < 1 || abs_diff > 3)
    {
      if (!unsafe_removed)
      {
        std::vector<int> tmp1 = report;
        tmp1.erase(tmp1.begin() + i - 1);
        if (is_safe_with_problem_dampener(tmp1, comparator, true)) return true;

        report.erase(report.begin() + i);
        if (is_safe_with_problem_dampener(report, comparator, true)) return true;
      }
      return false;
    }
  }
  return true;
}

int count_safe_reports_with_problem_dampener(std::vector<std::vector<int>>& reports)
{
  int count = 0;
  for (auto& report : reports)
  {
    if (is_safe_with_problem_dampener(report, get_diff_comparator(report))) count++;
  }
  return count;
}

int main()
{
  auto reports = read_reports();
  std::cout << count_safe_reports(reports) << std::endl;
  std::cout << count_safe_reports_with_problem_dampener(reports) << std::endl;
  return 0;
}
