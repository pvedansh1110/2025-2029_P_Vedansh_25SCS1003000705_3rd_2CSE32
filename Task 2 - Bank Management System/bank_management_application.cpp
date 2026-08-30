#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstdio>
using namespace std;

class BankAccount {
private:
    int accountNumber;
    string customerName;
    double balance;

public:
    BankAccount() {
        accountNumber = 0;
        customerName = "";
        balance = 0.0;
    }

    BankAccount(int accNo, string name, double bal) {
        accountNumber = accNo;
        customerName = name;
        balance = bal;
    }

    int getAccountNumber() {
        return accountNumber;
    }

    string getCustomerName() {
        return customerName;
    }

    double getBalance() {
        return balance;
    }

    void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            cout << "Amount deposited successfully.\n";
        } else {
            cout << "Invalid amount.\n";
        }
    }

    void withdraw(double amount) {
        if (amount <= 0) {
            cout << "Invalid amount.\n";
        } else if (amount > balance) {
            cout << "Insufficient balance.\n";
        } else {
            balance -= amount;
            cout << "Amount withdrawn successfully.\n";
        }
    }

    void display() {
        cout << "\nAccount Number : " << accountNumber << endl;
        cout << "Customer Name  : " << customerName << endl;
        cout << fixed << setprecision(2);
        cout << "Balance        : Rs. " << balance << endl;
    }
};

// Save all account records to a file
void saveAccount(BankAccount account) {
    ofstream file("bank_accounts.txt", ios::app);

    if (!file) {
        cout << "Error: Could not open file.\n";
        return;
    }

    file << account.getAccountNumber() << "|"
         << account.getCustomerName() << "|"
         << fixed << setprecision(2) << account.getBalance() << "\n";

    file.close();
}

// Create a new account
void createAccount() {
    int accNo;
    string name;
    double initialDeposit;

    cout << "\nEnter Account Number: ";
    cin >> accNo;
    cin.ignore();

    cout << "Enter Customer Name: ";
    getline(cin, name);

    cout << "Enter Initial Deposit: ";
    cin >> initialDeposit;

    if (initialDeposit < 0) {
        cout << "Initial deposit cannot be negative.\n";
        return;
    }

    BankAccount account(accNo, name, initialDeposit);
    saveAccount(account);

    cout << "Account created successfully.\n";
}

// Find an account in the file and load it
bool findAccount(int searchAccount, BankAccount &account) {
    ifstream file("bank_accounts.txt");

    if (!file) {
        return false;
    }

    int accNo;
    string name;
    double balance;
    string line;

    while (getline(file, line)) {
        size_t p1 = line.find('|');
        size_t p2 = line.find('|', p1 + 1);

        if (p1 == string::npos || p2 == string::npos)
            continue;

        accNo = stoi(line.substr(0, p1));
        name = line.substr(p1 + 1, p2 - p1 - 1);
        balance = stod(line.substr(p2 + 1));

        if (accNo == searchAccount) {
            account = BankAccount(accNo, name, balance);
            file.close();
            return true;
        }
    }

    file.close();
    return false;
}

// Rewrite the file after a transaction
void updateAccount(BankAccount updatedAccount) {
    ifstream file("bank_accounts.txt");
    ofstream temp("temp.txt");

    string line;

    while (getline(file, line)) {
        size_t p1 = line.find('|');
        size_t p2 = line.find('|', p1 + 1);

        if (p1 == string::npos || p2 == string::npos)
            continue;

        int accNo = stoi(line.substr(0, p1));

        if (accNo == updatedAccount.getAccountNumber()) {
            temp << updatedAccount.getAccountNumber() << "|"
                 << updatedAccount.getCustomerName() << "|"
                 << fixed << setprecision(2)
                 << updatedAccount.getBalance() << "\n";
        } else {
            temp << line << "\n";
        }
    }

    file.close();
    temp.close();

    remove("bank_accounts.txt");
    rename("temp.txt", "bank_accounts.txt");
}

// Deposit money
void depositMoney() {
    int accNo;
    double amount;

    cout << "\nEnter Account Number: ";
    cin >> accNo;

    BankAccount account;

    if (!findAccount(accNo, account)) {
        cout << "Account not found.\n";
        return;
    }

    cout << "Enter Amount to Deposit: ";
    cin >> amount;

    account.deposit(amount);

    updateAccount(account);
}

// Withdraw money
void withdrawMoney() {
    int accNo;
    double amount;

    cout << "\nEnter Account Number: ";
    cin >> accNo;

    BankAccount account;

    if (!findAccount(accNo, account)) {
        cout << "Account not found.\n";
        return;
    }

    cout << "Enter Amount to Withdraw: ";
    cin >> amount;

    account.withdraw(amount);

    updateAccount(account);
}

// Check balance
void checkBalance() {
    int accNo;

    cout << "\nEnter Account Number: ";
    cin >> accNo;

    BankAccount account;

    if (findAccount(accNo, account)) {
        cout << "\nCurrent Balance: Rs. "
             << fixed << setprecision(2)
             << account.getBalance() << endl;
    } else {
        cout << "Account not found.\n";
    }
}

// Display all accounts
void displayAllAccounts() {
    ifstream file("bank_accounts.txt");

    if (!file) {
        cout << "\nNo account records found.\n";
        return;
    }

    int accNo;
    string name;
    double balance;
    string line;
    bool found = false;

    cout << "\n============== ALL ACCOUNTS ==============\n";

    while (getline(file, line)) {
        size_t p1 = line.find('|');
        size_t p2 = line.find('|', p1 + 1);

        if (p1 == string::npos || p2 == string::npos)
            continue;

        accNo = stoi(line.substr(0, p1));
        name = line.substr(p1 + 1, p2 - p1 - 1);
        balance = stod(line.substr(p2 + 1));

        cout << "Account Number: " << accNo << endl;
        cout << "Customer Name : " << name << endl;
        cout << "Balance       : Rs. "
             << fixed << setprecision(2) << balance << endl;
        cout << "------------------------------------------\n";

        found = true;
    }

    if (!found)
        cout << "No account records found.\n";

    file.close();
}

int main() {
    int choice;

    do {
        cout << "\n=====================================\n";
        cout << "       BANK MANAGEMENT SYSTEM\n";
        cout << "=====================================\n";
        cout << "1. Create Account\n";
        cout << "2. Deposit Money\n";
        cout << "3. Withdraw Money\n";
        cout << "4. Check Balance\n";
        cout << "5. Display All Accounts\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                createAccount();
                break;

            case 2:
                depositMoney();
                break;

            case 3:
                withdrawMoney();
                break;

            case 4:
                checkBalance();
                break;

            case 5:
                displayAllAccounts();
                break;

            case 6:
                cout << "Thank you for using the Bank Management System!\n";
                break;

            default:
                cout << "Invalid choice. Please try again.\n";
        }

    } while (choice != 6);

    return 0;
}
