#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cstdio>
#include <sstream>
#include <cstring>
#include <queue>
#include <tuple>

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::cerr;
using std::priority_queue;
using std::tuple;

//prototyping
int generateNodes(vector<int>& student_enrolled, string const& course_file_name);
int generateEdges(vector<vector<int>> & input_graph, string const& student_file_name);
void add_edge(vector<int>& student_data, vector<vector<int>>& input_graph);

//first is file manipulation
int generateNodes(vector<int>& student_enrolled, string const& course_file_name){
    std::ifstream course_file(course_file_name, std::ifstream::in);
    if(!course_file.is_open()){
        cerr << "Unable to open file: " << course_file_name << endl;
        return -1;
    }
    string line;
    int course_number, student_amount, course_amount = 0;
    while(std::getline(course_file, line)){
        sscanf(line.c_str(), "%d %d", &course_number, &student_amount);
        student_enrolled.push_back(student_amount);
        course_amount++;
    }
    course_file.close();
    return course_amount;
}

void add_edge(vector<int>& student_data, vector<vector<int>>& input_graph){
    for(auto i: student_data){
        for(auto j: student_data){
            if(i != j) input_graph[i].push_back(j);
        }
    }
}

int generateEdges(vector<vector<int>> & input_graph, string const& student_file_name){
    std::ifstream student_file(student_file_name, std::ifstream::in);
    if(!student_file.is_open()){
        cerr << "Unable to open file: " << student_file_name << endl;
        return -1;
    }
    string line;
    int number;
    vector<int> student_data;
    while(std::getline(student_file, line)){
        std::stringstream ss(line);
        student_data.clear();
        while (ss >> number) student_data.push_back(number - 1); //courses in file start from 1, we start from 0
        if(student_data.size() <= 1) continue;
        add_edge(student_data, input_graph);
    }
    student_file.close();
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
}

void insertVertexDsatur(int vertex_index, int current_saturation, vector<vector<int>>& input_graph, vector<int>& color_map, priority_queue<tuple<int, int, int>>& queue){
    for(auto i: input_graph[vertex_index]){
        if(color_map[i] != -1) continue;
        queue.push(std::make_tuple(current_saturation + 1, input_graph[i].size(), i));
    }
}

int connectedDijkstra(vector<vector<int>>& input_graph, vector<int>& color_map, priority_queue<tuple<int, int, int>>& queue){
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

void dsaturColorGeneration(vector<vector<int>>& input_graph, vector<int>& color_map){
    priority_queue<tuple<int, int, int>> queue;//saturation, degree, index
    while((connectedDijkstra(input_graph, color_map, queue)) == 0);
}

int main(int argc, char **argv) {
    if(argc <= 2) {
        cerr << "Usage: " << argv[0] << " Datafile Heuristic1\n";
        return -1;
    }
    int hard_choice = std::stoi(argv[2]);
    if(hard_choice < 0 || hard_choice > 3) return -1;

    string course_file = (argv[1]); course_file.append(".crs");
    string student_file = (argv[1]); student_file.append(".stu");

    vector<int> student_enrolled;
    int course_amount;
    if((course_amount = generateNodes(student_enrolled, course_file)) == -1) return -1;

    vector<vector<int>> input_graph(course_amount);
    if(generateEdges(input_graph, student_file) == -1) return -1;

//    priority_queue<tuple<int,int>> degree_hash; //1st element degree, 2nd element vertex
//    generateDegreeHash(input_graph, degree_hash);

    vector<int> color_map(course_amount, -1);
    switch (hard_choice) {
        case 2: //DSat Algo
            dsaturColorGeneration(input_graph, color_map);
            break;
        default:
            cerr << "Something Wrong\n";
    }

    for(int i = 0; i < color_map.size(); i++){
        cout << i+1 << ": " << color_map[i] << "\n";
    }
    return 0;
}
