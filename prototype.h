#ifndef SCHEDULINGALGORITHM_PROTOTYPE_H
#define SCHEDULINGALGORITHM_PROTOTYPE_H

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cstdio>
#include <sstream>
#include <queue>
#include <tuple>
#include <random>
#include <chrono>
#include <algorithm>
#include <random>
#include <iomanip>

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::cerr;
using std::priority_queue;
using std::tuple;

//prototyping
int
generateNodes
        (vector<int>& student_enrolled, string const& course_file_name);

int
generateEdges
        (vector<vector<int>> & input_graph, string const& student_file_name);

void
add_edge
        (vector<int>& student_data, vector<vector<int>>& input_graph);

void
kempeChainInterchange
        (int most_offensive_index, vector<int> &color_map, vector<vector<int>> &input_graph);

void
modifiedDFS
        (int vertex, int color1, int color2, vector<vector<int>> &input_graph, vector<int> &color_map,vector<bool> &visited);

void
color_switch
        (int vertex, int color1, int color2, vector<int> &color_map);

vector<int>
simulatedAnnealing
        (int choice, double init_temp, double final_temp, double cooling_rate, vector<vector<int>> &input_graph, vector<int> &color_map);

tuple<int, int>
calculate_penalty_highest_offence
        (vector<vector<int>> &input_graph, vector<int>& current);

void
generateDegreeHash
        (vector<vector<int>>& input_graph, priority_queue<tuple<int, int>>& degree_hash);

int
findPlausibleColor
        (int courseSize, vector<int>& neighbourhood, vector<int>& color_map);

void
insertVertexDsatur
        (int vertex_index, int current_saturation, vector<vector<int>>& input_graph, vector<int>& color_map, priority_queue<tuple<int, int, int>>& queue);

int
connectedDijkstra
        (vector<vector<int>>& input_graph, vector<int>& color_map, priority_queue<tuple<int, int, int>>& queue);

void
dsaturColorGeneration
        (vector<vector<int>>& input_graph, vector<int>& color_map);

void
pairwiseSwap
        (int index, vector<int> &color_map, vector<vector<int>> &input_graph);

void
largestDegreeColorGeneration
        (vector<vector<int>> &input_graph, vector<int> &color_map);

void
largestStudentColorGeneration
        (vector<vector<int>> &input_graph, vector<int>& student_enrolled, vector<int> &color_map);

void
randomStudentColorGeneration
        (vector<vector<int>> &input_graph, vector<int> &color_map);

void
generateLogFile
    (vector<vector<int>> &input_graph, vector<int> &color_map);

#endif
