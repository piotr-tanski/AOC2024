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

std::vector<int> load_input()
{
    std::ifstream input;
    input.open("./input.txt");
    if (!input.is_open()) throw std::runtime_error("Error opening file");

    std::vector<int> inputs;
    while (input)
    {
        std::string line;
        std::getline(input, line);
        if (line.empty()) continue;

        for (const int num : line)
          inputs.push_back(num - '0');
    }
    return inputs;
}

std::vector<int> memory_layout(std::vector<int>& input)
{
    int fileId = 0;
    bool isFile = true;

    std::vector<int> layout;
    for (const int block_size : input)
    {
        if (isFile)
        {
            for (int i = 0; i < block_size; i++)
                layout.push_back(fileId);
            fileId++;
        }
        else
        {
            for (int i = 0; i < block_size; i++)
                layout.push_back(-1);
        }
        isFile = !isFile;
    }
    return layout;
}

void compact(std::vector<int>& memory)
{
    int i = 0;
    int j = memory.size() - 1;

    auto findNextEmptySlot = [&memory, i, j]() mutable
    {
        while (i <= j)
        {
            if (memory[i] == -1) return i;
            ++i;
        }
        return -1;
    };
    auto findNextFile = [&memory, i, j]() mutable
    {
        while (i <= j)
        {
            if (memory[j] != -1) return j;
            --j;
        }
        return -1;
    };

    while (true)
    {
        i = findNextEmptySlot();
        j = findNextFile();
        if (i == -1 || j == -1 || i >= j) break;
        std::swap(memory[i], memory[j]);
    }
}

void smart_compact(std::vector<int>& memory)
{
    auto findNextEmptySlot = [&memory](int pos) mutable
    {
        int i = pos;
        while (i < memory.size())
        {
            if (memory[i] == -1) return i;
            ++i;
        }
        return -1;
    };
    auto findNextFile = [&memory](int pos) mutable
    {
        int j = pos;
        while (j >= 0)
        {
            if (memory[j] != -1) return j;
            --j;
        }
        return -1;
    };
    auto lengthOfBlock = [&memory](int blockStart, bool reverse = false)
    {
        auto id = memory[blockStart];
        int k = blockStart;
        int size = 0;
        while (id == memory[k])
        {
            size++;
            if (reverse) k--;
            else k++;
        }
        return size;
    };

    int j = memory.size() - 1;
    while (j > 0)
    {
        int i = 0;
        j = findNextFile(j);
        if (j == -1) return;

        int fileSize = lengthOfBlock(j, true);
        j -= fileSize;

        int freeSpace = INT_MIN;
        while (fileSize > freeSpace)
        {
            i = findNextEmptySlot(i);
            if (i == -1 || i > j) break;

            freeSpace = lengthOfBlock(i);
            i += freeSpace;
        }
        if (freeSpace < fileSize) continue;

        i -= freeSpace;
        for (int k = 0; k < fileSize; k++)
        {
            std::swap(memory[i + k], memory[j + k + 1]);
        }
    }
}

unsigned long long int checksum(std::vector<int>& memory)
{
    unsigned long long int checksum = 0;
    for (int i = 0; i < memory.size(); i++)
    {
        if (memory[i] != -1)
            checksum += memory[i] * i;
    }
    return checksum;
}

int main()
{
    auto input = load_input();
    auto layout = memory_layout(input);
    smart_compact(layout);
    std::cout << checksum(layout) << std::endl;

    return 0;
}