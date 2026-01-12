#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <gtest/gtest.h>

// ============================================================================
// CLASA STUDENT
// ============================================================================
class Student {
private:
    std::string id;
    std::string name;
    int age;

public:
    // Constructor
    Student(const std::string& id, const std::string& name, int age)
        : id(id), name(name), age(age) {}

    // Getters
    std::string getId() const { return id; }
    std::string getName() const { return name; }
    int getAge() const { return age; }

    // Validare - decision points pentru testing
    bool isValid() const {
        if (name.empty()) {
            return false;  // Decision 1: nume gol
        }
        if (age < 16 || age > 100) {
            return false;  // Decision 2: vârstă invalidă
        }
        return true;
    }
};

// ============================================================================
// CLASA GRADE
// ============================================================================
class Grade {
private:
    std::string studentId;
    std::string subject;
    int value;

public:
    // Constructor
    Grade(const std::string& studentId, const std::string& subject, int value)
        : studentId(studentId), subject(subject), value(value) {}

    // Getters
    std::string getStudentId() const { return studentId; }
    std::string getSubject() const { return subject; }
    int getValue() const { return value; }

    // Validare - decision point
    bool isValid() const {
        return value >= 1 && value <= 10;  // Decision: valoare în interval
    }

    // Verifică dacă nota este de trecere - decision point
    bool isPassing() const {
        return value >= 5;  // Decision: nota >= 5
    }

    // Clasificare notă - multiple decision points
    std::string getLevel() const {
        if (value >= 1 && value <= 4) {
            return "FAIL";  // Decision 1
        } else if (value >= 5 && value <= 6) {
            return "PASS";  // Decision 2
        } else if (value >= 7 && value <= 8) {
            return "GOOD";  // Decision 3
        } else if (value >= 9 && value <= 10) {
            return "EXCELLENT";  // Decision 4
        }
        return "INVALID";
    }
};

// ============================================================================
// CLASA STUDENTMANAGER
// ============================================================================
class StudentManager {
private:
    std::map<std::string, Student> students;
    std::vector<Grade> grades;
    std::map<std::string, int> absences;
    int maxAbsences;

public:
    // Constructor
    StudentManager(int maxAbsences = 3) : maxAbsences(maxAbsences) {}

    // ========== OPERAȚII STUDENȚI ==========

    // Adaugă student - decision point: ID duplicat
    bool addStudent(const Student& s) {
        if (students.find(s.getId()) != students.end()) {
            return false;  // Decision: ID duplicat
        }
        students.insert({s.getId(), s});
        return true;
    }

    // Șterge student - decision point: student există
    bool removeStudent(const std::string& id) {
        auto it = students.find(id);
        if (it == students.end()) {
            return false;  // Decision: student inexistent
        }
        students.erase(it);
        return true;
    }

    // Caută student - decision point: student există
    Student* findStudent(const std::string& id) {
        auto it = students.find(id);
        if (it == students.end()) {
            return nullptr;  // Decision: student inexistent
        }
        return &(it->second);
    }

    // Număr studenți
    int getStudentCount() const {
        return students.size();
    }

    // ========== OPERAȚII NOTE ==========

    // Adaugă notă - decision point: student există
    bool addGrade(const Grade& g) {
        if (students.find(g.getStudentId()) == students.end()) {
            return false;  // Decision: student inexistent
        }
        grades.push_back(g);
        return true;
    }

    // Obține notele unui student
    std::vector<Grade> getGradesForStudent(const std::string& studentId) const {
        std::vector<Grade> result;
        for (const auto& grade : grades) {
            if (grade.getStudentId() == studentId) {
                result.push_back(grade);
            }
        }
        return result;
    }

    // Calculează media unui student - decision point: are note
    double calculateAverage(const std::string& studentId) const {
        std::vector<Grade> studentGrades = getGradesForStudent(studentId);
        if (studentGrades.empty()) {
            return 0.0;  // Decision: fără note
        }

        double sum = 0.0;
        for (const auto& grade : studentGrades) {
            sum += grade.getValue();
        }
        return sum / studentGrades.size();
    }

