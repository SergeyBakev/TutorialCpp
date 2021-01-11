#pragma once
#include <iostream>
#include <array>
#include <map>
#include <vector>
#include <conio.h>
#include <windows.h>
#include <xhash>

class GameField
{
    using Vertex = size_t;

    enum class eDirection
    {
        eNone,
        eUp,
        eDown,
        eRight,
        eLeft
    };

    struct DirectionInfo
    {
        size_t x;
        size_t y;
        size_t vertex;
    };

    struct NodePosInfo
    {
        std::map<eDirection, DirectionInfo> directions;
        Vertex GetVertexFromDirection(eDirection dir) const
        {
            if (auto it = directions.find(dir); it != std::end(directions))
                return it->second.vertex;

            return -1;
        }
    };


public:
    GameField(size_t weight, size_t height)
    {
        weight_ = weight;
        height_ = height;
        game_field_.resize(height_);
        for (auto& v : game_field_)
            v.resize(weight_);

        InitializeScene();
    }

    void Draw()
    {
        DrawGameField();
        DrawLog();
        if (log_.size() > 10)
            log_.clear();

    }

    const NGraph::Graph& GetGraph() const { return graph_; }

    void DoButtonAction(Button button)
    {
        switch (button)
        {
        case Button::eUp:
            GoUp();
            log_.push_back("eUp\n");
            break;
        case Button::eDown:
            GoDown();
            log_.push_back("eDown\n");
            break;
        case Button::eRight:
            GoRight();
            log_.push_back("eRight\n");
            break;
        case Button::eLeft:
            GoLeft();
            log_.push_back("eLeft\n");
            break;
        default:
            return;
        }
    }

private:

    void InitializeScene()
    {
        /*
        ////1
        //graph_.AddEdge(1, 2);
        //graph_.AddEdge(1, 5);

        ////2
        //graph_.AddEdge(2, 1);
        //graph_.AddEdge(2, 3);

        ////3
        //graph_.AddEdge(3, 2);
        //graph_.AddEdge(3, 4);

        ////4
        //graph_.AddEdge(4, 3);
        //graph_.AddEdge(4, 8);

        ////5
        //graph_.AddEdge(5, 1);
        //graph_.AddEdge(5, 6);
        //graph_.AddEdge(5, 9);

        ////6
        //graph_.AddEdge(6, 5);

        ////7
        //graph_.AddEdge(7, 11);
        //graph_.AddEdge(7, 8);

        ////8
        //graph_.AddEdge(8, 7);
        //graph_.AddEdge(8, 4);

        ////9
        //graph_.AddEdge(9, 13);
        //graph_.AddEdge(9, 5);

        ////10
        //graph_.AddEdge(10, 14);
        //graph_.AddEdge(10, 11);

        ////11
        //graph_.AddEdge(11, 12);
        //graph_.AddEdge(11, 10);
        //graph_.AddEdge(11, 7);

        ////12
        //graph_.AddEdge(12, 16);
        //graph_.AddEdge(12, 11);

        ////13
        //graph_.AddEdge(13, 9);
        //graph_.AddEdge(13, 14);

        ////14
        //graph_.AddEdge(14, 13);
        //graph_.AddEdge(14, 10);
        //graph_.AddEdge(14, 15);

        ////15
        //graph_.AddEdge(15, 14);
        //graph_.AddEdge(15, 16);

        ////16
        //graph_.AddEdge(16, 15);
        //graph_.AddEdge(16, 12);
        //
        */
        //test scene
        graph_.AddEdge(1, 2);
        graph_.AddEdge(1, 3);

        {
            //1 - 2,3
            NodePosInfo info;
            info.directions.insert({ eDirection::eRight,{1,3} });
            info.directions.insert({ eDirection::eDown,{2,1 } });
            node_info_.insert({ 1, std::move(info) });
        }

        graph_.AddEdge(2, 1);
        graph_.AddEdge(2, 4);
        {
            //2 - 1,4
            NodePosInfo info;
            info.directions.insert({ eDirection::eLeft,{1,1} });
            info.directions.insert({ eDirection::eDown,{1,4} });
            node_info_.insert({ 1, std::move(info) });
        }

        graph_.AddEdge(3, 1);
        graph_.AddEdge(3, 4);
        {
            //3 - 1,4
            NodePosInfo info;
            /*info.directions.insert({ eDirection::eUp,1 });
            info.directions.insert({ eDirection::eRight,4 });*/
            node_info_.insert({ 1, std::move(info) });
        }

        graph_.AddEdge(4, 3);
        graph_.AddEdge(4, 2);
        {
            //4 - 3,2
            NodePosInfo info;
            /*info.directions.insert({ eDirection::eLeft,3 });
            info.directions.insert({ eDirection::eUp,2 });*/
            node_info_.insert({ 1, std::move(info) });
        }

        for (size_t i = 0; i < game_field_.size(); i++)
        {
            game_field_[0][i] = block_;
            game_field_[game_field_.size() - 1][i] = block_;
            game_field_[i][0] = block_;
            game_field_[i][game_field_.size() - 1] = block_;

        }

        for (size_t i = 1; i < game_field_.size() - 1; i++)
        {
            for (size_t j = 1; j < game_field_.size() - 1; j++)
            {
                game_field_[i][j] = none;
            }
        }

        game_field_[cur_pos_x_][cur_pos_y_] = person;
    }

