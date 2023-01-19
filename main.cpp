#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cstdio>
#include <sstream>
#include <cstring>

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::cerr;

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
    return 0;
}

int main(int argc, char **argv) {
    if(argc <= 1) {
        cerr << "Usage: " << argv[0] << " Datafile\n";
        return -1;
    }

    string course_file = (argv[1]); course_file.append(".crs");
    string student_file = (argv[1]); student_file.append(".stu");

    vector<int> student_enrolled;
    int course_amount;
    if((course_amount = generateNodes(student_enrolled, course_file)) == -1) return -1;

    vector<vector<int>> input_graph(course_amount);
    if(generateEdges(input_graph, student_file) == -1) return -1;

    return 0;
}
