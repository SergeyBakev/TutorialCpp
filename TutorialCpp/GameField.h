#pragma once
#include "enums.h"
#include <string>
#include <vector>
#include <set>


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

struct Edge
{
    Vertex vertex;
    size_t weight = 1;

};

class GameLevel
{
public:

    GameLevel()
    {
        GenerateLevel();
    }

    void Draw()
    {
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

        auto& edge = map[dir];        
        prev_ = current_;

        current_.x = edge.vertex.x;
        current_.y = edge.vertex.y;

        field_[current_.x][current_.y] = person;
        field_[prev_.x][prev_.y] = i_free;

        if (current_ == finish_)
        {
            std::cout << "Game finished\n";
            IncreaseDifficulty();
            GenerateLevel();
        }
    }

    void SetCur(const Vertex vertex)
    {
        field_[vertex.x][vertex.y] = 9;
    }

    void GenerateLevel()
    {
        RemoveField();
        InitializeScene();
        CreateGraph();
        SetStartFinish();
    }

    bool AlgorithmDijkstra2(const Vertex& from, const Vertex& to, std::vector<Vertex>& path)
    {
        size_t max = std::numeric_limits<size_t>::max() - graph_.size() * 5;
        std::vector<std::tuple<Vertex,size_t, bool>> distances;
        std::map<Vertex, size_t> helper;

        size_t idx = 0;
        
        for (auto& [vertex, edges] : graph_)
        {
            distances.push_back({vertex,max,false});
            helper.insert({ vertex,idx++});
        }
            
        //set start vertex distance
        std::get<1>(distances[helper[from]]) = 0;
        
        for (auto& [vertex, edges] : graph_)
        {
            auto& min = distances[FindMin(distances)];

            std::get<2>(min) = true;
            size_t distance_to_min = std::get<1>(min);
            const auto& neighbors = graph_[std::get<0>(min)];
            for (const auto& neighbor : neighbors)
            {
                auto& [idx, distance_to_neighbor, is_neighbor_used] = distances[helper[neighbor.vertex]];
                if (neighbor.weight + distance_to_min < distance_to_neighbor && !is_neighbor_used)
                    distance_to_neighbor = neighbor.weight + distance_to_min;
            }


        }




        path.clear();
        size_t cur_dist = std::get<1>(distances[helper[to]]);
        if (cur_dist != max)
        {
            Vertex pos = to;
            path.push_back(pos);

            while (cur_dist != 0)
            {

                const auto& neighbors = graph_[pos];
                for (const auto& neighbor : neighbors)
                {
                    const size_t& dist_to_neighbor_vertex = std::get<1>(distances[helper[neighbor.vertex]]);
                    if (cur_dist - neighbor.weight == dist_to_neighbor_vertex)
                    {
                        cur_dist = dist_to_neighbor_vertex;
                        pos = neighbor.vertex;
                        path.push_back(pos);
                        break;
                    }
                }
            }
            
            return true;
        }
        return false;
    }

