
# Hệ Thống Đăng Nhập và Quản Lý Ví Điểm Thưởng

## Giới thiệu dự án

Đây là đồ án môn học **Ngôn ngữ lập trình C++**, với mục tiêu xây dựng một hệ thống dòng lệnh có khả năng:
- Quản lý tài khoản người dùng (đăng ký, đăng nhập, thay đổi thông tin).
- Xác thực bảo mật bằng OTP.
- Quản lý ví điểm và giao dịch chuyển điểm giữa các tài khoản.
- Lưu trữ và sao lưu dữ liệu bằng file.

---

## Thành viên nhóm & vai trò

| Họ và Tên               | Nhiệm vụ                                       | Công việc chi tiết                                                   |
|-------------------------|------------------------------------------------|------------------------------------------------------------------------|
| Lại Thu Hiền            | Thực hiện phần A                               | Khởi tạo hệ thống, OTP, hàm main, khôi phục dữ liệu                  |
| La Vĩnh Tiến            | Thực hiện phần B                               | Class User, Group, cập nhật thông tin, backup                        |
| Nguyễn Thanh Tuyền      | Thực hiện phần C                               | Wallet, giao dịch, lịch sử, master wallet                            |
| Nguyễn Dương Thanh Mai  | Thực hiện phần D                               | Giao diện admin, chuyển điểm, thống kê                               |

---

## Phân tích & đặc tả chức năng

### A. Khởi tạo hệ thống & OTP
- Tạo class OTP sinh mã xác thực gồm 6 chữ số.
- Khởi tạo hệ thống và gọi menu chính thông qua SystemManager.
- OTP dùng cho các thao tác quan trọng như cập nhật và giao dịch.

### B. Quản lý người dùng & nhóm
- Class User quản lý thông tin người dùng: tên, email, mật khẩu (băm), v.v.
- Class Group quản lý nhóm người dùng.
- Cho phép cập nhật thông tin cá nhân hoặc cập nhật hộ (yêu cầu OTP).
- Lưu backup người dùng vào file backup_users.txt.

### C. Ví và giao dịch
- Class Wallet lưu số dư và lịch sử giao dịch.
- Chuyển điểm giữa người dùng → OTP xác nhận.
- Lưu thông tin giao dịch vào transactions.txt.
- Hệ thống có ví tổng (MASTER_WALLET) dùng để cấp điểm cho ví mới.

### D. Giao diện quản lý
- Tạo tài khoản mới cho người dùng.
- Cập nhật tài khoản khác (cần xác thực OTP).
- Xem thông tin nhóm, lịch sử giao dịch toàn hệ thống.
- Cho phép quản trị viên chuyển điểm hộ cho người dùng.

---

## Lưu trữ & sao lưu

- Dữ liệu người dùng lưu vào: users.txt
- Dữ liệu giao dịch: transactions.txt
- Sao lưu dữ liệu người dùng: backup_users.txt
- Mỗi bản ghi lưu trên một dòng, dùng ký tự | để phân cách.

---

## Chú thích thư viện

| Thư viện              | Mục đích sử dụng                                 |
|------------------------|--------------------------------------------------|
| `#include <iostream>`  | Nhập xuất cơ bản                                 |
| `#include <fstream>`   | Đọc/ghi file                                     |
| `#include <string>`    | Xử lý chuỗi                                      |
| `#include <vector>`    | Lưu danh sách người dùng, giao dịch              |
| `#include <ctime>`     | Sinh OTP, thời gian                              |
| `#include <cstdlib>`   | rand(), srand(), sinh mật khẩu tự động          |
| `#include <sstream>`   | Đọc file và tách chuỗi                           |
| `#include <map>`       | Lưu danh sách người dùng theo username           |
| `#include <clocale>`   | Hỗ trợ hiển thị Unicode, tiếng Việt              |


---

## Hướng dẫn biên dịch & chạy chương trình

