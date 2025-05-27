#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <map>
#include <vector>
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

int main() {
    setlocale(LC_ALL, "");
    SystemManager sys;
    sys.mainMenu();
    return 0;
}
