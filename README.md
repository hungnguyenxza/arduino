# Arduino
Arduino projects

## Project tự tìm hiểu và học Arduino

### Tài liệu tham khảo:
1. [arduino.vn](http://arduino.vn/reference/howto)
2. [mlab.vn](https://mlab.vn/)


### Lập trình
* Arduino IDE v2.3.2

### Linh kiện
1. [tuhu - bách khoa](https://shopee.vn/dientutuhu)
2. [linh kiện 3M](https://chotroihn.vn/)
    * [link shopee](https://shopee.vn/linhkiendientu3m)

---

Hướng dẫn Nạp Code cho ESP8266 (NodeMCU) - Web Server WOL
=========================================================

Tài liệu này hướng dẫn cách cấu hình Arduino IDE và nạp code cho module **ESP8266 NodeMCU CH340** để chạy ứng dụng Wake-on-LAN.

1\. Chuẩn bị phần mềm
---------------------

1.  **Tải Arduino IDE:** Truy cập [arduino.cc](https://www.arduino.cc/en/software) và cài đặt phiên bản mới nhất.
    
2.  **Cài đặt Driver CH340:** Nếu máy tính không nhận cổng COM khi cắm module, hãy tải và cài đặt driver [tại đây](http://www.wch-ic.com/downloads/CH341SER_EXE.html).
    

2\. Cấu hình Arduino IDE
------------------------

Để nạp được code cho ESP8266, bạn cần thêm thư viện bo mạch vào IDE:

1.  Mở Arduino IDE, vào **File > Preferences**.
    
2.  Tại mục **Additional Boards Manager URLs**, dán dòng sau:\
[http://arduino.esp8266.com/stable/package/esp8266com/index.json](http://arduino.esp8266.com/stable/package/esp8266com/index.json)
    
3.  Vào **Tools > Board > Boards Manager...**
    
4.  Tìm kiếm từ khóa esp8266, chọn bản mới nhất và nhấn **Install**.
    

3\. Cấu hình trước khi nạp (Trong Code)
---------------------------------------

Trước khi nhấn nút nạp, bạn **BẮT BUỘC** phải chỉnh sửa các thông tin sau trong file code:

*   **Thông tin WiFi:** ssid và password.
    
*   **Địa chỉ IP tĩnh:** local\_IP, gateway, broadcastIP (phải đúng dải mạng của router nhà bạn).
    
*   **Địa chỉ MAC:** Thay địa chỉ MAC của máy tính cần bật vào mảng targets.
    
*   **Mật khẩu Web:** webUser và webPass.
    

4\. Các bước nạp code
---------------------

1.  Kết nối module ESP8266 với máy tính bằng cáp Micro-USB tốt (có chức năng truyền dữ liệu).
    
2.  Trong Arduino IDE, chọn đúng Board:
    
    *   **Tools > Board > ESP8266 Boards > NodeMCU 1.0 (ESP-12E Module)**.
        
3.  Chọn đúng cổng COM:
    
    *   **Tools > Port > (Chọn cổng COM tương ứng, thường là COM3, COM4...)**.
        
4.  Cấu hình tốc độ nạp (Upload Speed): Chọn **115200** để ổn định nhất.
    
5.  Nhấn nút **Upload** (mũi tên trỏ sang phải) trên thanh công cụ.
    

5\. Kiểm tra hoạt động
----------------------

1.  Sau khi báo Done uploading, mở **Serial Monitor** (biểu tượng kính lúp góc trên bên phải).
    
2.  Chọn tốc độ baud là **115200**.
    
3.  Nhấn nút **RST** trên module. Nếu thấy dòng Connected! ESP IP: 192.168.1.97, bạn đã thành công.
    
4.  Dùng điện thoại hoặc máy tính khác trong cùng mạng, gõ IP đó vào trình duyệt để test giao diện.
    

6\. Lưu ý vận hành (Dành cho bản chạy 24/7)
-------------------------------------------

*   **Nguồn điện:** Sử dụng adapter 5V-1A hoặc 5V-2A ổn định. Không nên cấp nguồn từ cổng USB máy tính quá lâu.
    
*   **Port Forwarding:** Để truy cập từ Internet, bạn cần vào cấu hình Router nhà mạng, tìm mục **Virtual Server/Port Forwarding** để trỏ Port 80 về địa chỉ IP của ESP.
    
*   **Bảo mật:** Luôn giữ tính năng Base Auth và đổi port mặc định nếu có thể để tránh bị bot tấn công.