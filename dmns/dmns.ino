// Arduino embedded code for some sort of science, I dunno

// *** Trial Types ***
// 1 -> beads front
// 2 -> beads back
// 3 -> yarn front
// 4 -> yarn back            ________
//                          / _____  |
// *** Servos ***           ||    _|_|_    < study door
// s0 -> study door         ||   |     |
// s1 -> study spinner      ||   |    0|0  < study spinner
// s2 -> delay door
// s3 -> test spinner
// s4 -> return door

// *** Task Phase ***
// 1 -> Set up the current trial.
// 2 -> Rat does a nose poke and runs on the treadmill.
// 3 -> Rat opens the gate.
// 4 -> Rat makes a choice.


#include <Wire.h> // for the i dont know?
#include <Adafruit_PWMServoDriver.h>  // This is for the servo driver

Adafruit_PWMDriver pwm = Adafruit_PWMDriver(); // start the servo driver
// set the pwm duty cycles
#define SERVOMIN 150;
#define SERVOMAX 600;

// need to incorporate builtin timer here
// How long does the rat need to hold nose-poke?
#define BEAMTIME 9600;
// For how long does the rat get water?
#define WATERTIME 3200; // a third of a second ish
#define FEEDTIME 3200;  // a third of a second ish

// define the trial types and objects

int trialobj[]    = {1, 1, 2, 2};
int trialpos[]    = {1, 2, 1, 2};
int servopos[]    = {SERVOMIN, SERVOMAX};

// match IR beams with trial types
int studypins[]   = {2, 2, 3, 3}; //study beads study yarn
int testcpins[]   = {5, 5, 6, 6}; //test beads test ayarn
int testicpins[]  = {6, 6, 5, 5}; //test yarn test beads


// match the correct output pins
int studyfeed[]   = {7, 7, 8, 8}; //stidy beads study yarn
int testfeed[]    = {9, 9, 10, 10}; //test beads test yarn

// dont need the incorrect solenoids, we wont trigger them
//int testbeep[]    = {10, 10, 9, 9}; //test yarn test beads

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
  pinMode(7, OUTPUT); // study beads water
  pinMode(8, OUTPUT); // study yarn water
  pinMode(9, OUTPUT); // test beads water
  pinMode(10, OUTPUT); // test yarn water

  pinMode(11, OUTPUT); // feeding indicator

  // initialize output pins to defaults
  digitalWrite(7, 1);
  digitalWrite(8, 1);
  digitalWrite(9, 1);
  digitalWrite(10, 1);
  // only goes to the map box, so it can stay low
  digitalWrite(11, 0);

  // chirp 5 times in 1 second
  for (int ii = 0; ii > 6; ii++)
  {
    tone(22, 261);
    delay(100);
    noTone(22);
    delay(100);
  }


} // setup()

void loop ()
{

  if (taskphase == 1)
  {
    // We are waiting for the study trial setup

    // Are there any data waiting for us?
    if (Serial.available() > 0)
    {
      // There are data waiting for us.
      // read the input as an integer thats 1 through 4
      serialdata = Serial.parseInt();

      // expect the read data to be the trial type
      // position the spinner
      pwm.setPWM(1, 0, servopos[trialobj[serialdata]]);
      delay(100);

      // open the study door
      pwm.setPWM(0, 0, 300);

      // advance task phase
      taskphase = 2;
      // not sure you need to return here
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
      // delay(WATERTIME);


      // set up the test spinner
      pwm.setPWM(3, 0, servopos[trialpos[serialdata]]);

      // reset general veriables
      beamstat  = 0;
      holdtime  = 0;

      // advance task phase
      taskphase = 3;

      // signal MATLAB that the rat did a nose poke and we fed him
      Serial.println(1);
      // cant wait too long because the rat may have already proceeded to the tradmill
      delay(100);
    }
  }

  else if (taskphase == 3)
  {
    // listen to treadmill beam
    beamstat  = !digitalRead(4);

    // if beam is broken, send treadmill response
    if (beamstat == 1)
    {
      // signal MATLAB that the rat did a nose poke
      Serial.println(2);

      // TODO: RUN THE TREADMILL, wait for a computer signal to open the door

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

    // close door once either beam is broken so he cant go back to study
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
        Serial.println(3);
      }
      else
      {
        // rat made incorrect choice
        // cue the sad music
        tone(22, 261);
        delay(9600);
        noTone(22);

        // signal MATLAB that the rat wasn't fed
        Serial.println(4);
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
