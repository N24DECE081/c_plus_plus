#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
using namespace std;

extern class OTP;
extern class Wallet;

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

    void updateUserInfo(OTP& otp, bool isAdmin = false);
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
