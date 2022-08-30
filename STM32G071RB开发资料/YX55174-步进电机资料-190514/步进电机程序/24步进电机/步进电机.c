#include <reg52.h>


unsigned char code F_Rotation[4]={0x02,0x04,0x08,0x10};//正转表格
unsigned char code B_Rotation[4]={0x10,0x08,0x040,0x02};//反转表格

void Delay(unsigned int i)//延时
{
 while(--i);
}

main()
{
 
 unsigned char i;

 while(1)
 {
  for(i=0;i<4;i++)      //4相
     {
     P1=F_Rotation[i];  //输出对应的相 可以自行换成反转表格
     Delay(500);        //改变这个参数可以调整电机转速
	 }
  }
}