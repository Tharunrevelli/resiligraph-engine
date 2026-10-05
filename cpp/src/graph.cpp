#include "graph.hpp"
#include <chrono>
#include <random>
#include <iostream>

namespace resiligraph {

ResiliGraph::ResiliGraph(int n) {
    nodes_.reserve(n);
    adj_.resize(n);
    level_.resize(n);
    ptr_.resize(n);
}

int ResiliGraph::addNode(const std::string& name, const std::string& country, NodeType type, double failureProb) {
    int id = static_cast<int>(nodes_.size());
    nodes_.emplace_back(id, name, country, type, failureProb);
    if (static_cast<size_t>(id) >= adj_.size()) {
        adj_.resize(id + 1);
        level_.resize(id + 1);
        ptr_.resize(id + 1);
    }
    return id;
}

void ResiliGraph::addEdge(int from, int to, long long capacity, double transitCost) {
    Edge forward(to, static_cast<int>(adj_[to].size()), capacity, transitCost);
    Edge backward(from, static_cast<int>(adj_[from].size()), 0, -transitCost);
    adj_[from].push_back(forward);
    adj_[to].push_back(backward);
}

const Node& ResiliGraph::getNode(int id) const {
    return nodes_.at(id);
}

Node& ResiliGraph::getNode(int id) {
    return nodes_.at(id);
}

bool ResiliGraph::dinicBFS(int s, int t, const std::vector<bool>& activeNodes) {
    std::fill(level_.begin(), level_.end(), -1);
    level_[s] = 0;
    std::queue<int> q;
    q.push(s);

    while (!q.empty()) {
        int v = q.front();
        q.pop();

        for (const auto& edge : adj_[v]) {
            if (edge.capacity - edge.flow > 0 && level_[edge.to] == -1) {
                if (activeNodes[edge.to]) {
                    level_[edge.to] = level_[v] + 1;
                    q.push(edge.to);
                }
            }
        }
    }

    return level_[t] != -1;
}

long long ResiliGraph::dinicDFS(int v, int t, long long pushed, const std::vector<bool>& activeNodes) {
    if (pushed == 0 || v == t) return pushed;

    for (size_t& cid = ptr_[v]; cid < adj_[v].size(); ++cid) {
        auto& edge = adj_[v][cid];
        int tr = edge.to;

        if (level_[v] + 1 != level_[tr] || edge.capacity - edge.flow == 0 || !activeNodes[tr]) {
            continue;
        }

        long long trPushed = dinicDFS(tr, t, std::min(pushed, edge.capacity - edge.flow), activeNodes);
        if (trPushed == 0) continue;

        edge.flow += trPushed;
        adj_[tr][edge.revIndex].flow -= trPushed;
        return trPushed;
    }

    return 0;
}

long long ResiliGraph::computeMaxFlow(int source, int sink) {
    // Reset all flows
    for (auto& nList : adj_) {
        for (auto& edge : nList) {
            edge.flow = 0;
        }
    }

    std::vector<bool> active(nodes_.size(), true);
    for (size_t i = 0; i < nodes_.size(); ++i) {
        active[i] = nodes_[i].isOperational;
    }

    if (!active[source] || !active[sink]) return 0;

    long long totalFlow = 0;
    while (dinicBFS(source, sink, active)) {
        std::fill(ptr_.begin(), ptr_.end(), 0);
        while (long long pushed = dinicDFS(source, sink, std::numeric_limits<long long>::max(), active)) {
            totalFlow += pushed;
        }
    }

    return totalFlow;
}

void ResiliGraph::tarjanDFS(int u, int p, int& timer, std::vector<int>& tin, std::vector<int>& low,
                            std::vector<bool>& isCut, std::vector<std::pair<int, int>>& bridges) {
    tin[u] = low[u] = ++timer;
    int children = 0;

    for (const auto& edge : adj_[u]) {
        if (edge.capacity <= 0) continue; // Only forward physical supply edges
        int to = edge.to;
        if (to == p) continue;

        if (tin[to] != 0) {
            // Back edge
            low[u] = std::min(low[u], tin[to]);
        } else {
            // Forward tree edge
            children++;
            tarjanDFS(to, u, timer, tin, low, isCut, bridges);
            low[u] = std::min(low[u], low[to]);

            // Articulation point condition
            if (low[to] >= tin[u] && p != -1) {
                isCut[u] = true;
            }

            // Bridge condition
            if (low[to] > tin[u]) {
                bridges.push_back({std::min(u, to), std::max(u, to)});
            }
        }
    }

    if (p == -1 && children > 1) {
        isCut[u] = true;
    }
}

SPOFAnalysisResult ResiliGraph::detectSPOFs() {
    int n = static_cast<int>(nodes_.size());
    std::vector<int> tin(n, 0);
    std::vector<int> low(n, 0);
    std::vector<bool> isCut(n, false);
    std::vector<std::pair<int, int>>& bridges = *(new std::vector<std::pair<int, int>>());
    int timer = 0;

    for (int i = 0; i < n; ++i) {
        if (tin[i] == 0) {
            tarjanDFS(i, -1, timer, tin, low, isCut, bridges);
        }
    }

    SPOFAnalysisResult res;
    for (int i = 0; i < n; ++i) {
        if (isCut[i]) {
            res.articulationNodes.push_back(i);
        }
    }
    res.criticalBridges = bridges;
    delete &bridges;

    // Vulnerability score based on proportion of critical cut vertices
    res.spofVulnerabilityScore = n > 0 ? (static_cast<double>(res.articulationNodes.size()) / n) : 0.0;
    return res;
}

std::pair<double, std::vector<int>> ResiliGraph::shortestPath(int source, int sink, const std::vector<bool>& activeNodes) {
    int n = static_cast<int>(nodes_.size());
    std::vector<double> dist(n, std::numeric_limits<double>::infinity());
    std::vector<int> parent(n, -1);

    if (!activeNodes[source] || !activeNodes[sink]) {
        return {std::numeric_limits<double>::infinity(), {}};
    }

    // Min-heap storing (distance, node)
    using PDI = std::pair<double, int>;
    std::priority_queue<PDI, std::vector<PDI>, std::greater<PDI>> pq;

    dist[source] = 0.0;
    pq.push({0.0, source});

    while (!pq.empty()) {
        double d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (d > dist[u]) continue;
        if (u == sink) break;

        for (const auto& edge : adj_[u]) {
            if (edge.capacity <= 0) continue; // Only forward edges with non-negative transit cost
            int to = edge.to;
            if (!activeNodes[to]) continue;

            double weight = edge.transitCost;
            if (dist[u] + weight < dist[to]) {
                dist[to] = dist[u] + weight;
                parent[to] = u;
                pq.push({dist[to], to});
            }
        }
    }

    if (dist[sink] == std::numeric_limits<double>::infinity()) {
        return {dist[sink], {}};
    }

    std::vector<int> path;
    for (int curr = sink; curr != -1; curr = parent[curr]) {
        path.push_back(curr);
    }
    std::reverse(path.begin(), path.end());
    return {dist[sink], path};
}

DisruptionImpact ResiliGraph::evaluateNodeSeverance(int nodeId, int source, int sink) {
    DisruptionImpact impact;
    impact.severedNodeId = nodeId;
    impact.severedNodeName = nodes_.at(nodeId).name;

    // Baseline flow
    long long baseFlow = computeMaxFlow(source, sink);
    impact.baselineMaxFlow = baseFlow;

    // Sever node
    nodes_[nodeId].isOperational = false;
    long long disruptedFlow = computeMaxFlow(source, sink);
    impact.disruptedMaxFlow = disruptedFlow;

    // Restore node
    nodes_[nodeId].isOperational = true;

    impact.lostCapacityPercent = baseFlow > 0 ? (1.0 - static_cast<double>(disruptedFlow) / baseFlow) * 100.0 : 0.0;
    impact.causesDisconnectedComponents = (disruptedFlow == 0 && baseFlow > 0);

    // Shortest path rerouting cost
    std::vector<bool> active(nodes_.size(), true);
    active[nodeId] = false;
    auto sp = shortestPath(source, sink, active);
    impact.reroutedAverageTransitCost = sp.first;

    return impact;
}

double ResiliGraph::computeEnterpriseResilienceIndex(int source, int sink, int trials) {
    long long baseline = computeMaxFlow(source, sink);
    if (baseline == 0) return 0.0;

    std::mt19937 rng(42);
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    double totalFlowRetentionRatio = 0.0;

    for (int t = 0; t < trials; ++t) {
        // Randomly fail nodes based on their failureProbability
        for (auto& node : nodes_) {
            if (node.id != source && node.id != sink) {
                node.isOperational = (dist(rng) > node.failureProbability);
            }
        }

        long long flow = computeMaxFlow(source, sink);
        totalFlowRetentionRatio += static_cast<double>(flow) / baseline;

        // Restore
        for (auto& node : nodes_) {
            node.isOperational = true;
        }
    }

    return (totalFlowRetentionRatio / trials) * 100.0;
}

} // namespace resiligraph
