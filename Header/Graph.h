#ifndef GRAPH_H
#define GRAPH_H
#include<unordered_map>
#include<unordered_set>
#include <vector>
#include "Answer.h"

using namespace std;

class Graph {
private:
  //all nodes will have an edge but not more than 4
  //so adjacency matrix would take up too much space
  std::unordered_map<int, std::unordered_set<int>>* vEdges;
  //edges will be where answer has one question from and another to
  void generateEdges(const std::vector<Answer>& fromTo);
public:
  Graph();
  //questionID as vertex
  Graph(const std::vector<int>& vertexes, const std::vector<Answer>& fromTo);
  ~Graph();
  std::unordered_map<int, std::unordered_set<int>>* getEdges();
};

#endif //GRAPH_H
