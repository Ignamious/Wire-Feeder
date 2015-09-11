const float C_PI = 3.1415926;
const float C_DIAM = 1.234;
const int STEPS = 200;
const int MICROSTEP = 8;
float CIRC = (2*C_PI*(C_DIAM/2));
float CIRC_PER_STEP = CIRC / (STEPS*MICROSTEP);
int length = 78;
float steps_til_done = (length/CIRC_PER_STEP);


void setup(){
pinMode(1, OUTPUT);

}

void loop(){
  float steps_til_done = (length/CIRC_PER_STEP);

  if ((steps_til_done - int(steps_til_done)) >= .5){
    steps_til_done = steps_til_done+1;
  }

  for (int x = 0; x < steps_til_done; x++) {
    int length_left = x*CIRC_PER_STEP;
    digitalWrite(1, HIGH);   // sets the LED on
  delay(.2);                  // waits for a second
  digitalWrite(1, LOW);    // sets the LED off
  delay(.2);  
  }
}





