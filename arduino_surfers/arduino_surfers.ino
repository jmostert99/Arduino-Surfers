static const int dummyvar = 0;

#include <Wire.h>
#include <hd44780.h> // include hd44780 library header file
#include <hd44780ioClass/hd44780_I2Cexp.h> // i/o expander/backpack class
hd44780_I2Cexp lcd; // auto detect backpack and pin mappings

// LCD geometry
const int LCD_COLS = 16;
const int LCD_ROWS = 2;

const int LongDelay = 5000;
const int ShortDelay = 800;

int customcharRow = 1;

const int button_up = 13;
const int button_down = 12;

uint8_t spike_up[8]  = {0x1F,0x1F,0x1F,0x0E,0x0E,0x0E,0x04,0x04};
uint8_t spike_down[8]  = {0x04,0x04,0x0e,0x0e,0x0e,0x1f,0x1f,0x1f};
uint8_t player_up[8] = {0x06,0x06,0x04,0x0e,0x0f,0x0c,0x06,0x09};
uint8_t player_down[8] = {0x09,0x06,0x0c,0x0f,0x0e,0x04,0x06,0x06};
uint8_t clear[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

int spike_info_col[3] = {15, 18, 25};
int spike_info_row[3] = {1, 0, 1};

int playcol = 0;

bool lost = false;
int score = 0;

int listDelete(int list[], int adress){

  for (int i = adress; i <= sizeof(list) / sizeof(list[0]) - 1; i++){
    list[i] = list[i + 1];
  }

  return list;
}

int listAppend(int List[], int append){
  int list[sizeof(List) / sizeof(List[0]) + 1];

  for (int i = 0; i <= sizeof(List) / sizeof(List[0]); i++){
    list[i] = List[i];
  }

  list[sizeof(List) / sizeof(List[0]) + 1] = append;

  return list;
}

int spikes(int col, int row, int playcol, bool lost){
  if (lost == false)  
    {lcd.setCursor(col, row);
    if (col <= 0){
      return 0;
    }
    else{

      if (row == 0){
        lcd.print("\001");
      }
      if (row == 1){
        lcd.print("\002");
      }}}
}

bool losing(int playcol, int spike_row, int spike_col){
  if (playcol == spike_row && spike_col == 1){
    lcd.clear();
    lcd.setCursor(1, 0);

    lost = true;
    }
  
  // Serial.println(lost);
  return lost;
}

void reset(bool lost){
  if (lost && !digitalRead(button_up)){
    int spike_info_col[3] = {15, 18, 25};
    int spike_info_row[3] = {1, 0, 1};
    playcol = 0;
    lost = false;
    Serial.println("reset");
  }
}

int player(bool lost){
  if (lost == false)
    {if (digitalRead(button_up) == false){
      playcol = 0;
    }

    else if (digitalRead(button_down) == false){
      playcol = 1;
    }

    lcd.setCursor(1, playcol);
    
    if (playcol == 0){
      lcd.print("\004");
    }

    else if (playcol == 1){
      lcd.print("\003");
    }

    return playcol;}
}

void setup()
{
  Serial.begin(9600);

int status;

	status = lcd.begin(LCD_COLS, LCD_ROWS);
	if(status) // non zero status means it was unsuccesful
	{
		// begin() failed so blink error code using the onboard LED if possible
		hd44780::fatalError(status); // does not return
	}

  lcd.print("Arduino Surfers!");
  lcd.setCursor(4, 1);
  lcd.print("Kies HSS!");
  delay(1000);
  lcd.clear();

}
void loop(void)
{
  int genspike = random(0, 10);
  // Serial.println(genspike);

	// create 8 custom characters
	lcd.createChar(1, spike_up);
	lcd.createChar(2, spike_down);
	lcd.createChar(3, player_up);
	lcd.createChar(4, player_down);

	if(LCD_ROWS < 2)
	{
		customcharRow = 0;
		delay(LongDelay);
		lcd.clear();
	}

	// lcd.setCursor(0, customcharRow);
  // playcol = player(lost);

  // reset(lost);

  for (int i = 0; i <= sizeof(spike_info_col) / sizeof(spike_info_col[0]) - 1; i++){
    lost = losing(playcol, spike_info_row[i], spike_info_col[i]);

    // Serial.println(spike_info_col[i]);

    if (lost){
      lcd.setCursor(4, 0);
      lcd.print("Score: ");
      lcd.setCursor(11, 0);
      lcd.print(score);
      delay(1000);
      lcd.setCursor(0, 1);
      lcd.print("Press up button");
      if (!digitalRead(button_up)){
        //int spike_info_col[3] = {15, 18, 25};
        spike_info_col[0]=random(10, 30);
        spike_info_col[1]=random(10, 30);
        spike_info_col[2]=random(10, 30);
        
        playcol = 0;
        lost = false;
        score = 0;

        lcd.clear();
        lcd.print("Arduino Surfers!");
        lcd.setCursor(4, 1);
        lcd.print("Kies HSS!");
        delay(2000);
        lcd.clear();
      }
    }
    //Serial.println(spike_info_col[i]);

    if (spike_info_col[i] >= 0 && !lost){
      spikes(spike_info_col[i], spike_info_row[i], playcol, lost);
      spike_info_col[i]--;
    }
    else if(!lost){
      spike_info_col[i] = random(10, 30);
      spike_info_row[i] = random(0, 2);
      score++;
      // Serial.println(spike_info_col[i]);
    }
    
  lcd.setCursor(0, customcharRow);
  playcol = player(lost);
}
  // reset(lost);

  delay(250);
  lcd.clear();
}
