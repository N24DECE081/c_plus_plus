#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <fstream>

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

// ----------------- CLASS UserAccount -------------------
class UserAccount {
private:
    string username;
    string password;
    string name;
    string email;
    string phone;
    Role role;

public:
    UserAccount(const string& user, const string& pass, const string& name, const string& email, const string& phone, Role role)
        : username(user), password(pass), name(name), email(email), phone(phone), role(role) {}

    string getUsername() const { return username; }
    string getPassword() const { return password; }
    string getName() const { return name; }
    string getEmail() const { return email; }
    string getPhone() const { return phone; }
    Role getRole() const { return role; }

    void showPersonalInfo() const {
        cout << "\n--- Thông tin cá nhân ---\n";
        cout << "Tên đăng nhập: " << username << "\n";
        cout << "Họ tên: " << name << "\n";
        cout << "Email: " << email << "\n";
        cout << "SĐT: " << phone << "\n";
        cout << "Loại người dùng: " << (role == Role::Manager ? "Quản lý" : "Người dùng thường") << "\n";
    }

    void requestUpdateInfo(bool isManager, OTP& otpSystem) {
        string newName, newPassword, newEmail, newPhone;

        cout << "\n=== " << (isManager ? "Quản trị viên" : "Người dùng") << " cập nhật tài khoản ===\n";
        cout << "Tên đăng nhập: " << username << "\n";
        cout << "Nhập họ tên mới: "; getline(cin, newName);
        cout << "Nhập mật khẩu mới: "; getline(cin, newPassword);
        cout << "Nhập email mới: "; getline(cin, newEmail);
        cout << "Nhập số điện thoại mới: "; getline(cin, newPhone);

        cout << "\n>> " << (isManager ? "Admin muốn cập nhật tài khoản người dùng (" + username + "):" : "Bạn muốn cập nhật thông tin:") << "\n";
        cout << "- Họ tên mới: " << newName << "\n";
        cout << "- Mật khẩu mới: ********\n";
        cout << "- Email mới: " << newEmail << "\n";
        cout << "- SĐT mới: " << newPhone << "\n";

        string otp = otpSystem.generateOTP();
        cout << "\n[OTP đã được gửi đến người dùng (" << email << ")]\n";
        cout << "[Mã OTP là: " << otp << "]\n";  // Hiển thị OTP ra màn hình
        cout << username << " vui lòng nhập mã OTP để xác nhận:\n> ";

        string userInput;
        getline(cin, userInput);

        if (otpSystem.validateOTP(userInput)) {
            name = newName;
            password = newPassword;
            email = newEmail;
            phone = newPhone;
            cout << "\u2705 Thông tin đã được cập nhật bởi " << (isManager ? "Admin." : "bạn.") << "\n";
        }
        else {
            cout << "\u274c OTP không hợp lệ. Hủy cập nhật.\n";
        }
    }

    static UserAccount createNewAccount() {
        string user, pass, name, email, phone;
        cout << "\n--- Tạo tài khoản mới ---\n";
        cout << "Tên đăng nhập: "; getline(cin, user);
        cout << "Mật khẩu: "; getline(cin, pass);
        cout << "Họ tên: "; getline(cin, name);
        cout << "Email: "; getline(cin, email);
        cout << "Số điện thoại: "; getline(cin, phone);

        return UserAccount(user, pass, name, email, phone, Role::User);
    }
};

// ----------------- CLASS Group -------------------
class Group {
private:
    string groupName;
    vector<UserAccount*> members;

public:
    Group(const string& name) : groupName(name) {}

    void addMember(UserAccount* user) {
        members.push_back(user);
    }

    string getGroupName() const {
        return groupName;
    }

    void showGroupInfo() const {
        cout << "\n--- Nhóm: " << groupName << " ---\n";
        if (members.empty()) {
            cout << "Chưa có thành viên.\n";
        }
        else {
            cout << "Thành viên:\n";
            for (const auto& member : members) {
                cout << "- Tên đăng nhập: " << member->getUsername() << "\n";
                cout << "  Họ tên: " << member->getName() << "\n";
                cout << "  Email: " << member->getEmail() << "\n";
                cout << "  SĐT: " << member->getPhone() << "\n\n";
            }
        }
    }

    void saveGroupInfoToFile(const string& filename) const {
        ofstream out(filename);
        if (!out) {
            cerr << "\u274c Không thể mở file để ghi.\n";
            return;
        }

        out << "--- Nhóm: " << groupName << " ---\n";
        if (members.empty()) {
            out << "Chưa có thành viên.\n";
        }
        else {
            out << "Thành viên:\n";
            for (const auto& member : members) {
                out << "- Tên đăng nhập: " << member->getUsername() << "\n";
                out << "  Họ tên: " << member->getName() << "\n";
                out << "  Email: " << member->getEmail() << "\n";
                out << "  SĐT: " << member->getPhone() << "\n\n";
            }
        }

        out.close();
        cout << "\u2705 Đã lưu thông tin nhóm vào file \"" << filename << "\"\n";
    }
};

