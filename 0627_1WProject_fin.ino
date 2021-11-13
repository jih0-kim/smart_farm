//IoT WEEK1 Project LKC, KJH

#include "DHT.h"
#include <VitconBrokerComm.h>
using namespace vitcon;
#include <SoftPWM.h>
SOFTPWM_DEFINE_CHANNEL(A3);
#define DHTPIN A1
#define DHTTYPE DHT22
#define SOILHUMI A6
#define PUMP 16
#define LAMP 17
DHT dht(DHTPIN, DHTTYPE);

//<<FAN>>--------------------------------------------------------------
int connectcheck = 0;
int soilhumi = 0;
int pwmpower = 60;
int pwmpower2 = 60;

uint32_t TstartTime = 0;
uint32_t DataCaptureDelay = 3000;
uint32_t DataCapture_ST = 0;
uint32_t TimePushDelay = 0;
uint32_t StartFantimer = 0;

int32_t Fantimer = 0;
int32_t act_temp_write = 0;
int32_t act_temp_write2 = 0;

float Temp = 0;
float humi = 0;
float remainingfan = 0;

bool fan_macro_write=false;
bool Interval_Tup_status = false;
bool FanLedon = false ;
bool pwmupbutton = false;
bool pwmupbutton2 = false;

//<<PUMP>>--------------------------------------------------------------
int Second = 1;

int32_t pumptime = 0;                     
int32_t StartSoilHumi = 0;
int32_t FinSoilHumi = 0;

uint32_t TimeSum = 0;
uint32_t TimeCompare;

float remainingpump = 0;

bool pumpled = false;                    
bool timeset = false;
bool pumpset = false;
bool selfstart;
bool Interval_Sup_status = false;

uint32_t TimePushDelay2 = 0;
uint32_t TimeStartTime = 0;
uint32_t PumpStartTime = 0;

//<<LED>>--------------------------------------------------------------
int LED_Hour = 0;
int LED_Minute = 0;
int LED_Second = 1;

int32_t ledtime = 0;                        

uint32_t LED_TimeSum = 0;
uint32_t LED_TimeCompare; 

float remainingled = 0;

bool ledbutton = false;                    
bool ledtimeset = false;                   
bool ledset = false;                        
bool selfledstart = false;               

bool ledsetbutton = false;
bool ledresetbutton = false;

bool LED_LED_Interval_Hup_status;
bool LED_LED_Interval_Mup_status;
bool LED_LED_Interval_Sup_status;

uint32_t LED_TimePushDelay = 0;
uint32_t LED_TimeStartTime = 0;
uint32_t LED_LedStartTime = 0;

//<<FAN>>--------------------------------------------------------------
void _FanOnTimeToggleWrite(int32_t val) {
  Fantimer = val;
}
void Interval_Tup(bool val) {
  Interval_Tup_status = val;
}
void _PwmUpButton(bool val) {
  pwmupbutton = val;
}
void _PwmUpButton2(bool val) {
  pwmupbutton2 = val;
}
void _FanMacroWrite(bool val) {
  fan_macro_write = val;
}
void _ActTempWrite(int32_t val) {
  act_temp_write = val;
}
void _ActTempWrite2(int32_t val) {
  act_temp_write2 = val;
}
void IntervalReset(bool val) {
  if (fan_macro_write && val) {
    pwmpower = 60;
    pwmpower2 = 60;
  }
}

//<<PUMP>>--------------------------------------------------------------
void pump_runtime(int32_t val){       
    pumptime = val;
}

void pump_selfrun(bool val){          
    selfstart = val;
}

void pump_autostart(int32_t val){    
    StartSoilHumi = val;
}

void pump_autofin(int32_t val){      
    FinSoilHumi = val;
}

void pumpset_out(bool val){        
    pumpset = val;
}

void timeset_out(bool val){          
    timeset = val;
}

void Interval_Sup(bool val){        
    Interval_Sup_status = val;
}
//<<LED>>--------------------------------------------------------------
void led_runtime(int32_t val){              
    ledtime = val;
}