    // Calculează media pe materie
    double calculateSubjectAverage(const std::string& subject) const {
        std::vector<Grade> subjectGrades;
        for (const auto& grade : grades) {
            if (grade.getSubject() == subject) {
                subjectGrades.push_back(grade);
            }
        }

        if (subjectGrades.empty()) {
            return 0.0;
        }

        double sum = 0.0;
        for (const auto& grade : subjectGrades) {
            sum += grade.getValue();
        }
        return sum / subjectGrades.size();
    }

    // ========== OPERAȚII ABSENȚE ==========

    // Înregistrează absență - decision point: student există
    bool recordAbsence(const std::string& studentId) {
        if (students.find(studentId) == students.end()) {
            return false;  // Decision: student inexistent
        }
        absences[studentId]++;
        return true;
    }

    // Obține numărul de absențe
    int getAbsences(const std::string& studentId) const {
        auto it = absences.find(studentId);
        if (it == absences.end()) {
            return 0;
        }
        return it->second;
    }

    // Verifică dacă studentul poate da examen - decision point: absențe < max
    bool canTakeExam(const std::string& studentId) const {
        int studentAbsences = getAbsences(studentId);
        return studentAbsences < maxAbsences;  // Decision: sub limită
    }
};

// ============================================================================
// HELPER FUNCTIONS - DRIVER PATTERN
// ============================================================================

// Driver function pentru testare boundary values pe vârstă
void testMultipleAges(const std::vector<int>& ages, bool expectedValid) {
    for (int age : ages) {
        Student s("TEST_ID", "Test Student", age);
        EXPECT_EQ(expectedValid, s.isValid())
            << "Failed for age: " << age;
    }
}

// ============================================================================
// TEST SUITE: StudentTest (4 teste)
// ============================================================================

// Test 1: Pozitiv - Student valid
TEST(StudentTest, ValidStudent_CreatesSuccessfully) {
    Student s("S001", "Ion Popescu", 20);

    // Verificări
    EXPECT_EQ("S001", s.getId());
    EXPECT_EQ("Ion Popescu", s.getName());
    EXPECT_EQ(20, s.getAge());
    EXPECT_TRUE(s.isValid());
}

// Test 2: Negativ + Boundary - Vârstă invalidă
TEST(StudentTest, InvalidAge_ReturnsInvalid) {
    Student s1("S002", "Maria", 15);  // Sub limită
    Student s2("S003", "George", 101); // Peste limită

    EXPECT_FALSE(s1.isValid());
    EXPECT_FALSE(s2.isValid());
}

// Test 3: Negativ - Nume gol
TEST(StudentTest, EmptyName_ReturnsInvalid) {
    Student s("S004", "", 20);

    EXPECT_FALSE(s.isValid());
}

// Test 4: Boundary + Driver Pattern - Valori limită valide
TEST(StudentTest, ValidAge_BoundaryValues) {
    // Folosim driver function pentru a testa multiple valori
    testMultipleAges({16, 17, 99, 100}, true);   // Valori valide la limită
    testMultipleAges({15, 0, -5, 101, 150}, false); // Valori invalide
}

// ============================================================================
// TEST SUITE: GradeTest (5 teste)
// ============================================================================

// Test 5: Pozitiv + Clasă echivalență - Notă validă
TEST(GradeTest, ValidGrade_CreatesSuccessfully) {
    Grade g("S001", "Math", 7);

    EXPECT_EQ("S001", g.getStudentId());
    EXPECT_EQ("Math", g.getSubject());
    EXPECT_EQ(7, g.getValue());
    EXPECT_TRUE(g.isValid());
    EXPECT_TRUE(g.isPassing());
}

