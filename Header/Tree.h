#ifndef TREE_H
#define TREE_H

#include<unordered_set>
#include<vector>
#include"Graph.h"

using namespace std;

class Graph;

class Tree {
private:
  struct Node{
    int value;
    std::unordered_set<Node*> children;
    ~Node();
  };
  Node* root;
  std::vector<int> path(const Node* n,const int& goal, std::vector<int> curPath);

public:
  Tree();
  ~Tree();
  std::vector<int> path(const int& goal);
  static Tree* bfs(int root, Graph* g);
};

#endif //TREE_H
