#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100
#define FILENAME "students.txt"

typedef struct {
    int id;
    char fullName[50];
    char course[30];
    int year;
    float gpa;
} Student;

// Function prototypes
void addStudent(Student students[], int *count);
void displayStudents(Student students[], int count);
void searchStudent(Student students[], int count);
void sortByGPA(Student students[], int count);
void saveToFile(Student students[], int count);
int loadFromFile(Student students[]);

int main() {
    Student students[MAX];
    int count = 0, choice;

    // Load existing records from file
    count = loadFromFile(students);

    do {
        printf("\n--- Student Record Management ---\n");
        printf("1. Add Student Record\n");
        printf("2. Display All Records\n");
        printf("3. Search Student by ID\n");
        printf("4. Sort Records by GPA (Descending)\n");
        printf("5. Save Records\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        if(scanf("%d", &choice) != 1) {
            printf("Invalid input! Please enter a number.\n");
            while(getchar() != '\n'); // clear buffer
            continue;
        }

        switch(choice) {
            case 1:
                addStudent(students, &count);
                break;
            case 2:
                displayStudents(students, count);
                break;
            case 3:
                searchStudent(students, count);
                break;
            case 4:
                sortByGPA(students, count);
                displayStudents(students, count);
                break;
            case 5:
                saveToFile(students, count);
                break;
            case 0:
                printf("Exiting program...\n");
                break;
            default:
                printf("Invalid choice! Try again.\n");
        }
    } while(choice != 0);

    return 0;
}

// Add a new student record
void addStudent(Student students[], int *count) {
    if(*count >= MAX) {
        printf("Cannot add more students. Maximum reached.\n");
        return;
    }

    printf("Enter Student ID: ");
    if(scanf("%d", &students[*count].id) != 1) {
        printf("Invalid ID!\n");
        while(getchar() != '\n'); 
        return;
    }

    while(getchar() != '\n'); // clear input buffer

    printf("Enter Full Name: ");
    fgets(students[*count].fullName, sizeof(students[*count].fullName), stdin);
    students[*count].fullName[strcspn(students[*count].fullName, "\n")] = '\0';

    printf("Enter Course: ");
    fgets(students[*count].course, sizeof(students[*count].course), stdin);
    students[*count].course[strcspn(students[*count].course, "\n")] = '\0';

    printf("Enter Year of Study: ");
    if(scanf("%d", &students[*count].year) != 1) {
        printf("Invalid Year!\n");
        while(getchar() != '\n'); 
        return;
    }

    printf("Enter GPA: ");
    if(scanf("%f", &students[*count].gpa) != 1) {
        printf("Invalid GPA!\n");
        while(getchar() != '\n'); 
        return;
    }

    (*count)++;
    printf("Student added successfully!\n");
}

// Display all student records
void displayStudents(Student students[], int count) {
    if(count == 0) {
        printf("No student records available.\n");
        return;
    }

    printf("\n%-10s %-25s %-15s %-10s %-5s\n", "ID", "Full Name", "Course", "Year", "GPA");
    printf("---------------------------------------------------------------------\n");
    for(int i = 0; i < count; i++) {
        printf("%-10d %-25s %-15s %-10d %-5.2f\n", 
            students[i].id, students[i].fullName, students[i].course, students[i].year, students[i].gpa);
    }
}

// Search for a student by ID
void searchStudent(Student students[], int count) {
    if(count == 0) {
        printf("No student records available.\n");
        return;
    }

    int id, found = 0;
    printf("Enter Student ID to search: ");
    if(scanf("%d", &id) != 1) {
        printf("Invalid ID!\n");
        while(getchar() != '\n'); 
        return;
    }

    for(int i = 0; i < count; i++) {
        if(students[i].id == id) {
            printf("\nStudent Found:\n");
            printf("ID: %d\nFull Name: %s\nCourse: %s\nYear: %d\nGPA: %.2f\n",
                students[i].id, students[i].fullName, students[i].course, students[i].year, students[i].gpa);
            found = 1;
            break;
        }
    }

    if(!found) {
        printf("Student with ID %d not found.\n", id);
    }
}

// Sort students by GPA in descending order
void sortByGPA(Student students[], int count) {
    if(count == 0) {
        printf("No student records available.\n");
        return;
    }

    for(int i = 0; i < count-1; i++) {
        for(int j = 0; j < count-i-1; j++) {
            if(students[j].gpa < students[j+1].gpa) {
                Student temp = students[j];
                students[j] = students[j+1];
                students[j+1] = temp;
            }
        }
    }
    printf("Records sorted by GPA in descending order.\n");
}

// Save all records to file
void saveToFile(Student students[], int count) {
    FILE *fp = fopen(FILENAME, "w");
    if(fp == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    for(int i = 0; i < count; i++) {
        fprintf(fp, "%d,%s,%s,%d,%.2f\n", 
            students[i].id, students[i].fullName, students[i].course, students[i].year, students[i].gpa);
    }

    fclose(fp);
    printf("Records saved to file successfully.\n");
}

// Load records from file
int loadFromFile(Student students[]) {
    FILE *fp = fopen(FILENAME, "r");
    if(fp == NULL) {
        return 0; // no file found, start fresh
    }

    int count = 0;
    while(fscanf(fp, "%d,%49[^,],%29[^,],%d,%f\n", 
                 &students[count].id, students[count].fullName, students[count].course, 
                 &students[count].year, &students[count].gpa) == 5) {
        count++;
    }

    fclose(fp);
    return count;
}
