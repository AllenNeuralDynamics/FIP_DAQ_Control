/////////Serial_Lchika_Teensy_FIP//////////
// To trigger camera and LEDs/Lasers sequencially
//
// Kenta Hagihara@AIND started 2022 May 
// 220731 modified for Thor FIP implementation
// 221217 modified for OptoStim (also bug-fix in mode "d")
// 230307 modified for OptoStim2 (param flexibility + log in serial)
// 230308 modified for OptoStim2_2 (added 20Hz, 5Hz stim modes, PulseWidth externally controlable)
// 230608 modified for OptoStim2_3 (added some log functions)
//
// Operation Modes
// a:fixed length alt-trigger operation
// b:slow alt-trigger mode for testing purpose
// c:continuas alt-trigger mode, stops with "s", starts with 10 "noLED" frames
// d:all LEDs continuously on, stops with "s", for FiberBleaching
// o:OptoStim1, 10Hz, stops with "s" (but need to wait for 1 trial ~20s)
// p:OptoStim2, 20Hz, stops with "s" (but need to wait for 1 trial ~20s)
// q:OptoStim3,  5Hz, stops with "s" (but need to wait for 1 trial ~20s)


///////////////////////////////////////

void setup() {
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps

  pinMode(12, OUTPUT); //LED1 TTL (470nm)
  pinMode(11, OUTPUT); //LED2 TTL (415nm)
  pinMode(10, OUTPUT); //LED3 TTL (565nm)
  pinMode(9, OUTPUT); //camera TTL (Green) 
  pinMode(8, OUTPUT); //camera TTL (Red)
  pinMode(7, OUTPUT); //OptoStim 

  analogWriteResolution(12);   
}

