#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstdio>
using namespace std;

struct Student {
    int rollNo;
    string name;
    int age;
    string course;
};

// Add a new student
void addStudent() {
    Student s;

    cout << "\nEnter Roll Number: ";
    cin >> s.rollNo;
    cin.ignore();

    cout << "Enter Name: ";
    getline(cin, s.name);

    cout << "Enter Age: ";
    cin >> s.age;
    cin.ignore();

    cout << "Enter Course: ";
    getline(cin, s.course);

    ofstream file("students.txt", ios::app);

    if (!file) {
        cout << "Error: Could not open file.\n";
        return;
    }

    file << s.rollNo << "|" << s.name << "|" << s.age << "|" << s.course << "\n";
    file.close();

    cout << "Student added successfully!\n";
}

// Display all students
void displayStudents() {
    ifstream file("students.txt");

    if (!file) {
        cout << "\nNo student records found.\n";
        return;
    }

    Student s;
    string line;

    cout << "\n---------------- STUDENT RECORDS ----------------\n";
    cout << left << setw(10) << "Roll No"
         << setw(25) << "Name"
         << setw(10) << "Age"
         << "Course\n";
    cout << "--------------------------------------------------\n";

    bool found = false;

    while (getline(file, line)) {
        size_t p1 = line.find('|');
        size_t p2 = line.find('|', p1 + 1);
        size_t p3 = line.find('|', p2 + 1);

        if (p1 == string::npos || p2 == string::npos || p3 == string::npos)
            continue;

        s.rollNo = stoi(line.substr(0, p1));
        s.name = line.substr(p1 + 1, p2 - p1 - 1);
        s.age = stoi(line.substr(p2 + 1, p3 - p2 - 1));
        s.course = line.substr(p3 + 1);

        cout << left << setw(10) << s.rollNo
             << setw(25) << s.name
             << setw(10) << s.age
             << s.course << "\n";

        found = true;
    }

    if (!found)
        cout << "No student records found.\n";

    file.close();
}

// Update a student's information
void updateStudent() {
    int searchRoll;
    cout << "\nEnter Roll Number to update: ";
    cin >> searchRoll;

    ifstream file("students.txt");
    ofstream temp("temp.txt");

    if (!file) {
        cout << "No student records found.\n";
        return;
    }

    Student s;
    string line;
    bool found = false;

    while (getline(file, line)) {
        size_t p1 = line.find('|');
        size_t p2 = line.find('|', p1 + 1);
        size_t p3 = line.find('|', p2 + 1);

        if (p1 == string::npos || p2 == string::npos || p3 == string::npos)
            continue;

        s.rollNo = stoi(line.substr(0, p1));
        s.name = line.substr(p1 + 1, p2 - p1 - 1);
        s.age = stoi(line.substr(p2 + 1, p3 - p2 - 1));
        s.course = line.substr(p3 + 1);

        if (s.rollNo == searchRoll) {
            found = true;

            cin.ignore();
            cout << "Enter New Name: ";
            getline(cin, s.name);

            cout << "Enter New Age: ";
            cin >> s.age;
            cin.ignore();

            cout << "Enter New Course: ";
            getline(cin, s.course);
        }

        temp << s.rollNo << "|" << s.name << "|" << s.age << "|" << s.course << "\n";
    }

    file.close();
    temp.close();

    remove("students.txt");
    rename("temp.txt", "students.txt");

    if (found)
        cout << "Student updated successfully!\n";
    else
        cout << "Student with that Roll Number was not found.\n";
}

// Delete a student
void deleteStudent() {
    int searchRoll;
    cout << "\nEnter Roll Number to delete: ";
    cin >> searchRoll;

    ifstream file("students.txt");
    ofstream temp("temp.txt");

    if (!file) {
        cout << "No student records found.\n";
        return;
    }

    Student s;
    string line;
    bool found = false;

    while (getline(file, line)) {
        size_t p1 = line.find('|');
        size_t p2 = line.find('|', p1 + 1);
        size_t p3 = line.find('|', p2 + 1);

        if (p1 == string::npos || p2 == string::npos || p3 == string::npos)
            continue;

        s.rollNo = stoi(line.substr(0, p1));
        s.name = line.substr(p1 + 1, p2 - p1 - 1);
        s.age = stoi(line.substr(p2 + 1, p3 - p2 - 1));
        s.course = line.substr(p3 + 1);

        if (s.rollNo == searchRoll) {
            found = true;
            continue; // Do not copy this record to the new file
        }

        temp << s.rollNo << "|" << s.name << "|" << s.age << "|" << s.course << "\n";
    }

    file.close();
    temp.close();

    remove("students.txt");
    rename("temp.txt", "students.txt");

    if (found)
        cout << "Student deleted successfully!\n";
    else
        cout << "Student with that Roll Number was not found.\n";
}

int main() {
    int choice;

    do {
        cout << "\n====================================\n";
        cout << "      STUDENT MANAGEMENT SYSTEM\n";
        cout << "====================================\n";
        cout << "1. Add Student\n";
        cout << "2. Update Student\n";
        cout << "3. Delete Student\n";
        cout << "4. Display Students\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                addStudent();
                break;

            case 2:
                updateStudent();
                break;

            case 3:
                deleteStudent();
                break;

            case 4:
                displayStudents();
                break;

            case 5:
                cout << "Thank you for using the Student Management System!\n";
                break;

            default:
                cout << "Invalid choice. Please try again.\n";
        }

    } while (choice != 5);

    return 0;
}
