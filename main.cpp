#include <experimental/filesystem>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <vector>

#include "TreeHasher.h"

using namespace std;

int main() {
  int n;
  cin >> n;
  vector<unordered_set<Graph, TreeHasher::TreeHash, TreeHasher::TreeCmp>>
      layers(2);
  Graph firstGraph(2, vector<unsigned>());
  firstGraph[0] = {1};
  firstGraph[1] = {0};
  layers[1].insert(firstGraph);
  for (int layerNumber = 2; layerNumber < n; layerNumber++) {
    layers.push_back({});
    unordered_set<Graph, TreeHasher::TreeHash, TreeHasher::TreeCmp> &layer =
        layers[layerNumber];
    unordered_set<Graph, TreeHasher::TreeHash, TreeHasher::TreeCmp> &prevLayer =
        layers[layerNumber - 1];
    for (auto graph : prevLayer) {
      for (int j = 0; j < layerNumber; j++) {
        Graph newGraph = graph;
        newGraph.push_back({});
        newGraph[j].push_back(layerNumber);
        newGraph[layerNumber].push_back(j);
        layer.insert(newGraph);
      }
    }
  }
  int it = 0;
  system("rm -rf output");
  system("mkdir output");
  if (n == 0) {
    return 0;
  }
  if (n == 1) {
    std::string name = "output/graph1";
    ofstream file(name + ".dot");
    file << "graph G {" << endl;
    file << "0;" << endl;
    file << "}" << endl;
    system(("dot -Tpng " + name + ".dot" + " -o " + name + ".png").c_str());
    return 0;
  }
  for (auto graph : layers.back()) {
    it++;
    std::string name = "output/graph" + std::to_string(it);
    ofstream file(name + ".dot");
    file << "graph G {" << endl;
    for (int i = 0; i < graph.size(); i++) {
      for (int j : graph[i]) {
        file << i << " -- " << j << ";" << endl;
      }
    }
    file << "}" << endl;
    system(("dot -Tpng " + name + ".dot" + " -o " + name + ".png").c_str());
  }
}