void loop() {
  
  char key; 
  char key2;
  int var;
  int var1;
  int var2;
  int var3;
  int del1;
  int del2;
  int del3;
  int del4;
  int Amp1;
  int Amp2;
  
  int Amp3;
  int AmpOpt1;
  int AmpOpt2;
  int AmpOpt3;
  int AmpOpt4;
  int OptoTrialN;
  int PulseW;
  int OptoDuration;
  int OptoBase;
  int OptoInterval;
  
  int repN;

  ////Analog Amplitude////
  Amp1 = 4095;  //0-4095 (12bit) for photometry excitation
  Amp2 = 4095;
  Amp3 = 4095;
  
  AmpOpt1 = 1000; // for OptoExcitation
  AmpOpt2 = 2000;
  AmpOpt3 = 3000;
  AmpOpt4 = 4095; // Maximum


  // Operation Speed Selection //  
  
  ////// 60Hz ///////
  // del1 + del2 + del3 + del4 = 16666
  del1 = 15350; //in microsec, signal
  del2 = 666; //in microsec, off
  del3 = 600; //in microsec, safety margin between LED-on and Camera-on
  del4 = 50; //in microsec, safety margin between LED-off and Camera-off

  repN=100; // Trial number for mode "a"


  ///OptoStim Params (10Hz)
  OptoTrialN = 10; //Trial number
  //OptoTrialN = 2;
  PulseW = 10000; //in microsec, default: 10 msec = 10000
  OptoDuration = 2; // in sec, duration of 10Hz pulse
  OptoBase = 120; //in sec, length of baseline before stimulation
  //OptoBase = 10;
  OptoInterval = 28; // in sec, baseline after each stim train


 //////////MAIN LOOP//////////////////////////////////////////////////////////////////////// 

  if ( Serial.available() ) {       
    char key = Serial.read();
    Serial.write( key );
    int value = Serial.parseInt(); // read the integer value (if provided)            

    // switch based on "key"
    switch( key ) {
 
      case 'a': //fixed length operation
      var = 0;
      while(var < repN){
          // 470 + Cam1
          analogWrite(12,Amp1);
          digitalWrite(9,HIGH);
          delayMicroseconds(del1);
          analogWrite(12,0);
          digitalWrite(9,LOW);
          delayMicroseconds(del2);
          
          // 415 + Cam1
          analogWrite(11,Amp2);
          digitalWrite(9,HIGH);
          delayMicroseconds(del1);
          analogWrite(11,0);
          digitalWrite(9,LOW);
          delayMicroseconds(del2);

          // 565 + Cam2
          analogWrite(10,Amp3);
          digitalWrite(8,HIGH);
          delayMicroseconds(del1);
          analogWrite(10,0);
          digitalWrite(8,LOW);
          delayMicroseconds(del2);
          var++;
          } //while
          break;

      case 'b':  // slow testing mode
      var = 0;
      while(var < 50){
          // 470 + Cam1
          analogWrite(12,Amp1);
          digitalWrite(9,HIGH);
          delayMicroseconds(del1*5);
          analogWrite(12,0);
          digitalWrite(9,LOW);
          delayMicroseconds(del2*100);
          // 415 + Cam1
          analogWrite(11,Amp2);
          digitalWrite(9,HIGH);
          delayMicroseconds(del1*5);
          analogWrite(11,0);
          digitalWrite(9,LOW);
          delayMicroseconds(del2*100);
          // 565 + Cam2
          analogWrite(10,Amp3);
          digitalWrite(8,HIGH);
          delayMicroseconds(del1*5);
          analogWrite(10,0);
          digitalWrite(8,LOW);
          delayMicroseconds(del2*100);
          var++;
          } //while
          break;
      
      
      ////////////////////////////////////////////
      case 'c':   // start:c, stop with "s"
      var = 0;
      while(var < 10){        //initial roop (no excitation; for camera floor noise ctrl)
          // 470 + Cam1
          delayMicroseconds(del3);
          digitalWrite(9,HIGH);
          delayMicroseconds(del1);
          digitalWrite(9,LOW);          
          delayMicroseconds(del4);
          delayMicroseconds(del2);
          // 415 + Cam1
          delayMicroseconds(del3);
          digitalWrite(9,HIGH);
          delayMicroseconds(del1);
          digitalWrite(9,LOW);    
          delayMicroseconds(del4);
          delayMicroseconds(del2);
          // 565 + Cam2
          delayMicroseconds(del3);
          digitalWrite(8,HIGH);
          delayMicroseconds(del1);
          digitalWrite(8,LOW);
          delayMicroseconds(del4);
          delayMicroseconds(del2);
          var++;
          } //initial roop

      while(1>0){      
      var = 0;
      while(var < 10){        //small roop
          // 470 + Cam1
          analogWrite(12,Amp1);
          delayMicroseconds(del3);
          digitalWrite(9,HIGH);
          delayMicroseconds(del1);
          digitalWrite(9,LOW);          
          delayMicroseconds(del4);
          analogWrite(12,0);
          delayMicroseconds(del2);
          // 415 + Cam1
          analogWrite(11,Amp2);
          delayMicroseconds(del3);
          digitalWrite(9,HIGH);
          delayMicroseconds(del1);
          digitalWrite(9,LOW);    
          delayMicroseconds(del4);
          analogWrite(11,0);
          delayMicroseconds(del2);
          // 565 + Cam2
          analogWrite(10,Amp3);
          delayMicroseconds(del3);
          digitalWrite(8,HIGH);
          delayMicroseconds(del1);
          digitalWrite(8,LOW);
          delayMicroseconds(del4);
          analogWrite(10,0);
          delayMicroseconds(del2);
          var++;
          } //small roop

      if ( Serial.available() ) {       
        key2 = Serial.read();            
        Serial.write( key2 );            
        switch( key2 ) {
          case 's':
          goto hogehoge;
          }
          }

      } //while large
      break;

      ////////////////////////////////////////////
      case 'd':   // start:d, stop with "s"

      while(1>0){      
      var = 0;
      while(var < 10){        //small roop
          analogWrite(12,Amp1);
          analogWrite(11,Amp3);
          analogWrite(10,Amp2);
          delay(100);
          var++;
          } //small roop

      if ( Serial.available() ) {       
        key2 = Serial.read();            
        Serial.write( key2 );            
        switch( key2 ) {
          case 's':
          goto hogehoge;
          }
          }

      } //while large
      break;

     ////////////////////////////////////////////
      case 'o':

      if (value == 0) {
          Serial.println("");
         Serial.println("Received command o without pulsewidth value");
         } 
      else {
          Serial.println("");
          Serial.print("Received command o with pulsewidth value ");
          Serial.println(value);
          PulseW = value;
         }

      Serial.println("");
      Serial.println("OptoStim10Hz_____________");
      Serial.print("OptoTrialN: ");
      Serial.println(OptoTrialN);
      Serial.print("PulseW(um): ");
      Serial.println(PulseW);
      Serial.print("OptoDuration(s): ");
      Serial.println(OptoDuration);
      Serial.print("OptoInterval(s): ");
      Serial.println(OptoInterval);
      Serial.print("OptoBase(s): ");
      Serial.println(OptoBase);
      Serial.print("excitation_delay1: ");
      Serial.println(del1);
      Serial.print("excitation_delay2: ");
      Serial.println(del2);
      Serial.print("excitation_delay3: ");
      Serial.println(del3);
      Serial.print("excitation_delay4: ");
      Serial.println(del4);

      var = 0;
      while(var < OptoBase*20){        //baseline
          // 470 + Cam1
          analogWrite(12,Amp1);
          delayMicroseconds(del3);
          digitalWrite(9,HIGH);
          delayMicroseconds(del1);
          digitalWrite(9,LOW);
          delayMicroseconds(del4);
          analogWrite(12,0);
          delayMicroseconds(del2);
          // 415 + Cam1
          analogWrite(11,Amp2);
          delayMicroseconds(del3);
          digitalWrite(9,HIGH);
          delayMicroseconds(del1);
          digitalWrite(9,LOW);    
          delayMicroseconds(del4);
          analogWrite(11,0);
          delayMicroseconds(del2);
          // 565 + Cam2
          analogWrite(10,Amp3);
          delayMicroseconds(del3);
          digitalWrite(8,HIGH);
          delayMicroseconds(del1);
          digitalWrite(8,LOW);
          delayMicroseconds(del4);
          analogWrite(10,0);
          delayMicroseconds(del2);
          var++;
          } //baseline
      

      var1 = 0;
      while(var1 < OptoTrialN){    //Stim+Baseline Loop Trials

      var = 0;
      while(var < OptoDuration*10){    //Stim Loop 10Hz

      // Stim
      // 470 + Cam1
      analogWrite(12,Amp1);
      delayMicroseconds(del3);
      digitalWrite(9,HIGH);
      delayMicroseconds(del1);
      digitalWrite(9,LOW);
      delayMicroseconds(del4);
      analogWrite(12,0);
      delayMicroseconds(del2);
      // 415 + Cam1
      analogWrite(11,Amp2);
      delayMicroseconds(del3);
      digitalWrite(9,HIGH);
      delayMicroseconds(del1);
      digitalWrite(9,LOW);    
      delayMicroseconds(del4);
      analogWrite(11,0);
      delayMicroseconds(del2);
      // 565 + Cam2 + Opt
      analogWrite(10,Amp3);
      analogWrite(7,AmpOpt4); //StimOn
      delayMicroseconds(del3);
      digitalWrite(8,HIGH);
      delayMicroseconds(PulseW-del3); // OptoStim PulseW ms
      analogWrite(7,0);       //StimOff
      delayMicroseconds(del1-PulseW+del3);
      digitalWrite(8,LOW);
      delayMicroseconds(del4);
      analogWrite(10,0);
      delayMicroseconds(del2);

      // 470 + Cam1
      analogWrite(12,Amp1);
      delayMicroseconds(del3);
      digitalWrite(9,HIGH);
      delayMicroseconds(del1);
      digitalWrite(9,LOW);
      delayMicroseconds(del4);
      analogWrite(12,0);
      delayMicroseconds(del2);
      // 415 + Cam1
      analogWrite(11,Amp2);
      delayMicroseconds(del3);
      digitalWrite(9,HIGH);
      delayMicroseconds(del1);
      digitalWrite(9,LOW);    
      delayMicroseconds(del4);
      analogWrite(11,0);
      delayMicroseconds(del2);
      // 565 + Cam2
      analogWrite(10,Amp3);
      delayMicroseconds(del3);
      digitalWrite(8,HIGH);
      delayMicroseconds(del1);
      digitalWrite(8,LOW);
      delayMicroseconds(del4);
      analogWrite(10,0);
      delayMicroseconds(del2);
      
      var++;
      } //Stim Loop 10Hz

      var = 0;
      while(var < OptoInterval*20){        //post stim baseline
          // 470 + Cam1
          analogWrite(12,Amp1);
          delayMicroseconds(del3);
          digitalWrite(9,HIGH);
          delayMicroseconds(del1);
          digitalWrite(9,LOW);
          delayMicroseconds(del4);
          analogWrite(12,0);
          delayMicroseconds(del2);
          // 415 + Cam1
          analogWrite(11,Amp2);
          delayMicroseconds(del3);
          digitalWrite(9,HIGH);
          delayMicroseconds(del1);
          digitalWrite(9,LOW);    
          delayMicroseconds(del4);
          analogWrite(11,0);
          delayMicroseconds(del2);
          // 565 + Cam2
          analogWrite(10,Amp3);
          delayMicroseconds(del3);
          digitalWrite(8,HIGH);
          delayMicroseconds(del1);
          digitalWrite(8,LOW);
          delayMicroseconds(del4);
          analogWrite(10,0);
          delayMicroseconds(del2);
          var++;
          } //baseline

       if ( Serial.available() ) {       
          key2 = Serial.read();           
          Serial.write( key2 );            
          switch( key2 ) {
          case 's':
          goto hogehoge;
          }
          }
     
     var1++;
     }    //Stim+Baseline Loop Trials
     break;
     
     ////////////////////////////////////////////
      case 'p':

      if (value == 0) {
          Serial.println("");
         Serial.println("Received command p without pulsewidth value");
         } 
      else {
          Serial.println("");
          Serial.print("Received command p with pulsewidth value ");
          Serial.println(value);
          PulseW = value;
         }
    
      Serial.println("");
      Serial.println("OptoStim20Hz_____________");
      Serial.print("OptoTrialN: ");
      Serial.println(OptoTrialN);
      Serial.print("PulseW(um): ");
      Serial.println(PulseW);
      Serial.print("OptoDuration(s): ");
      Serial.println(OptoDuration);
      Serial.print("OptoInterval(s): ");
      Serial.println(OptoInterval);
      Serial.print("OptoBase(s): ");
      Serial.println(OptoBase);
      
 
      var = 0;
      while(var < OptoBase*20){        //baseline
          // 470 + Cam1
          analogWrite(12,Amp1);
          delayMicroseconds(del3);
          digitalWrite(9,HIGH);
          delayMicroseconds(del1);
          digitalWrite(9,LOW);
          delayMicroseconds(del4);
          analogWrite(12,0);
          delayMicroseconds(del2);
          // 415 + Cam1
          analogWrite(11,Amp2);
          delayMicroseconds(del3);
          digitalWrite(9,HIGH);
          delayMicroseconds(del1);
          digitalWrite(9,LOW);    
          delayMicroseconds(del4);
          analogWrite(11,0);
          delayMicroseconds(del2);
          // 565 + Cam2
          analogWrite(10,Amp3);
          delayMicroseconds(del3);
          digitalWrite(8,HIGH);
          delayMicroseconds(del1);
          digitalWrite(8,LOW);
          delayMicroseconds(del4);
          analogWrite(10,0);
          delayMicroseconds(del2);
          var++;
          } //baseline
      
      
      
      var1 = 0;
      while(var1 < OptoTrialN){    //Stim+Baseline Loop Trials

      var = 0;
      while(var < OptoDuration*20){    //Stim Loop 20Hz

      // Stim
      // 470 + Cam1
      analogWrite(12,Amp1);
      delayMicroseconds(del3);
      digitalWrite(9,HIGH);
      delayMicroseconds(del1);
      digitalWrite(9,LOW);
      delayMicroseconds(del4);
      analogWrite(12,0);
      delayMicroseconds(del2);
      // 415 + Cam1
      analogWrite(11,Amp2);
      delayMicroseconds(del3);
      digitalWrite(9,HIGH);
      delayMicroseconds(del1);
      digitalWrite(9,LOW);    
      delayMicroseconds(del4);
      analogWrite(11,0);
      delayMicroseconds(del2);
      // 565 + Cam2 + Opt
      analogWrite(10,Amp3);
      analogWrite(7,AmpOpt4); //StimOn
      delayMicroseconds(del3);
      digitalWrite(8,HIGH);
      delayMicroseconds(PulseW-del3); // OptoStim PulseW ms
      analogWrite(7,0);       //StimOff
      delayMicroseconds(del1-PulseW+del3);
      digitalWrite(8,LOW);
      delayMicroseconds(del4);
      analogWrite(10,0);
      delayMicroseconds(del2);
      var++;
      } //Stim Loop 20Hz

      var = 0;
      while(var < OptoInterval*20){        //post stim baseline
          // 470 + Cam1
          analogWrite(12,Amp1);
          delayMicroseconds(del3);
          digitalWrite(9,HIGH);
          delayMicroseconds(del1);
          digitalWrite(9,LOW);
          delayMicroseconds(del4);
          analogWrite(12,0);
          delayMicroseconds(del2);
          // 415 + Cam1
          analogWrite(11,Amp2);
          delayMicroseconds(del3);
          digitalWrite(9,HIGH);
          delayMicroseconds(del1);
          digitalWrite(9,LOW);    
          delayMicroseconds(del4);
          analogWrite(11,0);
          delayMicroseconds(del2);
          // 565 + Cam2
          analogWrite(10,Amp3);
          delayMicroseconds(del3);
          digitalWrite(8,HIGH);
          delayMicroseconds(del1);
          digitalWrite(8,LOW);
          delayMicroseconds(del4);
          analogWrite(10,0);
          delayMicroseconds(del2);
          var++;
          } //baseline

       if ( Serial.available() ) {       
          key2 = Serial.read();           
          Serial.write( key2 );            
          switch( key2 ) {
          case 's':
          goto hogehoge;
          }
          }
     
     var1++;
     }    //Stim+Baseline Loop Trials
      break;

      ////////////////////////////////////////////
      case 'q':

      if (value == 0) {
          Serial.println("");
         Serial.println("Received command q without pulsewidth value");
         } 
      else {
          Serial.println("");
          Serial.print("Received command q with pulsewidth value ");
          Serial.println(value);
          PulseW = value;
         }

      Serial.println("");
      Serial.println("OptoStim5Hz_____________");
      Serial.print("OptoTrialN: ");
      Serial.println(OptoTrialN);
      Serial.print("PulseW(um): ");
      Serial.println(PulseW);
      Serial.print("OptoDuration(s): ");
      Serial.println(OptoDuration);
      Serial.print("OptoInterval(s): ");
      Serial.println(OptoInterval);
      Serial.print("OptoBase(s): ");
      Serial.println(OptoBase);
      
 
      var = 0;
      while(var < OptoBase*20){        //baseline
          // 470 + Cam1
          analogWrite(12,Amp1);
          delayMicroseconds(del3);
          digitalWrite(9,HIGH);
          delayMicroseconds(del1);
          digitalWrite(9,LOW);
          delayMicroseconds(del4);
          analogWrite(12,0);
          delayMicroseconds(del2);
          // 415 + Cam1
          analogWrite(11,Amp2);
          delayMicroseconds(del3);
          digitalWrite(9,HIGH);
          delayMicroseconds(del1);
          digitalWrite(9,LOW);    
          delayMicroseconds(del4);
          analogWrite(11,0);
          delayMicroseconds(del2);
          // 565 + Cam2
          analogWrite(10,Amp3);
          delayMicroseconds(del3);
          digitalWrite(8,HIGH);
          delayMicroseconds(del1);
          digitalWrite(8,LOW);
          delayMicroseconds(del4);
          analogWrite(10,0);
          delayMicroseconds(del2);
          var++;
          } //baseline
      

      var1 = 0;
      while(var1 < OptoTrialN){    //Stim+Baseline Loop Trials

      var = 0;
      while(var < OptoDuration*5){    //Stim Loop 5Hz

      // Stim
      // 470 + Cam1
      analogWrite(12,Amp1);
      delayMicroseconds(del3);
      digitalWrite(9,HIGH);
      delayMicroseconds(del1);
      digitalWrite(9,LOW);
      delayMicroseconds(del4);
      analogWrite(12,0);
      delayMicroseconds(del2);
      // 415 + Cam1
      analogWrite(11,Amp2);
      delayMicroseconds(del3);
      digitalWrite(9,HIGH);
      delayMicroseconds(del1);
      digitalWrite(9,LOW);    
      delayMicroseconds(del4);
      analogWrite(11,0);
      delayMicroseconds(del2);
      // 565 + Cam2 + Opt
      analogWrite(10,Amp3);
      analogWrite(7,AmpOpt4); //StimOn
      delayMicroseconds(del3);
      digitalWrite(8,HIGH);
      delayMicroseconds(PulseW-del3); // OptoStim PulseW ms
      analogWrite(7,0);       //StimOff
      delayMicroseconds(del1-PulseW+del3);
      digitalWrite(8,LOW);
      delayMicroseconds(del4);
      analogWrite(10,0);
      delayMicroseconds(del2);

      var3 = 0;
      while(var3 < 3){
      // 470 + Cam1
      analogWrite(12,Amp1);
      delayMicroseconds(del3);
      digitalWrite(9,HIGH);
      delayMicroseconds(del1);
      digitalWrite(9,LOW);
      delayMicroseconds(del4);
      analogWrite(12,0);
      delayMicroseconds(del2);
      // 415 + Cam1
      analogWrite(11,Amp2);
      delayMicroseconds(del3);
      digitalWrite(9,HIGH);
      delayMicroseconds(del1);
      digitalWrite(9,LOW);    
      delayMicroseconds(del4);
      analogWrite(11,0);
      delayMicroseconds(del2);
      // 565 + Cam2
      analogWrite(10,Amp3);
      delayMicroseconds(del3);
      digitalWrite(8,HIGH);
      delayMicroseconds(del1);
      digitalWrite(8,LOW);
      delayMicroseconds(del4);
      analogWrite(10,0);
      delayMicroseconds(del2);
      var3++;
      }
      
      var++;
      } //Stim Loop 5Hz

      var = 0;
      while(var < OptoInterval*20){        //post stim baseline
          // 470 + Cam1
          analogWrite(12,Amp1);
          delayMicroseconds(del3);
          digitalWrite(9,HIGH);
          delayMicroseconds(del1);
          digitalWrite(9,LOW);
          delayMicroseconds(del4);
          analogWrite(12,0);
          delayMicroseconds(del2);
          // 415 + Cam1
          analogWrite(11,Amp2);
          delayMicroseconds(del3);
          digitalWrite(9,HIGH);
          delayMicroseconds(del1);
          digitalWrite(9,LOW);    
          delayMicroseconds(del4);
          analogWrite(11,0);
          delayMicroseconds(del2);
          // 565 + Cam2
          analogWrite(10,Amp3);
          delayMicroseconds(del3);
          digitalWrite(8,HIGH);
          delayMicroseconds(del1);
          digitalWrite(8,LOW);
          delayMicroseconds(del4);
          analogWrite(10,0);
          delayMicroseconds(del2);
          var++;
          } //baseline

       if ( Serial.available() ) {       
          key2 = Serial.read();           
          Serial.write( key2 );            
          switch( key2 ) {
          case 's':
          goto hogehoge;
          }
          }
     
     var1++;
     }    //Stim+Baseline Loop Trials
     break;

     ////////////////////////////////////////////////

      case 's':
          hogehoge:    
          analogWrite(12,0);
          analogWrite(11,0);
          analogWrite(10,0);
          digitalWrite(9,LOW);
          digitalWrite(8,LOW);
          
          Serial.println("");
          Serial.println("Experiment Manually Stopped_____________");
                 
          break;
          default:
        break;

    } // switch end
  } // if
}