// ----------------- HÀM CHÍNH MAIN -------------------
int main() {
    vector<UserAccount*> users;
    vector<Group> groups;
    OTP otpSystem;

    users.push_back(new UserAccount("admin", "admin123", "Nguyễn Quản Lý", "admin@gmail.com", "0000000000", Role::Manager));
    Group groupA("Nhóm A");
    groups.push_back(groupA);

    while (true) {
        cout << "\n========== MENU CHÍNH ==========\n";
        cout << "1. Đăng nhập\n";
        cout << "2. Tạo tài khoản\n";
        cout << "3. Thoát\nChọn: ";
        int choice;
        cin >> choice;
        cin.ignore();

        if (choice == 3) break;

        if (choice == 1) {
            string username, password;
            cout << "\nTên đăng nhập: "; getline(cin, username);
            cout << "Mật khẩu: "; getline(cin, password);

            bool found = false;
            for (auto user : users) {
                if (user->getUsername() == username && password == user->getPassword()) {
                    found = true;
                    cout << "\n\u2705 Đăng nhập thành công!\n";
                    user->showPersonalInfo();

                    if (user->getRole() == Role::User) {
                        while (true) {
                            cout << "\n1. Cập nhật thông tin cá nhân\n2. Thoát\nChọn: ";
                            int opt;
                            cin >> opt;
                            cin.ignore();
                            if (opt == 1) {
                                user->requestUpdateInfo(false, otpSystem);
                            } else break;
                        }
                    } else if (user->getRole() == Role::Manager) {
                        while (true) {
                            cout << "\n1. Tạo tài khoản mới\n2. Cập nhật thông tin người dùng khác\n3. Xem danh sách nhóm\n4. Thoát\nChọn: ";
                            int opt;
                            cin >> opt;
                            cin.ignore();

                            if (opt == 1) {
                                UserAccount* newAcc = new UserAccount(UserAccount::createNewAccount());
                                users.push_back(newAcc);
                                groups[0].addMember(newAcc);
                                cout << "\u2705 Tạo tài khoản thành công.\n";
                            }
                            else if (opt == 2) {
                                string targetUsername;
                                cout << "Nhập tên đăng nhập người cần cập nhật: ";
                                getline(cin, targetUsername);

                                bool userFound = false;
                                for (auto u : users) {
                                    if (u->getUsername() == targetUsername && u->getUsername() != "admin") {
                                        userFound = true;
                                        u->requestUpdateInfo(true, otpSystem);
                                        break;
                                    }
                                }
                                if (!userFound)
                                    cout << "\u274c Không tìm thấy người dùng.\n";
                            }
                            else if (opt == 3) {
                                cout << "\n========== DANH SÁCH NHÓM ==========\n";
                                for (const auto& group : groups) {
                                    group.showGroupInfo();
                                    group.saveGroupInfoToFile(group.getGroupName() + ".txt");
                                }
                            }
                            else if (opt == 4) break;
                            else cout << "Lựa chọn không hợp lệ.\n";
                        }
                    }
                    break;
                }
            }

            if (!found) {
                cout << "\u274c Sai tên đăng nhập hoặc mật khẩu.\n";
            }
        }
        else if (choice == 2) {
            cout << "\n--- TẠO TÀI KHOẢN MỚI ---\n";
            string roleChoice;
            while (true) {
                cout << "Loại tài khoản (user/admin): ";
                getline(cin, roleChoice);
                if (roleChoice == "user" || roleChoice == "admin") break;
                cout << "Lựa chọn không hợp lệ. Vui lòng nhập lại.\n";
            }

            Role newRole = (roleChoice == "admin") ? Role::Manager : Role::User;

            string user, pass, name, email, phone;
            cout << "Tên đăng nhập: "; getline(cin, user);
            cout << "Mật khẩu: "; getline(cin, pass);
            cout << "Họ tên: "; getline(cin, name);
            cout << "Email: "; getline(cin, email);
            cout << "Số điện thoại: "; getline(cin, phone);

            UserAccount* newAcc = new UserAccount(user, pass, name, email, phone, newRole);
            users.push_back(newAcc);
            if (newRole == Role::User) {
    groups[0].addMember(newAcc);  // chỉ thêm nếu là người dùng thường
}

            cout << "\u2705 Tạo tài khoản " << (newRole == Role::Manager ? "quản lý" : "người dùng") << " thành công.\n";
        }
        else {
            cout << "\u274c Lựa chọn không hợp lệ.\n";
        }
    }

    for (auto user : users) {
        delete user;
    }

    return 0;
}
