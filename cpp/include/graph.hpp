#ifndef RESILIGRAPH_GRAPH_HPP
#define RESILIGRAPH_GRAPH_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include <limits>
#include <algorithm>

namespace resiligraph {

enum class NodeType {
    TIER1_SUPPLIER,
    PORT_CHOKEPOINT,
    REGIONAL_DC,
    ASSEMBLY_PLANT
};

inline std::string nodeTypeToString(NodeType type) {
    switch (type) {
        case NodeType::TIER1_SUPPLIER: return "Tier-1 Supplier";
        case NodeType::PORT_CHOKEPOINT: return "Port Chokepoint";
        case NodeType::REGIONAL_DC: return "Regional DC";
        case NodeType::ASSEMBLY_PLANT: return "Assembly Plant";
        default: return "Unknown";
    }
}

struct Node {
    int id;
    std::string name;
    std::string country;
    NodeType type;
    double failureProbability; // e.g. 0.05
    bool isOperational;

    Node(int id, std::string name, std::string country, NodeType type, double failureProb = 0.02)
        : id(id), name(std::move(name)), country(std::move(country)),
          type(type), failureProbability(failureProb), isOperational(true) {}
};

struct Edge {
    int to;
    int revIndex;
    long long capacity;
    long long flow;
    double transitCost;

    Edge(int to, int rev, long long cap, double cost)
        : to(to), revIndex(rev), capacity(cap), flow(0), transitCost(cost) {}
};

struct SPOFAnalysisResult {
    std::vector<int> articulationNodes; // Single Points of Failure nodes
    std::vector<std::pair<int, int>> criticalBridges; // Single Points of Failure corridors
    double spofVulnerabilityScore; // 0 to 1
};

struct DisruptionImpact {
    int severedNodeId;
    std::string severedNodeName;
    long long baselineMaxFlow;
    long long disruptedMaxFlow;
    double lostCapacityPercent;
    double reroutedAverageTransitCost;
    bool causesDisconnectedComponents;
};

class ResiliGraph {
public:
    ResiliGraph(int n);

    int addNode(const std::string& name, const std::string& country, NodeType type, double failureProb = 0.02);
    void addEdge(int from, int to, long long capacity, double transitCost);

    const Node& getNode(int id) const;
    Node& getNode(int id);
    int nodeCount() const { return static_cast<int>(nodes_.size()); }

    // Dinic's Algorithm for Maximum Flow O(V^2 * E)
    long long computeMaxFlow(int source, int sink);

    // Tarjan's Algorithm for Bridges & Articulation Points O(V + E)
    SPOFAnalysisResult detectSPOFs();

    // Dijkstra's Shortest Path with Indexed Binary Min-Heap O(E log V)
    std::pair<double, std::vector<int>> shortestPath(int source, int sink, const std::vector<bool>& activeNodes);

    // Disruption Stress-Tester: analyzes impact of severing a specific node
    DisruptionImpact evaluateNodeSeverance(int nodeId, int source, int sink);

    // Monte Carlo Resilience Assessment
    double computeEnterpriseResilienceIndex(int source, int sink, int trials = 1000);

private:
    std::vector<Node> nodes_;
    std::vector<std::vector<Edge>> adj_;
    std::vector<int> level_;
    std::vector<size_t> ptr_;

    // BFS for Dinic level graph construction
    bool dinicBFS(int s, int t, const std::vector<bool>& activeNodes);
    // DFS for Dinic blocking flow
    long long dinicDFS(int v, int t, long long pushed, const std::vector<bool>& activeNodes);

    // Tarjan DFS helper
    void tarjanDFS(int u, int p, int& timer, std::vector<int>& tin, std::vector<int>& low,
                   std::vector<bool>& isCut, std::vector<std::pair<int, int>>& bridges);
};

} // namespace resiligraph

#endif // RESILIGRAPH_GRAPH_HPP
