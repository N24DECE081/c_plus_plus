#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <map>
#include <vector>
#include <clocale>
using namespace std;
// ===PHÂN CHIA NGƯỜI DÙNG ===
enum class Role { User, Manager };

// ===OTP===
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

// ===KHÔI PHỤC DỮ LIỆU TỪ FILE BACKUP===
void restoreFromBackup() {
    ifstream file("backup_users.txt");
    if (!file) {
        cout << "Không tìm thấy backup.\n";
        return;
    }

    string line;
    int count = 0;
    string adminPass = "admin123";
    User adminUser;
    if (users.count("admin")) {
        adminUser = users["admin"];
    } else {
        adminUser = User("admin", adminPass, "Admin", "admin@gmail.com", "0908765432", Role::Manager);  //Khoi tao tk admin ban dau
        adminUser.wallet.deposit(5000);
        Transaction tx = {"MASTER", adminUser.wallet.getID(), 5000, "Cấp điểm cho Admin"};
        adminUser.wallet.addTransaction(tx);
        masterWallet.addTransaction(tx);
        saveTransactionToFile(tx);
    }

    users.clear();
    groups.clear();
    groups.push_back(Group("Nhóm A"));
    while (getline(file, line)) {
        User temp;
        temp.restoreFromBackupLine(line);
        users[temp.username] = temp;
        groups[0].addMember(&users[temp.username]);
        cout << "✅ Đã khôi phục tài khoản: " << temp.username << " | Mật khẩu: " << temp.password << "\n";
        count++;
    }
    users["admin"] = adminUser;
    groups[0].addMember(&users["admin"]);

    cout << "🎉 Khôi phục thành công " << count << " tài khoản từ backup.\n";
}

int main() {
    setlocale(LC_ALL, "");
    SystemManager sys;
    sys.mainMenu();
    return 0;
}
