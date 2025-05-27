// Giữ nguyên toàn bộ phần đầu
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <ctime>
#include <cstdlib>
#include <sstream>
#include <algorithm>
#include <clocale>

using namespace std;

enum class Role { User, Manager };

class OTP {
private:
    string currentOTP;
public:
    OTP() { srand(time(0)); }
    string generateOTP() {
        currentOTP.clear();
        for (int i = 0; i < 6; ++i) currentOTP += '0' + rand() % 10;
        cout << "[OTP (giả lập) được gửi: " << currentOTP << "]\n";
        return currentOTP;
    }
    bool validateOTP(const string& input) { return input == currentOTP; }
};

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
    bool withdraw(int amount) { if (balance >= amount) { balance -= amount; return true; } return false; }
    void deposit(int amount) { balance += amount; }
    void setBalance(int b) { balance = b; }
    void addTransaction(const Transaction& tx) { history.push_back(tx); }
    void showBalance() const { cout << "Ví: " << walletID << " | Số dư: " << balance << " điểm\n"; }
    void showHistory() const {
        cout << "\n--- Lịch sử ví " << walletID << " ---\n";
        for (const auto& tx : history) cout << tx.toString();
    }
};

class User {
public:
    string username, password, fullName, email, phone;
    Role role;
    Wallet wallet;
    bool mustChangePassword = false;

    User(string u = "", string p = "", string name = "", string em = "", string ph = "", Role r = Role::User)
        : username(u), password(p), fullName(name), email(em), phone(ph), role(r), wallet("WALLET_" + u, 0) {}

    void displayUserInfo() {
        cout << "\n--- Thông tin người dùng ---\n";
        cout << "Tên đăng nhập: " << username << "\nHọ tên: " << fullName << "\nEmail: " << email << "\nSĐT: " << phone;
        cout << "\nVai trò: " << (role == Role::Manager ? "Quản lý" : "Người dùng") << "\n";
    }

    void backupToFile() {
        time_t now = time(0);
        ofstream file("backup_users.txt", ios::app);
        if (file.is_open()) {
            file << username << "|" << password << "|" << fullName << "|" << email << "|" << phone << "|";
            file << (role == Role::Manager ? "Manager" : "User") << "|" << wallet.getBalance() << "|" << ctime(&now);
            file.close();
        }
    }

    void restoreFromBackupLine(const string& line) {
        stringstream ss(line);
        string token;
        getline(ss, username, '|');
        getline(ss, password, '|');
        getline(ss, fullName, '|');
        getline(ss, email, '|');
        getline(ss, phone, '|');
        getline(ss, token, '|');
        role = (token == "Manager") ? Role::Manager : Role::User;
        getline(ss, token, '|');
        wallet = Wallet("WALLET_" + username);
        wallet.setBalance(stoi(token));
        mustChangePassword = false;
    }

       void updateUserInfo(OTP& otp, bool isAdmin = false) {
    cin.ignore();
    string newName, newPass, newEmail, newPhone;

    if (isAdmin) {
        cout << "\nAdmin muốn cập nhật tài khoản người dùng (" << username << "):\n";
    } else {
        cout << "\n=== Cập nhật thông tin cá nhân ===\n";
    }

    cout << "Nhập họ tên mới: "; getline(cin, newName);
    cout << "Nhập mật khẩu mới: "; getline(cin, newPass);
    cout << "Nhập email mới: "; getline(cin, newEmail);
    cout << "Nhập số điện thoại mới: "; getline(cin, newPhone);

    cout << "\n - Họ tên mới: " << newName << "\n - Mật khẩu mới: ********\n";

    string code = otp.generateOTP();

    if (isAdmin) {
        cout << "\n[OTP đã được gửi đến người dùng (" << username << ")]: <ẩn>\n";
        cout << "\nNgười dùng (" << username << ") vui lòng nhập mã OTP để xác nhận:\n> ";
    } else {
        cout << "Nhập mã OTP đã gửi: ";
    }

    string input; getline(cin, input);

    if (otp.validateOTP(input)) {
        fullName = newName;
        password = newPass;
        email = newEmail;
        phone = newPhone;
        mustChangePassword = false;
        cout << "✅ Cập nhật thông tin thành công!\n";
        backupToFile();
    } else {
        cout << "❌ Sai OTP. Hủy cập nhật.\n";
    }
}
};

