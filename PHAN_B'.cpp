#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>

using namespace std;

// ----------------- ENUM Role -------------------
enum class Role { User, Manager };

// ----------------- CLASS OTP -------------------
class OTP {
private:
    string currentOTP;
public:
    OTP() {
        srand(static_cast<unsigned>(time(nullptr)));
    }

    string generateOTP() {
        currentOTP.clear();
        for (int i = 0; i < 6; ++i) {
            currentOTP += '0' + rand() % 10;
        }
        return currentOTP;
    }

    bool validateOTP(const string& input) {
        return input == currentOTP;
    }
};

// ----------------- CLASS Group -------------------
class Group {
private:
    string groupName;
    vector<string> members;

public:
    Group(const string& name) : groupName(name) {}

    void addMember(const string& username) {
        members.push_back(username);
    }

    string getGroupName() const { return groupName; }

    void showGroupInfo() const {
        cout << "\n--- Nhóm: " << groupName << " ---\n";
        if (members.empty()) {
            cout << "Chưa có thành viên.\n";
        } else {
            cout << "Thành viên:\n";
            for (const auto& member : members)
                cout << "- " << member << "\n";
        }
    }
};

// ----------------- CLASS UserAccount -------------------
class UserAccount {
private:
    string username;
    string password;
    string name;
    Role role;

public:
    UserAccount(const string& user, const string& pass, const string& name, Role role)
        : username(user), password(pass), name(name), role(role) {}

    string getUsername() const { return username; }
    Role getRole() const { return role; }

    void showPersonalInfo() const {
        cout << "\n--- Thông tin cá nhân ---\n";
        cout << "Tên đăng nhập: " << username << "\n";
        cout << "Họ tên: " << name << "\n";
        cout << "Loại người dùng: " << (role == Role::Manager ? "Quản lý" : "Người dùng thường") << "\n";
    }

    void requestUpdateInfo(bool isManager, OTP& otpSystem) {
        string newName, newPassword;

        cout << "\n--- Cập nhật thông tin ---\n";
        cout << "Nhập tên mới: ";
        getline(cin, newName);
        cout << "Nhập mật khẩu mới: ";
        getline(cin, newPassword);

        cout << "\n>> Hệ thống sẽ cập nhật:\n";
        cout << "Họ tên mới: " << newName << "\n";
        cout << "Mật khẩu mới: ********\n";

        string otp = otpSystem.generateOTP();
        cout << "[OTP gửi đến người dùng]: " << otp << "\n";

        string userInput;
        cout << "Nhập OTP để xác nhận: ";
        getline(cin, userInput);

        if (otpSystem.validateOTP(userInput)) {
            name = newName;
            password = newPassword;
            cout << "✅ Cập nhật thành công.\n";
        } else {
            cout << "❌ OTP không hợp lệ. Hủy cập nhật.\n";
        }
    }

    static UserAccount createNewAccount() {
        string user, pass, name;
        cout << "\n--- Tạo tài khoản mới ---\n";
        cout << "Tên đăng nhập: ";
        getline(cin, user);
        cout << "Mật khẩu: ";
        getline(cin, pass);
        cout << "Họ tên: ";
        getline(cin, name);

        return UserAccount(user, pass, name, Role::User);
    }
};

// ----------------- HÀM CHÍNH MAIN -------------------
int main() {
    vector<UserAccount> users;
    vector<Group> groups;
    OTP otpSystem;

    // Tạo tài khoản quản lý mặc định
    UserAccount manager("admin", "admin123", "Nguyễn Quản Lý", Role::Manager);
    users.push_back(manager);

    // Tạo nhóm mặc định
    Group groupA("Nhóm A");
    groups.push_back(groupA);

    while (true) {
        cout << "\n========== MENU CHÍNH ==========\n";
        cout << "1. Đăng nhập\n2. Thoát\nChọn: ";
        int choice;
        cin >> choice;
        cin.ignore();

        if (choice == 2) break;

        string username, password;
        cout << "\nTên đăng nhập: ";
        getline(cin, username);
        cout << "Mật khẩu: ";
        getline(cin, password);

        bool found = false;
        for (auto& user : users) {
            if (user.getUsername() == username && password == "admin123") {
                found = true;
                cout << "\n✅ Đăng nhập thành công!\n";
                user.showPersonalInfo();

                if (user.getRole() == Role::User) {
                    cout << "\n1. Cập nhật thông tin cá nhân\n2. Thoát\nChọn: ";
                    int opt;
                    cin >> opt;
                    cin.ignore();

                    if (opt == 1) {
                        user.requestUpdateInfo(false, otpSystem);
                    }
                } else if (user.getRole() == Role::Manager) {
                    cout << "\n1. Tạo tài khoản mới\n2. Cập nhật thông tin người dùng khác\n3. Xem danh sách nhóm\n4. Thoát\nChọn: ";
                    int opt;
                    cin >> opt;
                    cin.ignore();

                    if (opt == 1) {
                        UserAccount newAcc = UserAccount::createNewAccount();
                        users.push_back(newAcc);

                        // Gán tài khoản vào nhóm đầu tiên (ví dụ đơn giản)
                        groups[0].addMember(newAcc.getUsername());

                        cout << "✅ Tạo tài khoản thành công.\n";
                    } else if (opt == 2) {
                        string targetUsername;
                        cout << "Nhập tên đăng nhập người cần cập nhật: ";
                        getline(cin, targetUsername);

                        bool userFound = false;
                        for (auto& u : users) {
                            if (u.getUsername() == targetUsername && u.getUsername() != "admin") {
                                userFound = true;
                                u.requestUpdateInfo(true, otpSystem);
                                break;
                            }
                        }

                        if (!userFound)
                            cout << "❌ Không tìm thấy người dùng.\n";
                    } else if (opt == 3) {
                        cout << "\n========== DANH SÁCH NHÓM ==========\n";
                        for (const auto& group : groups) {
                            group.showGroupInfo();
                        }
                    }
                }

                break;
            }
        }

        if (!found) {
            cout << "❌ Sai tên đăng nhập hoặc mật khẩu.\n";
        }
    }

    return 0;
}
