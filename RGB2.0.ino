#include <TimerOne.h> //申明库文件
int ledPin=11;
int pirPin=10;

int pirValue;




//把TCS3200颜色传感器各控制引脚连到Arduino数字端口

#define S0    6   //物体表面的反射光越强，TCS3002D内置振荡器产生的方波频率越高，

#define S1    5  //S0和S1的组合决定输出信号频率比例因子，比例因子为2%

                 //比率因子为TCS3200传感器OUT引脚输出信号频率与其内置振荡器频率之比

#define S2     4   //S2和S3的组合决定让红、绿、蓝，哪种光线通过滤波器

#define S3     3

#define OUT    2  //TCS3200颜色传感器输出信号连接到Arduino中断0引脚，并引发脉冲信号中断

                  //在中断函数中记录TCS3200输出信号的脉冲个数

#define LED    7  //控制TCS3200颜色传感器是否点亮LED灯

float g_SF[3];     //从TCS3200输出信号的脉冲数转换为RGB标准值的RGB比例因子

int   g_count = 0;  // 计算与反射光强相对应TCS3200颜色传感器输出信号的脉冲数

// 数组用于存储在1s内TCS3200输出信号的脉冲数，它乘以RGB比例因子就是RGB标准值

int   g_array[3];  

int   g_flag = 0;   // 滤波器模式选择顺序标志

 

// 初始化TSC3200各控制引脚的输入输出模式

//设置TCS3002D的内置振荡器方波频率与其输出信号频率的比例因子为2%

void TSC_Init()

{

  pinMode(S0, OUTPUT);

  pinMode(S1, OUTPUT);

  pinMode(S2, OUTPUT);

  pinMode(S3, OUTPUT);

  pinMode(OUT, INPUT);

  pinMode(LED, OUTPUT);

  digitalWrite(S0, LOW); 

  digitalWrite(S1, HIGH);

}

 

//选择滤波器模式，决定让红、绿、蓝，哪种光线通过滤波器

void TSC_FilterColor(int Level01, int Level02)

{

  if(Level01 != 0)

    Level01 = HIGH;

  if(Level02 != 0)

    Level02 = HIGH;

  digitalWrite(S2, Level01);

  digitalWrite(S3, Level02);

}

 

//中断函数，计算TCS3200输出信号的脉冲数

void TSC_Count()

{

  g_count ++ ;

}

 

//定时器中断函数，每1s中断后，把该时间内的红、绿、蓝三种光线通过滤波器时，

//TCS3200输出信号脉冲个数分别存储到数组g_array[3]的相应元素变量中

void TSC_Callback()

{

  switch(g_flag)

  {

    case 0:

         Serial.println("->WB Start");

         TSC_WB(LOW, LOW);   //选择让红色光线通过滤波器的模式

         break;

    case 1:

         Serial.print("->Frequency R=");

         Serial.println(g_count);   //打印1s内的红光通过滤波器时，TCS3200输出的脉冲个数

         g_array[0] = g_count;    //存储1s内的红光通过滤波器时，TCS3200输出的脉冲个数

         TSC_WB(HIGH, HIGH);  //选择让绿色光线通过滤波器的模式

         break;

    case 2:

         Serial.print("->Frequency G=");

         Serial.println(g_count);   //打印1s内的绿光通过滤波器时，TCS3200输出的脉冲个数

         g_array[1] = g_count;    //存储1s内的绿光通过滤波器时，TCS3200输出的脉冲个数

         TSC_WB(LOW, HIGH);  //选择让蓝色光线通过滤波器的模式

         break;

 

    case 3:

         Serial.print("->Frequency B=");

         Serial.println(g_count);   //打印1s内的蓝光通过滤波器时，TCS3200输出的脉冲个数

         Serial.println("->WB End");

         g_array[2] = g_count;     //存储1s内的蓝光通过滤波器时，TCS3200输出的脉冲个数

         TSC_WB(HIGH, LOW);   //选择无滤波器的模式  

         break;

   default:

         g_count = 0;     //计数值清零

         break;

  }

}

//设置反射光中红、绿、蓝三色光分别通过滤波器时如何处理数据的标志

//该函数被TSC_Callback( )调用

void TSC_WB(int Level0, int Level1)    

