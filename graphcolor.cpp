#include "prototype.h"

void dsaturColorGeneration(vector<vector<int>>& input_graph, vector<int>& color_map){
    priority_queue<tuple<int, int, int>> queue;//saturation, degree, index
    while((connectedDijkstra(input_graph, color_map, queue)) == 0);
}

void generateDegreeHash(vector<vector<int>>& input_graph, priority_queue<tuple<int, int>>& degree_hash){
    for(int i = 0; i < input_graph.size(); i++){
        degree_hash.push(std::make_tuple(input_graph[i].size() - 1, i));
    }
}

int findPlausibleColor(int courseSize, vector<int>& neighbourhood, vector<int>& color_map){
    vector<uint64_t> bitmaps((courseSize / 64) + 1, ~0);
    int color;
    for(auto i: neighbourhood){
        color = color_map[i];
        if(color == -1) continue;
        bitmaps[color / 64] &= ~(1 << (color % 64));
    }
    for(int i = 0; i < courseSize; i++){
        if(bitmaps[i / 64] & (1 << (i % 64))) return i;
    }
    return 0;
}

void insertVertexDsatur(int vertex_index, int current_saturation, vector<vector<int>>& input_graph, vector<int>& color_map, priority_queue<tuple<int, int, int>>& queue){
    for(auto i: input_graph[vertex_index]){
        if(color_map[i] != -1) continue;
        queue.push(std::make_tuple(current_saturation + 1, input_graph[i].size(), i));
    }
}