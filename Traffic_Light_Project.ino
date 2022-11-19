#define r0 6 
#define g0 7
#define r1 8 
#define g1 9
#define r2 10 
#define g2 11
#define r3 12
#define g3 13

#define Am0 A0
#define Am1 A1
#define Am2 4
#define Am3 5
#define R A2
#define INTERRUPT_PIN 2

#define IR 0

int state = 0;
int stateAm = 0;
int stateR = 0;
int count = 0;
int i=0;
int i2=0;
//float value = 3035;                   //Preload timer value (3035 for 4 seconds)

volatile bool ledState = LOW;

void setup()
{
  //Serial.begin(115200);
  delay(2000);

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);

  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);

  pinMode(A5,INPUT);
  pinMode(A4,INPUT);
  pinMode(A3,INPUT);
  pinMode(1,INPUT);

  pinMode(0,OUTPUT);

  pinMode(INTERRUPT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), myISR, RISING); // trigger when button pressed, but not when released.

  noInterrupts();                       // disable all interrupts

  TCCR1A = 0;
  TCCR1B = 0;

  TCNT1 = 0;                        // preload timer
  TCCR1B |= (1 << CS12);            // 256 prescaler
  TIMSK1 |= (1 << TOIE1);                        
  interrupts();                         // enable all interrupts
}

ISR(TIMER1_OVF_vect)                    // interrupt service routine for overflow
{
  TCNT1 = 0;                                // preload timer
  if(state == 1)
  {
    if(stateAm == 0)
    {
      digitalWrite(g0, HIGH);
      digitalWrite(g1, LOW);
      digitalWrite(g2, LOW);
      digitalWrite(g3, LOW);

      digitalWrite(r0, LOW); 
      digitalWrite(r1, HIGH);
      digitalWrite(r2, HIGH);
      digitalWrite(r3, HIGH);
    }

    else if (stateAm == 1)
    {
      digitalWrite(g0, LOW);
      digitalWrite(g1, HIGH);
      digitalWrite(g2, LOW);
      digitalWrite(g3, LOW);

      digitalWrite(r0, HIGH); 
      digitalWrite(r1, LOW);
      digitalWrite(r2, HIGH);
      digitalWrite(r3, HIGH);
    }

    else if (stateAm == 2)
    {
      digitalWrite(g0, LOW);
      digitalWrite(g1, LOW);
      digitalWrite(g2, HIGH);
      digitalWrite(g3, LOW);

      digitalWrite(r0, HIGH); 
      digitalWrite(r1, HIGH);
      digitalWrite(r2, LOW);
      digitalWrite(r3, HIGH);
    }

    else if (stateAm == 3)
    {
      digitalWrite(g0, LOW);
      digitalWrite(g1, LOW);
      digitalWrite(g2, LOW);
      digitalWrite(g3, HIGH);

      digitalWrite(r0, HIGH); 
      digitalWrite(r1, HIGH);
      digitalWrite(r2, HIGH);
      digitalWrite(r3, LOW);
    }
    else if (stateAm == 4){
      if (count%2==0)
        digitalWrite(3,HIGH);
      else
        digitalWrite(3,LOW);
    }
  }
  if( state == 0)
  {
    if(count%16 == 0)
    { 
      digitalWrite(g0, HIGH);
      digitalWrite(g1, LOW);
      digitalWrite(g2, LOW);
      digitalWrite(g3, LOW);

      digitalWrite(r0, LOW); 
      digitalWrite(r1, HIGH);
      digitalWrite(r2, HIGH);
      digitalWrite(r3, HIGH);
    }
    else if(count%16 == 4)
    {
      digitalWrite(g0, LOW);
      digitalWrite(g1, HIGH);
      digitalWrite(g2, LOW);
      digitalWrite(g3, LOW);

      digitalWrite(r0, HIGH); 
      digitalWrite(r1, LOW);
      digitalWrite(r2, HIGH);
      digitalWrite(r3, HIGH);
    }
    else if(count%16 == 8)
    { 
      digitalWrite(g0, LOW);
      digitalWrite(g1, LOW);
      digitalWrite(g2, HIGH);
      digitalWrite(g3, LOW);

      digitalWrite(r0, HIGH); 
      digitalWrite(r1, HIGH);
      digitalWrite(r2, LOW);
      digitalWrite(r3, HIGH);
    }
    else if(count%16 == 12)
    { 
      digitalWrite(g0, LOW);
      digitalWrite(g1, LOW);
      digitalWrite(g2, LOW);
      digitalWrite(g3, HIGH);

      digitalWrite(r0, HIGH); 
      digitalWrite(r1, HIGH);
      digitalWrite(r2, HIGH);
      digitalWrite(r3, LOW);
    }
  }
  count++;
  ////Serial.println(count);
}

void myISR() {
    state = 1;
    //Serial.println();
    int a = analogRead(Am0);
    int b = analogRead(Am1);
    int c = analogRead(R);
    //Serial.println(a);
    //Serial.println(b);
    //Serial.println(digitalRead(4));
    //Serial.println(digitalRead(5));
    //Serial.println(c);
    //Serial.println();
    if(a > 1002)
    {
      stateAm = 0;
    }
    else if(b > 1002)
    {
      stateAm = 1;
    }
    else if (c > 1002){
      stateAm = 4;
      digitalWrite(g0, LOW);
      digitalWrite(g1, LOW);
      digitalWrite(g2, LOW);
      digitalWrite(g3, LOW);

      digitalWrite(r0, LOW); 
      digitalWrite(r1, LOW);
      digitalWrite(r2, LOW);
      digitalWrite(r3, LOW);      
    }
    else if(digitalRead(Am2) == HIGH)
    {
      stateAm = 2;
    }
    else if(digitalRead(Am3) == HIGH)
    {
      stateAm = 3;
    }
    else
    {
      digitalWrite(3,LOW);
      state = 0;
      stateAm = 0;
    }
    //Serial.println("Interrupt!");
    //Serial.println(stateAm);
}

void loop()
{
  int a = analogRead(A5);
  int b = analogRead(A4);
  int c = analogRead(A3);
  //Serial.println();
  //Serial.println(a);
  //Serial.println(b);
  //Serial.println(c);
  if (stateAm==4){
    digitalWrite(0,LOW);
  }  
  else if(a <500 && ((count%16>4 && state==0) || (state==1 && stateAm!=0)))
  {
    digitalWrite(0,HIGH);
    ////Serial.println("Buzzz!");
  }
  else if(b <500 && (((count%16>8 || count%16<=4) && state==0)|| (state==1 && stateAm!=1)))
  {
    digitalWrite(0,HIGH);
  }
  else if(c <500 && (((count%16>12 || count%16<=8) && state==0)|| (state==1 && stateAm!=2)))
  {
    digitalWrite(0,HIGH);
  }
  else if(digitalRead(1) == LOW && (((count%16<=12) && state==0)|| (state==1 && stateAm!=3)))
  {
    digitalWrite(0,HIGH);
  }
  else
  {
    digitalWrite(0,LOW);
  }
}