void led_autotimeset(bool val){            
    if (!ledset){
        ledsetbutton = val;
    }
}

void led_autotimereset(bool val){           
    if (!ledset && val){
        LED_Hour = 0;
        LED_Minute = 0;
        LED_Second = 1;
    }
}

void led_timeset(bool val){          
    selfledstart = val;
}

void ledset_out(bool val){           
    ledset = val;
}

void LED_Interval_Hup(bool val){          
    LED_LED_Interval_Hup_status = val;
}

void LED_Interval_Mup(bool val){         
    LED_LED_Interval_Mup_status = val;
}

void LED_Interval_Sup(bool val){          
    LED_LED_Interval_Sup_status = val;
}

//<<FAN>>--------------------------------------------------------------
IOTItemBin IntervalRST(IntervalReset);
IOTItemInt PwmPowerTrackBar;
IOTItemInt PwmPowerTrackBar2;
IOTItemFlo dht22_temp;
IOTItemFlo dht22_humi;
IOTItemInt SoilHumi;
IOTItemBin FanLight;
IOTItemBin FanOnPassive(Interval_Tup);
IOTItemInt FanOnTimeToggleIndex;
IOTItemInt FanOnTimeToggleWrite(_FanOnTimeToggleWrite);

IOTItemBin FanMacroIndex;
IOTItemBin FanMacroWrite(_FanMacroWrite);
IOTItemInt ActTempIndex;
IOTItemInt ActTempWrite(_ActTempWrite);
IOTItemInt ActTempIndex2;
IOTItemInt ActTempWrite2(_ActTempWrite2);

IOTItemBin PwmUpButton(_PwmUpButton);
IOTItemBin PwmUpButton2(_PwmUpButton2);
IOTItemInt RemainingFan;
IOTItemInt ConnectCheck;

//<<PUMP>>--------------------------------------------------------------
IOTItemBin PumpStatus;                   
IOTItemBin PumpStop(pumpset_out); 
IOTItemBin StopStatus;                  
IOTItemBin Stop(timeset_out);                     

IOTItemBin SelfRunStart(pump_selfrun);    
IOTItemBin PumpLight;
IOTItemBin IntervalSUP(Interval_Sup);     
IOTItemInt label_Sinterval;               

IOTItemInt PumpSelfStatus;                        
IOTItemInt SetSelfPumpTime(pump_runtime);           

IOTItemInt PumpStartStatus;                         
IOTItemInt SetAutoPumpStartHumi(pump_autostart);  

IOTItemInt PumpFinStatus;                           
IOTItemInt SetAutoPumpFinHumi(pump_autofin);
IOTItemInt RemainingPump;

//<<LED>>--------------------------------------------------------------
IOTItemBin LedStatus;                     
IOTItemBin LedStop(ledset_out);                     

IOTItemBin SelfLedRunStart(led_timeset);  
IOTItemBin LedLight;                     

IOTItemBin LED_IntervalHUP(LED_Interval_Hup);     
IOTItemBin LED_IntervalMUP(LED_Interval_Mup);     
IOTItemBin LED_IntervalSUP(LED_Interval_Sup);    

IOTItemInt LED_label_Hinterval;             
IOTItemInt LED_label_Minterval;               
IOTItemInt LED_label_Sinterval;              

IOTItemInt LedSelfStatus;                           
IOTItemInt SetSelfLedTime(led_runtime);             

IOTItemBin LedSetTime(led_autotimeset);             
IOTItemBin LedResetTime(led_autotimereset);         
IOTItemInt RemainingLed;

