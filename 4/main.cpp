#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <vector>
#include <array>

std::vector<std::vector<char>> load_words()
{
    std::ifstream input;
    input.open("./input.txt");
    if (!input.is_open()) throw std::runtime_error("Error opening file");

    std::vector<std::vector<char>> words;
    while (input)
    {
        std::string line;
        std::getline(input, line);
        if (line.empty()) continue;

        words.emplace_back();
        for (auto c : line) words.back().push_back(c);
    }
    return words;
}

struct LetterFound
{
    int x = -1;
    int y = -1;
    int letterIndex = 0;
    std::pair<int, int> direction = {0, 0};
};

static constexpr std::array<std::pair<int, int>, 8> directions = {
    std::make_pair(0, 1),
    {1, 1},
    {1, 0},
    {1, -1},
    {0, -1},
    {-1, -1},
    {-1, 0},
    {-1, 1}
};

int bfs(std::vector<std::vector<char>>& words, int x, int y, std::string& word)
{
    int wordCount = 0;
    std::queue<LetterFound> q;
    q.push(LetterFound{x, y, 0});
    while (!q.empty())
    {
        const auto [currX, currY, index, direction] = q.front();
        q.pop();

        if (index == word.size() - 1)
        {
            wordCount++;
            continue;
        }

        if (direction.first == 0 && direction.second == 0)
        {
            for (auto dir : directions)
            {
                int newX = currX + dir.first;
                int newY = currY + dir.second;
                if (newX < 0 || newX >= words.size() || newY < 0 || newY >= words[0].size()) continue;
                if (word[index + 1] != words[newX][newY]) continue;
                q.push(LetterFound{newX, newY, index + 1, dir});
            }
        }
        else
        {
            int newX = currX + direction.first;
            int newY = currY + direction.second;
            if (newX < 0 || newX >= words.size() || newY < 0 || newY >= words[0].size()) continue;
            if (word[index + 1] != words[newX][newY]) continue;
            q.push(LetterFound{newX, newY, index + 1, direction});
        }
    }
    return wordCount;
}

int count_occurrences(std::vector<std::vector<char>>& words, std::string word)
{
    int count = 0;
    for (int i = 0; i < words.size(); ++i)
    {
        for (int j = 0; j < words[i].size(); ++j)
        {
            if (words[i][j] == word[0])
            {
                count += bfs(words, i, j, word);
            }
        }
    }
    return count;
}

int count_x_mas(std::vector<std::vector<char>>& words)
{
    static constexpr std::array<std::pair<int, int>, 4> x_directions = {
        std::make_pair(-1, -1),
        {1, 1},
        {-1, 1},
        {1, -1}
    };

    std::vector page(words.size() + 2, std::vector<char>(words[0].size() + 2));
    for (int i = 0; i < page.size(); ++i)
    {
        for (int j = 0; j < page[i].size(); ++j)
        {
            if (i == 0 || j == 0 || i == page.size() - 1 || j == page[i].size() - 1) page[i][j] = '.';
            else page[i][j] = words[i - 1][j - 1];
        }
    }

    int count = 0;
    for (int i = 0; i < page.size(); ++i)
    {
        for (int j = 0; j < page[i].size(); ++j)
        {
            if (page[i][j] == 'A')
            {
                char upper_left = page[i + x_directions[0].first][j + x_directions[0].second];
                char down_right = page[i + x_directions[1].first][j + x_directions[1].second];
                char upper_right = page[i + x_directions[2].first][j + x_directions[2].second];
                char down_left = page[i + x_directions[3].first][j + x_directions[3].second];
                if (upper_left == 'M' && down_right == 'S')
                {
                    if (upper_right == 'M' && down_left == 'S') count++;
                    else if (upper_right == 'S' && down_left == 'M') count++;
                }
                else if (upper_left == 'S' && down_right == 'M')
                {
                    if (upper_right == 'M' && down_left == 'S') count++;
                    else if (upper_right == 'S' && down_left == 'M') count++;
                }
            }
        }
    }
    return count;
}

int main()
{
    auto word_search = load_words();
    std::cout << count_occurrences(word_search, "XMAS") << std::endl;
    std::cout << count_x_mas(word_search) << std::endl;
    return 0;
}