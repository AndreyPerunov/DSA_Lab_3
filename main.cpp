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
      std::cout << "  ⊛ " << value << std::endl;
        for (auto edge : edges) {
            if (edge.node != nullptr) {
                std::cout << "   ⤷――― " << edge.weight << " ――――> ⊛ " << edge.node->value << std::endl;
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
            //! UNCOMMENT FOR UNDIRECTED GRAPH:
            // nodes[j].addNext(&nodes[i], weight); // vertex2 -> vertex1 
            return;
          }
        }
      }
    }
  }

  std::vector<std::vector<int>> adjacencyMatrix() {
    std::vector<std::vector<int>> matrix;
    for (int i = 0; i < nodes.size(); i++) {
      std::vector<int> row;
      for (int j = 0; j < nodes.size(); j++) {
        int weight = 0;
        for (auto edge : nodes[i].edges) {
          if (edge.node != nullptr && edge.node->value == nodes[j].value) {
            weight = edge.weight;
            break;
          }
        }
        row.push_back(weight);
      }
      matrix.push_back(row);
    }
    return matrix;
  }

  void print(std::vector<std::vector<int>> matrix) {
    std::cout << "Adjacency Matrix: " << std::endl;
    for (std::vector<int> row : matrix) {
      for (int col : row) {
        std::cout << col << " ";
      }
      std::cout << std::endl;
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
  // Declare Graph
  Graph<std::string> graph;  

  // Read Data
  std::ifstream file("movies.data");
  //! There is � � � in the file, so I changed encoding to utf8

  if (!file.is_open() || !file) {
    std::cerr << "Error opening file" << std::endl;
    return 1;
  }

  std::string line;

  bool nodesEnds = false;
  while (getline(file, line)) {
    // Change state when nodes end
    if (line == "---------------") {
      nodesEnds = true;
      continue;
    }
    // Skip empty lines
    if (line.empty()) continue;
    try {
      if (!nodesEnds) {
        // Add line as a node to graph
        graph.addNode(line);
      } else {
        // Extract vertex1 from line
        std::string vertex1 = line.substr(0, line.find(","));
        line.erase(0, line.find(",") + 1);
        // Extract vertex2 from line
        std::string vertex2 = line.substr(0, line.find(","));
        line.erase(0, line.find(",") + 1);
        // Extract weight of an edge from line
        int weight = std::stoi(line);

        // Add edge between vertex1 and vertex2 with weight
        graph.addEdge(vertex1, vertex2, weight);
      }
    } catch (const std::invalid_argument& e) {
      std::cerr << "Error reading line number." << std::endl;
    }
  }

  file.close();

  // Print Graph
  graph.print();

  // Adjacency Matrix
  graph.print(graph.adjacencyMatrix());



  return 0;
}