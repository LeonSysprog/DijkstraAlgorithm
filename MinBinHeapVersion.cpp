#include <iostream>
#include <climits>
#include <queue>
#include <list>

#define V 8

using namespace std;

struct AdjListNode {
    int endPoint, weight;
    AdjListNode(int p, int w) : endPoint(p), weight(w) {}
};

struct Comparator {
    bool operator() (pair<int, int> a, pair<int, int> b) {
        return a.second > b.second;
    }
};

class MinHeap : public priority_queue<pair<int, int>, vector<pair<int, int>>, Comparator> {
    public:
        typedef typename priority_queue<pair<int, int>, vector<pair<int, int>>, Comparator>::container_type::iterator iterator;
        iterator find(int val) {
            auto first = this->c.begin();
            auto last = this->c.end();
            while (first != last) {
                if (first->second == val)
                    return first;
                ++first;
            }

            return last;
        }
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
            MinHeap *minHeap = new MinHeap();
            int dist[V];

            for (int v = 0; v < V; ++v) {
                if (v == src) {
                    minHeap->push(pair<int, int>(0, v));
                    dist[v] = 0;
                }
                else {
                    minHeap->push(pair<int, int>(INT_MAX, v));
                    dist[v] = INT_MAX;
                }
            }
 
            for (int count = 0; count < V; ++count) {

                // min distance
                int u = minHeap->top().second;
                minHeap->pop();
 
                auto pCrawl = array[u].begin();
                while (pCrawl != array[u].end()) {
                    int v = pCrawl->endPoint;

                    // update dist
                    if (dist[u] != INT_MAX && pCrawl->weight + dist[u] < dist[v]) {
                        dist[v] = dist[u] + pCrawl->weight;
                        minHeap->find(v)->first = dist[v];
                    }

                    ++pCrawl;
                }
            }

            printArr(dist, V);
        } 
};   
 
// Driver program to test above functions
int main()
{
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