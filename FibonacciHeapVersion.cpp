#include <iostream>
#include <list>
#include "FibonacciHeap.h"

#define V 8

using namespace std;

struct AdjListNode {
    int endPoint, weight;
    AdjListNode(int p, int w) : endPoint(p), weight(w) {}
};

// A utility function used to print the solution
void printArr(int dist[], int n) {
    printf("Vertex   Distance from Source\n");
    for (int i = 0; i < n; ++i)
        printf("%d \t\t %d\n", i, dist[i]);
} 
 
class Graph {
    private:
        list<AdjListNode> *array;

    public:
        Graph() {
            array = new list<AdjListNode>[V];
        }

        // Add edge in directed graph
        void addEdge(int src, int endPoint, int weight) {
            array[src].push_back(AdjListNode(endPoint, weight));
        }

        void dijkstra(int src) {
            FibonacciHeap *fibHeap = new FibonacciHeap();
            int dist[V];

            for (int v = 0; v < V; ++v) {
                if (v == src) {
                    fibHeap->insert(0, v); //<0, v>
                    dist[v] = 0;
                }
                else {
                    fibHeap->insert(INT_MAX, v);  //<INT_MAX, v>
                    dist[v] = INT_MAX;
                }
            }
 
            for (int count = 0; count < V; ++count) {

                // min distance
                int u = fibHeap->getMinKey();
                fibHeap->removeMin();
 
                auto pCrawl = array[u].begin();
                while (pCrawl != array[u].end()) {
                    int v = pCrawl->endPoint;

                    // update dist
                    if (dist[u] != INT_MAX && pCrawl->weight + dist[u] < dist[v]) {
                        dist[v] = dist[u] + pCrawl->weight;
                        fibHeap->find(fibHeap->getMinNode(), v)->weight = dist[v];
                    }

                    ++pCrawl;
                }
            }

            printArr(dist, V);
        } 
};

int main() {
    // create the graph given in above fugure
    Graph* graph = new Graph();
    graph->addEdge(0, 1, 4);
    graph->addEdge(0, 7, 8);
    graph->addEdge(1, 2, 8);
    graph->addEdge(1, 7, 11);
    graph->addEdge(2, 3, 7);
    graph->addEdge(2, 8, 2);
    graph->addEdge(2, 5, 4);
    graph->addEdge(3, 4, 9);
    graph->addEdge(3, 5, 14);
    graph->addEdge(4, 5, 10);
    graph->addEdge(5, 6, 2);
    graph->addEdge(6, 7, 1);
    graph->addEdge(6, 8, 6);
    graph->addEdge(7, 8, 7);
 
    graph->dijkstra(0);
}