class Group {
    string groupName;
    vector<User*> members;
public:
    Group(string name) : groupName(name) {}
    void addMember(User* u) { members.push_back(u); }
    void showGroupInfo() {
        cout << "\n--- Nhóm: " << groupName << " ---\n";
        for (auto m : members) m->displayUserInfo();
    }
};

class SystemManager {
    map<string, User> users;
    vector<Group> groups;
    Wallet masterWallet;
    OTP otp;

    void saveTransactionToFile(const Transaction& tx) {
        ofstream f("transactions.txt", ios::app);
        if (f.is_open()) f << tx.toString();
    }

public:
    SystemManager() : masterWallet("MASTER", 1000000) {
        users["admin"] = User("admin", "admin123", "Admin", "admin@gmail.com", "0908765432", Role::Manager);
        // Cấp điểm cho admin từ ví MASTER
        users["admin"].wallet.deposit(5000);
        Transaction tx = {"MASTER", users["admin"].wallet.getID(), 5000, "Cấp điểm cho Admin"};
        users["admin"].wallet.addTransaction(tx);
        masterWallet.addTransaction(tx);
        saveTransactionToFile(tx);

        groups.push_back(Group("Nhóm A"));
    }

   void restoreFromBackup() {
    ifstream file("backup_users.txt");
    if (!file) {
        cout << "Không tìm thấy backup.\n";
        return;
    }

    string line;
    int count = 0;

    // Xóa hết người dùng cũ (trừ admin nếu muốn giữ) và nhóm
    users.clear();
    groups.clear();

    groups.push_back(Group("Nhóm A"));  // Tạo nhóm mặc định

    while (getline(file, line)) {
        User temp;
        temp.restoreFromBackupLine(line);
        users[temp.username] = temp;
        groups[0].addMember(&users[temp.username]);  // Đưa user vào nhóm
       cout << "✅ Đã khôi phục tài khoản: " << temp.username << " | Mật khẩu: " << temp.password << "\n";

        count++;
    }

    cout << "🎉 Khôi phục thành công " << count << " tài khoản từ backup.\n";
}


    void registerUserAccount() {
        string u, p, n, e, ph;
        cout << "\n=== Đăng ký tài khoản ===\nTên đăng nhập: "; cin >> u;
        cout << "Mật khẩu (để trống để hệ thống tạo): "; cin.ignore(); getline(cin, p);
        bool autoGenerated = false;
        if (p.empty()) {
            string autoPass = "";
            string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
            for (int i = 0; i < 8; ++i)
                autoPass += chars[rand() % chars.size()];
            p = autoPass;
            autoGenerated = true;
            cout << "🔐 Mật khẩu tự sinh: " << p << "\n";
        }
        cout << "Họ tên: "; getline(cin, n);
        cout << "Email: "; getline(cin, e);
        cout << "SĐT: "; getline(cin, ph);

        users[u] = User(u, p, n, e, ph);
        users[u].wallet.deposit(1000);
        users[u].mustChangePassword = autoGenerated;

        Transaction tx = {"MASTER", users[u].wallet.getID(), 1000, "Cấp điểm ban đầu"};
        users[u].wallet.addTransaction(tx);
        masterWallet.addTransaction(tx);
        saveTransactionToFile(tx);
        groups[0].addMember(&users[u]);
        users[u].backupToFile();

        cout << "✅ Đã tạo tài khoản thành công với vai trò: Người dùng.\n";
    }

    void handleLogin() {
        string u, p;
        cout << "\n=== Đăng nhập ===\nTên đăng nhập: "; cin >> u;
        cout << "Mật khẩu: "; cin >> p;

        if (!users.count(u) || users[u].password != p) {
            cout << "❌ Sai tên đăng nhập hoặc mật khẩu.\n";
            return;
        }

        if (users[u].mustChangePassword) {
            cout << "🔐 Mật khẩu này là tạm thời. Vui lòng đổi mật khẩu mới: ";
            string newPass; cin >> newPass;
            users[u].password = newPass;
            users[u].mustChangePassword = false;
            users[u].backupToFile();
            cout << "✅ Đổi mật khẩu thành công.\n";
        }

        users[u].displayUserInfo();
        if (users[u].role == Role::Manager)
            managerMenu(u);
        else
            userMenu(u);
    }

