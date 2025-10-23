#include <stdio.h>

int main()
{
    int marks;

    printf("Enter Marks: ");
    scanf("%i", &marks);

    if (marks >= 0 && marks <= 49) {
        printf("Grade = E\n");
    }
    else if (marks >= 50 && marks <= 65) {
        printf("Grade = C\n");
    }
    else if (marks >= 66 && marks <= 75) {
        printf("Grade = B\n");
    }
    else if (marks >= 76 && marks <= 100) {
        printf("Grade = A\n");
    }
    else {
        printf("Enter VALID MARKS\n");
    }

    return 0;
}