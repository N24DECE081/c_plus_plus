#include <iostream>
#include <fstream>
#include <string>
using namespace std;

extern map<string, User> users;
extern vector<Group> groups;
extern OTP otp;
extern TransactionManager tm;

class SystemManager {
public:
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
                if (amt <= 0) {
                    cout << "❌ Số điểm phải lớn hơn 0.\n";
                    continue;
                }

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
