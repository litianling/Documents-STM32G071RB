	/*****************HJ-4WD慧净四驱蓝牙智能小车例程*****************
*  平台：HJ-4WD/HJ-E/HL-1智能小车 + Keil U4 + STC89C52
*  名称：
*  公司：慧净电子科技有限公司
*  淘宝：http://shop37031453.taobao.com       
*  网站：www.hjmcu.com	智能小车专用网站：WWW.HLMCU.COM
*  编写：
*  日期：2008-8-08
*  交流:智能车QQ:1526248688  开发板QQ:398115088
*  晶振:11.0592MHZ
*  说明：免费开源，不提供源代码分析，有问题直接到慧净论坛交流
*  论坛：http://hjdz.haotui.com 
*  免费结缘：5星好评赠送18个资料包，追加评价后再加送20G资料，网上网盘下载
*  视频教程：本小车配套详细视频教程，有需要请到慧净网站免费下载
******************************************************************/
    // 本程序不输入PWM，J21 PWM 4个跳线帽不要装上，把J20(IN)/J22(EN) 8个跳线帽装上，这样小车就能全速度运行。	
	//注意程序只做参考之用，要达到最理想的效果，还需要同学们细心调试。	
	//注意超声波接线，急性之人经常接错线，烧坏模块，请一定要核对好再通电试验，模块接错线烧坏不保修，不换货的。
	/****************************************************************************
	 硬件连接
	 测速模块有4条线定义：
	  VCC-----接到小车上的VCC
      GND-----接到小车上的GND
	  OUT1-----接到小车上的P3.2
	  OUT2-----接到小车上的P3.3
	 把模块用杜邦线正确接入单片机中	 --注意不要接错线，否则烧坏模块，不保修，不换货。
	****************************************************************************/

#include<AT89x51.H>

	//HJ-4WD小车驱动接线定义
    #define Left_moto_go      {P1_4=1,P1_5=0,P1_6=1,P1_7=0;}    //左边两个电机向前走
	#define Left_moto_back    {P1_4=0,P1_5=1,P1_6=0,P1_7=1;}    //左边两个电机向后转
	#define Left_moto_Stop    {P1_4=0,P1_5=0,P1_6=0,P1_7=0;}    //左边两个电机停转                     
	#define Right_moto_go     {P1_0=1,P1_1=0,P1_2=1,P1_3=0;}	//右边两个电机向前走
	#define Right_moto_back   {P1_0=0,P1_1=1,P1_2=0,P1_3=1;}	//右边两个电机向后走
	#define Right_moto_Stop   {P1_0=0,P1_1=0,P1_2=0,P1_3=0;}	//右边两个电机停转    



    #define Left_1_led        P3_7	 //P3_7接四路寻迹模块接口第一路输出信号即中控板上面标记为OUT1
	#define Left_2_led        P3_6	 //P3_6接四路寻迹模块接口第二路输出信号即中控板上面标记为OUT2	

    #define Right_1_led       P3_5	 //P3_5接四路寻迹模块接口第三路输出信号即中控板上面标记为OUT3
	#define Right_2_led       P3_4	 //P3_4接四路寻迹模块接口第四路输出信号即中控板上面标记为OUT4
	
	
	sbit led1    =P2^0;		   
    sbit led2    =P2^1;
    sbit led3    =P2^2;
    sbit led4    =P2^3;


    unsigned code table[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};	  //数码管断码
	unsigned code dis[]  ={0xfe,0xfd,0xfb,0xf7};	                              //扫描数码管客值
    unsigned char disbuff[5]={0};	
	unsigned char  time=0;											  //显示缓存
   	unsigned char  i =0; 		 //定义扫描数码管字数
	unsigned int  count1=0;	 //计左电机码盘脉冲值
	unsigned int   V=0;		  //定义其速度
   
/************************************************************************/	
//延时函数	
   void delay(unsigned int k)
{    
     unsigned int x,y;
	 for(x=0;x<k;x++) 
	   for(y=0;y<2000;y++);
}
/************************************************************************/
//显示数码管字程序

      	 void Display_SMG(void)
{
      if(++i>=4)i=0;
	  P0=table[disbuff[i]];
	  P2=dis[i];
 }

/************************************************************************/
///*TIMER0中断服务子函数产生2MS定时扫描数码管与产生0。5S*/
 	void timer0()interrupt 1   using 2
{
      TH0=(65536-2000)/256;		//2MS定时
      TL0=(65536-2000)%256;

	 time++;
	 Display_SMG();
	 if(time>=250)	//250次即是，0。5S
	 {
	  time=0;
	  V=count1*2;	 //计数公式：轮子直径*3.14/20格码盘=6.5Cm*3.14/20约=1cm  即一个脉冲走1CM距离	((count1*1))/0.5S= (count1*2)CM/S
	  count1=0;		 //清计数						   														  
	  disbuff[0]=V/1000;					   //更新显示																	
	  disbuff[1]=V%1000/100;
	  disbuff[2]=V%1000%100/10;
      disbuff[3]=V %1000%100%10;
	 }
 }	

/***************************************************/
 //外部0中断用于计算左轮的脉冲
   void intersvr1(void) interrupt 0 using 1

{
    count1++;	
}
/************************************************************************/
//前速前进
     void  run(void)
{
    
	 Left_moto_go ;   //左电机往前走
	 Right_moto_go ;  //右电机往前走
}

//前速后退
     void  backrun(void)
{
    
	 Left_moto_back ;   //左电机往前走
	 Right_moto_back ;  //右电机往前走
}

//左转
     void  leftrun(void)
{
    
	 Left_moto_back ;   //左电机往前走
	 Right_moto_go ;  //右电机往前走
}

//右转
     void  rightrun(void)
{
    
	 Left_moto_go ;   //左电机往前走
	 Right_moto_back ;  //右电机往前走
}
//停转
     void  stoprun(void)
{
    
	 Left_moto_Stop ;   //左电机往前走
	 Right_moto_Stop ;  //右电机往前走
}


/*********************************************************************/		 
/*--主函数--*/
	void main(void)
{

    TMOD=0X01;
    TH0=(65536-2000)/256;		//2MS定时
    TL0=(65536-2000)%256;
	TR0= 1;
	ET0= 1;

	EX0=1;	   //开启外部中断0
    IT0=1;	   //下降沿有效
	IE0=0;
	EA = 1;
    run();
	
		while(1)	//无限循环
	{ 
	 
			 //有信号为1  没有信号为0

              if(Left_2_led==1&&Right_1_led==1)

			  run();

			  else
			 {			  
				               if(Left_2_led==0&&Right_1_led==1)	    //左边检测到物体
			 	 {
			
				 Right_moto_go;						   //右边两个电机正转

				 Left_moto_Stop
			     }
			   
				 			    if(Right_1_led==0&&Left_2_led==1)		//右边检测到物体
				  {	  
		
			      Left_moto_go;						   //左边两个电机正转

				  Right_moto_Stop
				  }

				  				 			    if(Right_1_led==0&&Left_2_led==0)		//两边同时检测到物体
				  {	  
		
			      backrun();  //	前速后退
				  
				  delay(50);						   
				  leftrun();  //左转
				  delay(30);
				  			  				  
				  }
			}	 
	 }
}