    Vertex GetCurrentPos() const { return current_; }
    Vertex GetFinishPos() const { return finish_; }

private:
    size_t FindMin(std::vector<std::tuple<Vertex, size_t, bool>>& distances)
    {
        size_t min = std::numeric_limits<size_t>::max() - graph_.size() * 5;
        size_t index;

        for (size_t i = 0; i < distances.size(); i++)
        {
            const size_t& dist = std::get<1>(distances[i]);
            const bool& used = std::get<2>(distances[i]);
            if (!used && dist <= min)
            {
                min = dist;
                index = i;
            }
        }

        return index;
    }
    void RemoveField()
    {
        if (field_ != nullptr)
        {
            for (size_t i = 0; i < prev_height_; i++)
                delete[] field_[i];
            delete[] field_;
            field_ = nullptr;
        }
    }

   
    void InitializeScene()
    {
        field_ = new ptrdiff_t * [height_ ];
        for (size_t i = 0; i < height_; i++)
            field_[i] = new ptrdiff_t[weight_];

        for (size_t i = 0; i < height_; i++)
        {
            for (size_t j = 0; j < weight_; j++)
            {
                field_[i][j] = i_free;
            }
        }

        //generate borders
        {
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
        }
       

        size_t x_min = 2;
        size_t y_min = 2;
        size_t x_max = height_ - 3;
        size_t y_max = weight_ - 3;

        srand((uint32_t)time(nullptr));
        //generate blocked point
        {
            
            for (size_t i = 0; i < count_blocked_; i++)
            {
                size_t nx = x_min + rand() % x_max;
                size_t ny = y_min + rand() % y_max;
                field_[nx][ny] = i_blocked;
            }
        }

        //generate finish end person position
        {
            size_t nx = x_min + rand() % x_max;
            size_t ny = y_min + rand() % y_max;
            current_.x = nx;
            current_.y = nx;
        }

        field_[finish_.x][finish_.y] = i_free;
        field_[current_.x][current_.y] = i_free;
       /* field_[2][4] = i_blocked;
        field_[3][4] = i_blocked;
        field_[4][4] = i_blocked;
        field_[4][3] = i_blocked;

        field_[5][5] = i_blocked;
        field_[6][3] = i_blocked;

        field_[7][5] = i_blocked;
        field_[7][6] = i_blocked;

        field_[finish_.x][finish_.y] = i_free;*/
    }

    void SetStartFinish()
    {
        field_[current_.x][current_.y] = person;
    }

    void CreateGraph()
    {
        gameField_.clear();
        graph_.clear();
        const ptrdiff_t dx[4] = { 0,1,0,-1 };
        const ptrdiff_t dy[4] = { -1,0,1,0 };
        eDirection dir[4] = { eDirection::eLeft,eDirection::eDown,eDirection::eRight,eDirection::eUp };
        for (size_t i = 0; i < height_; i++)
        {
            for (size_t j = 0; j < weight_; j++)
            {
                Vertex v = { i,j };
                if (v == current_)
                {
                    size_t z = 3;
                }
                if (field_[i][j] != i_blocked)
                {               
                    auto it = graph_.insert({ v,{} });
                    gameField_.insert({ v,{} });
                    for (size_t d = 0; d < 4; d++)
                    {
                        size_t nx = i + dx[d];
                        size_t ny = j + dy[d];
                        if (field_[nx][ny] != i_blocked)
                        {
                            it.first->second.push_back({ nx,ny });
                            gameField_[v].insert({ dir[d],{nx,ny} });
                        }
                    }
                }
            }
        }
       
    }

    void IncreaseDifficulty()
    {
        count_blocked_+= 4;
        prev_height_ = height_;
        prev_weight_ = weight_;
        height_++;
        weight_++;

    }

private:
    std::map<Vertex, std::vector<Edge>> graph_;
    std::map<Vertex, std::map<eDirection, Edge>> gameField_;

    ptrdiff_t** field_ = nullptr;

    size_t height_ = 11;
    size_t weight_ = 11;

    size_t prev_height_ = height_;
    size_t prev_weight_ = height_;
    size_t count_blocked_ = 10;

    Vertex current_ = { 3,3 };
    Vertex prev_ = current_;
    Vertex finish_ = { 8,9 };

    ptrdiff_t i_blocked = -1;
    ptrdiff_t i_free = 0x9999F;
    size_t person = 0;
};

class Game
{
public:

    Game()
    {
    }

    void Run()
    {
        level_.Draw();
        while (true)
        {
            Button button;
            if (_kbhit())
            {
                system("cls");
                button = ToButton(_getch());
                DoButtonAction(button);
                level_.Draw();
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
            level_.SetCur(ButtoToDir(button));
            break;
        case Button::eRestart:
            level_.GenerateLevel();
            break;
        case Button::eX:
            {
                std::vector<Vertex> path;
                if (level_.AlgorithmDijkstra2(level_.GetCurrentPos(), level_.GetFinishPos(), path))
                {
                    for (auto& v : path)
                        level_.SetCur(v);
                }
               

            }
            
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
    GameLevel level_;
};