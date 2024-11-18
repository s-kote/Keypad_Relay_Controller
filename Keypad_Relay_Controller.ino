#include <Wire.h>  // ใช้ไลบรารี Wire สำหรับ I2C
#include <LiquidCrystal_I2C.h>  // ใช้ไลบรารี LiquidCrystal_I2C
#include <Keypad.h>

#define RELAY_PIN 11 // ประกาศ ขาพินรีเลย์

const byte ROWS = 4; // 4 แถว
const byte COLS = 4; // 4 คอลัมน์

char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6};  // พินแถวของ Keypad
byte colPins[COLS] = {5, 4, 3, 2};  // พินคอลัมน์ของ Keypad

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

const String password_1 = "113333"; // กำหนดรหัสผ่าน 1
const String password_2 = "AB123"; // กำหนดรหัสผ่าน 2

String input_password = "";  // ตัวแปรสำหรับเก็บรหัสผ่านที่กรอก

// กำหนดพินสำหรับการใช้ LCD 16x2 แบบ I2C
LiquidCrystal_I2C lcd(0x27, 16, 2);  // ที่อยู่ I2C (0x27) ขนาด 16x2

void setup() {
  Serial.begin(9600);  // เริ่มต้นการสื่อสารกับ Serial Monitor
  pinMode(RELAY_PIN, OUTPUT);  // ตั้งค่าพินรีเลย์ให้เป็น Output
  digitalWrite(RELAY_PIN, LOW);  // ปิดรีเลย์เริ่มต้น

  lcd.begin();  // เริ่มต้น LCD ขนาด 16x2
  lcd.print("Enter Password"); // แสดงข้อความเริ่มต้นที่ LCD
  delay(1000);  // หน่วงเวลา 1 วินาที
  lcd.clear();  // ล้างหน้าจอ LCD
}

void loop() {
  char key = keypad.getKey();  // อ่านค่าจาก Keypad

  if (key) {  // ถ้ามีการกดปุ่ม
    if (key == '*') {  // ถ้ากด '*', รีเซ็ตรหัสผ่านที่กรอก
      input_password = "";
      lcd.clear();
      lcd.print("Clear Password");  // แสดงข้อความบน LCD
      delay(1000);
      lcd.clear();
      lcd.print("Enter Password");
      Serial.println("\nPassword cleared.");  // แสดงข้อความใน Serial Monitor
    } 
    else if (key == '#') {  // ถ้ากด '#', ตรวจสอบรหัสผ่าน
      // แสดงรหัสผ่านที่กรอกแค่ครั้งเดียว
      Serial.print("Entered Password: ");
      Serial.println(input_password);  // แสดงรหัสผ่านที่กรอกทั้งหมดใน Serial Monitor

      lcd.clear();
      lcd.print("Password: ");  // แสดงข้อความ "Password: "
      lcd.print(input_password); // แสดงรหัสที่กรอก

      // ตรวจสอบรหัสผ่าน
      if (input_password == password_1 || input_password == password_2) {
        lcd.clear();
        lcd.print("Password Correct");
        Serial.println("Password Correct");
        digitalWrite(RELAY_PIN, HIGH);  // เปิดรีเลย์
        delay(5000);  // รีเลย์เปิด 5 วินาที
        digitalWrite(RELAY_PIN, LOW);  // ปิดรีเลย์
        lcd.clear();
        lcd.print("Relay Closed");
      } else {
        lcd.clear();
        lcd.print("Wrong Password");
        Serial.println("Incorrect Password");
      }

      input_password = "";  // รีเซ็ตรหัสผ่าน
      delay(1000);  // หน่วงเวลา 1 วินาที
      lcd.clear();
      lcd.print("Enter Password");
    } 
    else {  // ถ้ากดปุ่มตัวเลขหรืออักขระอื่นๆ
      input_password += key;  // เพิ่มคีย์ที่กดเข้าไปในรหัสผ่านที่กรอก

      lcd.setCursor(0, 1);  // วางเคอร์เซอร์ที่บรรทัดที่ 2
      lcd.print("Password: ");
      lcd.print(input_password);  // แสดงรหัสที่กรอกในบรรทัดที่ 2

      // แสดงรหัสที่กรอกใน Serial Monitor
      Serial.print("Current Input: ");
      Serial.println(input_password);  // แสดงรหัสที่กรอกใน Serial Monitor
    }
  }
}
