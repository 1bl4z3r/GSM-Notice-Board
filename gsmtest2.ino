//LiquidCrystal.h is required to make LCD work

#include <LiquidCrystal.h>

//Defining LCD_LIGHT(Backlight) and Contrast

#define LCD_LIGHT A4
#define CONTRAST A3

char str[100],msg[64];
int temp=0,i=0,x=0,k=0;
unsigned int t;

/*
 * Telling arduino that LCD PINS are connected to these PINS of Arduino 
 * LiquidCrystal lcd(RS,E,D4,D5,D6,D7);
 */
LiquidCrystal lcd(12,11,5,4,3,2);
void setup(){
  pinMode(CONTRAST,OUTPUT);
  /*
   * analogWrite(CONTRAST,100);
   * The above line is commented because my LCd doesnot support
   * multiple contrast levels
   */
  lcd.begin(16,2);//Initializing LCD. Dimensions of mine are 16*2
  Serial.begin(9600);
  
  //Activating LCD Backlight
  
  pinMode(LCD_LIGHT, OUTPUT);
  analogWrite(LCD_LIGHT,255);

  //Setting up cursor at 0,0 position on 16*2 LCD matrix 
  lcd.setCursor(0,0);

  //For asthetics, If anyone should forget what is it
  lcd.print("GSM Notice Board");
  
  // Look over to gsminit function
  gsminit();

  //Clearing previous message
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Notice Board");

  //AT+CMGF=1 sets GSM module to "Plain Text Only" mode
  Serial.println("AT+CMGF=1");
  delay(1000);// delay required if the module is a douchebag

  //AT+CNMI=2,2,0,0,0 sets module to receive SMS and extrsct only the contents of the SMS
  Serial.println("AT+CNMI=2,2,0,0,0");
  delay(1000);
}
void loop(){

  /*
   * The message can be 59998 characters long with @ in begining and * at the 
   * end of the message, asking Arduino to look for only those messages that
   * has the pattern of @<Meessage>*
   * We don't want carrier message to show up, or any other message.
   */
  for(t=0;t<60000;t++)
  {
    //Look for serialEvent() at the end
    serialEvent();
    //temp=1 is defined in serialEvent()
    if(temp==1)
    {
      x=0,k=0,temp=0;
      while(x<i)
      {
        while(str[x]=='@')
        {
          x++;
          while(str[x]!='*')
          {
            msg[k++]=str[x++];
          }
        }
        x++;
      }
      msg[k]='\0';
      /*
       * Message starting with @ and ending with * are taken into consideration
       * else message is /0
       */
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(".....NOTICE.....");
      lcd.setCursor(0,1);
      lcd.print(msg);
      Serial.println(msg);
      delay(1000);
      temp=0;
      i=0;
      x=0;
      k=0;
    }
}
lcd.scrollDisplayLeft();
}
void serialEvent(){
  //if function checks if there is any input stream of data
  if(Serial.available())
  {
    /*
     * while function writes the data to str[] character by character
     * with i++, being a postscript operator, that increments i after
     * character is written to str[]
     */
    while(Serial.available())
    {
      char ch=(char)Serial.read();
      str[i++]=ch;
      if(ch == '*')
      {
        temp=1;
        /*
         * It prints "Message Received" on LCD and serial monitor
         * as response, just to make sure it is working
         */
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(".....NOTICE.....");      
        lcd.setCursor(0,1);
        Serial.println("Message Received");
        lcd.print("Message Received");
        delay(1000);
      }
    }
  }
}
void gsminit()
{
  //"AT" checks if GSM is working properly
  boolean at_flag=1;
  while(at_flag)
  {
    Serial.println("AT");
    while(Serial.available()>0)
    {
      if(Serial.find("OK"))
      at_flag=0;
    }
    delay(1000);
  }
  //We don't want all our tasks to echo, so echo off
  boolean echo_flag=1;
  while(echo_flag)
  {
    Serial.println("ATE0");
    while(Serial.available()>0)
    {
      if(Serial.find("OK"))
      echo_flag=0;
    }
    delay(1000);
  }
  /*
    boolean net_flag=1;
    while(net_flag)
    {
      Serial.println("AT+CPIN?");
      while(Serial.available()>0)
      {
        if(Serial.find("+CPIN: READY"))
        net_flag=0;
      }
      delay(1000);
    }
    *Above is only if SIM card has PIN enabled 
    */
  
}
