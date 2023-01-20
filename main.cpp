#include "prototype.h"

int main(int argc, char **argv) {
    if(argc <= 3) {
        cerr << "Usage: " << argv[0] << " Datafile Heuristic1 Heuristic2\n";
        return -1;
    }
    int hard_choice = std::stoi(argv[2]);
    if(hard_choice < 0 || hard_choice > 3) return -1;

    int soft_choice = std::stoi(argv[3]);
    if(soft_choice < 0 || soft_choice > 1) return -1;

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
        case 0: //Highest Degree
            break;
        case 1: //Highest Student
            break;
        case 2: //DSat Algo
            dsaturColorGeneration(input_graph, color_map);
            break;
        case 3: //Random Assignment
            break;
        default:
            cerr << "Something Wrong\n";
            return -1;
    }

    //hill climbing algorithm
    int tolerance_level = 2000;
    simulatedAnnealing(soft_choice, 100, 1e-6, 0.99, input_graph, color_map);

    //find the highest color

    int max = 0;
    for(auto i: color_map){
        if(max > i) continue;
        max = i;
    }
    cout << "Highest color: " << max + 1 << "\n";

    return 0;
}


