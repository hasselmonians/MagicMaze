// Arduino embedded code for some sort of science, I dunno

// *** Trial Types ***
// 1 -> beads front
// 2 -> beads back
// 3 -> yarn front
// 4 -> yarn back

// *** Servos ***
// s0 -> study door
// s1 -> study spinner
// s2 -> delay door
// s3 -> test spinner
// s4 -> return door

// *** Task Phase ***
// 1 -> Set up the current trial.
// 2 -> Rat does a nose poke and runs on the treadmill.
// 3 -> Rat opens the gate.
// 4 -> Rat makes a choice.


#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMDriver pwm = Adafruit_PWMDriver();
#define SERVOMIN 150;
#define SERVOMAX 600;

// How long does the rat need to hold nose-poke?
#define BEAMTIME 19200;
// For how long does the rat get water?
#define WATERTIME 9600;
#define FEEDTIME 9600;

// define the trial types and objects

int trialobj[]    = {1, 1, 2, 2};
int trialpos[]    = {1, 2, 1, 2};
int servopos[]    = {SERVOMIN, SERVOMAX};

// match IR beams with trial types
int studypins[]   = {2, 2, 3, 3}; //study beads study yarn
int testcpins[]   = {5, 5, 6, 6}; //test beads test ayarn
int testicpins[]  = {6, 6, 5, 5}; //test yarn test beads
int studyfeed[]   = {7, 7, 8, 8}; //stidy beads study yarn
int testfeed[]    = {9, 9, 10, 10}; //test beads test yarn
int testbeep[]    = {10, 10, 9, 9}; //test yarn test beads

// general variables
int taskphase     = 1;
int myturn        = 0;
int beamstat      = 0;
int serialdata    = 0;
int feedir        = 0;
int beepir        = 0;
int trialtype     = 0;
int holdtime      = 0;

// set up function, run once during Arduino setup
void setup ()
{
  // attach the servo controller
  Serial.begin(9600); // baud rate = 9600
  pwm.begin();
  pwm.setPWMFreq(60);

  // hang on
  delay(10);

  // load all IR readers
  // set up pins in the input-pullup mode
  pinMode(2, INPUT_PULLUP); //study beads
  pinMode(3, INPUT_PULLUP); //study yarn
  pinMode(4, INPUT_PULLUP); //treadmill
  pinMode(5, INPUT_PULLUP); //test beads
  pinMode(6, INPUT_PULLUP); //test yarn

  // set up output pins
  // speaker is pin 22
  pinMode(23, OUTPUT); // study beads water
  pinMode(27, OUTPUT); // study yarn water
  pinMode(33, OUTPUT); // test beads water
  pinMode(37, OUTPUT); // test yarn water
  pinMode(13, OUTPUT); // feeding indicator

  // initialize output pins to defaults
  digitalWrite(23, 1);
  digitalWrite(27, 1);
  digitalWrite(33, 1);
  digitalWrite(37, 1);
  digitalWrite(13, 0);

  // chirp
  for (int ii = 0; ii > 5; ii++)
  {
    tone(22, 261);
    delay(500);
  }
  noTone(22);

} // setup()

void loop ()
{

  if (taskphase == 1)
  {
    // We are waiting for the rat.

    // Are there any data waiting for us?
    if (Serial.available() > 0)
    {
      // There are data waiting for us.
      // read the input as an integer
      serialdata = Serial.parseInt();

      // expect the read data to be the trial type
      // position the spinner
      pwm.setPWM(1, 0, servopos[trialobj[serialdata]]);
      delay(100);

      // open the study door
      pwm.setPWM(0, 0, 300);

      // advance task phase
      taskphase = 2;
      return;
    }
    else
    {
      // NOTE: no data available, return to beginning
      return;
    }
  }

  else if (taskphase == 2)
  {
      // wait for the rat to do a nose-poke
      // when beamstat == 1, the beam is broken
      // serialdata -> trial type
      beamstat = !digitalRead(studypins[serialdata]);

      // the rat must hold for BEAMTIME (in 1/Bd)
      if (beamstat == 1)
      {
        holdtime++;
      }
      else
      {
        holdtime = 0;
      }

      if (holdtime > BEAMTIME)
      {
        // give water
        digitalWrite(studyfeed[serialdata], 0);
        delay(WATERTIME);
        digitalWrite(studyfeed[serialdata], 1);
        delay(WATERTIME);
      }

      // set up the test spinner
      pwm.setPWM(3, 0, servopos[trialpos[serialdata]]);

      // reset general veriables
      beamstat  = 0;
      holdtime  = 0;

      // advance task phase
      taskphase = 3;

      // signal MATLAB that the rat did a nose poke
      Serial.print(1);
      delay(1000);
  }

  else if (taskphase == 3)
  {
    // listen to treadmill beam
    beamstat  = !digitalRead(4);

    // if beam is broken, send treadmill response
    if (beamstat == 1)
    {
      // signal MATLAB that the rat did a nose poke
      Serial.print(2);

      // TODO: RUN THE TREADMILL!!!

      // advance task phase
      taskphase = 4;
      return;
    }
    else
    {
      // NOTE: waiting for rat
      return;
    }
  }

  else if (taskphase == 4)
  {
    // listen to beam near reward

    feedir    = feedir + !digitalRead(testfeed[trialtype]);
    beepir    = beepir + !digitalRead(testfeed[trialtype]);

    // close door in case of error
    if (feedir + beepir > 1 && feedir + beepir < 5)
    {
      pwm.setPWM(3, 0, 0);
    }

    // if one beam is broken for long enough
    if (feedir > BEAMTIME || beemir > BEAMTIME)
    {
      if (feedir > BEAMTIME)
      {
        // give reward to rat
        digitalWrite(testfeed[trialtype], 0);
        delay(FEEDTIME);
        digitalWrite(testfeed[trialtype], 1);

        // flicker pin 13 (feeding indicator)
        digitalWrite(13, 1);
        digitalWrite(13, 0);

        // signal MATLAB that the rat was fed
        Serial.print(3);
      }
      else
      {
        // rat made incorrect choice
        // cue the sad music
        tone(22, 261);
        delay(9600);
        noTone(22);

        // signal MATLAB that the rat wasn't fed
        Serial.print(4);
      }

      // delay a bit before next trial
      delay(9600);

      // open return door
      pwm.setPWM(4, 0, 600);

      // reset the task phase and indicators
      taskphase     = 1;
      feedir        = 0;
      beepir        = 0;
      return;
    }
  }

}