#define ITEM_COUNT 50
IOTItem *items[ITEM_COUNT] = { &dht22_temp,&dht22_humi,&SoilHumi,&FanLight,&FanOnPassive,&FanOnTimeToggleIndex,
                                &FanOnTimeToggleWrite,&FanMacroIndex,&FanMacroWrite,&ActTempIndex,&ActTempWrite,
                                &ActTempIndex2,&ActTempWrite2,&PwmPowerTrackBar,&PwmPowerTrackBar2,&PwmUpButton,
                                &PwmUpButton2,&IntervalRST,&RemainingFan,&ConnectCheck,&PumpLight,&SelfRunStart,
                                &PumpSelfStatus,&SetSelfPumpTime,&PumpStatus,&PumpStop,&PumpStartStatus,&SetAutoPumpStartHumi,
                                &IntervalSUP,&label_Sinterval,&PumpFinStatus,&SetAutoPumpFinHumi,&StopStatus,&Stop,
                                &LedLight,&SelfLedRunStart,&LedSelfStatus,&SetSelfLedTime,&LedStatus,&LedStop,
                                &LED_label_Hinterval,&LED_label_Minterval,&LED_label_Sinterval,&LedSetTime,
                                &LED_IntervalHUP,&LED_IntervalMUP,&LED_IntervalSUP,&LedResetTime,&RemainingPump,&RemainingLed
                            };

const char device_id[] = "67e6f7584a6b3901f1782e6141602334";
BrokerComm comm(&Serial, device_id, items, ITEM_COUNT);

void setup() {
  Serial.begin(250000);
  comm.SetInterval(200);
  dht.begin();
  DataCapture_ST = millis();
  SoftPWM.begin(490);

  pinMode(SOILHUMI, INPUT);
  pinMode(PUMP, OUTPUT);

  pinMode(LAMP, OUTPUT);
    digitalWrite(LAMP, LOW);
}

void loop() {
  if (millis() - DataCapture_ST > DataCaptureDelay) {
    soilhumi = map(analogRead(SOILHUMI),0,1023,100,0);
    Temp = dht.readTemperature();
    humi = dht.readHumidity();
    DataCapture_ST = millis();
    connectcheck +=1;
  }

  if (!fan_macro_write) {
    
    if (Interval_Tup_status) {
      TstartTime = millis();
      StartFantimer = (Fantimer * 1000);
      Interval_Tup_status = false;
    }
    else {
      if (millis()  < (uint32_t)StartFantimer + TstartTime) {
        SoftPWM.set(60);
        FanLedon = true;
        remainingfan = (((uint32_t)StartFantimer + TstartTime)-millis())/1000;
      }
      else {
        FanLedon = false;
        SoftPWM.set(0);
        remainingfan = 0;
      }
    }
    FanLight.Set(FanLedon);
  }
  else {
    IntervalSet();
    FanLight.Set(false);
    remainingfan = 0;
    if (Temp < act_temp_write2 && Temp > act_temp_write) {
      SoftPWM.set(pwmpower);
    }
    else if (Temp > act_temp_write2) {
      SoftPWM.set(pwmpower2);
    }
    else {
      SoftPWM.set(0);
    }
  }
  PwmPowerTrackBar.Set(pwmpower);
  PwmPowerTrackBar2.Set(pwmpower2);
  FanMacroIndex.Set(fan_macro_write);
  dht22_temp.Set(Temp);
  dht22_humi.Set(humi);
  SoilHumi.Set(soilhumi);
  RemainingFan.Set(remainingfan);
  ConnectCheck.Set(connectcheck);

//================================
  InvervalSet2(timeset);
    soilhumi = map(analogRead(SOILHUMI),0,1023,100,0);
    if(pumpset && timeset){                            
      pumptime = 0;
      pumpled = false;
      if (TimeCompare % 2 == 0){
        if (soilhumi>=StartSoilHumi && soilhumi<=FinSoilHumi){
            digitalWrite(PUMP, HIGH);      
        }
        else{
            digitalWrite(PUMP, LOW);        
        }
      }
      else if (TimeCompare % 2 == 1){
        digitalWrite(PUMP, LOW);  
      }
    }
    else if(pumpset && !timeset) {                     
        pumptime = 0;
        pumpled = false;
        if (soilhumi>=StartSoilHumi && soilhumi<=FinSoilHumi){
            digitalWrite(PUMP, HIGH);      
        }
        else{
            digitalWrite(PUMP, LOW);        
        }
    }
    else if(!pumpset){
        if (selfstart){
            PumpStartTime = millis();
            selfstart = false;
        }
        else if (!selfstart){ 
            if (millis() < PumpStartTime + pumptime*1000){
                pumpled = true;
                remainingpump=((PumpStartTime + pumptime*1000)-millis())/1000;
                digitalWrite(PUMP, HIGH);  
            }
            else{
                digitalWrite(PUMP, LOW);
                remainingpump = 0;
                pumpled = false;
            }
            PumpLight.Set(pumpled);
        }
    }

    PumpStatus.Set(pumpset);
    PumpLight.Set(pumpled);
    
    StopStatus.Set(timeset);
    label_Sinterval.Set(Second);


//================================
  InvervalSet3(ledsetbutton);
    if (ledset){                           
        ledtime = 0;
        ledbutton = false;
        if (LED_TimeCompare % 2 == 0){
            digitalWrite(LAMP, HIGH);      
        }
        else if (LED_TimeCompare % 2 == 1){
            digitalWrite(LAMP, LOW);  
        }
    }
    else {
        if (selfledstart){
            LED_LedStartTime = millis();
            selfledstart = false;
        }
        else{
            if (millis() < ((uint32_t)ledtime*1000)+LED_LedStartTime){
              remainingled = ((((uint32_t)ledtime*1000)+LED_LedStartTime)-millis())/1000;
                ledbutton = true;
                digitalWrite(LAMP, HIGH);  
            }
            else{
                ledbutton = false;
                remainingled = 0;
                digitalWrite(LAMP, LOW);
            }
        }
    }

    LedStatus.Set(ledset);
    LedLight.Set(ledbutton);

    LED_label_Hinterval.Set(LED_Hour);
    LED_label_Minterval.Set(LED_Minute);
    LED_label_Sinterval.Set(LED_Second);

    RemainingPump.Set(remainingpump);
    RemainingLed.Set(remainingled);
    comm.Run();
}

