#include "prototype.h"

void dsaturColorGeneration(vector<vector<int>>& input_graph, vector<int>& color_map){
    priority_queue<tuple<int, int, int>> queue;//saturation, degree, index
    while((connectedDijkstra(input_graph, color_map, queue)) == 0);
}

int
connectedDijkstra
        (vector<vector<int>>& input_graph, vector<int>& color_map, priority_queue<tuple<int, int, int>>& queue){
    int max_degree = -1;
    int max_degree_vertex = -1;

    for(int i = 0; i < input_graph.size(); i++){
        if(color_map[i] != -1) continue;
        if(max_degree >= (int) input_graph[i].size()) continue;
        max_degree = input_graph[i].size();
        max_degree_vertex = i;
    }

    if(max_degree_vertex == -1) return -1;

    queue.push(std::make_tuple(1, max_degree, max_degree_vertex));

    while (!queue.empty()){
        int index = std::get<2>(queue.top()), current_saturation = std::get<0>(queue.top()); queue.pop();
        if (color_map[index] != -1) continue;

        color_map[index] = findPlausibleColor(input_graph.size(), input_graph[index], color_map);
        insertVertexDsatur(index, current_saturation, input_graph, color_map, queue);
    }
    return 0;
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

void largestDegreeColorGeneration(vector<vector<int>> &input_graph, vector<int> &color_map){
    priority_queue<tuple<int, int>> queue; //degree, index
    for(int i = 0; i < input_graph.size(); i++){
        queue.push(std::make_tuple(input_graph[i].size(), i));
    }

    while(!queue.empty()){
        int index = std::get<1>(queue.top()); queue.pop();
        color_map[index] = findPlausibleColor(input_graph.size(), input_graph[index], color_map);
    }
}

void largestStudentColorGeneration (vector<vector<int>> &input_graph, vector<int>& student_enrolled, vector<int> &color_map){
    priority_queue<tuple<int, int>> queue; //student, index
    for(int i = 0; i < input_graph.size(); i++){
        queue.push(std::make_tuple(student_enrolled[i], i));
    }

    while(!queue.empty()){
        int index = std::get<1>(queue.top()); queue.pop();
        color_map[index] = findPlausibleColor(input_graph.size(), input_graph[index], color_map);
    }
}

void randomStudentColorGeneration (vector<vector<int>> &input_graph, vector<int> &color_map){
    std::vector<int> node_list(input_graph.size());
    std::iota(node_list.begin(), node_list.end(), 0);
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    auto rng = std::default_random_engine (seed);
    std::shuffle(std::begin(node_list), std::end(node_list), rng);

    for(auto vertex: node_list){
        color_map[vertex] = findPlausibleColor(input_graph.size(), input_graph[vertex], color_map);
    }
}