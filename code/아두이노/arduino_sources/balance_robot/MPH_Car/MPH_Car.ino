#include <Wire.h>
#include <I2Cdev.h>
#include <JJ_MPU6050_DMP_6Axis.h>  // DMP work with a modified version of the library (see note inside)

#define DEBUG 0
#define CLR(x,y) (x&=(~(1<<y)))
#define SET(x,y) (x|=(1<<y))
#define ZERO_SPEED 65535 // zero speed
#define MAX_ACCEL 7 // Maximum ACCEL
#define MAX_THROTTLE 600 // Maximum throttle 530
#define MAX_STEERING 136 // Maximum steering
#define MAX_TARGET_ANGLE 12 // The maximum angle of 12 goals
#define I2C_SPEED 400000L // I2C speed
#define Gyro_Gain 0.03048 // Gyro gain
#define Gyro_Scaled(x) x*Gyro_Gain // Return of the second scaling raw gyro data
#define RAD2GRAD 57.2957795//57.2957795
#define GRAD2RAD 0.01745329251994329576923690768489//0.01745329251994329576923690768489

// The default control provisions  
#define KP 0.22 // 0.22        
#define KD 24  // 30 28  26  
#define KP_THROTTLE 0.065  //0.08//0.065
#define KI_THROTTLE 0.05//0.05

// for raiseup control gain
#define KP_RAISEUP 0.16 // kp raise
#define KD_RAISEUP 40   //kd raise
#define KP_THROTTLE_RAISEUP 0  // No raiseup speed control
#define KI_THROTTLE_RAISEUP 0.0 // Throttle rise
#define ITERM_MAX_ERROR 40   // ITERM saturation constant
#define ITERM_MAX 5000

// MPU Control / status Cristobal
bool dmpReady = false;  // 设置为true，如果DMP初始化成功
uint8_t mpuIntStatus;   // 拥有从MPU实际中断状态字节
uint8_t devStatus;      // 每个设备操作后返回的状态（0=成功！0=错误）
uint16_t packetSize;    // 预计DMP数据包大小（我们18字节）
uint16_t fifoCount;     // 目前在FIFO中的所有字节数
uint8_t fifoBuffer[18]; // FIFO存储缓冲器
Quaternion q;
uint8_t loop_counter;       // 要生成媒体40Hz的循环
uint8_t slow_loop_counter;  // 慢环2HZ
long timer_old;//计时器老
long timer_value;//定时器的值
int debug_counter;//调试计数器
float dt;
int lkf;
// 类的默认I2C地址为0X68
MPU6050 mpu;
float angle_adjusted;//角度调整
float angle_adjusted_Old;//角度调整
float Kp=KP;
float Kd=KD;
float Kp_thr=KP_THROTTLE;//油门
float Ki_thr=KI_THROTTLE;
float Kp_user=KP;
float Kd_user=KD;
float Kp_thr_user=KP_THROTTLE;
float Ki_thr_user=KI_THROTTLE;
bool newControlParameters = false;//布尔新的控制参数
bool modifing_control_parameters=false;//布尔新的控制参数
float PID_errorSum;
float PID_errorOld = 0;
float PID_errorOld2 = 0;
float setPointOld = 0;
float target_angle;
float throttle;
float kkll;
float steering;
float max_throttle = MAX_THROTTLE;//最大油门=最大油门
float max_steering = MAX_STEERING;
float max_target_angle = MAX_TARGET_ANGLE;
float control_output;
int16_t motor1;
int16_t motor2;
int16_t speed_m[2];           // 电机的实际转速
uint8_t dir_m[2];             // 步进电机的实际方向
int16_t actual_robot_speed;          // 整个机器人的速度（从踏步机速测）
int16_t actual_robot_speed_Old;          // 整个机器人的速度（从踏步机速测）
float estimated_speed_filtered;//估计速度过滤
uint16_t counter_m[2];        // 计数器周期
uint16_t period_m[2][8];      // 八子时期
uint8_t period_m_index[2];    //索引子时期
int freeRam () { //免费拉姆
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}
//DMP功能
//这个函数定义在传感器融合加速的重量
//默认值是0x80的
//官方InvenSense公司名称是inv_key_0_96（？）
void dmpSetSensorFusionAccelGain(uint8_t gain)
{
  // INV_KEY_0_96
  mpu.setMemoryBank(0);
  mpu.setMemoryStartAddress(0x60);
  mpu.writeMemoryByte(0);
  mpu.writeMemoryByte(gain);
  mpu.writeMemoryByte(0);
  mpu.writeMemoryByte(0);
}
// 快速计算，从四元解obtein披角度
float dmpGetPhi() {
   mpu.getFIFOBytes(fifoBuffer, 16); // 我们只看过四元
   mpu.dmpGetQuaternion(&q, fifoBuffer); 
   mpu.resetFIFO();  //我们始终复位FIFO 
   //返回( asin(-2*(q.x * q.z - q.w * q.y)) * 180/M_PI); //roll
   return (atan2(2*(q.y * q.z + q.w * q.x), q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z)* RAD2GRAD);
}
// PD的实施。 DT是毫秒
float stabilityPDControl(float DT, float input, float setPoint,  float Kp, float Kd)
{
  float error;
  float output;
  error = setPoint-input;
  // Kd的两部分实施
  //仅使用输入（传感器）的一部分而不是设定值输入输入（T-2）最大的一个
  //而第二个使用该设定值，使之更有点侵略性设定点设定点（T-1）
  output = Kp*error + (Kd*(setPoint - setPointOld) - Kd*(input - PID_errorOld2))/DT;       // + 错误 - PID_error_Old2
  //Serial.print(Kd*(error-PID_errorOld));Serial.print("\t");
  PID_errorOld2 = PID_errorOld;
  PID_errorOld = input;  // 误差为Kd值是唯一的输入组件
  setPointOld = setPoint;
  return(output);
}
//P控制实现。
float speedPControl(float input, float setPoint,  float Kp)
{
  float error;
  error = setPoint-input;
  return(Kp*error);
}
// PI实现。 DT是毫秒
float speedPIControl(float DT, float input, float setPoint,  float Kp, float Ki)
{
  float error;
  float output;
  error = setPoint-input;
  PID_errorSum += constrain(error,-ITERM_MAX_ERROR,ITERM_MAX_ERROR);
  PID_errorSum = constrain(PID_errorSum,-ITERM_MAX,ITERM_MAX);
  output = Kp*error + Ki*PID_errorSum*DT*0.001;
  return(output);
}
//在16MHz的200ns=>4条指令
void delay_200ns()  
{
  __asm__ __volatile__ (
		"nop" "\n\t"
                "nop" "\n\t"
                "nop" "\n\t"
		"nop"); 
}

