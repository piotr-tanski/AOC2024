#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <vector>

struct MulInstruction
{
  std::pair<int, int> params;
  bool enabled = true;
};

std::vector<std::string> read_memory()
{
  std::ifstream input;
  input.open("./input.txt");
  if (!input.is_open()) throw std::runtime_error("Error opening file");

  std::vector<std::string> memory_dump;
  while (input)
  {
    std::string line;
    std::getline(input, line);
    if (line.empty()) continue;

    memory_dump.push_back(line);
  }
  return memory_dump;
}

std::pair<int, int> get_mul_params(std::string& memory_line, size_t mul_position)
{
  size_t i = mul_position + 3;
  if (memory_line[i] != '(') return {};
  size_t opening_parenthesis = i;

  ++i;
  auto comma_pos = memory_line.find(',', i);
  if (comma_pos == std::string::npos || comma_pos - i > 3) return {};

  i = comma_pos + 1;
  auto closing_parenthesis = memory_line.find(')', i);
  if (closing_parenthesis == std::string::npos || closing_parenthesis - i > 3) return {};

  auto first = memory_line.substr(opening_parenthesis + 1, comma_pos - opening_parenthesis - 1);
  auto second = memory_line.substr(comma_pos + 1, closing_parenthesis - comma_pos - 1);
  return {std::stoi(first), std::stoi(second)};
}

std::pair<std::string, size_t> find_next_instruction(std::string& memory_line, size_t current_position)
{
  const auto pos1 = memory_line.find("mul", current_position);
  const auto pos2 = memory_line.find("do()", current_position);
  const auto pos3 = memory_line.find("don't()", current_position);
  if (pos1 == std::string::npos && pos2 == std::string::npos && pos3 == std::string::npos) return {"", std::string::npos};

  std::pair<std::string, size_t> p1{"mul", pos1};
  std::pair<std::string, size_t> p2{"do()", pos2};
  std::pair<std::string, size_t> p3{"don't()", pos3};
  return std::min({p1, p2, p3}, [](auto l, auto r) { return l.second < r.second; });
}

std::vector<MulInstruction> find_mul_instructions(std::vector<std::string>& memory)
{
  std::vector<MulInstruction> instructions;
  bool enable = true;
  for (auto& mem : memory)
  {
    size_t lastPos = 0;
    while (true)
    {
      const auto [instruction, pos] = find_next_instruction(mem, lastPos);
      if (pos == std::string::npos) break;

      if (instruction == "do()")
      {
        enable = true;
      }
      else if (instruction == "don't()")
      {
        enable = false;
      }
      else if (instruction == "mul")
      {
        const auto params = get_mul_params(mem, pos);
        instructions.emplace_back(params, enable);
      }
      lastPos = pos + 1;
    }
  }
  return instructions;
}

int calculate(std::vector<MulInstruction>& instructions)
{
  int result = 0;
  for (const auto& [params, isEnabled] : instructions)
  {
    if (isEnabled)
      result += params.first * params.second;
  }
  return result;
}

int main()
{
  auto memory = read_memory();
  auto mul_instructions = find_mul_instructions(memory);
  std::cout << calculate(mul_instructions) << std::endl;
  return 0;
}
