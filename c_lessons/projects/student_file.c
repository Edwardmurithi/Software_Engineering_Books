/* student_file.c
   Student Record Management with file storage
   Features: add, display, search, sort by GPA, load/save, console clear
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 200
#define FILENAME "students_db.txt"

#ifdef _WIN32
  #define CLEAR_CMD "cls"
#else
  #define CLEAR_CMD "clear"
#endif

typedef struct {
    int studentID;
    char fullName[100];
    char course[50];
    int yearOfStudy;
    float gpa;
} Student;

/* Function prototypes */
void clearConsole(void);
void pauseConsole(void);
int loadFromFile(Student students[]);
void saveToFile(Student students[], int count);
void addStudent(Student students[], int *count);
void displayStudents(Student students[], int count);
void searchStudent(Student students[], int count);
void sortByGPA(Student students[], int count);
int findStudentIndex(Student students[], int count, int id);

/* main */
int main(void) {
    Student students[MAX];
    int count = 0;
    int choice;

    count = loadFromFile(students);

    do {
        clearConsole();
        printf("=== Student Record Management ===\n");
        printf("1. Add New Student\n");
        printf("2. Display All Students\n");
        printf("3. Search Student by ID\n");
        printf("4. Sort Students by GPA (Descending)\n");
        printf("5. Save Records to File\n");
        printf("0. Exit\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n'); /* flush */
            pauseConsole();
            continue;
        }

        while (getchar() != '\n'); /* consume newline */

        clearConsole();
        switch (choice) {
            case 1:
                addStudent(students, &count);
                saveToFile(students, count);
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
                saveToFile(students, count);
                break;
            case 5:
                saveToFile(students, count);
                printf("Records saved successfully.\n");
                break;
            case 0:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }

        if (choice != 0) pauseConsole();
    } while (choice != 0);

    return 0;
}

/* clearConsole: cross-platform clear */
void clearConsole(void) {
    system(CLEAR_CMD);
}

/* pauseConsole: wait for Enter */
void pauseConsole(void) {
    printf("\nPress Enter to continue...");
    while (getchar() != '\n'); /* wait for Enter */
}

/* loadFromFile: returns count loaded */
int loadFromFile(Student students[]) {
    FILE *fp = fopen(FILENAME, "r");
    if (!fp) return 0; /* no file yet */

    int count = 0;
    /* file format: studentID|fullName|course|year|gpa\n
       using '|' as delimiter to allow spaces/commas in names */
    while (count < MAX) {
        int id, year;
        float gpa;
        char name[100];
        char course[50];

        int items = fscanf(fp, "%d|%99[^|]|%49[^|]|%d|%f\n",
                           &id, name, course, &year, &gpa);
        if (items != 5) break;

        students[count].studentID = id;
        strncpy(students[count].fullName, name, sizeof(students[count].fullName)-1);
        students[count].fullName[sizeof(students[count].fullName)-1] = '\0';
        strncpy(students[count].course, course, sizeof(students[count].course)-1);
        students[count].course[sizeof(students[count].course)-1] = '\0';
        students[count].yearOfStudy = year;
        students[count].gpa = gpa;

        count++;
    }

    fclose(fp);
    return count;
}

/* saveToFile: writes all records to file */
void saveToFile(Student students[], int count) {
    FILE *fp = fopen(FILENAME, "w");
    if (!fp) {
        printf("Error: could not open file for writing.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        /* same '|' delimited format */
        fprintf(fp, "%d|%s|%s|%d|%.2f\n",
                students[i].studentID,
                students[i].fullName,
                students[i].course,
                students[i].yearOfStudy,
                students[i].gpa);
    }

    fclose(fp);
}

/* addStudent: prompts and validates input */
void addStudent(Student students[], int *count) {
    if (*count >= MAX) {
        printf("Maximum student limit reached.\n");
        return;
    }

    Student s;
    int idOk = 0;

    /* Student ID */
    do {
        printf("Enter Student ID (positive integer): ");
        if (scanf("%d", &s.studentID) != 1) {
            printf("Invalid input.\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n'); /* consume newline */

        if (s.studentID <= 0) {
            printf("ID must be positive.\n");
            continue;
        }
        if (findStudentIndex(students, *count, s.studentID) != -1) {
            printf("ID already exists. Choose another.\n");
            continue;
        }
        idOk = 1;
    } while (!idOk);

    /* Full name */
    printf("Enter Full Name: ");
    fgets(s.fullName, sizeof(s.fullName), stdin);
    s.fullName[strcspn(s.fullName, "\n")] = '\0';

    /* Course */
    printf("Enter Course: ");
    fgets(s.course, sizeof(s.course), stdin);
    s.course[strcspn(s.course, "\n")] = '\0';

    /* Year of study */
    do {
        printf("Enter Year of Study (1-10): ");
        if (scanf("%d", &s.yearOfStudy) != 1) {
            printf("Invalid input.\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');
        if (s.yearOfStudy < 1 || s.yearOfStudy > 10) {
            printf("Year must be between 1 and 10.\n");
            continue;
        }
        break;
    } while (1);

    /* GPA */
    do {
        printf("Enter GPA (0.00 - 4.00): ");
        if (scanf("%f", &s.gpa) != 1) {
            printf("Invalid input.\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');
        if (s.gpa < 0.0f || s.gpa > 4.0f) {
            printf("GPA must be between 0.00 and 4.00.\n");
            continue;
        }
        break;
    } while (1);

    /* store */
    students[*count] = s;
    (*count)++;
    printf("Student added successfully.\n");
}

/* displayStudents: tabular output */
void displayStudents(Student students[], int count) {
    if (count == 0) {
        printf("No student records available.\n");
        return;
    }

    printf("%-8s %-30s %-20s %-6s %-5s\n", "ID", "Full Name", "Course", "Year", "GPA");
    printf("-----------------------------------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("%-8d %-30s %-20s %-6d %-5.2f\n",
               students[i].studentID,
               students[i].fullName,
               students[i].course,
               students[i].yearOfStudy,
               students[i].gpa);
    }
}

/* searchStudent: find by ID and display */
void searchStudent(Student students[], int count) {
    if (count == 0) {
        printf("No student records available.\n");
        return;
    }

    int id;
    printf("Enter Student ID to search: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid input.\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');

    int idx = findStudentIndex(students, count, id);
    if (idx == -1) {
        printf("Student with ID %d not found.\n", id);
        return;
    }

    Student *s = &students[idx];
    printf("\nStudent Details:\n");
    printf("ID: %d\nName: %s\nCourse: %s\nYear: %d\nGPA: %.2f\n",
           s->studentID, s->fullName, s->course, s->yearOfStudy, s->gpa);
}

/* sortByGPA: descending bubble sort */
void sortByGPA(Student students[], int count) {
    if (count < 2) {
        printf("Not enough records to sort.\n");
        return;
    }

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (students[j].gpa < students[j+1].gpa) {
                Student tmp = students[j];
                students[j] = students[j+1];
                students[j+1] = tmp;
            }
        }
    }
    printf("Records sorted by GPA (descending).\n");
}

/* findStudentIndex: returns index or -1 */
int findStudentIndex(Student students[], int count, int id) {
    for (int i = 0; i < count; i++) {
        if (students[i].studentID == id) return i;
    }
    return -1;
}
