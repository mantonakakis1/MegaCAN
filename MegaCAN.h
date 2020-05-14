#ifndef MegaCAN_h
#define MegaCAN_h

#include "Arduino.h"
const extern uint32_t baseID;

typedef struct MegaCAN_message_t {
  struct {
    uint32_t toTable = 0; // table on "To" device
    uint32_t toID = 0; // "To" device ID
    uint32_t fromID = 0; // "From" device ID
    uint32_t msgType = 0; // Type of message, "1" is a request, "2" is a response
    uint32_t toOffset = 0; // offset on "To" device
  } core; //unpacked
  uint32_t responseCore = 0; // response core back to requester
  struct {
    struct {
      uint8_t varByt = 0; // number of bytes requested
      uint16_t varOffset = 0;  // MS offset for storing repsonse data
      uint8_t varBlk = 0; // MS table for storing response data
    } request; //unpacked    
    uint8_t response[8] = { 0 }; // response data bytes back to requester, up to 8 bytes
  } data;  //packed
} MegaCAN_message_t;

typedef struct MegaCAN_broadcast_message_t {
	//offset 0
	uint16_t seconds = 0; //Time that ECU has been on, s
	float pw1 = 0; //Inj Bank 1 PW, ms
	float pw2 = 0; //Inj Bnad 2 PW, ms
	uint16_t rpm = 0; //Engine RPM

	//offset 1
	float adv_deg = 0; //Final ignition spark advance, deg BTDC
	uint8_t squirt = 0; //Bitfield of batch fire injector events
	uint8_t engine = 0; //Bitfield of engine status
	float afrtgt1 = 0; //Bank 1 AFR target, AFR
	float afrtgt2 = 0; //Bank 2 AFR target, AFR
	uint8_t wbo2_en1 = 0; //Not used
	uint8_t wbo2_en2 = 0; //Not used

	//offset 2
	float baro = 0; //Barometric pressure, kPa
	float map = 0; //Manifold air pressure, kPa
	float mat = 0; //Manifold air temp, deg F
	float clt = 0; //Coolant temp, deg F

	//offset 3
	float tps = 0; //Throttle position, %
	float batt = 0; //Battery voltage, V
	float afr1_old = 0; //AFR1, AFR
	float afr2_old = 0; //AFR2, AFR

	//offset 4
	float knock = 0; //Indication of knock input, %
	float egocor1 = 0; //EGO bank 1 correction, %
	float egocor2 = 0; //EGO bank 2 correction, %
	float aircor = 0; //Air density correction, %

	//offset 5
	float warmcor = 0; //Warmup correction, %
	float tpsaccel = 0; //TPS-based acceleration, %
	float tpsfuelcut = 0; //TPS-based fuel cut, %
	float barocor = 0; //Barometric fuel correction, %
	
	//offset 6
	float totalcor = 0; //Total fuel correction, %
	float ve1 = 0; //VE value table/bank 1, %
	float ve2 = 0; //VE value table/bank 2, %
	int16_t iacstep = 0; //Stepper idle step number, step
	float iacduty = 0; //PWM idle value duty, %
	
	//offset 7
	float cold_adv_deg = 0; //Cold advance, deg
	float TPSdot = 0; //Rate of change of TPS, %/sec
	int16_t MAPdot = 0; //Rate of change of MAP, kPa/s
	int16_t RPMdot = 0; //Rate of change of RPM, RPM/s
	
	//offset 8
	float MAFload = 0; //Sythetic 'load' from MAF, %
	float fuelload = 0; //'Load' used for fuel table lookup e.g. equals MAP in Speed-Density, %
	float fuelcor = 0; //Adjustment to fuel from Flex, %
	float MAF = 0; //Mass Air Flow (Scaling depend on range, 650g/s shown), g/s
	
	//offset 9
	float egoV1 = 0; //Voltage from O2#1 (Deprecated on MS3), V
	float egoV2 = 0; //Voltage from O2#2 (Deprecated on MS3), V
	float dwell = 0; //Main ignition dwell, ms
	float dwell_trl = 0; //Trailing ignition dwell, ms
	
	//offset 10
	uint8_t status1 = 0; //ECU status bitfield
	uint8_t status2 = 0; //ECU status bitfield
	uint8_t status3 = 0; //ECU status bitfield
	uint8_t status4 = 0; //Not typically used
	int16_t status5 = 0; //Not typically used
	uint8_t status6 = 0; //ECU status bitfield
	uint8_t status7 = 0; //ECU status bitfield
	
	//offset 11
	float fuelload2 = 0; //'Load' used for fuelling on modified table, %
	float ignload = 0; //'Load' used for ignition table lookup, %
	float ignload2 = 0; //'Load' used for modifier ignition table lookup, %
	float airtemp = 0; //Estimated intake air temperature, deg F
	
	//offset 12
	float wallfuel1 = 0; //Calculated volume of fuel on intake walls from EAE (in injector pulsewidth units), us
	float wallfuel2 = 0; //Second channel of same, us
	
	//offset 13
	float sensors1 = 0; //Generic sensor input 1 (gpioadc0 on MS2)
	float sensors2 = 0; //Generic sensor input 2 (gpioadc1 on MS2)
	float sensors3 = 0; //Generic sensor input 3 (gpioadc2 on MS2)
	float sensors4 = 0; //Generic sensor input 4 (gpioadc3 on MS2)
	
	//offset 14
	float sensors5 = 0; //Generic sensor input 5 (gpioadc4 on MS2)
	float sensors6 = 0; //Generic sensor input 6 (gpioadc5 on MS2)
	float sensors7 = 0; //Generic sensor input 7 (gpioadc6 on MS2)
	float sensors8 = 0; //Generic sensor input 8 (gpioadc7 on MS2)
	
	//offset 15
	float sensors9 = 0; //Generic sensor input 9 (adc6 on MS2)
	float sensors10 = 0; //Generic sensor input 10 (adc7 on MS2)
	float sensors11 = 0; //Generic sensor input 11
	float sensors12 = 0; //Generic sensor input 12
	
	//offset 16
	float sensors13 = 0; //Generic sensor input 13
	float sensors14 = 0; //Generic sensor input 14
	float sensors15 = 0; //Generic sensor input 15
	float sensors16 = 0; //Generic sensor input 16
	
	//offset 17
	float boost_targ_1 = 0; //Target boost - channel 1, kPa
	float boost_targ_2 = 0; //Target boost - channel 2, kPa
	uint8_t boostduty = 0; //Duty cycle on boost solenoid 1, %
	uint8_t boostduty2 = 0; //Duty cycle on boost solenoid 2, %
	float maf_volts = 0; //MAF voltage (synthesised for frequency MAFs), VE
	
	//offset 18
	float pwseq1 = 0; //Sequential Pulsewidth for cyl#1, ms
	float pwseq2 = 0; //Sequential Pulsewidth for cyl#2, ms
	float pwseq3 = 0; //Sequential Pulsewidth for cyl#3, ms
	float pwseq4 = 0; //Sequential Pulsewidth for cyl#4, ms	
	
	//offset 19
	float pwseq5 = 0; //Sequential Pulsewidth for cyl#5, ms
	float pwseq6 = 0; //Sequential Pulsewidth for cyl#6, ms
	float pwseq7 = 0; //Sequential Pulsewidth for cyl#7, ms
	float pwseq8 = 0; //Sequential Pulsewidth for cyl#8, ms
	
	//offset 20
	float pwseq9 = 0; //Sequential Pulsewidth for cyl#9, ms
	float pwseq10 = 0; //Sequential Pulsewidth for cyl#10, ms
	float pwseq11 = 0; //Sequential Pulsewidth for cyl#11, ms
	float pwseq12 = 0; //Sequential Pulsewidth for cyl#12, ms
	
	//offset 21
	float pwseq13 = 0; //Sequential Pulsewidth for cyl#13, ms
	float pwseq14 = 0; //Sequential Pulsewidth for cyl#14, ms
	float pwseq15 = 0; //Sequential Pulsewidth for cyl#15, ms
	float pwseq16 = 0; //Sequential Pulsewidth for cyl#16, ms
	
	//offset 22
	float egt1 = 0; //EGT 1, deg F
	float egt2 = 0; //EGT 2, deg F
	float egt3 = 0; //EGT 3, deg F
	float egt4 = 0; //EGT 4, deg F
	
	//offset 23
	float egt5 = 0; //EGT 5, deg F
	float egt6 = 0; //EGT 6, deg F
	float egt7 = 0; //EGT 7, deg F
	float egt8 = 0; //EGT 8, deg F
	
	//offset 24
	float egt9 = 0; //EGT 9, deg F
	float egt10 = 0; //EGT 10, deg F
	float egt11 = 0; //EGT 11, deg F
	float egt12 = 0; //EGT 12, deg F
	
	//offset 25
	float egt13 = 0; //EGT 13, deg F
	float egt14 = 0; //EGT 14, deg F
	float egt15 = 0; //EGT 15, deg F
	float egt16 = 0; //EGT 16, deg F
	
	//offset 26
	uint8_t nitrous1_duty = 0; //Duty cycle to nitrous solenoid 1, %
	uint8_t nitrous2_duty = 0; //Duty cycle to nitrous solenoid 2, %
	float nitrous_timer_out = 0; //Timer used internally for nitrous system, s
	float n2o_addfuel = 0; //Fuel pulsewidth added due to nitrous system, ms
	float n2o_retard = 0; //Timing retard due to nitrous system, deg
	
	//offset 27
	int16_t canpwmin1 = 0; //PWM period 1 from remote board
	int16_t canpwmin2 = 0; //PWM period 2 from remote board
	int16_t canpwmin3 = 0; //PWM period 3 from remote board
	int16_t canpwmin4 = 0; //PWM period 4 from remote board
	
	//offset 28
	uint16_t cl_idle_targ_rpm = 0; //Closed-loop idle target RPM, RPM
	int16_t tpsadc = 0; //ADC count from TPS
	float eaeload = 0; //'Load' used for EAE calc, %
	float afrload = 0; //'Load' used for AFR table lookups, %
	
	//offset 29
	float EAEfcor1 = 0; //Fuel correction from EAE - channel 1, %
	float EAEfcor2 = 0; //Fuel correction from EAE - channel 2, %
	float VSS1dot = 0; //Rate of change of VSS1, m/s^2
	float VSS2dot = 0; //Rate of change of VSS2, m/s^2
	
	//offset 30
	float accelx = 0; //External accelerometer X, m/s^2
	float accely = 0; //External accelerometer Y, m/s^2
	float accelz = 0; //External accelerometer Z, m/s^2
	uint8_t stream_level = 0; //Volume level on audio input
	uint8_t water_duty = 0; //Duty cycle to water injection solenoid, %
	
	//offset 31
	float AFR1 = 0; //AFR cyl#1, AFR
	float AFR2 = 0; //AFR cyl#2, AFR
	float AFR3 = 0; //AFR cyl#3, AFR
	float AFR4 = 0; //AFR cyl#4, AFR
	float AFR5 = 0; //AFR cyl#5, AFR
	float AFR6 = 0; //AFR cyl#6, AFR
	float AFR7 = 0; //AFR cyl#7, AFR
	float AFR8 = 0; //AFR cyl#8, AFR
	
	//offset 32
	float AFR9 = 0; //AFR cyl#9, AFR
	float AFR10 = 0; //AFR cyl#10, AFR
	float AFR11 = 0; //AFR cyl#11, AFR
	float AFR12 = 0; //AFR cyl#12, AFR
	float AFR13 = 0; //AFR cyl#13, AFR
	float AFR14 = 0; //AFR cyl#14, AFR
	float AFR15 = 0; //AFR cyl#15, AFR
	float AFR16 = 0; //AFR cyl#16, AFR
	
	//offset 33
	uint8_t duty_pwm_1 = 0; //Generic PWM duty 1, %
	uint8_t duty_pwm_2 = 0; //Generic PWM duty 2, %
	uint8_t duty_pwm_3 = 0; //Generic PWM duty 3, %
	uint8_t duty_pwm_4 = 0; //Generic PWM duty 4, %
	uint8_t duty_pwm_5 = 0; //Generic PWM duty 5, %
	uint8_t duty_pwm_6 = 0; //Generic PWM duty 6, %
	int8_t gear = 0; //Current gear selected
	uint8_t status8 = 0; //Engine status bitfield
	
	//offset 34
	float EGOv1 = 0; //Voltage from O2 cyl#1, V
	float EGOv2 = 0; //Voltage from O2 cyl#2, V
	float EGOv3 = 0; //Voltage from O2 cyl#3, V
	float EGOv4 = 0; //Voltage from O2 cyl#4, V
	
	//offset 35
	float EGOv5 = 0; //Voltage from O2 cyl#5, V
	float EGOv6 = 0; //Voltage from O2 cyl#6, V
	float EGOv7 = 0; //Voltage from O2 cyl#7, V
	float EGOv8 = 0; //Voltage from O2 cyl#8, V
	
	//offset 36
	float EGOv9 = 0; //Voltage from O2 cyl#9, V
	float EGOv10 = 0; //Voltage from O2 cyl#10, V
	float EGOv11 = 0; //Voltage from O2 cyl#11, V
	float EGOv12 = 0; //Voltage from O2 cyl#12, V
	
	//offset 37
	float EGOv13 = 0; //Voltage from O2 cyl#13, V
	float EGOv14 = 0; //Voltage from O2 cyl#14, V
	float EGOv15 = 0; //Voltage from O2 cyl#15, V
	float EGOv16 = 0; //Voltage from O2 cyl#16, V
	
	//offset 38
	float EGOcor1 = 0; //EGO correction cyl#1, %
	float EGOcor2 = 0; //EGO correction cyl#2, %
	float EGOcor3 = 0; //EGO correction cyl#3, %
	float EGOcor4 = 0; //EGO correction cyl#4, %
	
	//offset 39
	float EGOcor5 = 0; //EGO correction cyl#5, %
	float EGOcor6 = 0; //EGO correction cyl#6, %
	float EGOcor7 = 0; //EGO correction cyl#7, %
	float EGOcor8 = 0; //EGO correction cyl#8, %
	
	//offset 40
	float EGOcor9 = 0; //EGO correction cyl#9, %
	float EGOcor10 = 0; //EGO correction cyl#10, %
	float EGOcor11 = 0; //EGO correction cyl#11, %
	float EGOcor12 = 0; //EGO correction cyl#12, %
	
	//offset 41
	float EGOcor13 = 0; //EGO correction cyl#13, %
	float EGOcor14 = 0; //EGO correction cyl#14, %
	float EGOcor15 = 0; //EGO correction cyl#15, %
	float EGOcor16 = 0; //EGO correction cyl#16, %
	
	//offset 42
	float VSS1 = 0; //Vehicle Speed 1, m/s
	float VSS2 = 0; //Vehicle Speed 2, m/s
	float VSS3 = 0; //Vehicle Speed 3, m/s
	float VSS4 = 0; //Vehicle Speed 4, m/s
	
	//offset 43
	uint8_t synccnt = 0; //Sync-loss counter
	uint8_t syncreason = 0; //Sync-loss reason code
	uint16_t sd_filenum = 0; //SDcard file number
	uint8_t sd_error = 0; //SDcard error number
	uint8_t sd_phase = 0; //SDcard internal code
	uint8_t sd_status = 0; //SDcard status bitfield
	int8_t timing_err = 0; //calculated error in ignition timing, %
	
	//offset 44
	float vvt_ang1 = 0; //VVT actual angle 1, deg
	float vvt_ang2 = 0; //VVT actual angle 2, deg
	float vvt_ang3 = 0; //VVT actual angle 3, deg
	float vvt_ang4 = 0; //VVT actual angle 4, deg
	
	//offset 45
	float vvt_target1 = 0; //VVT target angle 1, deg
	float vvt_target2 = 0; //VVT target angle 2 deg
	float vvt_target3 = 0; //VVT target angle 3, deg
	float vvt_target4 = 0; //VVT target angle 4, deg
	
	//offset 46
	uint8_t vvt_duty1 = 0; //VVT solenoid 1 duty cycle, %
	uint8_t vvt_duty2 = 0; //VVT solenoid 2 duty cycle, %
	uint8_t vvt_duty3 = 0; //VVT solenoid 3 duty cycle, %
	uint8_t vvt_duty4 = 0; //VVT solenoid 4 duty cycle, %
	float inj_timing_pri = 0; //Injection Timing Angle (primary), deg BTDC
	float inj_timing_sec = 0; //Injection Timing Angle (secondary), deg BTDC
	
	//offset 47
	float fuel_pct = 0; //Ethanol content of fuel from Flex sensor, %
	float tps_accel = 0; //TPSdot based accel, %
	uint16_t SS1 = 0; //Shaft speed 1, RPM
	uint16_t SS2 = 0; //Shaft speed 2, RPM
	
	//offset 48
	float knock_cyl1 = 0; //Knock % cyl #1, %
	float knock_cyl2 = 0; //Knock % cyl #2, %
	float knock_cyl3 = 0; //Knock % cyl #3, %
	float knock_cyl4 = 0; //Knock % cyl #4, %
	float knock_cyl5 = 0; //Knock % cyl #5, %
	float knock_cyl6 = 0; //Knock % cyl #6, %
	float knock_cyl7 = 0; //Knock % cyl #7, %
	float knock_cyl8 = 0; //Knock % cyl #8, %
	
	//offset 49
	float knock_cyl9 = 0; //Knock % cyl #9, %
	float knock_cyl10 = 0; //Knock % cyl #10, %
	float knock_cyl11 = 0; //Knock % cyl #11, %
	float knock_cyl12 = 0; //Knock % cyl #12, %	
	float knock_cyl13 = 0; //Knock % cyl #13, %
	float knock_cyl14 = 0; //Knock % cyl #14, %
	float knock_cyl15 = 0; //Knock % cyl #15, %
	float knock_cyl16 = 0; //Knock % cyl #16, %
	
	//offset 50
	float map_accel = 0; //MAPdot based accel, %
	float total_accel = 0; //Total accel, %
	float launch_timer = 0; //Timer for timed-launch retard, s
	float launch_retard = 0; //Launch retard, deg
	
	//offset 51
	uint8_t porta = 0; //CPU portA bitfield
	uint8_t portb = 0; //CPU portB bitfield
	uint8_t porteh = 0; //CPU portE/portH bitfield
	uint8_t portk = 0; //CPU portK bitfield
	uint8_t portmj = 0; //CPU portM/portJ bitfield
	uint8_t portp = 0; //CPU portP bitfield
	uint8_t portt = 0; //CPU portT bitfield
	uint8_t cel_errorcode = 0; //CEL error code
	
	//offset 52
	uint8_t canin1 = 0; //CAN input 1 bitfield (CAN port 1 on MS2)
	uint8_t canin2 = 0; //CAN input 2 bitfield (CAN port 2 on MS2)
	uint8_t canout = 0; //CAN output 1 bitfield (CAN port 3 on MS2)
	float knk_rtd = 0; //Knock retard, deg
	uint16_t fuelflow = 0; //Average fuel flow, cc/min
	uint16_t fuelcons = 0; //Average fuel consumption, l/km
	
	//offset 53
	float fuel_press1 = 0; //Fuel pressure 1, kPa
	float fuel_press2 = 0; //Fuel pressure 2, kPa
	float fuel_temp1 = 0; //Fuel temperature 1, deg F
	float fuel_temp2 = 0; //Fuel temperature 2, deg F
	
	//offset 54
	float batt_cur = 0; //Battery current (alternator system), A
	uint16_t cel_status = 0; //CEL status bitfield
	float fp_duty = 0; //Fuel pump output duty, %
	uint8_t alt_duty = 0; //Alternator field output duty, %
	uint8_t load_duty = 0; //Alternator measured load-sense duty, %
	float alt_targv = 0; //Alternator target voltage, %
	
	//offset 55
	uint16_t looptime = 0; //Main code loop execution time, us
	float fueltemp_cor = 0; //Fuel temperature correction, %
	float fuelpress_cor = 0; //Fuel pressure correction, %
	float ltt_cor = 0; //Long term trim correction, %
	uint8_t sp1 = 0; //Unused
	
	//offset 56
	float tc_retard = 0; //Traction control retard, deg
	float cel_retard = 0; //CEL retard, deg
	float fc_retard = 0; //Fuel-cut (overrun) retard, deg
	float als_addfuel = 0; //ALS added fuel, ms
	
	//offset 57
	float base_advance = 0; //Base timing from tables, deg
	float idle_cor_advance = 0; //Idle correction advance, deg
	float mat_retard = 0; //MAT retard, deg
	float flex_advance = 0; //Flex advance, deg
	
	//offset 58
	float adv1 = 0; //Timing lookup from table 1, deg
	float adv2 = 0; //Timing lookup from table 2, deg
	float adv3 = 0; //Timing lookup from table 3, deg
	float adv4 = 0; //Timing lookup from table 4, deg
	
	//offset 59
	float revlim_retard = 0; //Revlimiter 'soft' retard, deg
	float als_timing = 0; //ALS timing change, deg
	float ext_advance = 0; //External advance (e.g. trans), deg
	float deadtime1 = 0; //Injector deadtime in use (#1), ms
	
	//offset 60
	float launch_timing = 0; //Launch control timing, deg
	float step3_timing = 0; //3-step timing, deg
	float vsslaunch_retard = 0; //Wheel-speed based launch retard, deg
	uint16_t cel_status2 = 0; //CEL status 2
	
	//offset 61
	int8_t gps_latdeg = 0; //External GPS latitude deg, deg
	uint8_t gps_latmin = 0; //GPS latitude minutes, min
	uint16_t gps_latmmin = 0; //GPS latitude milli-minutes, mmin
	int8_t gps_londeg = 0; //External GPS longitude deg, deg
	uint8_t gps_lonmin = 0; //GPS longitude minutes, min
	uint16_t gps_lonmmin = 0; //GPS longitude milli-minutes, mmin
	
	//offset 62
	uint8_t gps_outstatus = 0; //GPS status byte (bit 0 = E/W)
	int8_t gps_altk = 0; //GPS altitude km, km
	uint16_t gps_altm = 0; //GPS altitude m, m/s
	float gps_speed = 0; //GPS speed, m/s
	float gps_course = 0; //GPS course, deg
	
	//offset 63
	float generic_pid_duty1 = 0; //Generic close-loop duty 1, %
	float generic_pid_duty2 = 0; //Generic close-loop duty 2, %
} MegaCAN_broadcast_message_t;

class MegaCAN
{
  public:
    MegaCAN();
    void processMSreq(uint32_t msgCore, const uint8_t msgData[3], MegaCAN_message_t &msg);
    void setMSresp(MegaCAN_message_t recMsg, MegaCAN_message_t &respMsg, uint16_t var0, uint16_t var1, uint16_t var2, uint16_t var3);
	void getBCastData(uint32_t id, const uint8_t data[8], MegaCAN_broadcast_message_t &msg);
};

#endif