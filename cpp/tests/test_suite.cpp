#include "graph.hpp"
#include <iostream>
#include <cassert>

using namespace resiligraph;

void testDinicMaxFlow() {
    ResiliGraph g(4);
    int s = g.addNode("S", "X", NodeType::TIER1_SUPPLIER);
    int u = g.addNode("U", "X", NodeType::REGIONAL_DC);
    int v = g.addNode("V", "X", NodeType::REGIONAL_DC);
    int t = g.addNode("T", "X", NodeType::ASSEMBLY_PLANT);

    g.addEdge(s, u, 10, 1.0);
    g.addEdge(s, v, 10, 1.0);
    g.addEdge(u, v, 2, 1.0);
    g.addEdge(u, t, 4, 1.0);
    g.addEdge(v, t, 9, 1.0);

    long long flow = g.computeMaxFlow(s, t);
    // Max flow should be 13: u->t sends 4, v->t sends 9 (using u->v 0, s->u 4, s->v 9)
    assert(flow == 13);
    std::cout << "[PASS] testDinicMaxFlow\n";
}

void testTarjanSPOF() {
    ResiliGraph g(5);
    int a = g.addNode("A", "X", NodeType::TIER1_SUPPLIER);
    int b = g.addNode("B", "X", NodeType::REGIONAL_DC);
    int c = g.addNode("C", "X", NodeType::PORT_CHOKEPOINT);
    int d = g.addNode("D", "X", NodeType::REGIONAL_DC);
    int e = g.addNode("E", "X", NodeType::ASSEMBLY_PLANT);

    // Triangle A-B-C, then C-D bridge, then D-E bridge
    g.addEdge(a, b, 10, 1);
    g.addEdge(b, c, 10, 1);
    g.addEdge(c, a, 10, 1);
    g.addEdge(c, d, 10, 1);
    g.addEdge(d, e, 10, 1);

    auto spofs = g.detectSPOFs();
    // C and D should be articulation points
    bool foundC = false, foundD = false;
    for (int node : spofs.articulationNodes) {
        if (node == c) foundC = true;
        if (node == d) foundD = true;
    }
    assert(foundC && foundD);
    assert(!spofs.criticalBridges.empty());
    std::cout << "[PASS] testTarjanSPOF\n";
}

void testDijkstraRerouting() {
    ResiliGraph g(3);
    int s = g.addNode("S", "X", NodeType::TIER1_SUPPLIER);
    int m = g.addNode("M", "X", NodeType::PORT_CHOKEPOINT);
    int t = g.addNode("T", "X", NodeType::ASSEMBLY_PLANT);

    g.addEdge(s, m, 10, 20.0);
    g.addEdge(m, t, 10, 30.0);
    g.addEdge(s, t, 5, 100.0); // direct bypass path with higher cost

    std::vector<bool> activeAll(3, true);
    auto sp1 = g.shortestPath(s, t, activeAll);
    assert(sp1.first == 50.0); // via m

    std::vector<bool> activeSevered(3, true);
    activeSevered[m] = false;
    auto sp2 = g.shortestPath(s, t, activeSevered);
    assert(sp2.first == 100.0); // rerouted around severed port

    std::cout << "[PASS] testDijkstraRerouting\n";
}

int main() {
    std::cout << "--- Running ResiliGraph C++ Unit Tests ---\n";
    testDinicMaxFlow();
    testTarjanSPOF();
    testDijkstraRerouting();
    std::cout << "All ResiliGraph tests passed successfully!\n";
    return 0;
}
