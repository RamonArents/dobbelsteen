const int latchPin = 8; // pins connected to shift register
const int clockPin = 12;
const int dataPin = 11;
const int buttonPin = 2; // button pin
// hex values 1 to 6 for dice
unsigned char lookup_7seg[] = {0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x80};
// shaking the dice
unsigned char shake_dice[] = {0x63, 0x5C};
// rolling the dice
unsigned char roll_dice[] = {0x1C, 0x58, 0x54, 0x4C};
// make random numbers
int rand_seed;
int rand_num = 0;
unsigned char shake_toggle = 0; // animate shaking

int index = 0; // animate rolling
int shake_speed; // increase shaking speed

void setup() {
  // output for shift register
  pinMode(latchPin, OUTPUT);
  
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  // show . of segement display at startup
  digitalWrite(latchPin, LOW);

  shiftOut(dataPin, clockPin, MSBFIRST, lookup_7seg[6]);
  digitalWrite(latchPin, HIGH);
  randomSeed(analogRead(0)); // generate random seed

}

void loop() {
  if(digitalRead(buttonPin)){
    digitalWrite(13, HIGH);
        // reset shaking speed
    shake_speed = 150;
    delay(30);
    // generate random number and show animation
    while(digitalRead(buttonPin)){
      // random number
      rand_seed++;
      // animate shaking
      if(shake_toggle){
        AnimateDice(0, shake_dice);
        shake_toggle = 0;
      }
      else{
        AnimateDice(1, shake_dice);
        shake_toggle = 1;
      }
      // increase shaking speed
      delay(80 + shake_speed);
      if(shake_speed > 0){
        shake_speed -= 10;
      }
    }
    // animate rolling dices
    for(int rolls = 0; rolls < (rand_seed - 10+14); rolls++){
      AnimateDice(index, roll_dice);
      delay((1 + rolls) * 20);
      index++;
      if(index > 3){
        index = 0;
      }
    }
    rand_num = random(0, 6); // generate random number on dice

    DiceNumber(rand_num);
  }else{
    digitalWrite(13, LOW);
  }
}

// show number on 7 segment 
void DiceNumber(unsigned char num){
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, lookup_7seg[num]);
  digitalWrite(latchPin, HIGH);
}

// show frame of shaking or rolling dices
void AnimateDice(int seg, unsigned char *table){
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, table[seg]);
  digitalWrite(latchPin, HIGH);
}
