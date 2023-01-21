#include "prototype.h"

vector<int>
simulatedAnnealing
(int choice, double init_temp, double final_temp, double cooling_rate, vector<vector<int>> &input_graph, vector<int> &color_map) {
    vector<int> current(color_map), neighbour(color_map);
    auto tuple_value = calculate_penalty_highest_offence(input_graph, current);
    unsigned most_offensive_index = std::get<1>(tuple_value);
    long long int currentPenalty = std::get<0>(tuple_value), neighbourPenalty;
    double del_E;
    double current_temp = init_temp;

    std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<> distribution(-1,1);
    std::uniform_int_distribution<> distribution1(0 , input_graph.size() - 1);

    vector<bool> switched_vertex(input_graph.size(), false);

    while (current_temp > final_temp){
        switched_vertex[most_offensive_index] = true;

        if(choice == 0)
            kempeChainInterchange(most_offensive_index, neighbour, input_graph);
        else
            pairwiseSwap(most_offensive_index, neighbour, input_graph);

        tuple_value = calculate_penalty_highest_offence(input_graph, neighbour);
        neighbourPenalty = std::get<0>(tuple_value);
        most_offensive_index = std::get<1>(tuple_value);

        if(switched_vertex[most_offensive_index]) most_offensive_index = distribution1(generator);

        del_E = (double) (neighbourPenalty - currentPenalty);

        double error = del_E;
        double acceptanceProbability = exp(- error / (double) current_temp);

        if(choice == 1) acceptanceProbability -= 1; // for pairwise swap, decrease acceptance rate, seems to perform better

        if (del_E < 0 || distribution(generator) < acceptanceProbability){
            current = neighbour;
            currentPenalty = neighbourPenalty;
        }

        current_temp *= cooling_rate;
    }
    return current;
}

void pairwiseSwap(int index, vector<int> &color_map, vector<vector<int>> &input_graph) {
    std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int>distribution = std::uniform_int_distribution<int>(0, input_graph.size() - 1);

    int init_vertex = index;
    int color1 = color_map[index];
    vector<int> init_vertex_color_conflicts;
    for(auto neighbours: input_graph[init_vertex]){
        init_vertex_color_conflicts.push_back(color_map[neighbours]);
    }

    int new_vertex, color2;
    bool vertex_found = false;
    int max_count = 100;
    int count = 0;
    while (!vertex_found){
        if(count > max_count) return;
        count++;

        new_vertex = distribution(generator);
        color2 = color_map[new_vertex];

        if(new_vertex == init_vertex || color1 == color2) continue;
        vertex_found = true;

        for(auto neighbours: input_graph[new_vertex]){
            if(color_map[neighbours] == color1) {
                vertex_found = false;
                continue;
            }
        }
        if(!vertex_found) continue;

        auto it = std::find (init_vertex_color_conflicts.begin(), init_vertex_color_conflicts.end(), color2);
        if (it != init_vertex_color_conflicts.end()) vertex_found = false;

        if(!vertex_found) continue;
    }
    //cout << "Switched " << init_vertex << "," << new_vertex << ": " << color1 << "," << color2 << "\n";
    color_switch(init_vertex, color1, color2, color_map);
    color_switch(new_vertex, color1, color2, color_map);
}

tuple<int, int>
calculate_penalty_highest_offence
(vector<vector<int>> &input_graph, vector<int>& current) {
    long long int totalPenalty = 0;
    int penalty = 0;
    unsigned distance = 0;
    int max_penalty = 0;
    int max_degree = 0;
    unsigned max_penalty_index = 0;
    for(int vertex = 0; vertex < input_graph.size(); vertex++){
        penalty = 0;
        for(auto neighbour: input_graph[vertex]){
            distance = abs(current[vertex] - current[neighbour]);
            if(distance < 5) {
                penalty += pow(2, (5 - distance));
                //penalty = 2 * (5 - distance);
            }
        }
        totalPenalty += penalty;
        if(penalty > max_penalty) {
            max_penalty_index = vertex;
            max_penalty = (int) penalty;
            max_degree = input_graph[vertex].size();
        }else if(penalty == max_penalty && max_degree < input_graph[vertex].size() ) {
            max_penalty_index = vertex;
            max_degree = input_graph[vertex].size();
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
    int len = input_graph[init_vertex].size();
    if(len == 0) return;
    distribution = std::uniform_int_distribution<int>(0,  len - 1);
    int color_1 = color_map[init_vertex];
    int random_number = distribution(generator);
    int color_2 = color_map[input_graph[init_vertex][random_number]];

    //cout << "Switched " << init_vertex << "\n";

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
