#if 0
  #include <Wire.h>
  #include <LiquidCrystal_I2C.h>
  LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
#else
  #include <LiquidCrystal.h>
  LiquidCrystal lcd(A0,A1,13,12,11,10);
#endif

#define VER  1.0f 
#define val_cnt 4 

int chk_init = 0;
int val, val2 = 0;
int key_val = 0;
int set_cal = 0;
int cal_end = 0;
long digit4_sum_1 = 0;
long digit4_sum_2 = 0;
double digit4_total = 0;

const int numRows = 4;
const int numCols = 4; 
int input_num = 0;  

int reset_val = 0;  
int reading;  

long time = 0;        // LED ON/OFF 
long debounce = 100;  // Debounce 1000

int pinRows[numRows] = {9, 8, 7, 6};
int pinCols[numCols] = {5, 4, 3, 2};

void Reset();

void setup()
{
    pinMode(13, OUTPUT); //LED HOPE
  	
    //MsTimer2::set(500, TimerISR); // 0.5sec
    //MsTimer2::start();

    Serial.begin(9600);    // init Serial communication
    Serial.print("Simple Calc Program");
    Serial.print("VER :");
    Serial.println(VER);
    
#if 0    
    lcd.init();       // initialize the lcd I2C
    lcd.begin(16,2);
    lcd.backlight();  // backlight on
    lcd.print("Digital Calculator");
    lcd.setCursor(0,1);
    lcd.print("Ready to input");
    lcd.setCursor(0,0);
#else     
    lcd.begin(16,2);
    lcd.print("Simple Calculator");
    lcd.setCursor(0,1);
    lcd.print("Ready to input");
    lcd.setCursor(0,0);
#endif    

    digitalWrite(A3, HIGH);
    delay(200);
    // initialize row pins as INPUT
    for(int i=0; i<numRows; i++)
    {
        pinMode(pinRows[i], INPUT_PULLUP);
    }
    
    // initialize column pins as OUTPUT
    for(int j=0; j<numCols; j++)
    {
        pinMode(pinCols[j], OUTPUT);
        digitalWrite(pinCols[j], HIGH);
    } // set initial output as HIGH
  
  	pinMode(A3, OUTPUT);
    delay(200);
}

//void(*resetFunc)(void) = 0;

void clear_init()
{
    lcd.clear();
    val = 0;
    val2 = 0;
    digit4_sum_1 = 0;
    digit4_sum_2 = 0;
    digit4_total = 0;
    set_cal = 0;
    cal_end = 0;
}

void loop()
{
    // Check input
    for(int j=0; j<numCols; j++)
    {
        digitalWrite(pinCols[j], LOW);    // set as LOW to check button press
        for(int i=0; i<numRows; i++)
        {
            input_num = (i * numRows) + j;
                      
            reading = digitalRead(pinRows[i]);
                                  
            if(reading == LOW) 
            {
                if(millis() - time > debounce)
                {
                  if(!cal_end) 
                  {
                    if(input_num == 0) key_val = 1;
                    else if(input_num == 1) key_val = 2;
                    else if(input_num == 2) key_val = 3;
                    else if(input_num == 3) key_val = 10; // +
                    else if(input_num == 4) key_val = 4;
                    else if(input_num == 5) key_val = 5;
                    else if(input_num == 6) key_val = 6;
                    else if(input_num == 7) key_val = 11; // -
                    else if(input_num == 8) key_val = 7;
                    else if(input_num == 9) key_val = 8;
                    else if(input_num == 10) key_val = 9;
                    else if(input_num == 11) key_val = 12; // *
                    else if(input_num == 13) key_val = 0;
                    else if(input_num == 14) key_val = 13; // =
                    else if(input_num == 15) key_val = 14; // /  
                  }
                    if(input_num == 12) key_val = 15; // clear_init(); 
                    
                    if(!chk_init)
                    {
                        lcd.clear();
                        chk_init = 1;
                    }
                    //Serial.println(input_num);
                    time = millis();
                  
                    if(!digit4_sum_1) val=0;
                                      
                    switch(key_val)
                    {
                        case 0 ... 9:
                            if(!set_cal) 
                            {
                                if(val < val_cnt){
                                  Serial.println(val);
                                  val++;
                                  lcd.setCursor(0,0);
                                  digit4_sum_1 = digit4_sum_1 * 10 + key_val;
                                  lcd.print(digit4_sum_1);
                                }
                            }
                            else 
                            {
                                if(val2 < val_cnt){
                                  Serial.println(val2);
                                  val2++;
                                  lcd.setCursor(val+1,0);
                                  digit4_sum_2 = digit4_sum_2 * 10 + key_val;
                                  lcd.print(digit4_sum_2);
                                }
                            }
                            if(digit4_sum_1 == 8800)
                                reset_val = 1;
                            break;
                      
                        case 10: 
                            if(!set_cal)
                              lcd.print("+");
                            set_cal = 1;
                            break;
                      
                        case 11: 
                            if(!set_cal)
                              lcd.print("-");
                            set_cal = 2;
                            break;
                      
                        case 12: 
                            if(!set_cal)
                              lcd.print("*");
                            set_cal = 3;
                            break;
                      
                        case 14: 
                            if(!set_cal)
                              lcd.print("/");
                            set_cal = 4;
                            break;
                      
                        case 15: 
                            clear_init();
                            break;    
                      
                        case 13: 
                            lcd.setCursor(0,1);
                            lcd.print("=");
                            cal_end = 1; 
                            switch(set_cal){
                                case 1:
                                    digit4_total = digit4_sum_1 + digit4_sum_2;                               
                                    lcd.print(digit4_total);
                                    break;
                                case 2:
                                    digit4_total = digit4_sum_1 - digit4_sum_2;                               
                                    lcd.print(digit4_total);
                                    break;
                                case 3:
                                    digit4_total = digit4_sum_1 * digit4_sum_2;                               
                                    lcd.print(digit4_total);
                                    break;
                                case 4:
                                    digit4_sum_2 == 0 ? lcd.print("Invalid") : digit4_total = (float)digit4_sum_1 / (float)digit4_sum_2;
                                    lcd.print(digit4_total);
                                    break;
                          }
                    }
                }
            }
        }
        digitalWrite(pinCols[j], HIGH);    // set as default (HIGH)
    }

    if(reset_val){ //HW reset
        Reset();
    }
    digitalWrite(A3, HIGH);
}

void Reset()//HW reset
{
    //asm volatile ("jmp 0");
    delay(1000);
    digitalWrite(A3, LOW);
    //resetFunc();
}
