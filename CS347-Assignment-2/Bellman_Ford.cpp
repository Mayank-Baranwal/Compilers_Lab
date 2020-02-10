#include<bits/stdc++.h>
#define F first
#define S second

using namespace std;

void relax (int u, int v, int w, int dist[]){
    // cout<<u<<" "<<v<<"\n";
    if (dist[u] == INT_MAX)
        return;
    dist[v] = min(dist[u]+w, dist[v]);
}

void bellman_ford(vector<pair<int, int>> edge[], int n, int src, int dist[]){
    dist[src] = 0;
    for (int i=1; i<=n; i++){
        for (int j=1; j<=n; j++){
            for (int k=0; k<edge[j].size(); k++)
                relax(j,edge[j][k].F, edge[j][k].S, dist);
        }
    }
}

int main(){
    int n,e,u,v,w;
    cin>>n>>e;
    vector<pair<int, int>> edge[n+1];
    for (int i=1; i<=e; i++){
        cin>>u>>v>>w;
        edge[u].push_back({v,w});
    }
    int dist[n+1];
    for (int i=1; i<=n; i++)    
        dist[i] = INT_MAX;
    bellman_ford(edge, n, 1, dist);
    for (int i=1; i<=n; i++){
        cout<<dist[i]<<" ";
    }
    cout<<"\n";

}
