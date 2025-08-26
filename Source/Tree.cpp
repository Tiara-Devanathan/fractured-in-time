#include "Tree.h"
#include <queue>

using namespace std;

Tree::Tree(){
  root = nullptr;
}

Tree::~Tree(){
  delete root;
}

//Find shortest path, works with progrss bar
Tree* Tree::bfs(int root, Graph* g){
  Tree* result = new Tree();
  result->root = new Node{root, std::unordered_set<Node*>()};
  const std::unordered_map<int, std::unordered_set<int>>* edges = g->getEdges();
  std::unordered_set<int> visited;
  std::queue<Node*> q;
  q.push(result->root);
  visited.insert(result->root->value);

  while(!q.empty()){
    Node* cur = q.front();

    for(const int& x : edges->find(cur->value)->second){
      if(visited.find(x) == visited.end()){
        Node* toAdd = new Node{x, std::unordered_set<Node*>()};
        cur->children.insert(toAdd);
        q.push(toAdd);
        visited.insert(x);
      }
    }
    q.pop();
  }
  return result;
}

std::vector<int> Tree::path(const int& goal) {
    if (!root) return {};  // Handle empty tree case

    std::vector<int> initialPath;
    initialPath.push_back(root->value);
    return path(root, goal, initialPath);
}

std::vector<int> Tree::path(const Node* n, const int& goal, std::vector<int> curPath) {
    // Check current node
    if (n->value == goal) {
        return curPath;
    }

    // Search through children
    for (const Node* child : n->children) {
        std::vector<int> newPath = curPath;  // Make a copy for this branch
        newPath.push_back(child->value);

        std::vector<int> result = path(child, goal, newPath);
        if (!result.empty()) {  // If path found in this branch
            return result;
        }
    }

    return {};  // Return empty vector if no path found
}

//Deconstructor
Tree::Node::~Node()
{
    for (auto x : this->children) {
        delete x;
    }
}
