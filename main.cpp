#include <iostream>
#include <fstream>
#include <string>
#include <vector>  

template <typename T>
struct Edge {
  T* node;
  int weight;
};

template <typename T>
class Node {
  public:
    T value;
    std::vector<Edge<Node<T>>> edges;

    Node(T value) {
      this->value = value;
      Edge<Node<T>> edge = {nullptr, 0};
      this->edges.push_back(edge);
    }

    Node (T value, Node<T>* next, int weight) {
      this->value = value;
      Edge<Node<T>> edge = {next, weight};
      this->edges.push_back(edge);
      edge = {nullptr, 0};
      this->edges.push_back(edge);
    }

    void addNext(Node<T>* next, int weight) {
      Edge<Node<T>> edge = {next, weight};
      this->edges.insert (this->edges.begin(), edge);
    }

    void print() {    
      std::cout << "  " << value << std::endl;
        for (auto edge : edges) {
            if (edge.node != nullptr) {
                std::cout << "   ⤷――― " << edge.weight << " ――――> " << edge.node->value << std::endl;
            }
        }
    }
};



template <typename T>
class Graph {
public:
  std::vector<Node<T>> nodes;
  Graph() {
    
  }

  void addNode(T value) {
    Node<T> node = Node<T>(value);
    nodes.push_back(node);
  }

  void addEdge(T vertex1, T vertex2, int weight) {
    for(int i = 0; i < nodes.size(); i++) {
      if (nodes[i].value == vertex1) {
        for (int j = 0; j < nodes.size(); j++)
        {
          if (nodes[j].value == vertex2) {
            nodes[i].addNext(&nodes[j], weight); // vertex1 -> vertex2
            nodes[j].addNext(&nodes[i], weight); // vertex2 -> vertex1
            return;
          }
        }
      }
    }
  }

  void print() {
    std::cout << "Graph: " << std::endl;
    for (auto node : nodes) {
      node.print();
    }
  }
};


int main() {
  Graph<std::string> graph;
  graph.addNode("A");
  graph.addNode("B");
  graph.addNode("C");
  graph.addNode("D");
  graph.addNode("E");
  graph.addNode("F");
  graph.addNode("G");
  graph.addEdge("A", "B", 1);
  graph.addEdge("A", "D", 5);
  graph.addEdge("D", "C", 5);
  graph.addEdge("D", "B", 8);
  graph.print();

  // // Initialize Data Structure
  // Graph<std::string> graph;  

  // // Read Data
  // std::ifstream file("movies.data");

  // if (!file.is_open()) {
  //   std::cerr << "Error opening file" << std::endl;
  //   return 1;
  // }

  // std::string line;

  // while (getline(file, line)) {
  //   try {
  //     // Add line to data structure
  //     graph.insertNode(line);
  //   } catch (const std::invalid_argument& e) {
  //     std::cerr << "Error reading line number." << std::endl;
  //   }
  // }

  // file.close();

  // // Process Data
  // graph.printGraph();

  return 0;
}