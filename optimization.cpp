#include "prototype.h"

void
simulatedAnnealing
(int choice, double init_temp, double final_temp, double cooling_rate, vector<vector<int>> &input_graph, vector<int> &color_map) {
    vector<int> current(color_map);
    long long unsigned currentPenalty = calculate_penalty(input_graph, current);
    vector<int> neighbour(color_map);
    long long unsigned neighbourPenalty;
    long long int del_E;
    double current_temp = init_temp;

    std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<> distribution(-1,1);

    while (current_temp > final_temp){
        cout << "Current Penalty: " << currentPenalty << "\n";
        kempeChainInterchange(input_graph, neighbour);
        neighbourPenalty = calculate_penalty(input_graph, neighbour);
        del_E = neighbourPenalty - currentPenalty;
        double acceptanceProbability = exp(-(double)del_E / current_temp);

        if (del_E < 0 || distribution(generator) < acceptanceProbability){
            current = neighbour;
            currentPenalty = neighbourPenalty;
        }

        current_temp *= cooling_rate;
    }

}

long long unsigned
calculate_penalty
(vector<vector<int>> &input_graph, vector<int>& current) {
    long long unsigned totalPenalty = 0;
    for(int vertex = 0; vertex < input_graph.size(); vertex++){
        for(int neighbour = 0; neighbour < input_graph[vertex].size(); neighbour++){
            unsigned distance = abs(current[vertex] - current[neighbour]);
            unsigned penalty = 0;
            if(distance < 5) penalty = pow(2, (5 - distance));
            totalPenalty += penalty;
        }
    }
    return totalPenalty / 2;
}

void
kempeChainInterchange
(vector<vector<int>> &input_graph, vector<int> &color_map) {
    //randomGeneration Stuff
    std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> distribution(0,input_graph.size() - 1);

    //generate random until founding a vertex that has neighbour
    int init_vertex = distribution(generator);
    while(input_graph[init_vertex].empty()){
        init_vertex = distribution(generator);
    }

    //change distribution to find neighbours
    distribution = std::uniform_int_distribution<int>(0, input_graph[init_vertex].size() - 1);
    int color_1 = color_map[init_vertex];
    int color_2 = color_map[input_graph[init_vertex][distribution(generator)]];

    vector<bool> visited(input_graph.size(), false);
    modifiedDFS(init_vertex, color_1, color_2, input_graph, color_map, visited);
}

void
modifiedDFS
(int vertex, int color1, int color2, vector<vector<int>> &input_graph, vector<int> &color_map, vector<bool> &visited) {
    visited[vertex] = true;
    color_switch(vertex, color1, color2, color_map);

    for(auto neighbour: input_graph[vertex]){
        if (visited[neighbour]) continue;
        if (color_map[neighbour] != color1 && color_map[neighbour] != color2) continue;
        modifiedDFS(neighbour, color1, color2, input_graph, color_map, visited);
    }
}

void
color_switch
(int vertex, int color1, int color2, vector<int> &color_map) {
    if(color_map[vertex] == color1) color_map[vertex] = color2;
    else color_map[vertex] = color1;
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