// Test 6: Boundary - Note la limită (minim și maxim)
TEST(GradeTest, BoundaryGrades_MinAndMax) {
    Grade g1("S001", "Math", 1);   // Minim valid
    Grade g2("S001", "Math", 10);  // Maxim valid

    EXPECT_TRUE(g1.isValid());
    EXPECT_FALSE(g1.isPassing());  // 1 nu e trecut
    EXPECT_EQ("FAIL", g1.getLevel());

    EXPECT_TRUE(g2.isValid());
    EXPECT_TRUE(g2.isPassing());   // 10 e trecut
    EXPECT_EQ("EXCELLENT", g2.getLevel());
}

// Test 7: Negativ + Boundary - Notă sub minim
TEST(GradeTest, InvalidGrade_BelowMinimum) {
    Grade g1("S001", "Math", 0);
    Grade g2("S001", "Math", -1);

    EXPECT_FALSE(g1.isValid());
    EXPECT_FALSE(g2.isValid());
}

// Test 8: Negativ + Boundary - Notă peste maxim
TEST(GradeTest, InvalidGrade_AboveMaximum) {
    Grade g1("S001", "Math", 11);
    Grade g2("S001", "Math", 15);

    EXPECT_FALSE(g1.isValid());
    EXPECT_FALSE(g2.isValid());
}

// Test 9: Decision Coverage - Toate cele 4 nivele de clasificare
TEST(GradeTest, GradeLevel_AllCategories) {
    Grade fail("S001", "Math", 3);
    Grade pass("S001", "Math", 5);
    Grade good("S001", "Math", 7);
    Grade excellent("S001", "Math", 9);

    EXPECT_EQ("FAIL", fail.getLevel());
    EXPECT_EQ("PASS", pass.getLevel());
    EXPECT_EQ("GOOD", good.getLevel());
    EXPECT_EQ("EXCELLENT", excellent.getLevel());
}

// ============================================================================
// TEST SUITE: StudentManagerTest (13 teste)
// ============================================================================

// Test 10: Pozitiv - Adăugare student cu succes
TEST(StudentManagerTest, AddStudent_Success) {
    StudentManager mgr;
    Student s("S001", "John Doe", 20);

    EXPECT_EQ(0, mgr.getStudentCount());
    EXPECT_TRUE(mgr.addStudent(s));
    EXPECT_EQ(1, mgr.getStudentCount());
}

// Test 11: Negativ - Adăugare student cu ID duplicat
TEST(StudentManagerTest, AddStudent_DuplicateId) {
    StudentManager mgr;
    Student s1("S001", "John Doe", 20);
    Student s2("S001", "Jane Doe", 22);  // ID duplicat

    EXPECT_TRUE(mgr.addStudent(s1));
    EXPECT_FALSE(mgr.addStudent(s2));  // Ar trebui să eșueze
    EXPECT_EQ(1, mgr.getStudentCount());
}

// Test 12: Pozitiv - Ștergere student existent
TEST(StudentManagerTest, RemoveStudent_ExistingStudent) {
    StudentManager mgr;
    Student s("S001", "John Doe", 20);

    mgr.addStudent(s);
    EXPECT_TRUE(mgr.removeStudent("S001"));
    EXPECT_EQ(0, mgr.getStudentCount());
    EXPECT_EQ(nullptr, mgr.findStudent("S001"));
}

// Test 13: Negativ - Ștergere student inexistent
TEST(StudentManagerTest, RemoveStudent_NonExistent) {
    StudentManager mgr;

    EXPECT_FALSE(mgr.removeStudent("S999"));  // Student inexistent
}

// Test 14: Pozitiv - Căutare student existent
TEST(StudentManagerTest, FindStudent_Exists) {
    StudentManager mgr;
    Student s("S001", "John Doe", 20);

    mgr.addStudent(s);
    Student* found = mgr.findStudent("S001");

    ASSERT_NE(nullptr, found);
    EXPECT_EQ("S001", found->getId());
    EXPECT_EQ("John Doe", found->getName());
    EXPECT_EQ(20, found->getAge());
}

