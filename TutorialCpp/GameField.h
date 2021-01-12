#pragma once
#include "enums.h"
#include <string>
#include <vector>

struct Edge
{
    size_t x;
    size_t y;
    eDirection dir;
    std::string ToString() const
    {
        std::string strDir;
        switch (dir)
        {
        case eDirection::eLeft:
            strDir = "Left";
            break;
        case eDirection::eDown:
            strDir = "Down";
            break;
        case eDirection::eRight:
            strDir = "Right";
            break;
        case eDirection::eUp:
            strDir = "Up";
            break;
        default:
            break;
        }

        return std::to_string(x) + " " + std::to_string(y) + " " + strDir;
    }
};

struct Vertex
{
    size_t x;
    size_t y;

    bool operator<=(const Vertex& p) const
    {
        // dictionary order
        return ((x < p.x) ? true : ((x == p.x && y <= p.y) ? true : false));
    }

    bool operator == (const Vertex& v) const
    {
        return x == v.x && v.y == y;
    }

    bool operator>=(const Vertex& p) const
    {
        // dictionary order
        return ((x > p.x) ? true : ((x == p.x && y >= p.y) ? true : false));
    }

    bool operator<(const Vertex& p) const
    {
        // dictionary order
        return ((x < p.x) ? true : ((x == p.x && y < p.y) ? true : false));
    }
};

class GameLevel
{
public:

    GameLevel()
    {
        size_t height_ = 11;
        size_t weight_ = 11;
        field_ = new ptrdiff_t * [height_];
        for (size_t i = 0; i < height_; i++)
            field_[i] = new ptrdiff_t[weight_];

        for (size_t i = 0; i < height_; i++)
        {
            for (size_t j = 0; j < weight_; j++)
            {
                field_[i][j] = i_free;
            }
        }

        for (size_t j = 0; j < 2; j++)
        {
            for (size_t x = 0; x < height_; x++)
            {
                field_[j][x] = i_blocked;
                field_[height_ - j - 1][x] = i_blocked;

                field_[x][j] = i_blocked;
                field_[x][weight_ - j - 1] = i_blocked;
            }
        }


        field_[2][4] = i_blocked;
        field_[3][4] = i_blocked;
        field_[4][4] = i_blocked;
        field_[4][3] = i_blocked;

        field_[5][5] = i_blocked;
        field_[6][3] = i_blocked;

        field_[7][5] = i_blocked;
        field_[7][6] = i_blocked;

        field_[finish_.x][finish_.y] = i_free;

        InitializeScene();
    }

    void Draw()
    {
        size_t height_ = 11;
        size_t weight_ = 11;
        for (size_t i = 0; i < height_; i++)
        {
            for (size_t j = 0; j < weight_; j++)
            {
                std::cout << field_[i][j] << "\t";
            }
            std::cout << std::endl;
        }
    }

    void SetCur(eDirection dir)
    {
        auto& map  = gameField_[current_];
        if (!map.contains(dir))
            return;

        auto& vertex = map[dir];        
        prev_ = current_;

        current_.x = vertex.x;
        current_.y = vertex.y;

        field_[current_.x][current_.y] = person;
        field_[prev_.x][prev_.y] = i_free;

        if (current_ == finish_)
        {
            std::cout << "Game finished\n";
        }
    }

private:

    void InitializeScene()
    {
        size_t height_ = 11;
        size_t weight_ = 11;
        const ptrdiff_t dx[4] = { 0,1,0,-1 };
        const ptrdiff_t dy[4] = { -1,0,1,0 };
        eDirection dir[4] = { eDirection::eLeft,eDirection::eDown,eDirection::eRight,eDirection::eUp };
        for (size_t i = 2; i < height_; i++)
        {
            for (size_t j = 2; j < weight_; j++)
            {
                if (field_[i][j] != i_blocked)
                {
                    Vertex v = { i,j };
                    auto it = graph_.insert({ v,{} });
                    gameField_.insert({ v,{} });
                    for (size_t d = 0; d < 4; d++)
                    {
                        size_t nx = i + dx[d];
                        size_t ny = j + dy[d];
                        if (field_[nx][ny] != i_blocked)
                        {
                            it.first->second.push_back({ nx,ny,dir[d] });
                            gameField_[v].insert({ dir[d],{nx,ny} });
                        }
                    }
                }
            }
        }
        field_[current_.x][current_.y] = person;
    }


private:
    std::map<Vertex, std::vector<Edge>> graph_;
    std::map<Vertex, std::map<eDirection, Edge>> gameField_;
    ptrdiff_t** field_ = nullptr;

    Vertex current_ = { 3,3 };
    Vertex prev_ = current_;
    Vertex finish_ = { 4,1 };
    ptrdiff_t i_blocked = -1;
    ptrdiff_t i_free = 0x9999F;
    size_t person = 0;
};

class Game
{
public:

    Game()
    {
        levels_.insert({0, GameLevel()});
    }

    void Run()
    {
        while (true)
        {
            Button button;
            if (_kbhit())
            {
                system("cls");
                button = ToButton(_getch());
                DoButtonAction(button);
                levels_[0].Draw();
            }
            else
            {

            }
        }
    }

private:

    void DoButtonAction(Button button)
    {
        if (button == Button::eUndefined)
            return;

        switch (button)
        {
        case Button::eUp:
        case Button::eDown:
        case Button::eRight:
        case Button::eLeft:
            levels_[0].SetCur(ButtoToDir(button));
            break;
        default:
            break;
        }
    }

    void Go(Button button)
    {

    }
private:
    size_t level = 0;
    std::map<size_t, GameLevel> levels_;
};