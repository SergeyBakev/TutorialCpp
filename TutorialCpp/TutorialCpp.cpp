#include <iostream>
#include <functional>
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

namespace NGraph
{
    namespace Test
    {
       

        struct Vertex1d
        {
            size_t x;
            bool operator<(const Vertex1d& p) const
            {
                // dictionary order
                return x < p.x;
            }
        };

        template<class _Ty, class Weight>

        struct Edge2
        {
            using Vertex = std::remove_const_t<std::remove_reference_t<_Ty>>;
            Vertex from;
            Vertex to;
            Weight weight;

            bool operator<(const Edge2& other)
            {
                return weight < other.weight;
            }
        };

        using VectorEdges1d = std::vector<Edge2<Vertex1d, size_t>>;

        void Prima();
        void Kruskal();
        void Kruskal2();
        void FDSU();

        std::map<Vertex1d, std::vector<std::pair<Vertex1d, size_t>>> graph_;

        void Main()
        {
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
            graph_[{9}].push_back({ {8},1 });
            graph_[{9}].push_back({ {7},4 });

            graph_[{10}].push_back({ {2},9 });
            graph_[{10}].push_back({ {4},9 });
            graph_[{10}].push_back({ {6},7 });
            graph_[{10}].push_back({ {8},5 });
            graph_[{10}].push_back({ {9},4 });
           // Prima();
            //FDSU();
            Kruskal();
            //Kruskal2();
           
        }

        void Prima()
        {
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
                    if (used[v_idx])
                        continue;
                    spanning_tree.push_back(v);
                    used[helper[v]] = true;
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

        VectorEdges1d SelectAllAdge()
        {
            VectorEdges1d edges;
            for (auto& [vertex, neighbors]: graph_)
            {
                for (auto& neighbor : neighbors)
                {
                    edges.push_back({ vertex,neighbor.first,neighbor.second });
                }
            }

            return edges;
        }

        class DSUBase
        {
        public:

            DSUBase()
            {
                set_.resize(10, max);
                rank_.resize(10, 1);
            }

            DSUBase(size_t size)
            {
                set_.resize(size, max);
                rank_.resize(size, 1);
            }

            void MakeSet(size_t vertex)
            {
                if (set_.size() <= vertex)
                {
                    set_.resize(set_.size() * 2, max);
                    rank_.resize(set_.size() * 2, 1);
                }
                set_[vertex] = vertex;
                rank_[vertex] = 0;
            }

            size_t Find(size_t vertex) const
            {
                if (set_[vertex] == vertex || vertex  == max)
                    return vertex;

                return set_[vertex] = Find(set_[vertex]);
            }

            bool Check(size_t v1, size_t v2) const
            {
                return Find(v1) == Find(v2);
            }

            bool Union(size_t v1, size_t v2)
            {
                size_t ra = Find(v1);
                size_t rb = Find(v2);
                if (ra == rb)
                    return false;

                if (rank_[ra] < rank_[rb])
                    std::swap(ra, rb);

                set_[rb] = ra;
                if (rank_[ra] == rank_[rb])
                    ++rank_[ra];

                return true;
            }

            std::vector<size_t> GetMaxSet() const
            {

            }

        private:
            mutable std::vector<size_t> set_;
            std::vector<size_t> rank_;
            size_t max = std::numeric_limits<size_t>::max();

        };

        template <class T>
        class DSU2
        {
        public:
            static constexpr size_t npos = -1;
            DSU2(size_t size) : dsu(size)
            {

            }

            bool MakeSet(const T& value)
            {
                const auto& [iterator,is_inserted] =  data2_.insert({ value, data2_.size()});
                if (is_inserted)
                    dsu.MakeSet(iterator->second);
                
               
                return is_inserted;
            }

            size_t Find(const T& vertex) const
            {
                if (!data2_.contains(vertex))
                    return npos;

                return dsu.Find(data2_[vertex]);
            }
            
            bool Union(const T& v1, const T& v2)
            {
                if (!data2_.contains(v1) || !data2_.contains(v2))
                    return false;

                return dsu.Union(data2_[v1], data2_[v2]);
            }

        

        private:
            DSUBase dsu;
            std::map<T,size_t> data2_;
            
        };

        void FDSU()
        {
            DSUBase dsu(14);
            for (size_t i = 0; i < 14; i++)
                dsu.MakeSet(i);

            //first set
            dsu.Union(0, 1);
            dsu.Union(1, 2);
            dsu.Union(2, 3);

            //second set 
            dsu.Union(4, 5);
            dsu.Union(4, 6);

            //thrid set
            dsu.Union(7, 8);
            dsu.Union(8, 9);
            dsu.Union(9, 10);

            //fourth set
            dsu.Union(11, 12);
            dsu.Union(11, 13);
            
            dsu.Union(4, 2);

            dsu.Union(7, 11);

            dsu.Union(2, 11);
            return;
        }

        void Kruskal()
        {
            auto edges = SelectAllAdge();

            std::sort(std::begin(edges), std::end(edges));
            DSUBase dsu;
            for (auto& [vertex, neighbor] : graph_)
                dsu.MakeSet(vertex.x);
            
            size_t max_weight = 0;
            for (auto& edge : edges)
            {
                if (dsu.Union(edge.from.x, edge.to.x))
                {
                    printf("from: %zu to: %zu weight: %zu\n", edge.from.x, edge.to.x, edge.weight);
                    //std::cout << "from: " << edge.from.x << " to: " << edge.to.x << " weight: " << edge.weight << std::endl;
                }
            }
            return;
            /*std::function<bool(int,int)> merge = []*/
        }

        void Kruskal2()
        {
            auto edges = SelectAllAdge();
            std::sort(std::begin(edges), std::end(edges));

            DSU2<Vertex1d> dsu(graph_.size());
            for (auto& [vertex, neighbor] : graph_)
                dsu.MakeSet(vertex);
            size_t max_weight = 0;
            for (auto& edge : edges)
            {
                if (dsu.Union(edge.from, edge.to))
                {
                    printf("from: %zu to: %zu weight: %zu\n", edge.from.x, edge.to.x, edge.weight);
                    max_weight += edge.weight;
                }
            }

            return;
        }
    }
    
}


namespace TBTree
{
    