Yêu cầu:
- Trình biên dịch hỗ trợ C++11 trở lên

Biên dịch:

```bash
g++ system_main_otp.cpp -o system
g++ user_group.cpp -o usergroup
g++ transactions_masterwallet.cpp -o wallet
g++ admin_interface.cpp -o admin
```

Chạy chương trình:

```bash
./system  # Gọi menu chính, sử dụng đầy đủ chức năng
```

---

## Kiểm thử chức năng

| Chức năng               | Input                           | Output mong đợi                         |
|-------------------------|----------------------------------|------------------------------------------|
| Đăng ký                 | username, mật khẩu               | ✅ Tạo thành công                        |
| Đăng nhập               | username + mật khẩu              | ✅/❌ tùy đúng sai                        |
| Đổi mật khẩu            | mật khẩu cũ + mới                | ✅ Thành công                            |
| Cập nhật thông tin      | tên/email/sđt mới + OTP          | ✅ Hoặc ❌ nếu sai OTP                    |
| Sao lưu người dùng      | -                                | ✅ Ghi ra file backup_users.txt          |
| Giao dịch chuyển điểm   | người nhận + số điểm + OTP       | ✅ Giao dịch thành công hoặc lỗi         |
| Xem lịch sử             | -                                | Hiển thị danh sách giao dịch hoặc ví     |
| Admin chuyển điểm       | tài khoản nhận + điểm + OTP      | ✅ Thành công / lỗi nếu sai OTP          |

---

## Bảo mật

- Mật khẩu mã hóa bằng DJB2 hash function.
- OTP gồm 6 chữ số, hiển thị giả lập, chỉ dùng 1 lần.
- OTP áp dụng cho:
  - Cập nhật thông tin người dùng
  - Giao dịch chuyển điểm
  - Thao tác cập nhật hộ
## 💻 Minh họa thực thi chương trình (terminal)

## 👤 Người dùng

