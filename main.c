#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 100

// ---------------- STRUCTS ----------------
struct Account {
    int accountNumber;
    char name[50];
    float balance;
};

struct Transaction {
    int accountNumber;
    char type[10];
    float amount;
    char datetime[30];
};

// ---------------- GLOBAL ----------------
struct Account accounts[MAX];
struct Transaction transactions[MAX];

int accCount = 0, transCount = 0;
int nextAccNo = 1001;

// ---------------- UTIL ----------------
void readLine(char *buffer, int size) {
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
}

void getCurrentTime(char *buffer) {
    time_t t;
    time(&t);
    strcpy(buffer, ctime(&t));
    buffer[strcspn(buffer, "\n")] = 0;
}

int findAccountIndex(int accNo) {
    for (int i = 0; i < accCount; i++) {
        if (accounts[i].accountNumber == accNo)
            return i;
    }
    return -1;
}

// ---------------- FILE HANDLING ----------------
void saveData() {
    FILE *fp1 = fopen("accounts.dat", "wb");
    FILE *fp2 = fopen("transactions.dat", "wb");

    if (fp1 && fp2) {
        fwrite(&accCount, sizeof(int), 1, fp1);
        fwrite(&nextAccNo, sizeof(int), 1, fp1);
        fwrite(accounts, sizeof(struct Account), accCount, fp1);

        fwrite(&transCount, sizeof(int), 1, fp2);
        fwrite(transactions, sizeof(struct Transaction), transCount, fp2);
    }

    if (fp1) fclose(fp1);
    if (fp2) fclose(fp2);
}

void loadData() {
    FILE *fp1 = fopen("accounts.dat", "rb");
    FILE *fp2 = fopen("transactions.dat", "rb");

    if (fp1) {
        fread(&accCount, sizeof(int), 1, fp1);
        fread(&nextAccNo, sizeof(int), 1, fp1);
        fread(accounts, sizeof(struct Account), accCount, fp1);
        fclose(fp1);
    }

    if (fp2) {
        fread(&transCount, sizeof(int), 1, fp2);
        fread(transactions, sizeof(struct Transaction), transCount, fp2);
        fclose(fp2);
    }
}

// ---------------- ACCOUNT ----------------
void createAccount() {
    if (accCount >= MAX) {
        printf("Limit reached!\n");
        return;
    }

    struct Account acc;
    acc.accountNumber = nextAccNo++;

    getchar();
    printf("Enter Name: ");
    readLine(acc.name, sizeof(acc.name));

    if (strlen(acc.name) == 0) {
        printf("Name cannot be empty!\n");
        return;
    }

    acc.balance = 0;
    accounts[accCount++] = acc;

    printf("Account Created! Acc No: %d\n", acc.accountNumber);
}

void viewAccounts() {
    if (accCount == 0) {
        printf("No accounts found.\n");
        return;
    }

    for (int i = 0; i < accCount; i++) {
        printf("\n-------------------------\n");
        printf("Acc No : %d\n", accounts[i].accountNumber);
        printf("Name   : %s\n", accounts[i].name);
        printf("Balance: %.2f\n", accounts[i].balance);
    }
}

void searchAccount() {
    int accNo;
    printf("Enter Account Number: ");
    scanf("%d", &accNo);

    int idx = findAccountIndex(accNo);

    if (idx == -1) {
        printf("Account not found!\n");
        return;
    }

    printf("\nAcc No : %d\n", accounts[idx].accountNumber);
    printf("Name   : %s\n", accounts[idx].name);
    printf("Balance: %.2f\n", accounts[idx].balance);
}

// ---------------- TRANSACTION ----------------
void addTransaction(int accNo, char type[], float amount) {
    struct Transaction t;

    t.accountNumber = accNo;
    strcpy(t.type, type);
    t.amount = amount;
    getCurrentTime(t.datetime);

    transactions[transCount++] = t;
}

void depositMoney() {
    int accNo;
    float amount;

    printf("Enter Account Number: ");
    scanf("%d", &accNo);

    int idx = findAccountIndex(accNo);
    if (idx == -1) {
        printf("Account not found!\n");
        return;
    }

    printf("Enter Amount: ");
    scanf("%f", &amount);

    if (amount <= 0) {
        printf("Invalid amount!\n");
        return;
    }

    accounts[idx].balance += amount;
    addTransaction(accNo, "Deposit", amount);

    printf("Deposited Successfully!\n");
}

void withdrawMoney() {
    int accNo;
    float amount;

    printf("Enter Account Number: ");
    scanf("%d", &accNo);

    int idx = findAccountIndex(accNo);
    if (idx == -1) {
        printf("Account not found!\n");
        return;
    }

    printf("Enter Amount: ");
    scanf("%f", &amount);

    if (amount <= 0) {
        printf("Invalid amount!\n");
        return;
    }

    if (accounts[idx].balance < amount) {
        printf("Insufficient Balance!\n");
        return;
    }

    accounts[idx].balance -= amount;
    addTransaction(accNo, "Withdraw", amount);

    printf("Withdraw Successful!\n");
}

// ---------------- VIEW TRANSACTIONS ----------------
void viewTransactions() {
    if (transCount == 0) {
        printf("No transactions found.\n");
        return;
    }

    for (int i = 0; i < transCount; i++) {
        printf("\n-------------------------\n");
        printf("Acc No : %d\n", transactions[i].accountNumber);
        printf("Type   : %s\n", transactions[i].type);
        printf("Amount : %.2f\n", transactions[i].amount);
        printf("Time   : %s\n", transactions[i].datetime);
    }
}

// ---------------- MAIN ----------------
int main() {
    int choice;

    loadData();

    while (1) {
        printf("\n===== BANKING SYSTEM =====\n");
        printf("1. Create Account\n");
        printf("2. View Accounts\n");
        printf("3. Search Account\n");
        printf("4. Deposit Money\n");
        printf("5. Withdraw Money\n");
        printf("6. View Transactions\n");
        printf("7. Save & Exit\n");

        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: createAccount(); break;
            case 2: viewAccounts(); break;
            case 3: searchAccount(); break;
            case 4: depositMoney(); break;
            case 5: withdrawMoney(); break;
            case 6: viewTransactions(); break;
            case 7:
                saveData();
                printf("Data Saved. Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice!\n");
        }
    }

    return 0;
}