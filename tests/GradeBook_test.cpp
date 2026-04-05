#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/benchmark/catch_constructor.hpp>
#include <catch2/generators/catch_generators_range.hpp>

#include "../src/GradeBook.hpp"

static Gradebook make_gradebook() {
    Gradebook gb;
    gb.add_student("Bob",  "Bobberson", "ABC123");
    gb.add_student("Sam",  "Sammerson", "DEF456");
    gb.add_student("Jess", "Jesserson", "HIJ789");
    gb.add_assignment("Quiz 1", 100);
    gb.add_assignment("Lab 1",   50);
    gb.enter_grade("Bob Bobberson",  "Quiz 1", 85);
    gb.enter_grade("Bob Bobberson",  "Lab 1",   0);
    gb.enter_grade("Jess Jesserson", "Quiz 1", 93);
    gb.enter_grade("Jess Jesserson", "Lab 1",  49);
    gb.enter_grade("Sam Sammerson",  "Quiz 1", 95);
    // Sam has no Lab 1 grade
    return gb;
}


TEST_CASE("enter_grade accepts valid grades", "[enter_grade]") {
    Gradebook gb;
    gb.add_student("Bob", "Bobberson", "ABC123");
    gb.add_assignment("Quiz 1", 100);

    CHECK(gb.enter_grade("Bob Bobberson", "Quiz 1",   0) == true);
    CHECK(gb.enter_grade("Bob Bobberson", "Quiz 1", 100) == true);
    CHECK(gb.enter_grade("Bob Bobberson", "Quiz 1",  85) == true);
}

TEST_CASE("enter_grade rejects bad input", "[enter_grade]") {
    Gradebook gb;
    gb.add_student("Bob", "Bobberson", "ABC123");
    gb.add_assignment("Quiz 1", 100);

    CHECK(gb.enter_grade("Nobody Person",  "Quiz 1",  80) == false);
    CHECK(gb.enter_grade("Bob Bobberson",  "Midterm", 80) == false);
    CHECK(gb.enter_grade("Bob Bobberson",  "Quiz 1",  -1) == false);
    CHECK(gb.enter_grade("Bob Bobberson",  "Quiz 1", 101) == false);
}


TEST_CASE("report contains all student names and IDs", "[report]") {
    Gradebook gb = make_gradebook();
    std::string r = gb.report();

    CHECK(r.find("Bobberson") != std::string::npos);
    CHECK(r.find("Sammerson") != std::string::npos);
    CHECK(r.find("Jesserson") != std::string::npos);
    CHECK(r.find("ABC123")    != std::string::npos);
    CHECK(r.find("DEF456")    != std::string::npos);
}

TEST_CASE("report contains all assignment names", "[report]") {
    Gradebook gb = make_gradebook();
    std::string r = gb.report();

    CHECK(r.find("Quiz 1") != std::string::npos);
    CHECK(r.find("Lab 1")  != std::string::npos);
}

TEST_CASE("report shows 'none' for missing grade and '0' for a zero grade", "[report]") {
    Gradebook gb = make_gradebook();
    std::string r = gb.report();

    CHECK(r.find("none") != std::string::npos);    // Sam's missing Lab 1
    // Bob's Lab 1 is 0 — verify it appears as a real grade
    CHECK(r.find("Bobberson,Bob,ABC123,85,0") != std::string::npos);
}

TEST_CASE("report averages are calculated correctly", "[report]") {
    Gradebook gb = make_gradebook();
    std::string r = gb.report();

    CHECK(r.find("56.7%") != std::string::npos);
    CHECK(r.find("94.7%") != std::string::npos);
    CHECK(r.find("95.0%") != std::string::npos);
}

TEST_CASE("report shows N/A average when student has no grades", "[report]") {
    Gradebook gb;
    gb.add_student("New", "Student", "NEW001");
    gb.add_assignment("Quiz 1", 100);
    std::string r = gb.report();

    CHECK(r.find("N/A") != std::string::npos);
}

TEST_CASE("assignment_report shows scores and average", "[assignment_report]") {
    Gradebook gb = make_gradebook();
    std::string r = gb.assignment_report("Lab 1");

    CHECK(r.find("Score")   != std::string::npos);
    CHECK(r.find("Average") != std::string::npos);
    // Bob = 0, Jess = 49, Sam = none → average of 0 and 49 = 24.5
    CHECK(r.find("24.5")    != std::string::npos);
    CHECK(r.find("/ 50")    != std::string::npos);
    CHECK(r.find("none")    != std::string::npos);  // Sam's missing grade
}

TEST_CASE("assignment_report returns error for unknown assignment", "[assignment_report]") {
    Gradebook gb = make_gradebook();
    CHECK(gb.assignment_report("Nonexistent").find("not found") != std::string::npos);
}

TEST_CASE("assignment_report shows N/A when no grades entered", "[assignment_report]") {
    Gradebook gb;
    gb.add_student("Bob", "Bobberson", "ABC123");
    gb.add_assignment("Quiz 1", 100);
    std::string r = gb.assignment_report("Quiz 1");

    CHECK(r.find("N/A") != std::string::npos);
}