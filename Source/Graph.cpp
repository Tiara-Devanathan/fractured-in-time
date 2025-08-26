#include "Graph.h"

using namespace std;

Graph::Graph(const std::vector<int>& vertexes, const std::vector<Answer>& fromTo){
  vEdges = new std::unordered_map<int, std::unordered_set<int>>();
  for(int vertex : vertexes){
    vEdges->insert(std::make_pair(vertex, std::unordered_set<int>()));
  }
  generateEdges(fromTo);
}

Graph::~Graph(){
  delete vEdges;
}

std::unordered_map<int, std::unordered_set<int>>* Graph::getEdges(){
  return vEdges;
}

void Graph::generateEdges(const std::vector<Answer>& fromTo){
  //to question be the vertex
  //since we want end to be the root for the bfs
  // an edge will be from question to predesessor
    for (const auto& ft : fromTo) {
        if (vEdges->contains(ft.toQuestionID)) {
            vEdges->at(ft.toQuestionID).insert(ft.fromQuestionID);
        }
    }
}

Graph::Graph()
{
    vEdges = nullptr;
}

