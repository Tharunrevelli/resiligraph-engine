#include "graph.hpp"
#include <iostream>
#include <iomanip>

using namespace resiligraph;

void printBanner() {
    std::cout << "======================================================================\n";
    std::cout << "  ResiliGraph: C++ Network Flow & Supply Chain Resilience Simulator  \n";
    std::cout << "  Dinic Max-Flow, Tarjan SPOF Detection & Monte Carlo Stress-Testing  \n";
    std::cout << "======================================================================\n\n";
}

int main() {
    printBanner();

    // 1. Initialize Network Topology
    std::cout << "[Step 1/4] Constructing Global Multi-Modal Logistical Graph...\n";
    ResiliGraph g(10);

    // Nodes
    int sShenzhen = g.addNode("Shenzhen Tier-1 Fab", "China", NodeType::TIER1_SUPPLIER, 0.03); // 0
    int sBusan    = g.addNode("Busan Precision Foundry", "South Korea", NodeType::TIER1_SUPPLIER, 0.02); // 1
    int sDresden  = g.addNode("Dresden Semiconductor", "Germany", NodeType::TIER1_SUPPLIER, 0.01); // 2

    int pSingapore = g.addNode("Port of Singapore", "Singapore", NodeType::PORT_CHOKEPOINT, 0.08); // 3 (Critical SPOF)
    int pRotterdam = g.addNode("Port of Rotterdam", "Netherlands", NodeType::PORT_CHOKEPOINT, 0.04); // 4
    int pPanama    = g.addNode("Panama Canal Transit", "Panama", NodeType::PORT_CHOKEPOINT, 0.06); // 5

    int rdcAntwerp = g.addNode("Antwerp Central Logistics Hub", "Belgium", NodeType::REGIONAL_DC, 0.02); // 6
    int rdcChicago = g.addNode("Chicago Multi-Modal RDC", "USA", NodeType::REGIONAL_DC, 0.01); // 7

    int plantDetroit = g.addNode("Detroit Assembly Gigafactory", "USA", NodeType::ASSEMBLY_PLANT, 0.00); // 8

    // Virtual supersource
    int superSource = g.addNode("Global Supplier Consortium (Virtual Source)", "Global", NodeType::TIER1_SUPPLIER, 0.0); // 9

    // Capacities (TEU / month) and transit costs ($/TEU)
    // Virtual source connects to tier 1 suppliers
    g.addEdge(superSource, sShenzhen, 9000, 100);
    g.addEdge(superSource, sBusan,    7000, 120);
    g.addEdge(superSource, sDresden,  6000, 140);

    // Suppliers to Ports
    g.addEdge(sShenzhen, pSingapore, 6000, 450);
    g.addEdge(sShenzhen, pPanama,    3000, 850);
    g.addEdge(sBusan,    pSingapore, 4000, 420);
    g.addEdge(sBusan,    pPanama,    3000, 800);
    g.addEdge(sDresden,  pRotterdam, 6000, 250);

    // Ports to RDCs
    g.addEdge(pSingapore, rdcChicago, 7500, 1200);
    g.addEdge(pSingapore, rdcAntwerp, 2500, 950);
    g.addEdge(pRotterdam, rdcAntwerp, 5000, 300);
    g.addEdge(pRotterdam, rdcChicago, 2000, 1100);
    g.addEdge(pPanama,    rdcChicago, 5000, 650);

    // RDCs to Assembly Gigafactory (Sink)
    g.addEdge(rdcAntwerp, plantDetroit, 6000, 850);
    g.addEdge(rdcChicago, plantDetroit, 14000, 180);

    std::cout << "  Constructed network with " << g.nodeCount() << " nodes representing intercontinental corridors.\n\n";

    // 2. Compute Baseline Maximum Flow via Dinic's Algorithm
    std::cout << "[Step 2/4] Executing Dinic's Maximum Flow Algorithm O(V^2 E)...\n";
    long long baselineMaxFlow = g.computeMaxFlow(superSource, plantDetroit);
    std::cout << "  Baseline Global Network Throughput: " << baselineMaxFlow << " TEU/month\n";
    std::cout << "  Flow Algorithm: BFS Level Graph + DFS Blocking Flow Augmentation\n\n";

    // 3. Tarjan's Single Point of Failure (SPOF) Detection
    std::cout << "[Step 3/4] Running Tarjan's Bridge & Cut Vertex Discovery O(V + E)...\n";
    SPOFAnalysisResult spofRes = g.detectSPOFs();
    std::cout << "  Identified " << spofRes.articulationNodes.size() << " Critical Articulation Nodes (Single Points of Failure):\n";
    for (int nodeId : spofRes.articulationNodes) {
        std::cout << "    -> Node [" << nodeId << "] " << g.getNode(nodeId).name
                  << " (" << nodeTypeToString(g.getNode(nodeId).type) << ")\n";
    }
    std::cout << "  Identified " << spofRes.criticalBridges.size() << " Critical Bridge Bottleneck Corridors:\n";
    for (const auto& br : spofRes.criticalBridges) {
        std::cout << "    -> Bridge Link: [" << g.getNode(br.first).name
                  << " <---> " << g.getNode(br.second).name << "]\n";
    }
    std::cout << "  Enterprise Graph Vulnerability Index: " << std::fixed << std::setprecision(2)
              << (spofRes.spofVulnerabilityScore * 100.0) << "%\n\n";

    // 4. Disruption Stress-Testing & Dijkstra Dynamic Rerouting
    std::cout << "[Step 4/4] Simulating Geopolitical Chokepoint & Port Severance Shocks...\n";
    int criticalNodesToTest[] = {pSingapore, pPanama, rdcChicago};

    for (int testNode : criticalNodesToTest) {
        DisruptionImpact impact = g.evaluateNodeSeverance(testNode, superSource, plantDetroit);
        std::cout << "  Scenario: Severing [" << impact.severedNodeName << "]\n";
        std::cout << "    - Capacity Lost: " << std::setprecision(1) << impact.lostCapacityPercent
                  << "% (Throughput drops to " << impact.disruptedMaxFlow << " TEU/mo)\n";
        std::cout << "    - Dijkstra Rerouted Transit Cost: $" << std::setprecision(2)
                  << impact.reroutedAverageTransitCost << " / TEU\n";
        std::cout << "    - Network Partition Risk: "
                  << (impact.causesDisconnectedComponents ? "CRITICAL OUTAGE" : "Mitigated via Alternative Corridors") << "\n\n";
    }

    // Monte Carlo Resilience Analysis
    int trials = 1000;
    std::cout << "  Running 1,000-Trial Monte Carlo Network Resilience Assessment...\n";
    double eri = g.computeEnterpriseResilienceIndex(superSource, plantDetroit, trials);
    std::cout << "  Computed Enterprise Resilience Index (ERI): " << std::setprecision(2) << eri << "% Flow Retention\n";

    std::cout << "\n======================================================================\n";
    std::cout << "  ResiliGraph Engine Simulation Completed Successfully.\n";
    std::cout << "======================================================================\n";

    return 0;
}
