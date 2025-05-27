#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

struct Transaction {
    string fromWallet, toWallet;
    int amount;
    string status;

    string toString() const {
        return "From: " + fromWallet + " -> To: " + toWallet +
               " | Amount: " + to_string(amount) +
               " | Status: " + status + "\n";
    }
};

class Wallet {
private:
    string walletID;
    int balance;
    vector<Transaction> history;
public:
    Wallet(string id = "", int initialBalance = 0) : walletID(id), balance(initialBalance) {}

    string getID() const { return walletID; }
    int getBalance() const { return balance; }
    void setBalance(int b) { balance = b; }

    bool withdraw(int amount) {
        if (balance >= amount) {
            balance -= amount;
            return true;
        }
        return false;
    }

    void deposit(int amount) { balance += amount; }

    void addTransaction(const Transaction& tx) { history.push_back(tx); }

    void showBalance() const {
        cout << "Ví: " << walletID << " | Số dư: " << balance << " điểm\n";
    }

    void showHistory() const {
        cout << "\n--- Lịch sử ví " << walletID << " ---\n";
        for (const auto& tx : history) cout << tx.toString();
    }
};

class TransactionManager {
public:
    void saveTransactionToFile(const Transaction& tx) {
        ofstream f("transactions.txt", ios::app);
        if (f.is_open()) f << tx.toString();
    }
};

class TransferService {
public:
    static bool transferPoints(Wallet& from, Wallet& to, int amount, const string& reason, TransactionManager& tm) {
        if (from.withdraw(amount)) {
            to.deposit(amount);
            Transaction tx = {from.getID(), to.getID(), amount, reason};
            from.addTransaction(tx);
            to.addTransaction(tx);
            tm.saveTransactionToFile(tx);
            return true;
        }
        return false;
    }
};