//<<FAN>>================================
void IntervalSet() {
  
  if (pwmupbutton) {
    if (millis() > TimePushDelay + 1000) {
      pwmpower += 1;
      if (pwmpower >= 100)pwmpower = 0;
      TimePushDelay = millis();
    }
  }
  if (pwmupbutton2) {
    if (millis() > TimePushDelay + 1000) {
      pwmpower2 += 1;
      if (pwmpower2 >= 100)pwmpower2 = 0;
      TimePushDelay = millis();
    }
  }
}

//<<PUMP>>================================
void InvervalSet2(bool timeset){
  if (!timeset){                            
    TimeSum = (uint32_t)Second*1000;
    TimeStartTime = millis();

    if (millis()>TimePushDelay2+500){
      Second += Interval_Sup_status;          
      TimePushDelay2 = millis();
    }
  }
  else if (timeset) {                       
    TimeCompare = (millis() - TimeStartTime) / TimeSum;
  }
}

//<<LED>>================================
void InvervalSet3(bool ledsetbutton){
    if (millis() > LED_TimePushDelay + 500){
        LED_Hour += LED_LED_Interval_Hup_status;
        if (LED_Hour >= 24) LED_Hour = 0;
        LED_Minute += LED_LED_Interval_Mup_status;
        if (LED_Minute >= 60) LED_Minute = 0;
        LED_Second += LED_LED_Interval_Sup_status;                             
        if (LED_Second >= 60) LED_Minute = 0;

        LED_TimePushDelay = millis();
    }

    if (ledsetbutton){                                                          
        LED_TimeSum = (uint32_t)((LED_Hour*60+LED_Minute)*60+LED_Second)*1000;
        LED_TimeStartTime = millis();
    }
    else if (LED_TimeSum > 0 && !ledsetbutton) {                                
        LED_TimeCompare = (millis() - LED_TimeStartTime) / LED_TimeSum;
    }
    else{
        LED_TimeCompare = 1;
    }
}

//================================
