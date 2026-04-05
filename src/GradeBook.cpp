#include "Gradebook.hpp"
#include <sstream>
#include <iomanip>

void Gradebook::add_student(const std::string& first, const std::string& last, const std::string& id) {
    students_.push_back({first, last, id});
    grades_[first + " " + last] = {};
}

void Gradebook::add_assignment(const std::string& name, int max_points) {
    assignments_.push_back({name, max_points});
}

bool Gradebook::enter_grade(const std::string& full_name, const std::string& assignment, int grade) {
    if (grades_.find(full_name) == grades_.end()) return false;
    for (const auto& a : assignments_) {
        if (a.name == assignment) {
            if (grade < 0 || grade > a.max_points) return false;
            grades_[full_name][assignment] = grade;
            return true;
        }
    }
    return false;
}

std::string Gradebook::report() const {
    std::ostringstream out;
    out << "Last_Name,First_Name,ID";
    for (const auto& a : assignments_) out << "," << a.name;
    out << ",Average\n";

    for (const auto& s : students_) {
        std::string full = s.first_name + " " + s.last_name;
        out << s.last_name << "," << s.first_name << "," << s.id;

        int earned = 0, possible = 0;
        for (const auto& a : assignments_) {
            auto it = grades_.at(full).find(a.name);
            if (it != grades_.at(full).end()) {
                out << "," << it->second;
                earned   += it->second;
                possible += a.max_points;
            } else {
                out << ",none";
            }
        }

        if (possible > 0)
            out << "," << std::fixed << std::setprecision(1) << (100.0 * earned / possible) << "%";
        else
            out << ",N/A";
        out << "\n";
    }
    return out.str();
}

std::string Gradebook::assignment_report(const std::string& assignment) const {
    int max_points = -1;
    for (const auto& a : assignments_)
        if (a.name == assignment) { max_points = a.max_points; break; }
    if (max_points == -1) return "Assignment not found.\n";

    std::ostringstream out;
    out << "Last_Name,First_Name,ID,Score\n";

    int total = 0, count = 0;
    for (const auto& s : students_) {
        std::string full = s.first_name + " " + s.last_name;
        out << s.last_name << "," << s.first_name << "," << s.id;
        auto it = grades_.at(full).find(assignment);
        if (it != grades_.at(full).end()) {
            out << "," << it->second;
            total += it->second;
            ++count;
        } else {
            out << ",none";
        }
        out << "\n";
    }

    out << "\nAverage: ";
    if (count > 0)
        out << std::fixed << std::setprecision(1) << (double)total / count << " / " << max_points;
    else
        out << "N/A";
    out << "\n";
    return out.str();
}