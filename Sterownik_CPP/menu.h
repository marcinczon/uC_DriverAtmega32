#ifndef menu_H
#define menu_H

//******************************************************//
//**********************BIBLIOTEKI**********************//
//******************************************************//
#include "lcd.cpp"
#include "twi.cpp"
#include "eeprom.cpp"
#include "uart.cpp"	
#include "led_rgb.cpp"
#include "one_wire.cpp"
#include "rtc_pcf8583.cpp"
#include "ds18b20.cpp"
#include "keys.cpp"
#include "relay.cpp"
#include "convert.cpp"

//*****************************************************//
//**********************DEFINICJE**********************//
//*****************************************************//
//Definicje przekaünikow

#define K1 4;
#define K2 2;
#define K3 1;
#define K4 0;

//*****************************************************//
//**********************ZMIENNE************************//
//*****************************************************//
//Zmienne do stanu menu
int g_iMenu=0;
int g_iSubMenu=1;
int g_iPoziomyMainMenu=5;
int g_iPoziomSubMenu=0;
bool g_fMenu_1=true, g_fMainMenu=false,g_fSubMenu=false;
								

//Inne zmienne
bool g_fISR_INT1=false;

//UART COMMAND PROCESS
static unsigned int _zakres=0;
static bool _start_funkcja=false,_start_dane=false;
static bool _fCommandExist=false;


//*****************************************************//
//**********************OBIEKTY************************//
//*****************************************************//



//*****************************************************//
//**********************FUNKCJE************************//
//*****************************************************//

void Clock_Set_Time(void);
int Clock_Check_Second(void);
int Clock_Check_Second(void);

void Relay_K1_Set_Time(void);
void Relay_K2_Set_Time(void);
void Relay_K3_Set_Time(void);
void Relay_K4_Set_Time(void);
void Relay_General_Set_Time(void);
void Relay_Set_State(void);
void Relay_Time_Info(void);
void Relay_Time_Reset(void);
void Relay_Show_Status(void);
void Relay_Set_Mode(void);
void Relay_Full_Auto_Mode(void);
void Relay_Semi_Auto_Mode(void);

void LED_Set_Time_Day(void);
void LED_Set_Time_Night(void);
void LED_Color(void);
void LED_Set_Mode(void);

void USART_Instruction_Process(void);
void USART_Command(void);

void Checking_Function(void);
void Checking_Function(void);
void Show_Time(void);
void Sub_Menu_Function_1(int);
void Sub_Menu_Function_2(int);
void Sub_Menu_Function_3(int);
void Sub_Menu_Function_4(int);
void Sub_Menu_Function_5(int);
void Menu_Function(int , int );
void Show_Sub_Menu(void);
void Selection_Sub_Menu(void);
void Show_Menu(void);
void Selection_Menu(void);
void Main_Loop(void);

void Init_Przerwania(void);
void Init_Button(void);
void Init_Przekazniki(void);
void Init_Start_Lcd(void);
void Init_Termometr(void);
void Init_Led(void);
void Inicjalizacje(void);




#endif