// Test 15: Negativ - Căutare student inexistent
TEST(StudentManagerTest, FindStudent_NonExistent) {
    StudentManager mgr;

    Student* found = mgr.findStudent("S999");
    EXPECT_EQ(nullptr, found);
}

// Test 16: Pozitiv - Adăugare notă pentru student existent
TEST(StudentManagerTest, AddGrade_ValidStudent) {
    StudentManager mgr;
    Student s("S001", "John Doe", 20);
    mgr.addStudent(s);

    Grade g("S001", "Math", 8);
    EXPECT_TRUE(mgr.addGrade(g));

    std::vector<Grade> grades = mgr.getGradesForStudent("S001");
    EXPECT_EQ(1, grades.size());
    EXPECT_EQ(8, grades[0].getValue());
}

// Test 17: Negativ - Adăugare notă pentru student inexistent
TEST(StudentManagerTest, AddGrade_NonExistentStudent) {
    StudentManager mgr;

    Grade g("S999", "Math", 8);  // Student inexistent
    EXPECT_FALSE(mgr.addGrade(g));
}

// Test 18: Pozitiv + Stub inline - Calcul medie cu multiple note
TEST(StudentManagerTest, CalculateAverage_MultipleGrades) {
    StudentManager mgr;
    Student s("S001", "John Doe", 20);
    mgr.addStudent(s);

    // "Stub" inline - creăm date de test direct
    mgr.addGrade(Grade("S001", "Math", 8));
    mgr.addGrade(Grade("S001", "Physics", 9));
    mgr.addGrade(Grade("S001", "English", 7));

    double avg = mgr.calculateAverage("S001");
    EXPECT_DOUBLE_EQ(8.0, avg);  // (8+9+7)/3 = 8.0
}

// Test 19: Boundary - Calcul medie pentru student fără note
TEST(StudentManagerTest, CalculateAverage_NoGrades) {
    StudentManager mgr;
    Student s("S001", "John Doe", 20);
    mgr.addStudent(s);

    double avg = mgr.calculateAverage("S001");
    EXPECT_DOUBLE_EQ(0.0, avg);  // Fără note = 0.0
}

// Test 20: Pozitiv - Înregistrare absență pentru student existent
TEST(StudentManagerTest, RecordAbsence_ValidStudent) {
    StudentManager mgr;
    Student s("S001", "John Doe", 20);
    mgr.addStudent(s);

    EXPECT_EQ(0, mgr.getAbsences("S001"));
    EXPECT_TRUE(mgr.recordAbsence("S001"));
    EXPECT_EQ(1, mgr.getAbsences("S001"));
    EXPECT_TRUE(mgr.recordAbsence("S001"));
    EXPECT_EQ(2, mgr.getAbsences("S001"));
}

// Test 21: Pozitiv - Student poate da examen (sub limită absențe)
TEST(StudentManagerTest, CanTakeExam_BelowLimit) {
    StudentManager mgr(3);  // Max 3 absențe
    Student s("S001", "John Doe", 20);
    mgr.addStudent(s);

    mgr.recordAbsence("S001");
    mgr.recordAbsence("S001");  // 2 absențe

    EXPECT_TRUE(mgr.canTakeExam("S001"));  // 2 < 3, poate da examen
}

// Test 22: Boundary + Decision Coverage - Student NU poate da examen (la/peste limită)
TEST(StudentManagerTest, CanTakeExam_AtOrAboveLimit) {
    StudentManager mgr(3);  // Max 3 absențe
    Student s("S001", "John Doe", 20);
    mgr.addStudent(s);

    // Adăugăm 3 absențe (exact la limită)
    mgr.recordAbsence("S001");
    mgr.recordAbsence("S001");
    mgr.recordAbsence("S001");

    EXPECT_FALSE(mgr.canTakeExam("S001"));  // 3 >= 3, NU poate da examen
}

// ============================================================================
// MAIN FUNCTION
// ============================================================================
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
