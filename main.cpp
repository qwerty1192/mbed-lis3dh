#include "mbed.h"
#include <cstdint>

I2C     i2c(PB_7, PB_6); // sda, scl

int adrs = 0x19<<1;

void lis3dh_init(){
    char whoami = 0x0f;
    char data;
    i2c.write(adrs,&whoami,1,true);
    i2c.read(adrs,&data,1);
    printf("%x\n", data);

    char tdata[2];
    tdata[0] = 0x20;
    tdata[1] = 0x57;
    i2c.write(adrs,tdata,2);
    tdata[0] = 0x23;
    tdata[1] = 0x08;
    i2c.write(adrs,tdata,2);
}

int lis3dh_status(){
    char stadrs = 0x27;
    char state = 0;
    i2c.write(adrs,&stadrs,1,true);
    i2c.read(adrs,&state,1);
    if((state & 0x80) == 0) return 1;
    return 0;
}

void lis3dh_read(int *out_x,int *out_y,int *out_z){
    char outadrs = 0x28;
    char xh,xl,yh,yl,zh,zl;

    i2c.write(adrs,&outadrs,1,true);
    i2c.read(adrs,&xl,1);
    outadrs++;
    i2c.write(adrs,&outadrs,1,true);
    i2c.read(adrs,&xh,1);
    outadrs++;
    i2c.write(adrs,&outadrs,1,true);
    i2c.read(adrs,&yl,1);
    outadrs++;
    i2c.write(adrs,&outadrs,1,true);
    i2c.read(adrs,&yh,1);
    outadrs++;
    i2c.write(adrs,&outadrs,1,true);
    i2c.read(adrs,&zl,1);
    outadrs++;
    i2c.write(adrs,&outadrs,1,true);
    i2c.read(adrs,&zh,1);

    *out_x = (xh << 8 | xl) >> 4;
    *out_y = (yh << 8 | yl) >> 4;
    *out_z = (zh << 8 | zl) >> 4;

    if(*out_x>=2048) *out_x -= 4096;
    if(*out_y>=2048) *out_y -= 4096;
    if(*out_z>=2048) *out_z -= 4096;
}

int main()
{
    lis3dh_init();
    int out_x,out_y,out_z;
    while(true){
        while(lis3dh_status());
        lis3dh_read(&out_x,&out_y,&out_z);
        printf("%d,%d,%d\n",out_x,out_y,out_z);
    }
}