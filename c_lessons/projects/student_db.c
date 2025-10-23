#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

#define HOST "localhost"
#define USER "root"       // MySQL username
#define PASS "password"   // MySQL password
#define DB "student_system"

// MySQL connection
MYSQL *conn;
MYSQL_RES *res;
MYSQL_ROW row;

// Cross-platform console clear
void clearConsole() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Function prototypes
void addStudent();
void displayStudents();
void searchStudent();
void sortStudentsByGPA();
int studentExists(int studentID);

int main() {
    int choice;

    // Connect to MySQL
    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, HOST, USER, PASS, DB, 0, NULL, 0)) {
        fprintf(stderr, "Failed to connect to database: %s\n", mysql_error(conn));
        exit(1);
    }

    do {
        clearConsole();
        printf("--- Student Record Management System ---\n");
        printf("1. Add New Student\n");
        printf("2. Display All Students\n");
        printf("3. Search Student by ID\n");
        printf("4. Sort Students by GPA (Descending)\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        if(scanf("%d", &choice) != 1) {
            printf("Invalid input!\n");
            while(getchar() != '\n');
            continue;
        }

        clearConsole();
        switch(choice) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: sortStudentsByGPA(); break;
            case 0: printf("Exiting program...\n"); break;
            default: printf("Invalid choice! Try again.\n");
        }
        printf("\nPress Enter to continue...");
        while(getchar() != '\n'); // clear buffer
        getchar();
    } while(choice != 0);

    mysql_close(conn);
    return 0;
}

// Check if student exists
int studentExists(int studentID) {
    char query[256];
    sprintf(query, "SELECT * FROM students WHERE studentID=%d", studentID);
    if(mysql_query(conn, query)) {
        fprintf(stderr, "Query failed: %s\n", mysql_error(conn));
        return 0;
    }
    res = mysql_store_result(conn);
    int exists = mysql_num_rows(res) > 0;
    mysql_free_result(res);
    return exists;
}

// Add new student
void addStudent() {
    int id, year;
    char name[100], course[50];
    float gpa;

    printf("Enter Student ID: ");
    scanf("%d", &id);
    if(studentExists(id)) {
        printf("Student ID already exists!\n");
        return;
    }
    while(getchar() != '\n');

    printf("Enter Full Name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';

    printf("Enter Course: ");
    fgets(course, sizeof(course), stdin);
    course[strcspn(course, "\n")] = '\0';

    printf("Enter Year of Study: ");
    scanf("%d", &year);
    printf("Enter GPA: ");
    scanf("%f", &gpa);

    char query[512];
    sprintf(query, "INSERT INTO students VALUES(%d, '%s', '%s', %d, %.2f)", id, name, course, year, gpa);

    if(mysql_query(conn, query)) {
        fprintf(stderr, "Insert failed: %s\n", mysql_error(conn));
        return;
    }
    printf("Student added successfully!\n");
}

// Display all students
void displayStudents() {
    if(mysql_query(conn, "SELECT * FROM students")) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }
    res = mysql_store_result(conn);

    printf("%-10s %-30s %-20s %-5s %-5s\n", "ID", "Name", "Course", "Year", "GPA");
    printf("---------------------------------------------------------------\n");
    while((row = mysql_fetch_row(res))) {
        printf("%-10s %-30s %-20s %-5s %-5s\n", row[0], row[1], row[2], row[3], row[4]);
    }
    mysql_free_result(res);
}

// Search student by ID
void searchStudent() {
    int id;
    printf("Enter Student ID: ");
    scanf("%d", &id);

    char query[256];
    sprintf(query, "SELECT * FROM students WHERE studentID=%d", id);
    if(mysql_query(conn, query)) { fprintf(stderr, "%s\n", mysql_error(conn)); return; }

    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);
    if(!row) { printf("Student not found!\n"); mysql_free_result(res); return; }

    printf("\nID: %s\nName: %s\nCourse: %s\nYear: %s\nGPA: %s\n", row[0], row[1], row[2], row[3], row[4]);
    mysql_free_result(res);
}

// Sort students by GPA descending
void sortStudentsByGPA() {
    if(mysql_query(conn, "SELECT * FROM students ORDER BY gpa DESC")) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }
    res = mysql_store_result(conn);

    printf("%-10s %-30s %-20s %-5s %-5s\n", "ID", "Name", "Course", "Year", "GPA");
    printf("---------------------------------------------------------------\n");
    while((row = mysql_fetch_row(res))) {
        printf("%-10s %-30s %-20s %-5s %-5s\n", row[0], row[1], row[2], row[3], row[4]);
    }
    mysql_free_result(res);
}
