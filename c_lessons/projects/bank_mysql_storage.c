#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

#define HOST "localhost"
#define USER "root"       // Your MySQL username
#define PASS "password"   // Your MySQL password
#define DB "bank_system"

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
void createAccount();
void deposit();
void withdraw();
void transfer();
void displayAccount();
int accountExists(int accountNumber);

int main() {
    int choice;

    // Initialize connection
    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, HOST, USER, PASS, DB, 0, NULL, 0)) {
        fprintf(stderr, "Failed to connect to database: %s\n", mysql_error(conn));
        exit(1);
    }

    do {
        clearConsole();
        printf("--- Bank Transaction System (MySQL) ---\n");
        printf("1. Create New Account\n");
        printf("2. Deposit Money\n");
        printf("3. Withdraw Money\n");
        printf("4. Transfer Funds\n");
        printf("5. Display Account Details\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");

        if(scanf("%d", &choice) != 1) {
            printf("Invalid input! Try again.\n");
            while(getchar() != '\n');
            continue;
        }

        clearConsole();
        switch(choice) {
            case 1: createAccount(); break;
            case 2: deposit(); break;
            case 3: withdraw(); break;
            case 4: transfer(); break;
            case 5: displayAccount(); break;
            case 0: printf("Exiting program...\n"); break;
            default: printf("Invalid choice! Try again.\n");
        }

        printf("\nPress Enter to continue...");
        while(getchar() != '\n'); // clear buffer
        getchar(); // wait for Enter
    } while(choice != 0);

    mysql_close(conn);
    return 0;
}

// Check if account exists
int accountExists(int accountNumber) {
    char query[256];
    sprintf(query, "SELECT * FROM customers WHERE accountNumber=%d", accountNumber);
    if(mysql_query(conn, query)) {
        fprintf(stderr, "Query failed: %s\n", mysql_error(conn));
        return 0;
    }
    res = mysql_store_result(conn);
    int exists = mysql_num_rows(res) > 0;
    mysql_free_result(res);
    return exists;
}

// Create account
void createAccount() {
    int accountNumber;
    char holderName[50];
    float balance;

    printf("Enter Account Number: ");
    scanf("%d", &accountNumber);
    if(accountExists(accountNumber)) {
        printf("Account already exists!\n");
        return;
    }
    while(getchar() != '\n');

    printf("Enter Account Holder Name: ");
    fgets(holderName, sizeof(holderName), stdin);
    holderName[strcspn(holderName, "\n")] = '\0';

    printf("Enter Initial Balance: ");
    scanf("%f", &balance);

    char query[512];
    sprintf(query, "INSERT INTO customers VALUES(%d, '%s', %.2f, 'Account created.\\n')", 
            accountNumber, holderName, balance);

    if(mysql_query(conn, query)) {
        fprintf(stderr, "Insert failed: %s\n", mysql_error(conn));
        return;
    }
    printf("Account created successfully!\n");
}

// Deposit
void deposit() {
    int accountNumber;
    float amount;
    printf("Enter Account Number: ");
    scanf("%d", &accountNumber);
    if(!accountExists(accountNumber)) {
        printf("Account not found!\n");
        return;
    }
    printf("Enter amount to deposit: ");
    scanf("%f", &amount);

    char query[512];
    sprintf(query, "UPDATE customers SET balance = balance + %.2f, "
                   "transactionHistory = CONCAT(transactionHistory, 'Deposited %.2f\\n') "
                   "WHERE accountNumber = %d", amount, amount, accountNumber);

    if(mysql_query(conn, query)) {
        fprintf(stderr, "Deposit failed: %s\n", mysql_error(conn));
        return;
    }
    printf("Deposit successful!\n");
}

// Withdraw
void withdraw() {
    int accountNumber;
    float amount;
    printf("Enter Account Number: ");
    scanf("%d", &accountNumber);

    char query[512];
    sprintf(query, "SELECT balance FROM customers WHERE accountNumber=%d", accountNumber);
    if(mysql_query(conn, query)) { fprintf(stderr, "%s\n", mysql_error(conn)); return; }
    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);
    if(!row) { printf("Account not found!\n"); mysql_free_result(res); return; }

    float balance = atof(row[0]);
    mysql_free_result(res);

    printf("Enter amount to withdraw: ");
    scanf("%f", &amount);

    if(amount > balance) { printf("Insufficient funds!\n"); return; }

    sprintf(query, "UPDATE customers SET balance = balance - %.2f, "
                   "transactionHistory = CONCAT(transactionHistory, 'Withdrew %.2f\\n') "
                   "WHERE accountNumber = %d", amount, amount, accountNumber);
    if(mysql_query(conn, query)) { fprintf(stderr, "%s\n", mysql_error(conn)); return; }
    printf("Withdrawal successful!\n");
}

// Transfer
void transfer() {
    int fromAcc, toAcc;
    float amount;

    printf("Enter Sender Account Number: ");
    scanf("%d", &fromAcc);
    if(!accountExists(fromAcc)) { printf("Sender not found!\n"); return; }

    printf("Enter Receiver Account Number: ");
    scanf("%d", &toAcc);
    if(!accountExists(toAcc)) { printf("Receiver not found!\n"); return; }

    printf("Enter amount to transfer: ");
    scanf("%f", &amount);

    char query[512];
    // Check sender balance
    sprintf(query, "SELECT balance FROM customers WHERE accountNumber=%d", fromAcc);
    if(mysql_query(conn, query)) { fprintf(stderr, "%s\n", mysql_error(conn)); return; }
    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);
    float senderBalance = atof(row[0]);
    mysql_free_result(res);

    if(amount > senderBalance) { printf("Insufficient funds!\n"); return; }

    // Deduct from sender
    sprintf(query, "UPDATE customers SET balance = balance - %.2f, "
                   "transactionHistory = CONCAT(transactionHistory, 'Transferred %.2f to account %d\\n') "
                   "WHERE accountNumber = %d", amount, amount, toAcc, fromAcc);
    if(mysql_query(conn, query)) { fprintf(stderr, "%s\n", mysql_error(conn)); return; }

    // Add to receiver
    sprintf(query, "UPDATE customers SET balance = balance + %.2f, "
                   "transactionHistory = CONCAT(transactionHistory, 'Received %.2f from account %d\\n') "
                   "WHERE accountNumber = %d", amount, amount, fromAcc, toAcc);
    if(mysql_query(conn, query)) { fprintf(stderr, "%s\n", mysql_error(conn)); return; }

    printf("Transfer successful!\n");
}

// Display account
void displayAccount() {
    int accountNumber;
    printf("Enter Account Number: ");
    scanf("%d", &accountNumber);

    char query[512];
    sprintf(query, "SELECT * FROM customers WHERE accountNumber=%d", accountNumber);
    if(mysql_query(conn, query)) { fprintf(stderr, "%s\n", mysql_error(conn)); return; }

    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);
    if(!row) { printf("Account not found!\n"); mysql_free_result(res); return; }

    printf("\nAccount Number: %s\n", row[0]);
    printf("Account Holder: %s\n", row[1]);
    printf("Balance: %s\n", row[2]);
    printf("Transaction History:\n%s\n", row[3]);
    mysql_free_result(res);
}
