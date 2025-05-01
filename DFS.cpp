#include <iostream>
#include <vector>
#include <ctime>
#include <omp.h>

using namespace std;

const int MAX = 100000;
vector<int> graph[MAX];
bool visited[MAX];

void parallel_dfs(int node) {
    #pragma omp task firstprivate(node)
    {
        if (!visited[node]) {
            #pragma omp critical
            {
                if (!visited[node]) {
                    visited[node] = true;
                    cout << node << " ";
                } else {
                    #pragma omp cancel task
                }
            }

            for (int i = 0; i < graph[node].size(); i++) {
                int neighbor = graph[node][i];
                if (!visited[neighbor]) {
                    parallel_dfs(neighbor);
                }
            }
        }
    }
}

int main() {
    int n, m, start_node;
    cout << "Enter number of nodes, edges, and start node: ";
    cin >> n >> m >> start_node;
    
    cout << "Enter pairs of edges:\n";
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        visited[i] = false;
    }

    // Using clock() instead of omp_get_wtime()
    clock_t start = clock();
    
    #pragma omp parallel
    {
        #pragma omp single
        {
            parallel_dfs(start_node);
            #pragma omp taskwait
        }
    }
    
    clock_t end = clock();
    cout << "\nDFS took " << (double)(end - start)/CLOCKS_PER_SEC << " seconds\n";

    return 0;
}