    void DrawGameField()
    {
        system("cls");
        for (auto& col : game_field_)
        {
            for (auto& row : col)
                std::cout << row << "\t";

            std::cout << std::endl;
        }
    }

    void DrawLog()
    {
        std::cout << "Log action:\n";
        for (auto& msg : log_)
            std::cout << msg;
        std::cout << std::endl;
    }

    void SetCurPos(size_t x, size_t y)
    {
        game_field_[x][y] = person;
        prev_cur_pos_x_ = cur_pos_x_;
        prev_cur_pos_y_ = cur_pos_y_;
        cur_pos_x_ = x;
        cur_pos_y_ = y;
        game_field_[prev_cur_pos_x_][prev_cur_pos_y_] = none;
    }

    void GoUp()
    {
        Go(eDirection::eUp);
    }

    void GoDown()
    {

    }

    void GoLeft()
    {

    }

    void GoRight()
    {

    }

    void Go(eDirection dir)
    {

    }

    NGraph::Graph::Matrix GetExtenedMatrix() const
    {
        NGraph::Graph::Matrix matrix = graph_.GetMatirx();
        NGraph::Graph::Matrix matrix_ex(matrix.size() + 2);
        for (auto& c : matrix_ex)
            c.resize(matrix[0].size() + 2);

        for (size_t i = 1; i < matrix_ex.size() - 1; i++)
        {
            for (size_t j = 1; j < matrix_ex.size() - 1; j++)
            {
                matrix_ex[i][j] = matrix[i - 1][j - 1];
            }
        }

        return matrix_ex;
    }
private:



    size_t cur_pos_x_ = 1;
    size_t cur_pos_y_ = 2;
    Vertex cur_vertex = 1;

    size_t prev_cur_pos_x_ = 1;
    size_t prev_cur_pos_y_ = 2;
    Vertex prev_cur_vertex = 1;
    size_t weight_;
    size_t height_;

    char block_ = '*';
    char person = 'x';
    char none = '0';

    std::vector<std::vector<char>> game_field_;
    std::map<size_t, NodePosInfo> node_info_;
    std::vector<std::string> log_;
    NGraph::Graph graph_;
};

void DoGame()
{
    GameField gf(4, 4);
    /* auto m = gf.GetExtenedMatrix();
     for (auto& c : m)
     {
         for (auto& r : c)
             std::cout << r << "\t";
         std::cout << std::endl;
     }*/
    gf.Draw();
    while (true)
    {
        Button button;
        if (_kbhit())
        {
            button = ToButton(_getch());
            gf.DoButtonAction(button);
            gf.Draw();
        }
        else
        {

        }
    }
}