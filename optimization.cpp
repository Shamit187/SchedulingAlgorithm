#include "prototype.h"

vector<int>
simulatedAnnealing
(int choice, double init_temp, double final_temp, double cooling_rate, vector<vector<int>> &input_graph, vector<int> &color_map) {
    vector<int> current(color_map), neighbour(color_map);
    auto tuple_value = calculate_penalty_highest_offence(input_graph, current);
    int most_offensive_index = std::get<1>(tuple_value);
    long long unsigned currentPenalty = std::get<0>(tuple_value), neighbourPenalty;
    long long int del_E;
    double current_temp = init_temp;

    std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<> distribution(-1,1);
    while (current_temp > final_temp){
        if(choice == 0) kempeChainInterchange(most_offensive_index, neighbour, input_graph);
        else pairwiseSwap(most_offensive_index, neighbour, input_graph);
        tuple_value = calculate_penalty_highest_offence(input_graph, neighbour);
        neighbourPenalty = std::get<0>(tuple_value);
        del_E = neighbourPenalty - currentPenalty;
        double acceptanceProbability = exp(-(double)del_E / current_temp);

        if (del_E < 0 || distribution(generator) < acceptanceProbability){
            current = neighbour;
            currentPenalty = neighbourPenalty;
        }

        current_temp *= cooling_rate;
    }
    return current;
}

void pairwiseSwap(int index, vector<int> &color_map, vector<vector<int>> &input_graph) {

}

tuple<int, int>
calculate_penalty_highest_offence
(vector<vector<int>> &input_graph, vector<int>& current) {
    long long unsigned totalPenalty = 0;
    unsigned penalty = 0;
    unsigned distance = 0;
    int max_penalty = -1;
    int max_penalty_index = 0;
    for(int vertex = 0; vertex < input_graph.size(); vertex++){
        for(auto neighbour: input_graph[vertex]){
            distance = abs(current[vertex] - current[neighbour]); penalty = 0;
            if(distance < 5) {
                penalty = pow(2, (5 - distance));
                //penalty = 2 * (5 - distance);
            }
            totalPenalty += penalty;
            if(penalty <= max_penalty) continue;
            max_penalty_index = vertex;
            max_penalty = penalty;
        }
    }
    return std::make_tuple(totalPenalty / 2, max_penalty_index);
}

void
kempeChainInterchange
(int most_offensive_index, vector<int> &color_map, vector<vector<int>> &input_graph) {
    //generate random until founding a vertex that has neighbour
    //old code
    std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> distribution(0,input_graph.size() - 1);

    //generate random until founding a vertex that has neighbour
    //    int init_vertex = distribution(generator);
    //    while(input_graph[init_vertex].empty()){
    //        init_vertex = distribution(generator);
    //    }


    int init_vertex = most_offensive_index;

    //change distribution to find neighbours
    //std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
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