```text
=== MENU CHÍNH ===
1. Đăng nhập
2. Đăng ký tài khoản
3. Khôi phục từ backup
4. Thoát
Chọn: 2

=== Đăng ký tài khoản ===
Tên đăng nhập: bot1
Mật khẩu (để trống để hệ thống tạo): 
🔐 Mật khẩu tự sinh: ribufGDi
Họ tên: bot1
Email: bot1@gmail.com
SĐT: 111
✅ Đã tạo tài khoản thành công với vai trò: Người dùng.

=== MENU CHÍNH ===
1. Đăng nhập
2. Đăng ký tài khoản
3. Khôi phục từ backup
4. Thoát
Chọn: 1

=== Đăng nhập ===
Tên đăng nhập: bot1
Mật khẩu: ribufGDi
🔐 Mật khẩu này là tạm thời. Vui lòng đổi mật khẩu mới: bot1
✅ Đổi mật khẩu thành công.

--- Thông tin người dùng ---
Tên đăng nhập: bot1
Họ tên: bot1
Email: bot1@gmail.com
SĐT: 111
Vai trò: Người dùng

--- Menu người dùng ---
1. Xem ví
2. Chuyển điểm
3. Cập nhật thông tin
4. Sao lưu tài khoản
5. Thoát
Chọn: 1
Ví: WALLET_bot1 | Số dư: 1000 điểm

--- Lịch sử ví WALLET_bot1 ---
From: MASTER -> To: WALLET_bot1 | Amount: 1000 | Status: Cấp điểm ban đầu

--- Menu người dùng ---
1. Xem ví
2. Chuyển điểm
3. Cập nhật thông tin
4. Sao lưu tài khoản
5. Thoát
Chọn: 2
Chuyển đến (username): bot2
❌ Không tìm thấy người nhận.

--- Menu người dùng ---
1. Xem ví
2. Chuyển điểm
3. Cập nhật thông tin
4. Sao lưu tài khoản
5. Thoát
Chọn: 3

=== Cập nhật thông tin cá nhân ===
Nhập họ tên mới: bot11
Nhập mật khẩu mới: bot11
Nhập email mới: bot11@gmail.com
Nhập số điện thoại mới: 1111

 - Họ tên mới: bot11
 - Mật khẩu mới: ********
[OTP (giả lập) được gửi: 198603]
Nhập mã OTP đã gửi: 198603
✅ Cập nhật thông tin thành công!

--- Menu người dùng ---
1. Xem ví
2. Chuyển điểm
3. Cập nhật thông tin
4. Sao lưu tài khoản
5. Thoát
Chọn: 4
✅ Đã sao lưu tài khoản.

--- Menu người dùng ---
1. Xem ví
2. Chuyển điểm
3. Cập nhật thông tin
4. Sao lưu tài khoản
5. Thoát
Chọn: 5

=== MENU CHÍNH ===
1. Đăng nhập
2. Đăng ký tài khoản
3. Khôi phục từ backup
4. Thoát
Chọn: 2

=== Đăng ký tài khoản ===
Tên đăng nhập: bot2
Mật khẩu (để trống để hệ thống tạo): bot2
Họ tên: bot2
Email: bot2@gmail.com
SĐT: 222
✅ Đã tạo tài khoản thành công với vai trò: Người dùng.

=== MENU CHÍNH ===
1. Đăng nhập
2. Đăng ký tài khoản
3. Khôi phục từ backup
4. Thoát
Chọn: 1

=== Đăng nhập ===
Tên đăng nhập: bot1
Mật khẩu: bot11

--- Thông tin người dùng ---
Tên đăng nhập: bot1
Họ tên: bot11
Email: bot11@gmail.com
SĐT: 1111
Vai trò: Người dùng

--- Menu người dùng ---
1. Xem ví
2. Chuyển điểm
3. Cập nhật thông tin
4. Sao lưu tài khoản
5. Thoát
Chọn: 2
Chuyển đến (username): bot2
Số điểm: 99
[OTP (giả lập) được gửi: 680582]
Nhập mã OTP: 680582
✅ Giao dịch thành công.

--- Menu người dùng ---
1. Xem ví
2. Chuyển điểm
3. Cập nhật thông tin
4. Sao lưu tài khoản
5. Thoát
Chọn: 2
Chuyển đến (username): bot2
Số điểm: 1
[OTP (giả lập) được gửi: 26844]
Nhập mã OTP: 26855
❌ Sai OTP.
```

## 🧑‍💼 Quản lý / Admin

