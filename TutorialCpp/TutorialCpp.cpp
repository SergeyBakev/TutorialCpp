#include <iostream>
#include <array>
#include <map>
#include <vector>
#include <conio.h>
#include <xhash>
#include <queue>
#include "Graph.h"
#include "GameField.h"
#include "boost/graph/edge_list.hpp"

namespace Task1
{  
    //построить матрицу смежности для множества M={1,2,3,5,6}
    //x+y <= 6

    using namespace std;
    bool Func(size_t x, size_t y)
    {
        return (x + y) <= 6;
    }

    void BuildMatrixTask1()
    {
        std::array<size_t, 7> vertex = { 0,1,2,3,4,5,6 };
        bool matrix[7][7]; //матрица смежности
        std::map<size_t, std::vector<size_t>> list; //список смежных верших

        for (size_t i = 0; i < vertex.size(); i++)
        {
            size_t vertex_i = vertex[i];
            std::vector<size_t> v;
            for (size_t j = 0; j < vertex.size(); j++)
            {
                
                size_t vertex_j = vertex[j];
                bool r = Func(vertex_i, vertex_j);
                if(r)
                    v.push_back(r);
                matrix[vertex_i][vertex_j] = Func(vertex_i, vertex_j);
               
            }
            list.insert({ vertex_i,v });

        }

        
        for (size_t i = 0; i < vertex.size(); i++)
        {
            for (size_t j = 0; j < vertex.size(); j++)
            {
               cout << matrix[i][j] << "\t";
            }
            cout << endl;
        }
        cout <<"____________________________________"<< endl;
        for (auto& it : list)
        {
            std::cout << it.first << ":\t";
            for(auto& v : it.second)
                std::cout << v << "\t";
            cout << endl;
        }

    }
}

namespace NGraph
{
    /*class Graph
    {
    public:
        using Matrix = std::vector<std::vector<double>>;
        using Way = std::vector<size_t>;
        using Neighbors = std::vector<std::pair<size_t, double>>;
        using Vertex = size_t;

        bool AddEdge(size_t from, size_t to, double weight = 1)
        {
            bool ret = false;
            auto edge = std::make_pair(to, weight);
            auto it = nodes_.find(from);
            if ( it == std::end(nodes_))
                it = nodes_.insert({ from,{} }).first;
            
            auto& vec = it->second;
            if (auto vit = std::find(std::begin(vec), std::end(vec), edge); vit == std::end(vec))
            {
                vec.push_back(edge);
                ret = true;
            }

            return ret;
        }

        const Neighbors& GetNeighbors(const size_t& vertex) const
        {
            return nodes_.at(vertex);
        }


        Matrix GetMatirx() const
        {
            Matrix matrix;
            matrix.resize(nodes_.size());
            for (auto& m : matrix)
                m.resize(nodes_.size());

            for (size_t i = 0; i < matrix.size(); i++)
            {
                auto& row = matrix[i];
                auto& d = nodes_.at(i + 1);
                for (auto& col : d)
                {
                    row[col.first-1] = col.second;
                }
            }

            return matrix;
        }
        
        Vertex GetCountVertexes() const { return nodes_.size(); }

        size_t GetCountEdges() const
        {
            size_t cnt = 0;
            for (auto& it : nodes_)
            {
                for (auto& edge : it.second)
                    cnt++;
            }

            return cnt;
        }

        Way LeeSearchWay(size_t from, size_t to)
        {
            if (auto it = nodes_.find(to); it == std::end(nodes_))
                return {};

            if (auto it = nodes_.find(from); it == std::end(nodes_))
                return {};


        }

    private:
        std::map<size_t, Neighbors> nodes_;
    };*/

    
    namespace Test
    {
      /*  void Test1();
        void Test2();
        void Test3Simple();
        void Test4Path();*/
        void Test5BestPath();

        void Main()
        {
            //Test1();
            //Test2();    
            //Test3Simple();
            //Test4Path();
            Test5BestPath();
           
        }


        //graph
        //*************2**************
        //************/-\*************
        //***********4***6************
        //**********/*****\***********
        //*********3<-->1**5**********
        //****************************
        void Test1()
        {
          /*  Graph graph_;
            graph_.AddEdge(2, 4);
            graph_.AddEdge(2, 6);
            graph_.AddEdge(6, 5);

            graph_.AddEdge(2, 4);

            graph_.AddEdge(4, 3);
            graph_.AddEdge(4, 1);

            graph_.AddEdge(3, 1);
            graph_.AddEdge(1, 3);

            auto vertexes = graph_.GetCountVertexes();
            auto edges = graph_.GetCountEdges();

            return;*/
        }


