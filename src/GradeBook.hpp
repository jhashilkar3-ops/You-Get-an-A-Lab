#ifndef GradeBook_HPP
#define GradeBook_HPP
#include <string>
#include <vector>
#include <map>

struct Student {
    std::string first_name;
    std::string last_name;
    std::string id;
};

struct Assignment {
    std::string name;
    int max_points;
};

class Gradebook {
public:
    void add_student(const std::string& first, const std::string& last, const std::string& id);
    void add_assignment(const std::string& name, int max_points);
    bool enter_grade(const std::string& full_name, const std::string& assignment, int grade);
    std::string report() const;
    std::string assignment_report(const std::string& assignment) const;

private:
    std::vector<Student>    students_;
    std::vector<Assignment> assignments_;
    std::map<std::string, std::map<std::string, int>> grades_;
};

#endif