```text
=== MENU CHÍNH ===
1. Đăng nhập
2. Đăng ký tài khoản
3. Khôi phục từ backup
4. Thoát
Chọn: 1

=== Đăng nhập ===
Tên đăng nhập: admin
Mật khẩu: admin123

--- Thông tin người dùng ---
Tên đăng nhập: admin
Họ tên: Admin
Email: admin@gmail.com
SĐT: 0908765432
Vai trò: Quản lý

--- Menu quản lý ---
1. Tạo tài khoản
2. Xem nhóm
3. Xem toàn bộ giao dịch
4. Cập nhật người dùng khác
5. Xem ví của chính mình
6. Chuyển điểm cho người dùng
7. Thoát
Chọn: 1

=== Đăng ký tài khoản ===
Tên đăng nhập: bot3
Mật khẩu (để trống để hệ thống tạo): 
🔐 Mật khẩu tự sinh: E109k2mp
Họ tên: bot3
Email: bot3@gmail.com
SĐT: 3333
✅ Đã tạo tài khoản thành công với vai trò: Người dùng.

--- Menu quản lý ---
1. Tạo tài khoản
2. Xem nhóm
3. Xem toàn bộ giao dịch
4. Cập nhật người dùng khác
5. Xem ví của chính mình
6. Chuyển điểm cho người dùng
7. Thoát
Chọn: 7

=== MENU CHÍNH ===
1. Đăng nhập
2. Đăng ký tài khoản
3. Khôi phục từ backup
4. Thoát
Chọn: 1

=== Đăng nhập ===
Tên đăng nhập: bot3
Mật khẩu: E109k2mp
🔐 Mật khẩu này là tạm thời. Vui lòng đổi mật khẩu mới: bot3
✅ Đổi mật khẩu thành công.

--- Thông tin người dùng ---
Tên đăng nhập: bot3
Họ tên: bot3
Email: bot3@gmail.com
SĐT: 3333
Vai trò: Người dùng

--- Menu người dùng ---
1. Xem ví
2. Chuyển điểm
3. Cập nhật thông tin
4. Sao lưu tài khoản
5. Thoát
Chọn: 5

=== MENU CHÍNH ===
1. Đăng nhập
2. Đăng ký tài khoản
3. Khôi phục từ backup
4. Thoát
Chọn: 1

=== Đăng nhập ===
Tên đăng nhập: admin
Mật khẩu: admin123

--- Thông tin người dùng ---
Tên đăng nhập: admin
Họ tên: Admin
Email: admin@gmail.com
SĐT: 0908765432
Vai trò: Quản lý

--- Menu quản lý ---
1. Tạo tài khoản
2. Xem nhóm
3. Xem toàn bộ giao dịch
4. Cập nhật người dùng khác
5. Xem ví của chính mình
6. Chuyển điểm cho người dùng
7. Thoát
Chọn: 2

--- Nhóm: Nhóm A ---

--- Thông tin người dùng ---
Tên đăng nhập: bot11
Họ tên: bot11
Email: bot11@gmail.com
SĐT: 1111
Vai trò: Người dùng

--- Thông tin người dùng ---
Tên đăng nhập: bot2
Họ tên: bot2
Email: bot2@gmail.com
SĐT: 2222
Vai trò: Người dùng

--- Thông tin người dùng ---
Tên đăng nhập: admin
Họ tên: Admin
Email: admin@gmail.com
SĐT: 0908765432
Vai trò: Quản lý

--- Thông tin người dùng ---
Tên đăng nhập: bot3
Họ tên: bot3
Email: bot3@gmail.com
SĐT: 3333
Vai trò: Người dùng

--- Menu quản lý ---
1. Tạo tài khoản
2. Xem nhóm
3. Xem toàn bộ giao dịch
4. Cập nhật người dùng khác
5. Xem ví của chính mình
6. Chuyển điểm cho người dùng
7. Thoát
Chọn: 3
From: MASTER -> To: WALLET_admin | Amount: 5000 | Status: Cấp điểm cho Admin
From: WALLET_bot11 -> To: WALLET_bot2 | Amount: 99 | Status: Thành công

--- Menu quản lý ---
1. Tạo tài khoản
2. Xem nhóm
3. Xem toàn bộ giao dịch
4. Cập nhật người dùng khác
5. Xem ví của chính mình
6. Chuyển điểm cho người dùng
7. Thoát
Chọn: 4
Nhập username: bot3

Admin muốn cập nhật tài khoản người dùng (bot3):
Nhập họ tên mới: bot33
Nhập mật khẩu mới: bot33
Nhập email mới: bot33@gmail.com
Nhập số điện thoại mới: 333  

 - Họ tên mới: bot33
 - Mật khẩu mới: ********
[OTP (giả lập) được gửi: 464035]

[OTP đã được gửi đến người dùng (bot3)]: <ẩn>

Người dùng (bot3) vui lòng nhập mã OTP để xác nhận:
> 464035
✅ Cập nhật thông tin thành công!

--- Menu quản lý ---
1. Tạo tài khoản
2. Xem nhóm
3. Xem toàn bộ giao dịch
4. Cập nhật người dùng khác
5. Xem ví của chính mình
6. Chuyển điểm cho người dùng
7. Thoát
Chọn: 6
Người nhận: bot3
Số điểm: 10
[OTP (giả lập) được gửi: 206777]
Nhập mã OTP: 206777
✅ Giao dịch thành công.

--- Menu quản lý ---
1. Tạo tài khoản
2. Xem nhóm
3. Xem toàn bộ giao dịch
4. Cập nhật người dùng khác
5. Xem ví của chính mình
6. Chuyển điểm cho người dùng
7. Thoát
Chọn: 5
Ví: WALLET_admin | Số dư: 4990 điểm

--- Lịch sử ví WALLET_admin ---
From: MASTER -> To: WALLET_admin | Amount: 5000 | Status: Cấp điểm cho Admin
From: WALLET_admin -> To: WALLET_bot3 | Amount: 10 | Status: Admin chuyển điểm
---
--- Menu quản lý ---
1. Tạo tài khoản
2. Xem nhóm
3. Xem toàn bộ giao dịch
4. Cập nhật người dùng khác
5. Xem ví của chính mình
6. Chuyển điểm cho người dùng
7. Thoát
Chọn: 4
Nhập username: bot2

Admin muốn cập nhật tài khoản người dùng (bot2):
Nhập họ tên mới: bot22
Nhập mật khẩu mới: bot22
Nhập email mới: bot22@gmail.com
Nhập số điện thoại mới: 22 

 - Họ tên mới: bot22
 - Mật khẩu mới: ********
[OTP (giả lập) được gửi: 111994]

[OTP đã được gửi đến người dùng (bot2)]: <ẩn>

Người dùng (bot2) vui lòng nhập mã OTP để xác nhận:
> 123234
❌ Sai OTP. Hủy cập nhật.

--- Menu quản lý ---
1. Tạo tài khoản
2. Xem nhóm
3. Xem toàn bộ giao dịch
4. Cập nhật người dùng khác
5. Xem ví của chính mình
6. Chuyển điểm cho người dùng
7. Thoát
Chọn: 6
Người nhận: admin
❌ Không thể chuyển cho chính mình.

--- Menu quản lý ---
1. Tạo tài khoản
2. Xem nhóm
3. Xem toàn bộ giao dịch
4. Cập nhật người dùng khác
5. Xem ví của chính mình
6. Chuyển điểm cho người dùng
7. Thoát
Chọn: 6
Người nhận: bob2
❌ Không tìm thấy người dùng.

--- Menu quản lý ---
1. Tạo tài khoản
2. Xem nhóm
3. Xem toàn bộ giao dịch
4. Cập nhật người dùng khác
5. Xem ví của chính mình
6. Chuyển điểm cho người dùng
7. Thoát
Chọn: 6
Người nhận: bot2
Số điểm: 12000000
[OTP (giả lập) được gửi: 640136]
Nhập mã OTP: 640136
❌ Số dư admin không đủ.

--- Menu quản lý ---
1. Tạo tài khoản
2. Xem nhóm
3. Xem toàn bộ giao dịch
4. Cập nhật người dùng khác
5. Xem ví của chính mình
6. Chuyển điểm cho người dùng
7. Thoát
Chọn: 6
Người nhận: bot2
Số điểm: 10
[OTP (giả lập) được gửi: 496686]
Nhập mã OTP: 876543
❌ Sai OTP. Hủy giao dịch.

--- Menu quản lý ---
1. Tạo tài khoản
2. Xem nhóm
3. Xem toàn bộ giao dịch
4. Cập nhật người dùng khác
5. Xem ví của chính mình
6. Chuyển điểm cho người dùng
7. Thoát
Chọn: 7
```
---

## Tài liệu tham khảo

- [CPP_OTP](https://github.com/patzol768/cpp-otp)
- [COTP](https://github.com/tilkinsc/COTP)
- [ACID Transactions](https://200lab.io/blog/acid-la-gi/)
- StackOverflow – Hash DJB2: https://stackoverflow.com/questions/19892609

