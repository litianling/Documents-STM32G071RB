int SensorINPUTA = 2;      //中断0，也就是数字引脚2---电机A相
int SensorINPUTB = 10;      //电机B相
int i=0;

void exti_moter1(void)
{       
  if(digitalRead(SensorINPUTB) == 1){
    i++;
  }else{
    i--;
  }
  Serial.println(i);
}

void setup() {
  Serial.begin(115200);      //初始化串口，波特率115200
  Serial.println("starting...."); 
  pinMode(SensorINPUTA, INPUT_PULLUP);        
  pinMode(SensorINPUTB, INPUT_PULLUP);        

  //下降沿触发，触发中断0，调用exti_moter1函数
  attachInterrupt(0, exti_moter1, RISING);
}

void loop() { 
  delay(500);
}
