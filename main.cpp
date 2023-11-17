#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <stack>

const int INF = INT_MAX;

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
      std::cout << "  * " << value << std::endl;
      // std::cout << "  ⊛ " << value << std::endl;
        for (auto edge : edges) {
            if (edge.node != nullptr) {
                std::cout << "   --- " << edge.weight << " ---> * " << edge.node->value << std::endl;
                // std::cout << "   ⤷――― " << edge.weight << " ――――> ⊛ " << edge.node->value << std::endl;
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

  int findIndex(T value) {
    for (int i = 0; i < nodes.size(); i++) {
      if (nodes[i].value == value) {
        return i;
      }
    }
    return -1;
  }

  void dfs(std::vector<Node<T>>& adjacencyList, int v, std::vector<int> &visited, bool &hasCycle) {
    visited[v] = 1;
    
    for (auto edge : adjacencyList[v].edges) {
      if (edge.node == nullptr) continue;
      int to = Graph::findIndex(edge.node->value);
      
      if(visited[to] == 0) {
        dfs(adjacencyList, to, visited, hasCycle);
      } else if (visited[to] == 1) {
        hasCycle = true;
      }
    }

    visited[v] = 2;
  }

  bool isThereCycle() {
    std::vector<int> visited(nodes.size(), 0);
    bool hasCycle = false;

    for (int i = 0; i < nodes.size(); i++) {
      if (!visited[i]) {
        dfs(nodes, i, visited, hasCycle);
      }
    }

    return hasCycle;
  }

  void print(std::vector<std::vector<int>> matrix) {
    std::cout << "Adjacency Matrix: " << std::endl;
    for (std::vector<int> row : matrix) {
      for (int col : row) {
        std::cout << col << " ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  }

  std::vector<int> convertToPath(std::vector<int> prevNodes, int start, int end) {
    std::vector<int> path;
    std::stack<int> tmpPath;
    int current = end;
    while (current != -1)
    {
      tmpPath.push(current);
      current = prevNodes[current];
    }
    while (!tmpPath.empty())
    {
      path.push_back(tmpPath.top());
      tmpPath.pop();
    }
    return path;    
  }

  std::vector<int> findPath(T start, T end) {
    int startIndex = findIndex(start);
    int endIndex = findIndex(end);
    std::vector<bool> visited(nodes.size(), false);
    std::vector<int> nodesWeights(nodes.size(), INF);
    std::vector<int> prevNodes(nodes.size(), -1);
    nodesWeights[startIndex] = 0;
    std::map<int, int> minDistance; // distance, vertex
    minDistance[0] = startIndex;
    while (!minDistance.empty()) {
      auto [weight, index] = *minDistance.begin();
      minDistance.erase(minDistance.begin());
      if (visited[index]) continue;
      visited[index] = true;

      for (int i = 0; i < nodes[index].edges.size()-1; i++) {
        int toIndex = findIndex(nodes[index].edges[i].node->value);
        int edgeWeight = nodes[index].edges[i].weight;
        if (!visited[toIndex]) {
          if (weight + edgeWeight < nodesWeights[toIndex]) {
            nodesWeights[toIndex] = weight + edgeWeight;
            prevNodes[toIndex] = index;
            minDistance[nodesWeights[toIndex]] = toIndex;
          }
        }
        
      }       
    }
    return convertToPath(prevNodes, startIndex, endIndex);
  }

  void print() {
    std::cout << "Graph: " << std::endl;
    for (auto node : nodes) {
      node.print();
    }
    std::cout << std::endl;
  }

  int nodeDegree (T value) {
    for (auto node : nodes) {
      if (node.value == value) {
        return (node.edges.size() - 1);
      }
    }
    return -1;
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
  std::cout << std::endl;

  // Print Graph
  graph.print();

  // Adjacency Matrix
  graph.print(graph.adjacencyMatrix());

  // Task 1: How many connections all graph nodes has?
  for (auto node : graph.nodes) {
    std::cout << "Node: " << node.value << " | Degree: " << graph.nodeDegree(node.value) << std::endl;
  }
  std::cout << std::endl;

  // Task 2: Is there a cycle in a Graph?
  if (graph.isThereCycle()) {
    std::cout << "There is a cycle in the graph." << std::endl << std::endl;
  } else {
    std::cout << "There is no cycle in the graph." << std::endl << std::endl;
  }

  // Task 3: Dijkstra's Algorithm
  std::vector<int> indexPath = graph.findPath(graph.nodes[0].value, graph.nodes[graph.nodes.size() - 1].value);
  std::cout << graph.nodes[indexPath[0]].value;
  for (int i = 1; i < indexPath.size(); i++) {
    std::cout << " -> " << graph.nodes[indexPath[i]].value;
  }
  std::cout << std::endl;

  return 0;
}