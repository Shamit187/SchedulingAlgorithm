#include "prototype.h"

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
