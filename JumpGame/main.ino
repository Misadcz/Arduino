#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

#define CS_PIN 10
#define RST_PIN 8
#define DC_PIN 9 

#define JUMP_PIN  3
#define RESTART_PIN 4

Adafruit_ST7789 tft = Adafruit_ST7789(CS_PIN,DC_PIN,RST_PIN);

int life;
int score;
int barell_pos;

void start()
{
  tft.fillScreen(ST77XX_BLACK);
  barell_pos = 240;
  score = 0;
  life = 3;
}

void setup()
{
  pinMode(3,INPUT);
  pinMode(4,INPUT);
  tft.init(240,240,SPI_MODE2);
  tft.setRotation(2);
  start();  
}

void HUD(int life, int score)
{
  //life
  tft.drawChar(150,20,'L',ST77XX_WHITE,ST77XX_BLACK,2);
  tft.drawChar(165,20,'I',ST77XX_WHITE,ST77XX_BLACK,2);
  tft.drawChar(180,20,'F',ST77XX_WHITE,ST77XX_BLACK,2);
  tft.drawChar(195,20,'E',ST77XX_WHITE,ST77XX_BLACK,2);
  tft.drawChar(210,20,':',ST77XX_WHITE,ST77XX_BLACK,2);
  tft.drawChar(225,20,life+48,ST77XX_WHITE,ST77XX_BLACK,2);

  //score
  tft.drawChar(10,20,'S',ST77XX_WHITE,ST77XX_BLACK,2);
  tft.drawChar(25,20,'C',ST77XX_WHITE,ST77XX_BLACK,2);
  tft.drawChar(40,20,'O',ST77XX_WHITE,ST77XX_BLACK,2);
  tft.drawChar(55,20,'R',ST77XX_WHITE,ST77XX_BLACK,2);
  tft.drawChar(70,20,'E',ST77XX_WHITE,ST77XX_BLACK,2);
  tft.drawChar(85,20,':',ST77XX_WHITE,ST77XX_BLACK,2);
  String temp  = String(score);
  tft.drawChar(100,20,temp[0],ST77XX_WHITE,ST77XX_BLACK,2);
  if(score>9)
  tft.drawChar(115,20,temp[1],ST77XX_WHITE,ST77XX_BLACK,2);

  //ground
  tft.drawLine(0, 140, 240, 140, ST77XX_WHITE);
}

void human(int x, int y,uint16_t color)
{
  //hlava
  tft.drawCircle(x, y-25, 5, color);  
  //trup
  tft.drawLine(x, y, x, y-20, color); 
  //ruce
  tft.drawLine(x, y-5, x+10, y-15, color);
  tft.drawLine(x, y-5, x-10, y-15, color);
  //nohy
  tft.drawLine(x, y, x+5, y+20, color);
  tft.drawLine(x, y, x-5, y+20, color);

}

void barell(int x,uint16_t color)
{
  tft.drawCircle(x, 130, 9, color);
}

bool isCollision(int human_pos, int barell_pos) //doupravit
{
  if((human_pos >= 95)&& (barell_pos <= 130) && (barell_pos >= 100))
    return true;
  return false;
}

void game_over()
{
  tft.fillScreen(ST77XX_BLACK);
  tft.drawChar(40,100,'G',ST77XX_RED,ST77XX_BLACK,3);
  tft.drawChar(60,100,'A',ST77XX_RED,ST77XX_BLACK,3);
  tft.drawChar(80,100,'M',ST77XX_RED,ST77XX_BLACK,3);
  tft.drawChar(100,100,'E',ST77XX_RED,ST77XX_BLACK,3);

  tft.drawChar(140,100,'O',ST77XX_RED,ST77XX_BLACK,3);
  tft.drawChar(160,100,'V',ST77XX_RED,ST77XX_BLACK,3);
  tft.drawChar(180,100,'E',ST77XX_RED,ST77XX_BLACK,3);
  tft.drawChar(200,100,'R',ST77XX_RED,ST77XX_BLACK,3);

  while(1)
  {
    if(digitalRead(RESTART_PIN) == HIGH)
    {
      start();
      break;
    }
  }
}

void repeat()
{
  tft.fillScreen(ST77XX_BLACK);
  barell_pos = 240;
  life--;
}

void loop()
{

  if(life == 0)
  game_over();


  int skip = 0;

  if(digitalRead(JUMP_PIN) == HIGH)
  {
    for(int i = 120 ; i >= 80 ; i -=10)
    {
      barell(barell_pos+5,ST77XX_BLACK);
      human(120,i,ST77XX_WHITE);
      barell(barell_pos,ST77XX_WHITE);
      barell_pos-=5;

      if(isCollision(i,barell_pos))
      {
        repeat();
        skip = 1;
      }

      if(skip == 1)
        break;
      
      delay(50);
      human(120,i,ST77XX_BLACK);
    }

    for(int i = 0 ; i < 8; i++)
    {
      barell(barell_pos+5,ST77XX_BLACK);
      barell(barell_pos,ST77XX_WHITE);
      barell_pos-=5;
      human(120,80,ST77XX_WHITE);

    if(isCollision(i,barell_pos))
    {
      repeat();
    }
    delay(50);

    if(skip == 1)
      {
        barell(barell_pos,ST77XX_BLACK);
        break;
      }
    }

    for(int i = 80 ; i <= 120 ; i +=10)
    {
      barell(barell_pos+5,ST77XX_BLACK);
      barell(barell_pos,ST77XX_WHITE);
      barell_pos-=5;
      human(120,i,ST77XX_WHITE);


    if(isCollision(i,barell_pos))
    {
    life--;
    repeat();
    barell(barell_pos,ST77XX_WHITE);
    }
      delay(50);
      human(120,i,ST77XX_BLACK);
      barell(barell_pos,ST77XX_BLACK);
    }
    
  }
  barell(barell_pos+5,ST77XX_BLACK);
barell(barell_pos,ST77XX_WHITE);
barell_pos-=5;
human(120,120,ST77XX_WHITE);

if(isCollision(120,barell_pos))
  {
  barell(barell_pos,ST77XX_WHITE);
  repeat();
  }


if(barell_pos <-20)
{
  score+=1;
  barell_pos = 240;
}

delay(50);
HUD(life,score);

}