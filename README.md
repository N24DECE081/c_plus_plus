# Hệ Thống Đăng Nhập và Quản Lý Ví Điểm Thưởng

## Giới thiệu dự án

Đây là đồ án môn học **Ngôn ngữ lập trình C++**, với mục tiêu xây dựng một hệ thống dòng lệnh có khả năng:
- Quản lý tài khoản người dùng (đăng ký, đăng nhập, thay đổi thông tin).
- Xác thực bảo mật bằng OTP.
- Quản lý ví điểm và giao dịch chuyển điểm giữa các tài khoản.
- Lưu trữ và sao lưu dữ liệu bằng file.

---

## Thành viên nhóm & vai trò

| Họ và Tên               | Vai trò                        | Công việc được giao                                                  |
|-------------------------|---------------------------------|---------------------------------------------------------------------|
| Lại Thu Hiền            | Thực hiện phần A               | Quản lý tài khoản người dùng: đăng ký, đăng nhập, OTP                |
| Nguyễn Dương Thanh Mai  | Thực hiện phần B               | Phân quyền người dùng, cập nhật thông tin, nhóm người dùng           |
| Nguyễn Thanh Tuyền      | Thực hiện phần C               | Giao dịch ví điểm, OTP cho chuyển điểm, ghi lịch sử                  |
| La Vĩnh Tiến            | Thực hiện phần C               | Giao dịch ví điểm, OTP cho chuyển điểm, ghi lịch sử                  |

---

## Phân tích & đặc tả chức năng

### Quản lý tài khoản (PHANA.cpp)
- Đăng ký người dùng mới, có hỗ trợ sinh mật khẩu tự động.
- Mã hóa mật khẩu bằng thuật toán DJB2.
- Lưu trữ dữ liệu người dùng vào `users.txt`, có chức năng sao lưu (`backup.txt`).
- Đăng nhập, thay đổi mật khẩu, yêu cầu đổi mật khẩu nếu là mật khẩu tự sinh.
- OTP được sinh ngẫu nhiên để xác minh khi cần.

### Phân quyền người dùng (PHAN_B.cpp)
- **Người dùng thường**: chỉ được thay đổi thông tin cá nhân (cần OTP xác nhận).
- **Người quản lý**:
  - Tạo tài khoản hộ người khác.
  - Cập nhật thông tin người dùng khác (chỉ mật khẩu).
  - Theo dõi danh sách nhóm.
- OTP được yêu cầu khi cập nhật thông tin.

### Quản lý ví & giao dịch (PHANC.cpp)
- Mỗi người dùng có một ví định danh (WALLET_ID).
- Ví tổng (`MASTER_WALLET`) cấp điểm khởi tạo.
- Giao dịch chuyển điểm giữa ví người dùng, yêu cầu xác thực OTP.
- Mọi giao dịch được ghi vào `transactions.txt`, có trạng thái rõ ràng (thành công/thất bại).
- Xem lịch sử ví cá nhân và toàn bộ giao dịch hệ thống (đối với quản lý).

---

## Lưu trữ & sao lưu

- Dữ liệu người dùng lưu vào: `users.txt`
- Dữ liệu giao dịch: `transactions.txt`
- Sao lưu dữ liệu người dùng: `backup.txt`
- Format file đơn giản: mỗi dòng là 1 bản ghi.
- Ưu tiên lưu **nhiều người dùng trong một file duy nhất** → dễ quản lý và backup đồng bộ.

---

## Chú thích thư viện

| Thư viện        | Mục đích sử dụng                                               |
|------------------|----------------------------------------------------------------|
| `<iostream>`     | Nhập xuất cơ bản                                               |
| `<fstream>`      | Đọc/ghi dữ liệu từ/to file (users.txt, transactions.txt)      |
| `<string>`       | Xử lý chuỗi cơ bản                                             |
| `<vector>`       | Lưu danh sách người dùng, lịch sử giao dịch                   |
| `<ctime>`        | Lấy thời gian phục vụ cho OTP, seed random                     |
| `<cstdlib>`      | Dùng `rand()` để sinh OTP, mật khẩu ngẫu nhiên                 |
| `<sstream>`      | Chuyển đổi chuỗi khi đọc file dữ liệu (tách theo ký tự `|`)    |
| `<map>`          | Lưu danh sách người dùng theo `username`                       |
| `<clocale>`      | Hỗ trợ định dạng tiếng Việt khi in giao diện                   |

---

## Hướng dẫn biên dịch & chạy chương trình

### Yêu cầu:
- Trình biên dịch C++ hỗ trợ chuẩn `C++11` hoặc mới hơn.

### Biên dịch:
```bash
g++ PHANA.cpp -o phana
g++ PHAN_B.cpp -o phanb
g++ PHANC.cpp -o phanc
```

### Chạy chương trình:
```bash
./phana  # Đăng ký, đăng nhập, thay đổi mật khẩu, backup
./phanb  # Quản lý người dùng, nhóm, cập nhật thông tin (có OTP)
./phanc  # Giao dịch ví, chuyển điểm, xem lịch sử
```

---

## Hướng dẫn sử dụng

1. **PHANA.cpp**:
   - Chạy menu để tạo tài khoản, đăng nhập, đổi mật khẩu, sao lưu.
   - Khi tạo tài khoản có thể chọn mật khẩu tự sinh hoặc nhập tay.

2. **PHAN_B.cpp**:
   - Đăng nhập theo vai trò.
   - Người dùng thường: xem/sửa thông tin cá nhân (OTP).
   - Người quản lý: tạo tài khoản, sửa mật khẩu người dùng khác (OTP), xem nhóm.

3. **PHANC.cpp**:
   - Đăng nhập để quản lý ví.
   - Chuyển điểm giữa ví → yêu cầu OTP xác nhận.
   - Hiển thị số dư và lịch sử giao dịch theo từng người dùng.

---

## Kiểm thử chức năng (Input / Output)

| Chức năng               | Input                                  | Output                            |
|-------------------------|-----------------------------------------|----------------------------------|
| Đăng ký tài khoản       | username, password, email,…             | Tạo tài khoản thành công         |
| Đăng nhập               | username + password                     | Đăng nhập thành công/thất bại    |
| Đổi mật khẩu            | username + old/new password             | Đổi mật khẩu thành công          |
| Giao dịch điểm          | người nhận + số điểm + OTP              | Giao dịch thành công / lỗi       |
| Cập nhật thông tin      | tên mới, mật khẩu mới + OTP             | Cập nhật thành công / OTP sai    |
| Sao lưu dữ liệu         | -                                       | Ghi backup.txt thành công        |
| Xem lịch sử giao dịch   | -                                       | Hiển thị chi tiết                |

---

## 🔐 Bảo mật & OTP

- Mật khẩu được mã hóa bằng thuật toán DJB2 trước khi lưu.
- OTP dài 6 chữ số được sinh ngẫu nhiên và chỉ dùng 1 lần.
- OTP áp dụng cho: cập nhật thông tin cá nhân, giao dịch ví, cập nhật hộ.

---

## 📚 Tài liệu tham khảo

- [CPP_OTP](https://github.com/patzol768/cpp-otp)
- [COTP](https://github.com/tilkinsc/COTP)
- [ACID Transactions](https://200lab.io/blog/acid-la-gi/)
- StackOverflow – Hash function (DJB2): https://stackoverflow.com/questions/19892609
