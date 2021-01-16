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
#include <queue>

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
        void Prima();

        void Main()
        {
            //Test1();
            //Test2();    
            //Test3Simple();
            //Test4Path();
            Prima();
           
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
        struct Vertex1d
        {
            size_t x;
            bool operator<(const Vertex1d& p) const
            {
                // dictionary order
                return x < p.x;
            }
        };

        struct Edge2
        {
            
        };
        

        void Prima()
        {
            std::map<Vertex1d, std::vector<std::pair<Vertex1d, size_t>>> graph_;
            graph_[{1}].push_back({ {2},6 });
            graph_[{1}].push_back({ {3},3 });
            graph_[{1}].push_back({ {5},9 });

            graph_[{2}].push_back({ {1},6 });
            graph_[{2}].push_back({ {3},4 });
            graph_[{2}].push_back({ {4},2 });
            graph_[{2}].push_back({ {10},9 });

            graph_[{3}].push_back({ {1},3 });
            graph_[{3}].push_back({ {2},4 });
            graph_[{3}].push_back({ {4},2 });
            graph_[{3}].push_back({ {5},9 });
            graph_[{3}].push_back({ {6},9 });

            graph_[{4}].push_back({ {2},2 });
            graph_[{4}].push_back({ {3},2 });
            graph_[{4}].push_back({ {10},9 });
            graph_[{4}].push_back({ {6},8 });

            graph_[{5}].push_back({ {1},9 });
            graph_[{5}].push_back({ {3},9 });
            graph_[{5}].push_back({ {6},8 });
            graph_[{5}].push_back({ {7},18 });

            graph_[{6}].push_back({ {5},8 });
            graph_[{6}].push_back({ {3},9 });
            graph_[{6}].push_back({ {4},8 });
            graph_[{6}].push_back({ {10},7 });
            graph_[{6}].push_back({ {8},9 });
            graph_[{6}].push_back({ {7},10 });

            graph_[{7}].push_back({ {8},3 });
            graph_[{7}].push_back({ {8},4 });
            graph_[{7}].push_back({ {6},10 });
            graph_[{7}].push_back({ {5},18 });

            graph_[{8}].push_back({ {10},5 });
            graph_[{8}].push_back({ {9},1 });
            graph_[{8}].push_back({ {7},3 });
            graph_[{8}].push_back({ {6},10 });

            graph_[{9}].push_back({ {10},4 });
            graph_[{9}].push_back({ {3},1 });
            graph_[{9}].push_back({ {7},4 });

            graph_[{10}].push_back({ {2},9 });
            graph_[{10}].push_back({ {4},9 });
            graph_[{10}].push_back({ {6},7 });
            graph_[{10}].push_back({ {8},3 });
            graph_[{10}].push_back({ {9},4 });

            //algorithm
            {
                std::vector<bool> used(graph_.size(),false);
                std::map<Vertex1d, size_t> helper;
                std::map<size_t, Vertex1d> helper2;

                size_t idx = 0;
                for (auto& [vertex, edges] : graph_)
                {
                    helper.insert({ vertex,idx });
                    helper2.insert({ idx++,vertex });
                }

                std::priority_queue<std::pair<size_t, Vertex1d>, std::vector<std::pair<size_t, Vertex1d>>, std::greater<std::pair<size_t,Vertex1d>>> q;
                size_t mst_weight = 0;
                q.push({ 0,{1} });
                
                std::vector<Vertex1d> spanning_tree;

                while (!q.empty())
                {
                    auto[dst,v] = q.top();
                    q.pop();

                    size_t v_idx = helper[v];
                    spanning_tree.push_back(v);
                    if (used[v_idx])
                        continue;

                    used[helper[{1}]] = true;
                    mst_weight += dst;
                    for (auto& [neighbor,weight] : graph_[v])
                    {
                        size_t u_idx = helper[neighbor];
                        if (!used[u_idx])
                            q.push({ weight,neighbor });
                    }


                }

                return;

            }
           
        }
    }
    
}





int main()
{
    NGraph::Test::Main();

}

