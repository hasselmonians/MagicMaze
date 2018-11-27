// so we'll have a very straightforward back and forth

// we'll have 2 value inputs, and we'll send 3 value outputs
// the inputs from matlab will be:
// a b
// a: 0 if study, 1 if delay, 2 if test 3 if ready for next trial
// b: 1:4 for the trial types
// 1=beads front, 2= beads back, 3= straws front, 4=straws back

// outputs to matlab will be:
// 1 value
// 1 is gave water, 2 is didnt, 3 is start treadmill, 4 is new trial

// so the sequece goes as follows

// Phase 1, the ITI and sample phase
// receive a 01 through 04
// setup study, open door to study platform
// transition into my turn
// send 1 for he sampled
// send 2 for start treadmill

// Phase 2, delay and test
// receive 10 for delay over, start test
// transition over to my turn
// send 10 or 20 for trial choice
// wait 4 secs, open return door
// send 4 for ready for next trial


// if ever receive 00,  freeze servos, reset and wait for 01:04

// Add the servo rack to control all the doors and spinners
// there will be 5 servos to control
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
#define SERVOMIN  150;
#define SERVOMAX  600;


// okay each of the trialtypes
// 1 is beads front, 2 is beads back, 3 yarn front, 4 is yarn back
int trialobj[] = {1, 1, 2, 2};
int trialpos[] = {1, 2, 1, 2}; // this is the beads/yarn, front/back
int servopos[] = {150, 600};

// match irs with trial types
int studypins[] = {2, 2, 3, 3}; //study beads study yarn
int testcpins[] = {5, 5, 6, 6}; //test beads test ayarn
int testicpins[] = {6, 6, 5, 5}; //test yarn test beads
int studyfeed[] = {7, 7, 8, 8}; //stidy beads study yarn
int testfeed[] = {9, 9, 10, 10}; //test beads test yarn
int testbeep[] = {10, 10, 9, 9}; //test yarn test beads
double holdtime = 0;

// five servos, this is servo lut
// s0 = study door
// s1 = study spinner
// s2 = delay door
// s3 = test spinner
// s4 = return door

// set up our general variables
int taskphase = 1;
int myturn = 0;
int beamstat = 0;
int serialdata = 0;
int feedir = 0;
int beepir = 0;
int trialtype = 0;

void setup() {
  // First attach the servo controller
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(60); // have to make sure this is right for our servos
  delay(10);

  // load all IR readers
  pinMode(2, INPUT_PULLUP); //study beads
  pinMode(3, INPUT_PULLUP); //study yarn
  pinMode(4, INPUT_PULLUP); //treadmill
  pinMode(5, INPUT_PULLUP); //test beads
  pinMode(6, INPUT_PULLUP); //test yarn

  // need to test whether we can bridge these pins directly into the map box
  pinMode(23, OUTPUT); //study beads water
  digitalWrite(23, 1); delay(100);
  
  pinMode(27, OUTPUT); //study yarn water
  digitalWrite(27, 1); delay(100);
  
  pinMode(33, OUTPUT); //test beads water
  digitalWrite(33, 1); delay(100);
  
  pinMode(37, OUTPUT); //test yarn water
  digitalWrite(37, 1); delay(100);

  
  pinMode(13, OUTPUT); //feeding indicator
  digitalWrite(13, 0); delay(100);
  
  pinMode(51, OUTPUT); // treadmill start
  digitalWrite(51, 1);


 // the speaker pin will be pin 22
}

void loop() {
  // if i'm waiting for input
  if (myturn == 0) {
    if (Serial.available() > 0) {
      // read input
      serialdata = Serial.parseInt();

      //                                //
      // IF we're setting up the trial  //
      //                                //

      if (serialdata < 5) {
        // setup all the study trials
        // position study spinner (for serialdata = 1 or 2, servo will go to open
        pwm.setPWM(1, 0, servopos[trialobj[serialdata]]);
        //give it time to position
        delay(100);
        // open the study door (door 0, up to 300 ms)
        pwm.setPWM(0, 0, 300);
        // change to its my turn, reset beams
        myturn = 1; taskphase = 1; beamstat = 0; holdtime = 0;
        feedir = 0; beepir = 0;
      }

      //                                              //
      // this is the return if the delay just stopped //
      // This sets up the test phase                  //

    
      if (serialdata > 10 && serialdata < 20) {
        // open treadmill door
        pwm.setPWM(2, 0, servopos[2]);
        // set the trial type
        trialtype = serialdata - 10;
        // set up trial
        myturn = 1; taskphase = 3;
        feedir = 0; holdtime = 0;
        beepir = 0;
      }

      //       //
      // RESET //
      //       //

      if (serialdata == 30) {
        // go back to first trial
        myturn = 0; taskphase = 1; feedir = 0; beepir = 0; holdtime = 0;
      }
    }
  }
  // ////////////////////////////////////////////////
  //                                               //
  // end my turn half, this will loop many times** //
  //                                               //
  // ////////////////////////////////////////////////
  if (myturn == 1) {
    if (taskphase == 1) {
      // listen to pin, beamstat ==1 when beam is broken
      beamstat = !digitalRead(studypins[serialdata]);
      // if pin broken add to hold time, otherwise reset
      if (beamstat == 1) {
        ++holdtime;
      }
      else {
        holdtime = 0;
      }

      // if he held long enough continuously % i think this is about 9600 loops per second
      if (holdtime > 10000) {
        // give water
        digitalWrite(studyfeed[serialdata], 0);
        delay(1000);
        digitalWrite(studyfeed[serialdata], 1);
        delay(100);

        // setup test spinner
        pwm.setPWM(3, 0, servopos[trialpos[serialdata]]);
        // advance task phase
        taskphase = 2; beamstat = 0;
        // signal the matlab that he poked his nose
        Serial.println(1);
        //close all the doors
        // pwm.setPWM(4, 0, 0);
        delay(100);
        // pwm.setPWM(0, 0, 0);
      }
    }
    
    if (taskphase == 2) {
      // listen to pin
      beamstat = !digitalRead(4);
      // if pin is broken,
      if (beamstat == 1) {
        // send treadmill response, go to phase 3 and wait for input
        Serial.println(2); myturn = 0; holdtime = 0;
      }
    }
    
    if (taskphase == 3) {
      // listen to pins
      feedir = feedir + !digitalRead(testfeed[trialtype]);
      beepir = beepir + !digitalRead(testbeep[trialtype]);

      // if we broke anything we can close the door
      if (feedir + beepir > 1 && feedir + beepir < 5) {
        pwm.setPWM(3, 0, 0);
      }
      // if one beak is broken
      // if a beam is broken for long enough
      if (feedir > 10000 || beepir > 10000) {
        // if its the feed ir beam
        if (feedir > 10000) {
          // feed rat
          digitalWrite(testfeed[trialtype], 0);
          delay(1000);
          digitalWrite(testfeed[trialtype], 1);
          // flip correct pin on and off
          digitalWrite(13, 1);
          digitalWrite(13, 0);

          Serial.println(10);
        }
        else {
          tone(22, 261); // middle c !!!!
          delay(500);
          noTone(22);
          Serial.println(20);
        }
        delay(2000);
        // open return door
        pwm.setPWM(4, 0, 600);
        taskphase = 1; myturn = 0;
        feedir = 0; beepir = 0;
      }
    }
  }
}






// it may be possible to do a very simple serial comm where you send like a
// 1 and then a random seed for the trial starter
// and then maybe we can stop if you send another input
