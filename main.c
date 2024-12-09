#include <stdio.h>
#include <stdint.h>

#include "system.h"
#include "io.h"
#include "timer.h"
#include "oled.h"
#include "hi2c.h"

float calibrationFactor = 97.0; // Kalibrasyon için sabit deðer, bu deðer sizin ölçümünüze göre deðiþir

void init(void)
{
  // I/O portlarý baþlangýç
  Sys_IoInit();
  
  // System Clock Tick baþlangýç
  //Sys_TickInit();
  
  // Konsol baþlangýç
  Sys_ConsoleInit();


  // LED baþlangýç
  IO_Write(IOP_LED, 1);
  IO_Init(IOP_LED, IO_MODE_OUTPUT_OD);  
  
  
     IO_Write(IOP_HX711_SCK, 0);
   IO_Init(IOP_HX711_SCK, IO_MODE_OUTPUT); 
   
   IO_Init(IOP_HX711_DT, IO_MODE_INPUT);
   
 
}

// HX711'dan veri okuma fonksiyonu
long HX711_Read(void) {
    unsigned long count = 0;
    int i;

    // DOUT pini yüksekse bekle
    while (IO_Read(IOP_HX711_DT));

    // 24 bitlik veri okuma döngüsü
    for (i = 0; i < 24; i++) {
        IO_Write(IOP_HX711_SCK, 1);  // SCK'yi 1 yap
        count = count << 1;  // Okunan veriyi sola kaydýr
        DelayUs(1);
        IO_Write(IOP_HX711_SCK, 0);  // SCK'yi 0 yap
        DelayUs(1);
        if (IO_Read(IOP_HX711_DT)) {
            count++;  // DOUT yüksekse, count'u artýr
        }
    }

    // 25. saatte fazladan bir clock sinyali gönder
    IO_Write(IOP_HX711_SCK, 1);
    DelayUs(1);
    count = count ^ 0x800000;  // Ýþaret bitini düzenle (2's complement)
    IO_Write(IOP_HX711_SCK, 0);
    DelayUs(1);

    return count;
}

// HX711'in dengesini ayarlama (tare) fonksiyonu
long HX711_Tare(void) {
    unsigned long tare = 0;
    for (int i = 0; i < 10; i++) {
        tare += HX711_Read();
    }
    return tare / 10;  // Ortalama deðeri hesapla
}



long weight;
long rawValue;
void Task_Print(void)
{
  OLED_SetCursor(1,0);
  OLED_ClearDisplay();
  printf("w: %ld",weight);
  
  OLED_SetCursor(3,0);
  printf("cal: %lf",calibrationFactor);
}



void main()
{
// Baþlangýç yapýlandýrmalarý
  init();//
  
  OLED_SetFont(FNT_SMALL);
 
long tareValue = HX711_Tare();

  while (1) 
  {
   // Yük hücresinden veri oku
        rawValue = HX711_Read();

       // Kalibrasyon faktörü ile aðýrlýðý hesapla
        weight = (rawValue - tareValue) / calibrationFactor;

        // Aðýrlýk deðeri burada iþlenebilir (örneðin, seri porttan gönderme)
    Task_Print();
  }
}


