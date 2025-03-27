#define WHEELRIGHT 0
#define WHEELLEFT 0
#define STEERINGWHEELBUTTON 0 
#define GASPEDALBUTTON 0
#define TURNSIGNALLEFT 0
#define TURNSIGNALRIGHT 0
#define AUDIOEN 0
#define AUDIOPIN0 0
#define AUDIOPIN1 0
#define AUDIOPIN2 0
#define ANALOGPIN 0
#define SEGONES0 0
#define SEGONES1 0
#define SEGONES2 0
#define SEGONES3 0
#define SEGTENS0 0
#define SEGTENS1 0
#define SEGTENS2 0
#define SEGTENS3 0
#define REDLIGHT 0
#define YELLOWLIGHT 0
#define REDLIGHT 0

// Variables for gameplay
int userScore = 0; // User's current score
int action = 0; // Randomly-generated action to take
int trafficLightCountdown = 0; // Controls when traffic light will change
bool trafficLightRed = false; // Tracks state of traffic light
unsigned long startTime = 0; // Facilitates timing of user inputs
unsigned long timeThreshold = 0; // Threshold by which user must finish input
bool achieved = false; // Checks whether user finished input in time
bool instantFail = false; // Checks whether user entered an incorrect input

// Variables for debounce
int lastState = HIGH;  // Last pin state
unsigned long lastDebounceTime = 0;  // Last time the pin state was checked

// Helper function to debounce buttons and switches
bool debounce(int pin) 
{ 

  // Read state of pin
  int reading = digitalRead(pin);  

  // Check if pin state has changed 
  if (reading != lastState) {
    lastDebounceTime = millis();  // Reset debounce timer
  }

  // If the button state has changed and debounce delay has passed
  if ((millis() - lastDebounceTime) > debounceDelay) 
  {

    // Check for transition from HIGH to LOW 
    if (reading == LOW && lastState == HIGH) 
    {
      lastState = HIGH;  // Reset for next input
      return true;  // Button was pressed
    }
  }

  lastState = reading;  // Save the current state 
  return false;  // No input registered
}

void setup() 
{

  // Initialize start button
  pinMode(STEERINGWHEELBUTTON, INPUT_PULLUP)

  // Initialize steering wheel
  pinMode(WHEELRIGHT, INPUT_PULLUP);
  pinMode(WHEELLEFT, INPUT_PULLUP);

  // Initialize turn signal
  pinMode(TURNSIGNALLEFT, INPUT_PULLUP);
  pinMode(TURNSIGNALRIGHT, INPUT_PULLUP);

  // Initialize gas pedal
  pinMode(GASPEDALBUTTON, INPUT_PULLUP);

  // Initialize audio control
  pinMode(AUDIOEN, OUTPUT);
  pinMode(AUDIOPIN0, OUTPUT);
  pinMode(AUDIOPIN1, OUTPUT);
  pinMode(AUDIOPIN2, OUTPUT);
  digitalWrite(AUDIOEN, HIGH);
  digitalWrite(AUDIOPIN0, HIGH);
  digitalWrite(AUDIOPIN1, HIGH);
  digitalWrite(AUDIOPIN2, HIGH);

  // Initialize 7-segment displays
  pinMode(SEGONES0, OUTPUT);
  pinMode(SEGONES1, OUTPUT);
  pinMode(SEGONES2, OUTPUT);
  pinMode(SEGONES3, OUTPUT);
  pinMode(SEGTENS0, OUTPUT);
  pinMode(SEGTENS1, OUTPUT);
  pinMode(SEGTENS2, OUTPUT);
  pinMode(SEGTENS3, OUTPUT);
  digitalWrite(SEGONES0, HIGH);
  digitalWrite(SEGONES1, HIGH);
  digitalWrite(SEGONES2, HIGH);
  digitalWrite(SEGONES3, HIGH);
  digitalWrite(SEGTENS0, HIGH);
  digitalWrite(SEGTENS1, HIGH);
  digitalWrite(SEGTENS2, HIGH);
  digitalWrite(SEGTENS3, HIGH);

  // Seed random number
  // pinMode?
  randomSeed(analogRead(ANALOGPIN));
}

void loop() 
{

  // Wait for horn honk to start game
  while (!debounce(STEERINGWHEELBUTTON))
  {

  }

  while (true)
  {

    // Turn traffic light red if countdown is up
    if (trafficLightCountdown == 0)
    {

      // Assert red light control signal

      // Generate a new random traffic light wait period
      trafficLightCountdown = random(0, 11);
    }

    // Game continues until user obtains score of 99 points
    if (userScore < 99)
    {

      // Choose a required action
      action = random(0, 3);

      // If turn signal, check for switch input
      if (action == 0)
      {

        // Play turn signal audio

        // Require user to enter input before time is up
        startTime = millis();
        while ((millis() - startTime) < timeThreshold) && !achieved && !instantFail)
        {

          // User should enter turn signal
          if (debounce(TURNSIGNALLEFT) || debounce(TURNSIGNALRIGHT))
          {

            // If traffic light is green or yellow, it is a success!
            if (!trafficLightRed)
            {
              achieved = true;
            }

            // If traffic light is red, instant failure
            else
            {
              instantFail = true;
            }
          }

          // If user enters gas pedal or steering wheel, instant failure
          else if (debounce(GASPEDALBUTTON) || debounce(WHEELLEFT) || debounce(WHEELRIGHT))
          {
            instantFail = true;
          }
        }
      }

      // If steering wheel, check for switch input
      else if (action == 1)
      {

        // Play steering wheel audio

        // Require user to enter input before time is up
        startTime = millis();
        while ((millis() - startTime) < timeThreshold) && !achieved && !instantFail)
        {

          // User should enter turn signal
          if (debounce(WHEELRIGHT) || debounce(WHEELLEFT))
          {

            // If traffic light is green or yellow, it is a success!
            if (!trafficLightRed)
            {
              achieved = true;
            }

            // If traffic light is red, instant failure
            else
            {
              instantFail = true;
            }
          }

          // If user enters gas pedal or turn signal, instant failure
          else if (debounce(GASPEDALBUTTON) || debounce(TURNSIGNALLEFT) || debounce(TURNSIGNALRIGHT))
          {
            instantFail = true;
          }
        }
      }

      // If gas pedal, check for button input
      else if (action == 2)
      {

        // Play gas pedal audio

        // Require user to enter input before time is up
        startTime = millis();
        while ((millis() - startTime) < timeThreshold) && !achieved && !instantFail)
        {

          // User should enter turn signal
          if (debounce(GASPEDALBUTTON))
          {

            // If traffic light is green or yellow, it is a success!
            if (!trafficLightRed)
            {
              achieved = true;
            }

            // If traffic light is red, instant failure
            else
            {
              instantFail = true;
            }
          }

          // If user enters turn signal or steering wheel, instant failure
          else if (debounce(TURNSIGNALLEFT) || debounce(TURNSIGNALRIGHT) || debounce(WHEELLEFT) || debounce(WHEELRIGHT))
          {
            instantFail = true;
          }
        }

      }

      // If user successfully entered input, decrease threshold time
      if (achieved)
      {
        timeThreshold -= 10; // Discuss with Aidan & Paul
        trafficLightCountdown--; 
        achieved = false;
        userScore++;

        // Update user score display
      }

      // If user entered wrong input, end the game
      if (instantFail)
      {

      }
    }

    // If user wins, play victory audio message
    else
    {

    }
  }
}
