Student Management System - Google Test Suite
This project implements a comprehensive student management system in C++ with extensive unit testing using Google Test (gtest). The application demonstrates software testing best practices including boundary value analysis, equivalence partitioning, decision coverage, and the driver pattern.

Core Components
Student Class (main.cpp:11-37) - Manages student information (ID, name, age) with validation logic ensuring students have non-empty names and ages between 16-100 years.

Grade Class (main.cpp:42-81) - Handles academic grades (1-10 scale) with classification into four levels: FAIL (1-4), PASS (5-6), GOOD (7-8), and EXCELLENT (9-10). Includes validation and passing threshold checks.

StudentManager Class (main.cpp:86-213) - Central management system providing:

Student operations: add, remove, search
Grade management: add grades, calculate averages per student or subject
Absence tracking: record absences and determine exam eligibility
Testing Strategy (22 Comprehensive Tests)
The test suite demonstrates multiple testing techniques:

Positive Testing: Valid operations with expected successful outcomes
Negative Testing: Invalid inputs and edge cases that should fail gracefully
Boundary Value Analysis: Testing limits (ages 16/100, grades 1/10, absence thresholds)
Equivalence Partitioning: Representative values from valid/invalid ranges
Decision Coverage: All conditional branches tested (grade levels, validation logic)
Driver Pattern: Helper function (main.cpp:220-226) for batch testing multiple values
Test Coverage
StudentTest (4 tests): Validation, boundary values, invalid data
GradeTest (5 tests): All grade levels, boundary conditions, validation
StudentManagerTest (13 tests): CRUD operations, averages, absence tracking, exam eligibility
The code emphasizes defensive programming with extensive decision points for error handling and validation, making it an excellent educational resource for learning both C++ development and systematic unit testing with Google Test framework.
