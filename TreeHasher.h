#ifndef TREE_HASHER_H
#define TREE_HASHER_H

#include <vector>
#include <deque>

using namespace std;

using Graph = vector<vector<unsigned>>;

class TreeHasher{

public:
static unsigned hash_tree(const Graph& g)
{
    vector<unsigned> v_hash;
    vector<unsigned> leafs;
    vector<unsigned> is_leaf;
    vector<unsigned> used;
    vector<unsigned> layers;

	unsigned hash_number = 1310975;
	unsigned m = g.size()-1;
	unsigned n = m + 1;

	leafs.clear();
	v_hash.assign(n, 1);
	is_leaf.assign(n, 0);
	for (unsigned i = 0; i < n; i++)
	{
		is_leaf[i] = g[i].size();
		if (g[i].size() == 1)
			leafs.push_back(i);
	}

	deque<unsigned> bfs;

	used.assign(n, -1);
	layers.assign(n, -1);

	for (auto i : leafs)
	{
		bfs.push_back(i);
		v_hash[i] = 1;
		used[i] = 0;
		layers[i] = 0;

	}
	vector<unsigned> last_vertex;
	while(bfs.size() > 0)
	{
		
		bool is_last = true;
		unsigned vertex = bfs[0];
		used[vertex] = 1;
		for (auto i : g[vertex])
		{
			is_leaf[i] -= 1;
			if (used[i] == -1 and is_leaf[i] == 1)
			{
				is_last = false;
				used[i] = 0;
				layers[i] = layers[vertex] + 1;
				v_hash[i] *= (v_hash[vertex] + hash_number);
				bfs.push_back(i);
			}
			else if (used[i] == 0 and is_leaf[i] == 0 and layers[i]>layers[vertex])
			{
				is_last = false;
				v_hash[i] *= (v_hash[vertex] + hash_number);
			}
			else if (used[i] == -1)
			{

				is_last = false;
				v_hash[i] *= (v_hash[vertex] + hash_number);
			}
		}
		if (is_last)
			last_vertex.push_back(vertex);
		bfs.pop_front();
	}
	unsigned last_hash = 1;
	for (auto i : last_vertex)
	{
		last_hash = (last_hash * (v_hash[i] + hash_number));
	}
	return last_hash;
}

public:
struct TreeHash {
    unsigned operator() (const Graph& graph1) const 
    {
        return hash_tree(graph1);
    }
};

struct TreeCmp {
    bool operator() (const Graph& graph1, const Graph& graph2) const 
    {
        return hash_tree(graph1) <= hash_tree(graph2);
    }
};
};

#endif