   /* class BTreeNode
    {
    public:

    };

    template <size_t N, class _Key, class _Value>
    class BTree
    {
    public:

        BTree()
        {

        }

    private:
        size_t t_ = N;
    };*/

    class BTree23Node : protected std::enable_shared_from_this<BTree23Node>
    {
    public:
        friend class BTree23;
        BTree23Node() 
        {
            for (size_t i = 0; i < keys_.size(); i++)
                keys_[i] = 0;
            
            for (size_t i = 0; i < childs_.size(); i++)
                childs_[i] = nullptr;
        };

    public:
        bool const IsLeaf() const { return isLeaf_; }
        
    private:

        static constexpr size_t t_ = 2;
        std::array<size_t,2 * t_> keys_;
        std::weak_ptr<BTree23Node> parent_;
        std::array<std::shared_ptr<BTree23Node>, 2 * t_ + 1> childs_;
        size_t count_ = 0;
        size_t countSons_ = 0;
        bool isLeaf_ = false;
      
    };

    class BTree23
    {
    public:
        using Node = BTree23Node;
        using NodePtr = std::shared_ptr<BTree23Node>;

        static NodePtr CreateNode()
        {
            return NodePtr(new BTree23Node());
        }
        NodePtr Insert(size_t key)
        {
            if (root_ == nullptr)
            {
                NodePtr newRoot = CreateNode();
                newRoot->keys_[0] = key;
                newRoot->count_ = 1;
                newRoot->isLeaf_ = true;
                root_ = newRoot;
                return newRoot;
            }
            else
            {
                NodePtr node = root_;
                constexpr size_t keysCnt = 2 * t_ - 1;
                while (!node->IsLeaf())
                {
                    for (size_t i = 0; i <= keysCnt; i++)
                    {
                        if (node->keys_[i] != 0)
                        {
                            if (key < node->keys_[i])
                            {
                                node = node->childs_[i];
                                break;
                            }
                        }

                        if ((node->keys_[i + 1] == 0) && (key > node->keys_[i]))
                        {
                            node = node->childs_[i + 1];
                            break;
                        }
                            
                    }
                }

                DoInsert(key, node);

                while (node->count_ == 2 * t_)
                {
                    DoRestruct(node);
                    if (node == root_)
                        break;
                    
                    if (!node->parent_.expired())
                        node = node->parent_.lock();
                }

                return node;
            }
        }
    private:

        NodePtr DoInsert(size_t key, const NodePtr& node)
        {
            node->keys_[node->count_] = key;
            ++node->count_;

            constexpr size_t keysCnt = 2 * t_ - 1;
            for (size_t i = 0; i < keysCnt; i++)
            {
                for (size_t j = i + 1; j <= keysCnt; j++)
                {
                    if ((node->keys_[i] != 0) && (node->keys_[j] != 0))
                    {
                        if ((node->keys_[i]) > (node->keys_[j]))
                        {
                            std::swap(node->keys_[i], node->keys_[j]);
                           
                        }
                    }
                    else break;
                }
            }
            return node;
        }

        void DoRestruct(const NodePtr& node)
        {
            if (node->count_ < (2 * t_ - 1)) return;

            NodePtr child1 = CreateNode();
            
            int j;
            for (j = 0; j <= t_ - 2; j++) child1->keys_[j] = node->keys_[j];
            for (j = t_ - 1; j <= (2 * t_ - 1); j++) child1->keys_[j] = 0;
            child1->count_ = t_ -1;

            NodePtr child2 = CreateNode();
            for (int j = 0; j <= (t_ - 1); j++) child2->keys_[j] = node->keys_[j + t_];
            for (int j = t_; j <= (2 * t_ - 1); j++) child2->keys_[j] = 0;
            child1->count_ = t_;

            if (node->countSons_ != 0)
            {
                
                
            }
            else
            {
                child2->isLeaf_ = true;
                child2->countSons_ = 0;

                child2->isLeaf_ = true;
                child2->countSons_ = 0;
            }

            if (!node->parent_.lock())
            {
                node->keys_[0] = node->keys_[t_ - 1];

                for (int j = 1; j <= (2 * t_ - 1); j++) node->keys_[j] = 0;

                node->childs_[0] = child1;
                node->childs_[1] = child2;
                node->isLeaf_ = false;
                node->count_ = 1;
                node->countSons_ = 2;
                child1->parent_ = node;
                child2->parent_ = node;
            }
        }
    private:
        static constexpr size_t t_ = 2;
        NodePtr root_;
    };
}


using namespace TBTree;
int main()
{
    BTree23 tree;
    tree.Insert(100); //1
    tree.Insert(50); //2
    tree.Insert(25); //2

    tree.Insert(70); //3








    return 0;
    //TBTree::BTree<2,size_t,size_t> tree;
}