{

  g_count = 0;   //计数值清零

  g_flag ++;     //输出信号计数标志

  TSC_FilterColor(Level0, Level1); //滤波器模式

  Timer1.setPeriod(1000000);     //设置输出信号脉冲计数时长1s

}
#define SSID        "joannahuadu" //改为你的Wi-Fi名称
#define PASSWORD    "haha0505"//Wi-Fi密码
#define HOST_NAME   "api.heclouds.com"
#define DEVICEID    "504941055" //OneNet上的设备ID
#define PROJECTID   "190589" //OneNet上的产品ID
#define HOST_PORT   (80)
String apiKey="aq89Stna4bgdh6iDersLNiECA18=";//与你的设备绑定的APIKey

#define INTERVAL_SENSOR   17000             //定义传感器采样时间间隔  597000
#define INTERVAL_NET      17000             //定义发送时间
//传感器部分================================   
#include <Wire.h>                                  //调用库  
#include <ESP8266.h>
#include <I2Cdev.h>                                //调用库  
/*******温湿度*******/
#include <Microduino_SHT2x.h>
/*******光照*******/
#define  sensorPin_1  A0
#define IDLE_TIMEOUT_MS  3000      // Amount of time to wait (in milliseconds) with no data 
                                   // received before closing the connection.  If you know the server
                                   // you're accessing is quick to respond, you can reduce this value.

//WEBSITE     
char buf[10];

#define INTERVAL_sensor 2000
unsigned long sensorlastTime = millis();

float tempOLED, humiOLED, lightnessOLED;

#define INTERVAL_OLED 1000

String mCottenData;
String jsonToSend;

//3,传感器值的设置 
float sensor_tem, sensor_hum, sensor_lux;                    //传感器温度、湿度、光照   
char  sensor_r_c[7], sensor_g_c[7], sensor_b_c[7] ;    //换成char数组传输
#include <SoftwareSerial.h>
#define EspSerial mySerial
#define UARTSPEED  9600
SoftwareSerial mySerial(8, 9); /* RX:D3, TX:D2 *///改为软串口（rx tx）
ESP8266 wifi(&EspSerial);
//ESP8266 wifi(Serial1);                                      //定义一个ESP8266（wifi）的对象
unsigned long net_time1 = millis();                          //数据上传服务器时间
unsigned long sensor_time = millis();                        //传感器采样时间计时器

//int SensorData;                                   //用于存储传感器数据
String postString;                                //用于存储发送数据的字符串
//String jsonToSend;                                //用于存储发送的json格式参数

Tem_Hum_S2 TempMonitor;

void setup(void)     //初始化函数  
{      

pinMode(11,OUTPUT);
pinMode(10,INPUT);
//digitalWrite(ledPin.LOW);
//serial.begin(9600);79olhynnnnnnnnnnnnnnnnnnnnnnhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh


  //初始化串口波特率  
    Wire.begin();
    Serial.begin(115200);
    while (!Serial); // wait for Leonardo enumeration, others continue immediately
    Serial.print(F("setup begin\r\n"));
    delay(100);
    pinMode(sensorPin_1, INPUT);

  WifiInit(EspSerial, UARTSPEED);

  Serial.print(F("FW Version:"));
  Serial.println(wifi.getVersion().c_str());

  if (wifi.setOprToStationSoftAP()) {
    Serial.print(F("to station + softap ok\r\n"));
  } else {
    Serial.print(F("to station + softap err\r\n"));
  }

  if (wifi.joinAP(SSID, PASSWORD)) {
    Serial.print(F("Join AP success\r\n"));

    Serial.print(F("IP:"));
    Serial.println( wifi.getLocalIP().c_str());
  } else {
    Serial.print(F("Join AP failure\r\n"));
  }

  if (wifi.disableMUX()) {
    Serial.print(F("single ok\r\n"));
  } else {
    Serial.print(F("single err\r\n"));
  }

  Serial.print(F("setup end\r\n"));
  TSC_Init();


  Timer1.initialize();   // defaulte is 1s

  Timer1.attachInterrupt(TSC_Callback); //设置定时器1的中断，中断调用函数为TSC_Callback()

  //设置TCS3200输出信号的上跳沿触发中断，中断调用函数为TSC_Count()

  attachInterrupt(0, TSC_Count, RISING);

  digitalWrite(LED, HIGH);//点亮LED灯

  delay(4000); //延时4s，以等待被测物体红、绿、蓝三色在1s内的TCS3200输出信号脉冲计数

  //通过白平衡测试，计算得到白色物体RGB值255与1s内三色光脉冲数的RGB比例因子

  g_SF[0] = 255.0/ g_array[0];     //红色光比例因子

  g_SF[1] = 255.0/ g_array[1] ;    //绿色光比例因子

  g_SF[2] = 255.0/ g_array[2] ;    //蓝色光比例因子

  //打印白平衡后的红、绿、蓝三色的RGB比例因子

  Serial.println(g_SF[0],5);

  Serial.println(g_SF[1],5);

  Serial.println(g_SF[2],5);

  //红、绿、蓝三色光分别对应的1s内TCS3200输出脉冲数乘以相应的比例因子就是RGB标准值

  //打印被测物体的RGB值

  for(int i=0; i<3; i++)

    Serial.println(int(g_array[i] * g_SF[i]));   
    
  
}
void loop(void)     //循环函数  
{   
 
pirValue=digitalRead(pirPin);
digitalWrite(ledPin,pirValue);


  if (sensor_time > millis())  sensor_time = millis();  
    
  if(millis() - sensor_time > INTERVAL_SENSOR)              //传感器采样时间间隔  
  {  
    getSensorData();                                        //读串口中的传感器数据
    sensor_time = millis();
  }  

    
  if (net_time1 > millis())  net_time1 = millis();
  
  if (millis() - net_time1 > INTERVAL_NET)                  //发送数据时间间隔
  {                
    updateSensorData();                                     //将数据上传到服务器的函数
    net_time1 = millis();
  }
   g_flag = 0;

   //每获得一次被测物体RGB颜色值需时4s

   delay(4000);

   //打印出被测物体RGB颜色值

   for(int i=0; i<3; i++)

      Serial.println(int(g_array[i] * g_SF[i])); 
}