    void userMenu(string u) {
        int ch;
        while (true) {
            cout << "\n--- Menu người dùng ---\n";
            cout << "1. Xem ví\n2. Chuyển điểm\n3. Cập nhật thông tin\n4. Sao lưu tài khoản\n5. Thoát\nChọn: ";
            cin >> ch;
            if (ch == 5) break;
            if (ch == 1) {
                users[u].wallet.showBalance();
                users[u].wallet.showHistory();
            } else if (ch == 2) {
                string to; int amt;
                cout << "Chuyển đến (username): "; cin >> to;
                if (!users.count(to)) { cout << "❌ Không tìm thấy người nhận.\n"; continue; }
                cout << "Số điểm: "; cin >> amt;
                string code = otp.generateOTP();
                cout << "Nhập mã OTP: "; string in; cin >> in;
                if (!otp.validateOTP(in)) { cout << "❌ Sai OTP.\n"; continue; }
                if (users[u].wallet.withdraw(amt)) {
                    users[to].wallet.deposit(amt);
                    Transaction tx = {users[u].wallet.getID(), users[to].wallet.getID(), amt, "Thành công"};
                    users[u].wallet.addTransaction(tx);
                    users[to].wallet.addTransaction(tx);
                    saveTransactionToFile(tx);
                    cout << "✅ Giao dịch thành công.\n";
                } else cout << "❌ Không đủ số dư.\n";
            } else if (ch == 3) {
                users[u].updateUserInfo(otp);
            } else if (ch == 4) {
                users[u].backupToFile();
                cout << "✅ Đã sao lưu tài khoản.\n";
            }
        }
    }

    void managerMenu(string u) {
        int ch;
        while (true) {
            cout << "\n--- Menu quản lý ---\n";
            cout << "1. Tạo tài khoản\n2. Xem nhóm\n3. Xem toàn bộ giao dịch\n4. Cập nhật người dùng khác\n";
            cout << "5. Xem ví của chính mình\n6. Chuyển điểm cho người dùng\n7. Thoát\nChọn: ";
            cin >> ch;
            if (ch == 7) break;
            if (ch == 1) registerUserAccount();
            else if (ch == 2) for (auto& g : groups) g.showGroupInfo();
            else if (ch == 3) {
                ifstream f("transactions.txt"); string line;
                while (getline(f, line)) cout << line << "\n";
            } else if (ch == 4) {
                string target; cout << "Nhập username: "; cin >> target;
                if (!users.count(target) || target == "admin") {
                    cout << "❌ Không thể cập nhật người này.\n"; continue;
                }
                users[target].updateUserInfo(otp, true);
            } else if (ch == 5) {
                users[u].wallet.showBalance();
                users[u].wallet.showHistory();
            } else if (ch == 6) {
                string target; int amt;
                cout << "Người nhận: "; cin >> target;

                if (!users.count(target)) {
                    cout << "❌ Không tìm thấy người dùng.\n";
                    continue;
                }

                if (target == u) {
                    cout << "❌ Không thể chuyển cho chính mình.\n";
                    continue;
                }

                cout << "Số điểm: "; cin >> amt;

                string code = otp.generateOTP();
                cout << "Nhập mã OTP: "; string in; cin >> in;

                if (!otp.validateOTP(in)) {
                    cout << "❌ Sai OTP. Hủy giao dịch.\n";
                    continue;
                }

                if (users[u].wallet.withdraw(amt)) {
                    users[target].wallet.deposit(amt);
                    Transaction tx = {users[u].wallet.getID(), users[target].wallet.getID(), amt, "Admin chuyển điểm"};
                    users[u].wallet.addTransaction(tx);
                    users[target].wallet.addTransaction(tx);
                    saveTransactionToFile(tx);
                    cout << "✅ Giao dịch thành công.\n";
                } else {
                    cout << "❌ Số dư admin không đủ.\n";
                }
            }
        }
    }

    void mainMenu() {
        int ch;
        while (true) {
            cout << "\n=== MENU CHÍNH ===\n";
            cout << "1. Đăng nhập\n2. Đăng ký tài khoản\n3. Khôi phục từ backup\n4. Thoát\nChọn: ";
            cin >> ch;
            if (ch == 4) break;
            if (ch == 1) handleLogin();
            else if (ch == 2) registerUserAccount();
            else if (ch == 3) restoreFromBackup();
        }
    }
};

int main() {
    setlocale(LC_ALL, "");
    SystemManager sys;
    sys.mainMenu();
    return 0;
}
