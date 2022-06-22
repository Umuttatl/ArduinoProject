#include <Wire.h>
#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <RFID.h>
#include <Servo.h>
Servo sg90;
LiquidCrystal_I2C lcd(0x27, 16, 2);

int Buton1 = 3;
int bt1;

const byte IR_plastik = 8;
int IR_plastik_deger;

const byte IR_cam = 7;
int IR_cam_deger;

const byte IR_kagit = 6;
int IR_kagit_deger;

const byte IR_metal = 5;
int IR_metal_deger;

int sayac = 0;

RFID rfid(10, 9);
byte kart[5] = {147,48,195,167,199};
int kredi = 0;
boolean izin = true;

void setup()
{
  Serial.begin(9600);
  lcd.begin();
  SPI.begin();
  rfid.init();
  sg90.attach(4);
 
  pinMode(Buton1,INPUT);
  pinMode(IR_plastik, INPUT);
  pinMode(IR_cam, INPUT);
  pinMode(IR_kagit, INPUT);
  pinMode(IR_metal, INPUT);
}

void loop()
{
  izin=true;
  bt1 = digitalRead(Buton1);
  IR_plastik_deger = digitalRead(IR_plastik);
  IR_cam_deger = digitalRead(IR_cam);
  IR_kagit_deger = digitalRead(IR_kagit);
  IR_metal_deger = digitalRead(IR_metal);
  
  if(bt1 == HIGH)
  sg90.write(90);
 
  else
  sg90.write(0);

  if (IR_plastik_deger == 0)
  {  
    sayac = sayac+5;
    lcd.print("PLASTIK ATILDI");
    lcd.setCursor(0,1);
    lcd.print("BAKIYENIZ = ");
    lcd.println(sayac);
    delay(400);  
  }
  else if (IR_cam_deger == 1)
  {
    sayac = sayac+10;
    lcd.print("CAM ATILDI");
    lcd.setCursor(0,1);
    lcd.print("BAKIYENIZ = ");
    lcd.println(sayac);
    delay(400);
  }
  else if (IR_kagit_deger == 1)
  {
    sayac = sayac+15;
    lcd.print("KAGIT ATILDI");
    lcd.setCursor(0,1);
    lcd.print("BAKIYENIZ = ");
    lcd.println(sayac);
    delay(400);
  }
  else if (IR_metal_deger == 1)
  {
    sayac = sayac+20;
    lcd.print("METAL ATILDI");
    lcd.setCursor(0,1);
    lcd.print("BAKIYENIZ = ");
    lcd.println(sayac);
    delay(400);
  } 
  else if(rfid.isCard())
  {
    if(rfid.readCardSerial())
    {
      Serial.print("Kart ID: ");
      Serial.print(rfid.serNum[0]);
      Serial.print(",");
      Serial.print(rfid.serNum[1]);
      Serial.print(",");
      Serial.print(rfid.serNum[2]);
      Serial.print(",");
      Serial.print(rfid.serNum[3]);
      Serial.print(",");
      Serial.println(rfid.serNum[4]);
    }
    for(int i=0; i<5; i++)
    {
      if(rfid.serNum[i] != kart[i])
      {
       izin = false;
      }
    }
    if(izin==true)
    {
     lcd.print("GIRIS BASARILI");
     delay(1000);
     lcd.clear();
     kredi = sayac+kredi;
     lcd.print("KREDI ALINDI = ");
     lcd.setCursor(0,1);
     lcd.println(kredi);
     delay(1000);
     sayac = 0;
    }
    else
    {
     lcd.println("YETKI YOK");
     delay(1000);
     lcd.clear();
     rfid.halt(); 
    }
  }
  else
  {
    lcd.clear();
  }
}