void getSensorData(){  
   /* sensor_tem = TempMonitor.getTemperature();  
    sensor_hum = TempMonitor.getHumidity();   
    //获取光照
    sensor_lux = analogRead(A0);    
    delay(1000);*/
     dtostrf(int(g_array[0] * g_SF[0]),3,1, sensor_r_c);
    dtostrf(int(g_array[1] * g_SF[1]),3,1, sensor_g_c);
    dtostrf(int(g_array[2] * g_SF[2]),3,1, sensor_b_c);
}
void updateSensorData() {
  if (wifi.createTCP(HOST_NAME, HOST_PORT)) { //建立TCP连接，如果失败，不能发送该数据
    Serial.print("create tcp ok\r\n");

jsonToSend="{\"R\":";
    dtostrf(int(g_array[0] * g_SF[0]),3,1,buf);
    jsonToSend+="\""+String(buf)+"\"";
    jsonToSend+=",\"G\":";
    dtostrf(int(g_array[1] * g_SF[1]),3,1,buf);
    jsonToSend+="\""+String(buf)+"\"";
    jsonToSend+=",\"B\":";
    dtostrf(int(g_array[2] * g_SF[2]),3,1,buf);
    jsonToSend+="\""+String(buf)+"\"";
    jsonToSend+="}";



    postString="POST /devices/";
    postString+=DEVICEID;
    postString+="/datapoints?type=3 HTTP/1.1";
    postString+="\r\n";
    postString+="api-key:";
    postString+=apiKey;
    postString+="\r\n";
    postString+="Host:api.heclouds.com\r\n";
    postString+="Connection:close\r\n";
    postString+="Content-Length:";
    postString+=jsonToSend.length();
    postString+="\r\n";
    postString+="\r\n";
    postString+=jsonToSend;
    postString+="\r\n";
    postString+="\r\n";
    postString+="\r\n";

  const char *postArray = postString.c_str();                 //将str转化为char数组
  Serial.println(postArray);
  wifi.send((const uint8_t*)postArray, strlen(postArray));    //send发送命令，参数必须是这两种格式，尤其是(const uint8_t*)
  Serial.println("send success");   
     if (wifi.releaseTCP()) {                                 //释放TCP连接
        Serial.print("release tcp ok\r\n");
        } 
     else {
        Serial.print("release tcp err\r\n");
        }
      postArray = NULL;                                       //清空数组，等待下次传输数据
  
  } else {
    Serial.print("create tcp err\r\n");
  }
}
