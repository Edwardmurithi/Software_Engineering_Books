#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

typedef struct {
    int accountNumber;
    char holderName[50];
    float balance;
    char transactionHistory[500]; // Stores a simple transaction log
} Customer;

// Function prototypes
void createAccount(Customer customers[], int *count);
void deposit(Customer customers[], int count);
void withdraw(Customer customers[], int count);
void transfer(Customer customers[], int count);
void displayAccount(Customer customers[], int count);
int findAccountIndex(Customer customers[], int count, int accNumber);

int main() {
    Customer customers[MAX];
    int count = 0, choice;

    do {
        printf("\n--- Bank Transaction System ---\n");
        printf("1. Create New Account\n");
        printf("2. Deposit Money\n");
        printf("3. Withdraw Money\n");
        printf("4. Transfer Funds\n");
        printf("5. Display Account Details\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");

        if(scanf("%d", &choice) != 1) {
            printf("Invalid input! Please enter a number.\n");
            while(getchar() != '\n'); // clear buffer
            continue;
        }

        switch(choice) {
            case 1:
                createAccount(customers, &count);
                break;
            case 2:
                deposit(customers, count);
                break;
            case 3:
                withdraw(customers, count);
                break;
            case 4:
                transfer(customers, count);
                break;
            case 5:
                displayAccount(customers, count);
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

// Create a new account
void createAccount(Customer customers[], int *count) {
    if(*count >= MAX) {
        printf("Cannot create more accounts. Maximum reached.\n");
        return;
    }

    printf("Enter Account Number: ");
    if(scanf("%d", &customers[*count].accountNumber) != 1) {
        printf("Invalid input!\n");
        while(getchar() != '\n'); 
        return;
    }

    while(getchar() != '\n'); // clear input buffer

    printf("Enter Account Holder Name: ");
    fgets(customers[*count].holderName, sizeof(customers[*count].holderName), stdin);
    customers[*count].holderName[strcspn(customers[*count].holderName, "\n")] = '\0';

    printf("Enter Initial Balance: ");
    if(scanf("%f", &customers[*count].balance) != 1) {
        printf("Invalid balance!\n");
        while(getchar() != '\n'); 
        return;
    }

    strcpy(customers[*count].transactionHistory, "Account created.\n");

    (*count)++;
    printf("Account created successfully!\n");
}

// Deposit money into an account
void deposit(Customer customers[], int count) {
    int accNumber;
    float amount;
    printf("Enter Account Number: ");
    if(scanf("%d", &accNumber) != 1) {
        printf("Invalid input!\n");
        while(getchar() != '\n'); 
        return;
    }

    int index = findAccountIndex(customers, count, accNumber);
    if(index == -1) {
        printf("Account not found!\n");
        return;
    }

    printf("Enter amount to deposit: ");
    if(scanf("%f", &amount) != 1 || amount <= 0) {
        printf("Invalid amount!\n");
        while(getchar() != '\n'); 
        return;
    }

    customers[index].balance += amount;

    char log[100];
    sprintf(log, "Deposited %.2f\n", amount);
    strcat(customers[index].transactionHistory, log);

    printf("Deposit successful! New balance: %.2f\n", customers[index].balance);
}

// Withdraw money with balance check
void withdraw(Customer customers[], int count) {
    int accNumber;
    float amount;
    printf("Enter Account Number: ");
    if(scanf("%d", &accNumber) != 1) {
        printf("Invalid input!\n");
        while(getchar() != '\n'); 
        return;
    }

    int index = findAccountIndex(customers, count, accNumber);
    if(index == -1) {
        printf("Account not found!\n");
        return;
    }

    printf("Enter amount to withdraw: ");
    if(scanf("%f", &amount) != 1 || amount <= 0) {
        printf("Invalid amount!\n");
        while(getchar() != '\n'); 
        return;
    }

    if(customers[index].balance < amount) {
        printf("Insufficient funds! Current balance: %.2f\n", customers[index].balance);
        return;
    }

    customers[index].balance -= amount;

    char log[100];
    sprintf(log, "Withdrew %.2f\n", amount);
    strcat(customers[index].transactionHistory, log);

    printf("Withdrawal successful! New balance: %.2f\n", customers[index].balance);
}

// Transfer funds between two accounts
void transfer(Customer customers[], int count) {
    int fromAcc, toAcc;
    float amount;

    printf("Enter Sender Account Number: ");
    if(scanf("%d", &fromAcc) != 1) {
        printf("Invalid input!\n");
        while(getchar() != '\n'); 
        return;
    }
    int fromIndex = findAccountIndex(customers, count, fromAcc);
    if(fromIndex == -1) {
        printf("Sender account not found!\n");
        return;
    }

    printf("Enter Receiver Account Number: ");
    if(scanf("%d", &toAcc) != 1) {
        printf("Invalid input!\n");
        while(getchar() != '\n'); 
        return;
    }
    int toIndex = findAccountIndex(customers, count, toAcc);
    if(toIndex == -1) {
        printf("Receiver account not found!\n");
        return;
    }

    printf("Enter amount to transfer: ");
    if(scanf("%f", &amount) != 1 || amount <= 0) {
        printf("Invalid amount!\n");
        while(getchar() != '\n'); 
        return;
    }

    if(customers[fromIndex].balance < amount) {
        printf("Insufficient funds! Sender balance: %.2f\n", customers[fromIndex].balance);
        return;
    }

    customers[fromIndex].balance -= amount;
    customers[toIndex].balance += amount;

    char log[100];
    sprintf(log, "Transferred %.2f to account %d\n", amount, customers[toIndex].accountNumber);
    strcat(customers[fromIndex].transactionHistory, log);

    sprintf(log, "Received %.2f from account %d\n", amount, customers[fromIndex].accountNumber);
    strcat(customers[toIndex].transactionHistory, log);

    printf("Transfer successful!\n");
}

// Display account details and transaction history
void displayAccount(Customer customers[], int count) {
    int accNumber;
    printf("Enter Account Number: ");
    if(scanf("%d", &accNumber) != 1) {
        printf("Invalid input!\n");
        while(getchar() != '\n'); 
        return;
    }

    int index = findAccountIndex(customers, count, accNumber);
    if(index == -1) {
        printf("Account not found!\n");
        return;
    }

    printf("\nAccount Number: %d\n", customers[index].accountNumber);
    printf("Account Holder: %s\n", customers[index].holderName);
    printf("Balance: %.2f\n", customers[index].balance);
    printf("Transaction History:\n%s", customers[index].transactionHistory);
}

// Helper function to find account index by account number
int findAccountIndex(Customer customers[], int count, int accNumber) {
    for(int i = 0; i < count; i++) {
        if(customers[i].accountNumber == accNumber)
            return i;
    }
    return -1;
}
