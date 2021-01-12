#pragma once
#include <vector>
#include <map>
#include <set>
namespace Common
{
	template <class T>
	class Graph
	{
	public:
		using Vertex = T;/* std::remove_pointer_t<std::remove_reference_t<T>>;*/
		struct Neighbor
		{
			Vertex v;
			double weigth;

			bool operator <(const Neighbor& other) const
			{
				return v < other.v && weigth < other.weigth;
			}
		};
		using Neighbors = std::vector<Neighbor>;
		using Matrix = std::vector<std::vector<double>>;
		
		Graph() = default;
		~Graph() = default;

		void Clear()
		{
			nodes_.clear();
		}

		bool AddVertex(Vertex vertex)
		{
			if (nodes_.contains(vertex))
				return false;

			nodes_.insert({ vertex,{} });
			return true;
		}

		bool AddEdge(const Vertex& from, const Vertex& to, double weight = 1)
		{
			if (!Contains(from))
				AddVertex(from);

			//Warring может быть добавленно ребро с одним и тем же весом
			Neighbors& edges = nodes_[from];
			if (edges.contains({ to,weight }))
				return false;

			edges.insert({ to,weight });
			return true;
		}

		bool Contains(const Vertex& vertex) const
		{
			return nodes_.contains(vertex);
		}

		bool Contains(const Vertex& from, const Vertex& to) const
		{
			if (!Contains(from))
				return false;

			Neighbors& neigh = nodes_[from]();
			if (neigh.contains(to))
				return true;

			return false;
		}

		bool HasNeighbors(const Vertex& vertex) const
		{
			return nodes_.contains(vertex);
		}

		size_t GetCountNeighbors(const Vertex& vertex) const
		{
			return GetNeighbors().size();
		}

		Neighbors GetNeighbors(const Vertex& vertex) const
		{
			if (HasNeighbors(vertex))
				return nodes_[vertex];

			return {};
		}

		

	private:
		std::map<Vertex, Neighbors> nodes_;
	};
}