ISR(TIMER1_COMPA_vect)
{
  counter_m[0]++;
  counter_m[1]++;
  if (counter_m[0] >= period_m[0][period_m_index[0]])
    {
    counter_m[0] = 0;
    if (period_m[0][0]==ZERO_SPEED)
      return;
    if (dir_m[0])
      SET(PORTB,4);  //DIR电机1
    else
      CLR(PORTB,4);
    // 我们需要等待，以免200ns的产生步进脉冲...
    period_m_index[0] = (period_m_index[0]+1)&0x07; // 周期M指数从0到7
    //delay_200ns();
    SET(PORTD,7); // 步进电机1
    delayMicroseconds(1);
    CLR(PORTD,7);
    }
  if (counter_m[1] >= period_m[1][period_m_index[1]])
    {
    counter_m[1] = 0;
    if (period_m[1][0]==ZERO_SPEED)
      return;
    if (dir_m[1])
      SET(PORTC,7);   // DIR电机2
    else
      CLR(PORTC,7);
    period_m_index[1] = (period_m_index[1]+1)&0x07;
    //delay_200ns();
    SET(PORTD,6); // 步进电机1
    delayMicroseconds(1);
    CLR(PORTD,6);
    }
}
void calculateSubperiods(uint8_t motor)
{
  int subperiod;
  int absSpeed;
  uint8_t j;
  
  if (speed_m[motor] == 0)
    {
    for (j=0;j<8;j++)
      period_m[motor][j] = ZERO_SPEED;
    return;
    }
  if (speed_m[motor] > 0 )   // 正速度
    {
    dir_m[motor] = 1;
    absSpeed = speed_m[motor];
    }
  else                       // 负速度
    {
    dir_m[motor] = 0;
    absSpeed = -speed_m[motor];
    }
    
  for (j=0;j<8;j++)
    period_m[motor][j] = 1000/absSpeed;
  // 计算亚期。如果模块<0.25=>子期间= 0，如果模块<0.5=>子周期= 1。如果模块<0.75子期间=2其他子期间=3
  subperiod = ((1000 % absSpeed)*8)/absSpeed;   // 优化代码来计算子期间（整数运算）
  if (subperiod>0)
   period_m[motor][1]++;
  if (subperiod>1)
   period_m[motor][5]++;
  if (subperiod>2)
   period_m[motor][3]++;
  if (subperiod>3)
   period_m[motor][7]++;
  if (subperiod>4)
   period_m[motor][0]++;
  if (subperiod>5)
   period_m[motor][4]++;
  if (subperiod>6)
   period_m[motor][2]++; 
}
void setMotorSpeed(uint8_t motor, int16_t tspeed)
{
  // 我们限制最大加速度
  if ((speed_m[motor] - tspeed)>MAX_ACCEL)
    speed_m[motor] -= MAX_ACCEL;
  else if ((speed_m[motor] - tspeed)<-MAX_ACCEL)
    speed_m[motor] += MAX_ACCEL;
  else
    speed_m[motor] = tspeed;  
  calculateSubperiods(motor);  //我们采用四个子周期来提高分辨率
  // 为了节省能量，当它没有运行...
  if ((speed_m[0]==0)&&(speed_m[1]==0))
    digitalWrite(4,HIGH);   //禁用电机
  else
    digitalWrite(4,LOW);   // 使电机
}
void setup() 
{ 
  // 步进引脚 
  pinMode(4,OUTPUT);  // ENABLE MOTORS
  pinMode(12,OUTPUT);  // STEP MOTOR 1 PORTD,7//6
  pinMode(13,OUTPUT);  // DIR MOTOR 1 8
  pinMode(6,OUTPUT); // STEP MOTOR 2 PORTD,6 12
  pinMode(8,OUTPUT); // DIR MOTOR 2 13
  digitalWrite(4,HIGH);   // Disbale motors
  
  Serial1.begin(9600);
  
  // 加入I2C总线
  Wire.begin();
  
  // 4000Khz fast mode
  TWSR = 0;
  TWBR = ((16000000L/I2C_SPEED)-16)/2;
  TWCR = 1<<TWEN;
  
  //mpu.initialize();
  mpu.setClockSource(MPU6050_CLOCK_PLL_ZGYRO);
  mpu.setFullScaleGyroRange(MPU6050_GYRO_FS_2000);
  mpu.setFullScaleAccelRange(MPU6050_ACCEL_FS_2);
  mpu.setDLPFMode(MPU6050_DLPF_BW_20);  //10,20,42,98,188
  mpu.setRate(4);   // 0=1khz 1=500hz, 2=333hz, 3=250hz 4=200hz
  mpu.setSleepEnabled(false);
  
  delay(1000);
  devStatus = mpu.dmpInitialize();
  if (devStatus == 0) {
        // 打开DMP，现在，它已经准备好
        mpu.setDMPEnabled(true);
        mpuIntStatus = mpu.getIntStatus();
        dmpReady = true;
        
    }
  // 陀螺仪的校准
  //机器人必须在初始化时是稳定的
  delay(1500);   // 一段时间来解决的事情从没有运动算法的偏置需要一些时间才能生效并重置陀螺仪偏置。1500
  timer_old = millis();
  //我们将覆盖定时器使用步进电机
  //步进电机初始化
  //定时器CTC模式
  TCCR1B &= ~(1<<WGM13);
  TCCR1B |=  (1<<WGM12);
  TCCR1A &= ~(1<<WGM11); 
  TCCR1A &= ~(1<<WGM10);
  // 输出模式=00（断开）
  TCCR1A &= ~(3<<COM1A0); 
  TCCR1A &= ~(3<<COM1B0); 
  // 设置定时器预分频器
  //通常我们使用的8分频器，产生16MHz的CPU的一个2MHz的计时器
  TCCR1B = (TCCR1B & ~(0x07<<CS10)) | (2<<CS10);
  OCR1A = 80;   // 25Khz
  TCNT1 = 0;
  delay(1000);
  //调整传感器融合增益
  dmpSetSensorFusionAccelGain(0x40);//0x20/////////////////////////////////////////////////////////调整传感器
  delay(1000);
  TIMSK1 |= (1<<OCIE1A);  // 启用定时器中断
  digitalWrite(4,LOW);    // 使步进驱动器
  // 小电机的振动，表明机器人已准备就绪
  for (uint8_t k=0;k<3;k++)
	{
	setMotorSpeed(0,3);
	setMotorSpeed(1,-3);
	delay(150);
	setMotorSpeed(0,-3);
	setMotorSpeed(1,3);
	delay(150);
	}  
  mpu.resetFIFO();
  timer_old = millis();
      
}
// 主循环//////////////////////////////////////////////////////////////////////////+++++++++++++++++++++++++++++++++++++++++++++++++++++++
void loop() 
{	   
   kongzhi();
   timer_value = millis();
  // 新的DMP定位解决方案？
  fifoCount = mpu.getFIFOCount();
  if (fifoCount>=18)
      {
      if (fifoCount>18)  // 如果我们有一个以上的数据包，我们采取简单的路径：丢弃缓冲区
        {
        mpu.resetFIFO();
        return;
        }
	loop_counter++;
        slow_loop_counter++;
    dt = (timer_value-timer_old);
    timer_old = timer_value;  
    angle_adjusted_Old = angle_adjusted;
    angle_adjusted = dmpGetPhi();
    mpu.resetFIFO();  // 我们始终复位FIFO
    // 我们计算估计机器人的速度
    actual_robot_speed_Old = actual_robot_speed;
    actual_robot_speed = (speed_m[1] - speed_m[0])/2;  // 正面：前锋
    int16_t angular_velocity = (angle_adjusted-angle_adjusted_Old)*90.0;     // 角速度角度调整角度调整旧
    int16_t estimated_speed = actual_robot_speed_Old - angular_velocity;     // 我们利用机器人速度（T-1）或（T-2），以补偿该延迟
    estimated_speed_filtered = estimated_speed_filtered*0.95 + (float)estimated_speed*0.05;//估计速度估计过滤滤速
    target_angle = speedPIControl(dt,estimated_speed_filtered,throttle,Kp_thr,Ki_thr); //目标角速度PIC ONTROL dt的速度估计过滤油门Kp_thr Ki_thr
    target_angle = constrain(target_angle,-max_target_angle,max_target_angle);   // 有限的输出  目标角度约束目标角度最大目标角度最大目标角度
    //我们整合输出（加速度）
    control_output += stabilityPDControl(dt,angle_adjusted,target_angle,Kp,Kd);	
    control_output = constrain(control_output,-800,800);   // 限制最大输出控制      
    // 控制的转向部分的输出直接注射
    motor1 = control_output + steering;
    motor2 = -control_output + steering;   //马达2反转    
	// 限制最大速度
    motor1 = constrain(motor1,-600,600);   
    motor2 = constrain(motor2,-600,600);
     setMotorSpeed(0,motor1);
     setMotorSpeed(1,motor2); 
      }
}
void kongzhi()
{
    lkf = Serial1.read();
  switch(lkf)
  {
  case 'F':
     qian();
     lkf=0;    
     break; 
   case 'B':
    hou();
    lkf=0; 
     break;     
   case 'R':
     zuo();
     lkf=0; 
     break;
   case 'L':
     you();
     lkf=0; 
     break; 
   case 'S':
    ting();
    lkf=0; 
     break;  
  case '0': 
     kkll=0;
     lkf=0; 
     break;
   case '1': 
     kkll=55;
     lkf=0; 
     break;
   case '2': 
    kkll=110;
    lkf=0; 
    break;
   case '3':
     kkll=165;
    lkf=0;
     break;
  case '4':
     kkll=180;
    lkf=0;
     break;
  case '5':
    kkll=200;
    lkf=0;
    break;
  case '6': 
     kkll=240;
    lkf=0;
     break;
   case '7': 
     kkll=280;
    lkf=0;
     break;
   case '8': 
    kkll=320;
    lkf=0;
    break;
   case '9':
     kkll=360;
     lkf=0;
     break;
   }
}
void qian()
{
   throttle=-kkll;
}
void hou()
{
  throttle = 200;
}
void zuo()
{
    steering = -80;
}
void you()
{
    steering = 80;
}
void ting()
{
    throttle = 0;
   steering =0;
}