        char blocked = '*';
        char free = ' ';
        char finish = '!';
        char person = 'x';
        char visited = 'v';

        ptrdiff_t i_blocked = - 1;
        ptrdiff_t i_free = 0x9999F;

        const size_t weight_ = 9;
        const size_t height_ = 9;

        const size_t weight2_ = 6;
        const size_t height2_ = 6;

        void PrintMatrix(char** field, size_t height, size_t weight)
        {
            for (size_t i = 0; i < height; i++)
            {
                for (size_t j = 0; j < weight; j++)
                {
                    std::cout << field[i][j];
                }
                std::cout << std::endl;
            }
        }

        void PrintMatrix(ptrdiff_t** field, size_t height, size_t weight)
        {
            for (size_t i = 0; i < height; i++)
            {
                for (size_t j = 0; j < weight; j++)
                {
                    std::cout << field[i][j] << "\t";
                }
                std::cout << std::endl;
            }
        }

        void PrintMatrix(bool** field, size_t height, size_t weight)
        {
            std::cout << std::boolalpha;
            for (size_t i = 0; i < height; i++)
            {
                for (size_t j = 0; j < weight; j++)
                {
                    std::cout << field[i][j] << "\t";
                }
                std::cout << std::endl;
            }
        }

//        bool FindDFS(char field[height_][weight_], size_t cur_x,size_t cur_y)
//        {
//            std::cout << "_________Iteration____________\n";
//           /* if (field[cur_x][cur_y] == finish)
//                return true;
//
//            else*/
//            {
//                bool bFind = false;
//                field[cur_x][cur_y] = visited;
//
//#ifdef _DEBUG
//                for (size_t i = 0; i < height_; i++)
//                {
//                    for (size_t j = 0; j < weight_; j++)
//                    {
//                        std::cout << field[i][j];
//                    }
//                    std::cout << std::endl;
//                }
//#endif // _DEBUG
//            
//                __int32 k[] = { 0,0,-1,1 };
//                __int32 l[] = { -1,1,0,0 };
//                size_t dir = 0;
//                for (size_t dir = 1; dir < 5; dir++)
//                {
//                    if (field[cur_x + k[dir - 1]][cur_y + l[dir - 1]] == free)
//                       FindDFS(field, cur_x + k[dir - 1], cur_y + l[dir - 1]);
//                
//                }
//                
//
//            }          
//            return true;
//        }
//
//        bool FindDFS2(char** field, size_t height, size_t weight, size_t cur_x, size_t cur_y)
//        {
//             if (field[cur_x][cur_y] == finish)
//                 return true;
//
//             if (field[cur_x][cur_y] == blocked || field[cur_x][cur_y] == visited)
//                 return false; 
//
//             std::cout << "_________Iteration____________\n";
//            field[cur_x][cur_y] = visited;
//
//#ifdef _DEBUG
//            PrintMatrix(field, height, weight);
//#endif // _DEBUG
//
//            __int32 k[] = { 0,1,0,-1 };
//            __int32 l[] = { -1,0,1,0 };
//            size_t dir = 0;
//            while (dir < (size_t)4 && !FindDFS2(field, height, weight, cur_x + k[dir], cur_y + l[dir]))
//                ++dir;
//           
//            if (dir < 4)
//                return true;
//                
//            return false;
//        }
//
//        void FindBFS(char** field, size_t height, size_t weight, size_t cur_x, size_t cur_y)
//        {
//            std::vector < std::pair<size_t, size_t>> waves;
//            std::vector < std::pair<size_t, size_t>> oldWaves;
//            __int32 dx[] = { 0,1,0,-1 };
//            __int32 dy[] = { -1,0,1,0 };
//            oldWaves.push_back({ cur_x,cur_y });
//            size_t distance = '0';
//            field[cur_x][cur_y] = (char)distance;
//            while(!oldWaves.empty())
//            {
//                distance++;
//                waves.clear();
//                for (auto& wave : oldWaves)
//                {
//                    for (size_t d = 0; d < 4; d++)
//                    {
//                        int nx = wave.first + dx[d];
//                        int ny = wave.second + dy[d];
//                        if (field[nx][ny] == free)
//                        {
//                            waves.push_back({ nx,ny });
//                            field[nx][ny] = (char)distance;
//                            
//                            std::cout << "_____ITERATION_____\n";
//                        }
//                        PrintMatrix(field, height, weight);
//                        if (nx == 4 && ny == 1)
//                        {
//                            field[nx][ny] = (char)distance;
//                            std::cout << "Finsih searched\n";
//                            goto done;
//                        }
//                    }
//                }
//                oldWaves = waves;
//            }
//            done:
//            
//            return;
//        }
//
//        void FindBFS2(char** field, size_t height, size_t weight, size_t from_x, size_t from_y, size_t to_x, size_t to_y)
//        {
//            __int32 dx[] = { 0,1,0,-1 };
//            __int32 dy[] = { -1,0,1,0 };
//            size_t distance = '0';
//            std::queue<std::pair<size_t, size_t>> waves;
//
//            field[from_x][from_y] = (char)distance;
//            waves.push({ from_x,from_y });
//            distance++;
//            while (!waves.empty())
//            {
//                auto& wave = waves.front();
//                waves.pop();
//
//                for (size_t d = 0; d < 4; d++)
//                {
//                    int nx = wave.first + dx[d];
//                    int ny = wave.second + dy[d];
//                    if (nx == to_x && ny == to_y)
//                    {
//                        field[nx][ny] = (char)distance;
//                        std::cout << "To point finded!\n";
//                        goto done;
//                    }
//
//                    if (field[nx][ny] == free)
//                    {
//                        waves.push({ nx,ny });
//                        field[nx][ny] = (char)distance;
//                    }
//                }
//                distance++;
//                PrintMatrix(field, height, weight);
//            }
//
//        done:
//            return;
//        }
//
//        using Point2D = std::pair<__int32, __int32>;
//
//        void Test2()
//        {
//            char** field = new char* [height_];
//            for (size_t i = 0; i < height_; i++)
//                field[i] = new char[weight_];
//
//            for (size_t i = 0; i < height_; i++)
//            {
//                for (size_t j = 1; j < weight_; j++)
//                {
//                    field[i][j] = free;
//                }
//            }
//
//            for (size_t j = 0; j < 2; j++)
//            {
//                for (size_t x = 0; x < height_; x++)
//                {
//                    field[j][x] = blocked;
//                    field[height_ - j -1][x] = blocked;
//
//                    field[x][j] = blocked;
//                    field[x][weight_ - j -1] = blocked;
//                }
//            }
//
//            //finish
//            field[4][1] = finish;
//
//            field[2][4] = blocked;
//            field[3][4] = blocked;
//            field[4][4] = blocked;
//            field[4][3] = blocked;
//
//            field[5][5] = blocked;
//            field[6][3] = blocked;
//
//
//            size_t cur_x = 3;
//            size_t cur_y = 5;
//            field[cur_x][cur_y] = person;
//
//            for (size_t i = 1; i < height_ -1; i++)
//            {
//                for (size_t j = 1; j < weight_ - 1; j++)
//                {
//                    std::cout << field[i][j];
//                }
//                std::cout << std::endl;
//            }
//            FindDFS2(field,height_,weight_, cur_x,cur_y);
//        }    
//
//        void Test3Simple()
//        {
//            char** field = new char*[height2_];
//            for (size_t i = 0; i < height2_; i++)
//                field[i] = new char [weight2_];
//
//            size_t cur_x = 2;
//            size_t cur_y = 2;
//            
//           
//            for (size_t i = 0; i < height2_; i++)
//            {
//                for (size_t j = 1; j < weight2_; j++)
//                {
//                    field[i][j] = free;
//                }
//            }
//
//            for (size_t j = 0; j < 2; j++)
//            {
//                for (size_t x = 0; x < height2_; x++)
//                {
//                    field[j][x] = blocked;
//                    field[height2_ - j - 1][x] = blocked;
//
//                    field[x][j] = blocked;
//                    field[x][weight2_ - j - 1] = blocked;
//                }
//            }
//            field[cur_x][cur_y] = person;
//
//            PrintMatrix(field, height2_, weight2_);
//            FindDFS2(field, height2_,weight2_, cur_x, cur_y);
//        }
//
//        class GraphAlgorithm
//        {
//        public:
//            GraphAlgorithm() = delete;
//            GraphAlgorithm(__int64 free, __int64 blocked) :free_(free),blocked_(blocked)
//            {
//
//            }
//
//            struct Node
//            {
//                __int32 x;
//                __int32 y;
//                __int32 distance;
//            };
//
//            bool BFS(__int32** field, __int32 height, __int32 weight, __int32 from_x, __int32 from_y, __int32 to_x, __int32 to_y,std::vector<Point2D>& path)
//            {
//                if (from_x == to_x && from_y == to_y)
//                {
//                    std::cout << "Point searched!\n";
//                    return true;
//                }
//
//                bool** visited = new bool* [height];
//                for (__int32 i = 0; i < height; i++)
//                    visited[i] = new  bool[weight];
//
//                for (__int32 i = 0; i < height; i++)
//                {
//                    for (__int32 j = 0; j < weight; j++)
//                        visited[i][j] = false;
//                }
//
//                visited[from_x][from_y] = true;
//                field[from_x][from_y] = 0;
//                __int32 min_dist = std::numeric_limits<__int32>::max();
//                std::queue<Node> waves;
//                waves.push({ from_x,from_y,0 });
//                while (!waves.empty())
//                {
//                    Node node = std::move(waves.front());
//                    waves.pop();
//
//                    if (node.x == to_x && node.y == to_y)
//                    {
//                        min_dist = node.distance;
//                        break;
//                    }
//
//                    for (size_t d = 0; d < 4; d++)
//                    {
//                        __int32 nx = node.x + dx[d];
//                        __int32 ny = node.y + dy[d];
//                        if (IsValid(visited, field, height, weight, nx, ny))
//                        {
//                            visited[nx][ny] = true;
//                            field[nx][ny] = node.distance + 1;
//                            if (nx == to_x && ny == to_y)
//                            {
//                                int z = 3;
//                            }
//                            waves.push({ nx,ny,node.distance + 1 });
//                        }
//                    }
//                }
//
//                if (min_dist != std::numeric_limits<__int32>::max())
//                {
//                    std::cout << "The shortest path from source to destination "
//                        "has length " << min_dist << std::endl;
//                    FindPath(path, from_x, from_y, to_x, to_y, field);
//                    return true;
//                }
//
//                return false;
//            }
//
//        private:
//
//            bool IsValid(bool** visited, __int32** field, __int32 height, __int32 weight, __int32 x, __int32 y)
//            {
//                if (!VerifySize(height, weight, x, y))
//                    return false;
//
//                return !IsBlocked(field,x,y) && !visited[x][y];
//            }
//
//            bool IsBlocked(__int32** field, __int32 x, __int32 y)
//            {
//                return field[x][y] == blocked_;
//            }
//
//            bool VerifySize(_int32 height, __int32 weight, __int32 x, __int32 y)
//            {
//                if (x > height || y > weight || x < 0 || y < 0)
//                    return false;
//
//                return true;
//            }
//            bool FindPath(std::vector<Point2D>& path, __int32 from_x, __int32 from_y, __int32 to_x, __int32 to_y, __int32** field)
//            {
//                __int32 n = field[to_x][to_y];
//                if (n == 0)
//                {
//                    std::reverse(std::begin(path), std::end(path));
//                    return true;
//                }
//
//                for (size_t d = 0; d < 4; d++)
//                {
//                    __int32 nx = to_x + dx[d];
//                    __int32 ny = to_y + dy[d];
//                    __int32 nn = field[nx][ny];
//                    if (!IsBlocked(field,nx,ny) && nn < n)
//                    {
//                        path.push_back({ nx,ny });
//                        if (FindPath(path, to_x, to_y, nx, ny, field))
//                            return true;
//                    }    
//                }
//
//                return false;
//
//
//            }
//        private:
//            __int64 free_;
//            __int64 blocked_;
//            const __int32 dx[4] = { 0,1,0,-1 };
//            const __int32 dy[4] = { -1,0,1,0 };
//
//        };
//
//        void Test4Path()
//        {
//            size_t height_ = 11;
//            size_t weight_ = 11;
//            __int32** field = new __int32* [height_];
//            for (size_t i = 0; i < height_; i++)
//                field[i] = new __int32[weight_];
//
//            for (size_t i = 0; i < height_; i++)
//            {
//                for (size_t j = 0; j < weight_; j++)
//                {
//                    field[i][j] = i_free;
//                }
//            }
// 
//            for (size_t j = 0; j < 2; j++)
//            {
//                for (size_t x = 0; x < height_; x++)
//                {
//                    field[j][x] = i_blocked;
//                    field[height_ - j - 1][x] = i_blocked;
//
//                    field[x][j] = i_blocked;
//                    field[x][weight_ - j - 1] = i_blocked;
//                }
//            }
//
//            size_t cur_x = 2;
//            size_t cur_y = 8;
//            size_t finish_x = 4;
//            size_t finish_y = 1;
//           
//
//            field[2][4] = i_blocked;
//            field[3][4] = i_blocked;
//            field[4][4] = i_blocked;
//            field[4][3] = i_blocked;
//
//            field[5][5] = i_blocked;
//            field[6][3] = i_blocked;
//
//            field[7][5] = i_blocked;
//            field[7][6] = i_blocked;
//
//            field[finish_x][finish_y] = i_free;
//            field[cur_x][cur_y] = 666;
//
//
//
//            PrintMatrix(field, height_, weight_);
//            GraphAlgorithm alg(i_free, i_blocked);
//            std::vector<Point2D> path;
//            alg.BFS(field, height_, weight_, cur_x, cur_y, finish_x, finish_y, path);
//            std::cout << std::endl;
//            for (const auto& [x,y] : path)
//            {
//                field[x][y] = 0;
//            }
//            PrintMatrix(field, height_, weight_);
//        }

       
        void Test5BestPath()
        {
            
            size_t height_ = 11;
            size_t weight_ = 11;
            ptrdiff_t** field = new ptrdiff_t * [height_];
            for (size_t i = 0; i < height_; i++)
                field[i] = new ptrdiff_t[weight_];

            for (size_t i = 0; i < height_; i++)
            {
                for (size_t j = 0; j < weight_; j++)
                {
                    field[i][j] = i_free;
                }
            }

            for (size_t j = 0; j < 2; j++)
            {
                for (size_t x = 0; x < height_; x++)
                {
                    field[j][x] = i_blocked;
                    field[height_ - j - 1][x] = i_blocked;

                    field[x][j] = i_blocked;
                    field[x][weight_ - j - 1] = i_blocked;
                }
            }

            size_t cur_x = 2;
            size_t cur_y = 8;
            size_t finish_x = 4;
            size_t finish_y = 1;


            field[2][4] = i_blocked;
            field[3][4] = i_blocked;
            field[4][4] = i_blocked;
            field[4][3] = i_blocked;

            field[5][5] = i_blocked;
            field[6][3] = i_blocked;

            field[7][5] = i_blocked;
            field[7][6] = i_blocked;

            //build graph
            //left downd right up
            const ptrdiff_t dx[4] = { 0,1,0,-1 };
            const ptrdiff_t dy[4] = { -1,0,1,0 };

            eDirection dir[4] = { eDirection::eLeft,eDirection::eDown,eDirection::eRight,eDirection::eUp };
            std::map < Vertex, std::vector<Edge>> nodes;
            std::map < Vertex, std::map<eDirection, Edge>> gamefield;

            {
                for (size_t i = 2; i < height_; i++)
                {
                    for (size_t j = 2; j < weight_; j++)
                    {
                        if (field[i][j] != i_blocked)
                        {
                            Vertex v = { i,j };
                            auto it = nodes.insert({ v,{} });
                            gamefield.insert({ v,{} });
                            for (size_t d = 0; d < 4; d++)
                            {
                                size_t nx = i + dx[d];
                                size_t ny = j + dy[d];
                                if (field[nx][ny] != i_blocked)
                                {
                                    it.first->second.push_back({ nx,ny });
                                    gamefield[v].insert({ dir[d],{nx,ny} });
                                }
                            }
                        }
                    }
                }
            }

            for (auto& [vertex, edges] : nodes)
            {
                std::cout << vertex.x << " " << vertex.y << ":";
                for (auto& edge : edges)
                {
                    std::cout << "Edge "<< edge.ToString() << " ";
                }
                std::cout << std::endl;
            }
        }
    }
    
}





int main()
{
    Game game;
    game.Run();

}

