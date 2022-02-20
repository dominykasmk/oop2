#include <iostream>
#include <string>
#include <limits>
#include <iomanip>
#include <algorithm>
#include <random>
#include <chrono>
#include <vector>


using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;


struct Student {
    string first_name;
    string last_name;

    unsigned num_of_scores{};
    vector<unsigned> scores{};

    double score_average{};
    double score_median{};
};


bool validate_input(string input);
template <typename T>
T test_validation(T &input, std::string prompt, std::string error_msg);
void input_valid_string(std::string &input);
void input_valid_num(int &input, int left_range, int right_range, int special_value = 0);
void calculate_averages(Student &student);
void ignore_line();
void print_students(const std::vector<Student> &students, unsigned option);
unsigned generate_random_score();


int main()
{
    vector<Student> students;
    cout << "Jeigu norite baigti įvestį, įveskite 'x'\n";

    while (true) {

        Student student;
        cout << "\nĮveskite studento vardą\n";
        string input;
        input_valid_string(input);
        
        if (input == "x")
            break;

        student.first_name = input;

        cout << "\nĮveskite studento pavardę\n";
        input_valid_string(input);
        student.last_name = input;


        int max_num_of_scores = 10;
        cout << "\nNorėtumėte sugeneruoti pažymius automatiškai?(1) ar ne?(2)\n";
        int option;
        input_valid_num(option, 1, 2);

        if (option == 1) {
                
            cout << "\nPasirinkite kiek pažymių sugeneruoti(1/10)\n";
            int amount_to_generate{};
            input_valid_num(amount_to_generate, 1, max_num_of_scores);

            unsigned score;
            cout << "\nSugeneruoti pažymiai: ";
            for (int i{}; i < amount_to_generate; i++) {
                    
                    score = generate_random_score();
                    cout << score << " ";
                    student.scores.push_back(score);
            }
            cout << "\n";
            calculate_averages(student);
        }
        else {

            int score;
            cout << "\nVeskite studento pažymius (paskutinis turi būti egzamino pažymys).\n"
                << "Įveskite -1 jei norite baigti įvedimą\n\n";

            while (true) {

                input_valid_num(score, 1, 10, -1);

                if (score == -1) {

                    if (student.scores.size() == 0) {
                        student.scores.push_back(0);
                        break;
                    }

                    // Suvedus visus pažymius suskaičiuojam vidurkį ir medianą
                    calculate_averages(student);
                    break;
                }
                    
                student.scores.push_back(score);
            }
        }
        
        cin.clear();
        ignore_line();
        students.push_back(student);
    }


    if (students.empty()) {
        cout << "\nNeįvedėte nei vieno studento.\n" << endl;
        return 0;
    }

    cout << "\nPasirinkite ar norite apskaičiuoti vidurkį (1) ar medianą (2)?" << endl;
    int option;
    input_valid_num(option, 1, 2);
    print_students(students, option);

    return 0;
}


void ignore_line()
{
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

//template <typename T>
//T test_validation(T &input, std::string prompt, std::string error_msg)
//{
//    while (true) {
//
//        std::cout << prompt;
//        std::cout << "> ";
//        std::cin >> input;
//
//        if (cin.good()) {
//            break;
//        }
//        else if (cin.fail()) {
//
//            std::cout << error_msg ;
//            std::cin.clear();
//            ignore_line();
//        }
//    }  
//
//    return input;
//}


void input_valid_string(std::string &input)
{
    while (true) {

        cout << "> ";
        getline(cin, input);

        if (input.empty()) {
            cout << "Nieko neįvedėte.\n";
            cin.clear();
            //ignore_line();
            continue;
        }


        bool valid_string = true;
        for (decltype(input.size()) i{}; i != input.size() && valid_string; i++) {
            if (!(std::isalnum(input[i])) || std::isspace(input[i])) {
                valid_string = false;
            }
        }

        if (valid_string)
            break;
    }
}

void input_valid_num(int &input, int left_range, int right_range, int special_value)
{
    do {

        cout << "> ";
        cin >> input;

        if (cin.good() && special_value != 0 && input == special_value) {
            break;
        }

        if (cin.fail() || (input < left_range || input > right_range)) {
            cout << "\nĮveskite skaičių tarp " << left_range << " ir " << right_range << "\n";
            cin.clear();
            ignore_line(); 
            continue;
        }
        else {
            break;
        }

    } while (true);
}


void calculate_averages(Student &student)
{
    double average{};
    for (auto &score : student.scores) {
        average += score;
    }
    student.score_average = average / student.scores.size();

    // Surikiuojam pažymius medianos apskaičiavimui
    std::sort(student.scores.begin(), student.scores.end());

    
    if (student.num_of_scores % 2 == 0) {
        int index2 = student.scores.size() / 2;
        int index1 = student.scores.size() / 2 - 1;
        double num_1 = student.scores[index1];
        double num_2 = student.scores[index2];
        student.score_median = (num_1 + num_2) / 2;
    }
    else {
        int temp = (int)student.num_of_scores / 2;
        student.score_median = student.scores[student.scores.size() / 2];
    }
}


void print_students(const std::vector<Student> &students, unsigned option)
{
    std::cout << "\n" << std::setw(15) << std::left << "Pavardė" << std::setw(15) << std::left << "Vardas"
        << std::left << "Galutinis ";
    (option == 1) ? std::cout << "(Vid.)" : std::cout << "(Med).";
    cout << endl << endl;

    std::string temp(50, '-');
    cout << temp << endl;

    for (const auto &student : students) {
        std::cout << std::setw(15) << std::left << student.first_name;
        std::cout << std::setw(15) << std::left << student.last_name;
        
        switch (option) {
            case 1:
                std::cout << std::setprecision(2) << student.score_average << std::endl << std::endl;
                break; 

            case 2:
                std::cout << std::setprecision(2) << student.score_median << std::endl << std:: endl;
                break; 
        }
    }
}

unsigned generate_random_score()
{
    using hrClock = std::chrono::high_resolution_clock;
    std::mt19937 mt(static_cast<long unsigned int>(hrClock::now().time_since_epoch().count()));
    std::uniform_int_distribution<int> dist(1, 10);
    
    return dist(mt);
}
