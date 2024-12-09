#include <stdio.h>
#include <stdint.h>

#include "system.h"
#include "io.h"
#include "timer.h"
#include "oled.h"
#include "hi2c.h"

float calibrationFactor = 97.0; // Kalibrasyon i�in sabit de�er, bu de�er sizin �l��m�n�ze g�re de�i�ir

void init(void)
{
  // I/O portlar� ba�lang��
  Sys_IoInit();
  
  // System Clock Tick ba�lang��
  //Sys_TickInit();
  
  // Konsol ba�lang��
  Sys_ConsoleInit();


  // LED ba�lang��
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

    // DOUT pini y�ksekse bekle
    while (IO_Read(IOP_HX711_DT));

    // 24 bitlik veri okuma d�ng�s�
    for (i = 0; i < 24; i++) {
        IO_Write(IOP_HX711_SCK, 1);  // SCK'yi 1 yap
        count = count << 1;  // Okunan veriyi sola kayd�r
        DelayUs(1);
        IO_Write(IOP_HX711_SCK, 0);  // SCK'yi 0 yap
        DelayUs(1);
        if (IO_Read(IOP_HX711_DT)) {
            count++;  // DOUT y�ksekse, count'u art�r
        }
    }

    // 25. saatte fazladan bir clock sinyali g�nder
    IO_Write(IOP_HX711_SCK, 1);
    DelayUs(1);
    count = count ^ 0x800000;  // ��aret bitini d�zenle (2's complement)
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
    return tare / 10;  // Ortalama de�eri hesapla
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
// Ba�lang�� yap�land�rmalar�
  init();//
  
  OLED_SetFont(FNT_SMALL);
 
long tareValue = HX711_Tare();

  while (1) 
  {
   // Y�k h�cresinden veri oku
        rawValue = HX711_Read();

       // Kalibrasyon fakt�r� ile a��rl��� hesapla
        weight = (rawValue - tareValue) / calibrationFactor;

        // A��rl�k de�eri burada i�lenebilir (�rne�in, seri porttan g�nderme)
    Task_Print();
  }
}


