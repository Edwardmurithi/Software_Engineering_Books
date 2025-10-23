#include <stdio.h>
// #include <time.h>
// #include <stdlib.h>

int main()
{
    int day = 3;

    switch(day) {
        case 1:
            printf("Monday\n");
            break;
        case 2:
            printf("Tuesday\n");
            break;
        case 3:
            printf("Wednesday\n");
            break;
        default:
            printf("%d is NOT a valid day\n", day);
    }


    return 0; // return O on success
}