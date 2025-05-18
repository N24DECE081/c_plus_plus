#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <ctime>
#include <cstdlib>
#include <clocale>
using namespace std;

// ---------------- ENUM Role ----------------
enum class Role { User, Manager };

// ---------------- CLASS OTP ----------------
class OTP {
private:
    string currentOTP;
public:
    OTP() { srand(time(0)); }

    string generateOTP() {
        currentOTP.clear();
        for (int i = 0; i < 6; ++i)
            currentOTP += '0' + rand() % 10;
        return currentOTP;
    }

    bool validateOTP(const string& input) {
        return input == currentOTP;
    }
};

// ---------------- CLASS Transaction ----------------
struct Transaction {
    string fromWallet;
    string toWallet;
    int amount;
    string status;

    string toString() const {
        return "From: " + fromWallet + " -> To: " + toWallet +
               " | Amount: " + to_string(amount) +
               " | Status: " + status + "\n";
    }
};

// ---------------- CLASS Wallet ----------------
class Wallet {
private:
    string walletID;
    int balance;
    vector<Transaction> history;

public:
    Wallet(const string& id, int initialBalance = 0)
        : walletID(id), balance(initialBalance) {}

    string getID() const { return walletID; }
    int getBalance() const { return balance; }

    bool withdraw(int amount) {
        if (balance >= amount) {
            balance -= amount;
            return true;
        }
        return false;
    }

    void deposit(int amount) {
        balance += amount;
    }

    void addTransaction(const Transaction& tx) {
        history.push_back(tx);
    }

    void showHistory() const {
        cout << "\n--- Lịch sử giao dịch của ví [" << walletID << "] ---\n";
        for (const auto& tx : history) {
            cout << tx.toString();
        }
    }

    void showBalance() const {
        cout << "Ví: " << walletID << " | Số dư: " << balance << " điểm\n";
    }
};

// ---------------- CLASS User ----------------
class User {
private:
    string username;
    Role role;
    Wallet wallet;

public:
    // Default constructor để hỗ trợ map::operator[]
    User()
        : username(""), role(Role::User), wallet("DEFAULT_WALLET", 0) {}

    User(const string& uname, Role r, const string& walletID, int initialBalance = 0)
        : username(uname), role(r), wallet(walletID, initialBalance) {}

    string getUsername() const { return username; }
    Role getRole() const { return role; }
    Wallet& getWallet() { return wallet; }

    void viewWallet() {
        wallet.showBalance();
        wallet.showHistory();
    }
};

// ---------------- CLASS WalletSystem ----------------
class WalletSystem {
private:
    map<string, User> users;
    OTP otp;

    void saveTransactionToFile(const Transaction& tx) {
        ofstream file("transactions.txt", ios::app);
        if (file.is_open()) {
            file << tx.toString();
            file.close();
        }
    }

public:
    void createUser(const string& username, Role role, int initialBalance = 0) {
        string walletID = "WALLET_" + username;
        users[username] = User(username, role, walletID, initialBalance);
        cout << "✅ Tạo người dùng " << username << " với ví " << walletID << " thành công.\n";
    }

    void transfer(const string& fromUser, const string& toUser, int amount) {
        if (users.find(fromUser) == users.end() || users.find(toUser) == users.end()) {
            cout << "❌ Người gửi hoặc người nhận không tồn tại.\n";
            return;
        }

        Wallet& senderWallet = users[fromUser].getWallet();
        Wallet& receiverWallet = users[toUser].getWallet();

        // OTP xác nhận
        string otpCode = otp.generateOTP();
        cout << ">> OTP gửi đến " << fromUser << ": " << otpCode << "\n";
        string inputOTP;
        cout << "Nhập OTP để xác nhận giao dịch: ";
        cin >> inputOTP;

        if (!otp.validateOTP(inputOTP)) {
            cout << "❌ OTP không hợp lệ. Hủy giao dịch.\n";
            return;
        }

        // Giao dịch nguyên tử
        string status = "Thành công";
        if (senderWallet.withdraw(amount)) {
            receiverWallet.deposit(amount);
        } else {
            status = "Thất bại - Không đủ số dư";
        }

        Transaction tx = {
            senderWallet.getID(),
            receiverWallet.getID(),
            amount,
            status
        };

        senderWallet.addTransaction(tx);
        receiverWallet.addTransaction(tx);
        saveTransactionToFile(tx);

        if (status == "Thành công")
            cout << "✅ Giao dịch thành công.\n";
        else
            cout << "❌ Giao dịch thất bại: Không đủ số dư.\n";
    }

    void showUserInfo(const string& username) {
        if (users.find(username) == users.end()) {
            cout << "❌ Người dùng không tồn tại.\n";
            return;
        }
        users[username].viewWallet();
    }

    void showAllTransactions() {
        ifstream file("transactions.txt");
        cout << "\n--- Toàn bộ lịch sử giao dịch hệ thống ---\n";
        string line;
        while (getline(file, line)) {
            cout << line << "\n";
        }
        file.close();
    }

    Role getUserRole(const string& username) {
        return users[username].getRole();
    }

    bool userExists(const string& username) {
        return users.find(username) != users.end();
    }
};

// ---------------- MAIN ----------------
int main() {
    setlocale(LC_ALL, "");// gỡ lỗi tiếng việt
    WalletSystem system;

    // Tạo người dùng mẫu
    system.createUser("admin", Role::Manager, 10000);
    system.createUser("alice", Role::User, 1000);
    system.createUser("bob", Role::User, 500);

    string currentUser;
    cout << "\n>> Đăng nhập (nhập tên người dùng): ";
    cin >> currentUser;

    if (!system.userExists(currentUser)) {
        cout << "❌ Người dùng không tồn tại.\n";
        return 0;
    }

    Role role = system.getUserRole(currentUser);

    int choice;
    while (true) {
        cout << "\n===== MENU (" << (role == Role::Manager ? "Quản lý" : "Người dùng") << ") =====\n";
        cout << "1. Chuyển điểm\n";
        cout << "2. Xem thông tin ví\n";
        if (role == Role::Manager) {
            cout << "3. Xem toàn bộ giao dịch hệ thống\n";
            cout << "4. Tạo người dùng mới\n";
            cout << "5. Thoát\n";
        } else {
            cout << "3. Thoát\n";
        }

        cout << "Chọn: ";
        cin >> choice;

        if ((role == Role::User && choice == 3) || (role == Role::Manager && choice == 5)) break;

        if (choice == 1) {
            string toUser;
            int amount;
            cout << "Chuyển đến ai: ";
            cin >> toUser;
            cout << "Số điểm: ";
            cin >> amount;
            system.transfer(currentUser, toUser, amount);
        } else if (choice == 2) {
            system.showUserInfo(currentUser);
        } else if (role == Role::Manager && choice == 3) {
            system.showAllTransactions();
        } else if (role == Role::Manager && choice == 4) {
            string uname;
            int balance;
            cout << "Tên người dùng mới: ";
            cin >> uname;
            cout << "Số điểm ban đầu: ";
            cin >> balance;
            system.createUser(uname, Role::User, balance);
        } else {
            cout << "❌ Lựa chọn không hợp lệ.\n";
        }
    }

    return 0;
}
