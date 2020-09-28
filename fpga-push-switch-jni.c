/* FPGA FPGA Push Switch Native Interface
File : fpga-push-switch-jni.c
Auth : largest@huins.com */

#include <string.h>
#include <jni.h>
#include <android/log.h>
#include <fcntl.h> 
#include "fpga_dot_font.h"

#define PUSH_SWITCH_DEVICE "/dev/fpga_push_switch"
#define MAX_BUTTON 9
#define BUZZER_DEVICE "/dev/fpga_buzzer"
#define STEP_DEVICE "/dev/fpga_step_motor"
#define LED_DEVICE "/dev/fpga_led"
#define MAX_DIGIT 4
#define LINE_BUFF 16
#define MAX_BUFF 32
#define FPGA_TEXT_LCD_DEVICE "/dev/fpga_text_lcd"
#define DOT_DEVICE "/dev/fpga_dot"
#define FND_DEVICE "/dev/fpga_fnd"
int gFd=-1;

int fpga_push_open(void)
{
	int dev;

	dev = open(PUSH_SWITCH_DEVICE, O_RDWR);
	if (dev<0) {
		__android_log_print(ANDROID_LOG_INFO, "Device Open Error", "Driver = %d", dev);
		return -1;
	} else {
		gFd = dev;
	}
	return 0;
}

int fpga_push_close(void)
{

	if (gFd<0) {
		__android_log_print(ANDROID_LOG_INFO, "Device not opend!", "Driver = %d", gFd);
		return 0;
	} else {
		close(gFd);
		__android_log_print(ANDROID_LOG_INFO, "Device Close!", "Driver = %d", gFd);
		return -1;
	}
}


int fpga_push_switch(void)
{
    int i;
    int dev;
    int buff_size;
    int retval;

    unsigned char push_sw_buff[MAX_BUTTON];

	
	if (gFd<0) {
		__android_log_print(ANDROID_LOG_INFO, "Device Open Error", "Driver = %d", dev);
		return -1;
	} else {
       		__android_log_print(ANDROID_LOG_INFO, "Device Open Success", "Driver = %d", dev);
		 buff_size=sizeof(push_sw_buff);
		read(gFd, &push_sw_buff, buff_size);

		retval=0;

	        for(i=0;i<MAX_BUTTON;i++) {
		if(push_sw_buff[i]!=0)
			retval |= 0x1 << i; // 키 값을 읽 해당 자리 만큼 시프트 하여 숫자를 			    
       		}  
             
        }
	return retval;
}



//////////////////*fpga_fnd start*//////////////////


int fpga_fnd(const char* str)
{
    int dev;
    unsigned char data[4];
    unsigned char retval;
    int i;
    int str_size;

    memset(data,0,sizeof(data));

    str_size=(strlen(str));
    if(str_size>MAX_DIGIT)
    {
        str_size=MAX_DIGIT;
    }

    for(i=0;i<str_size;i++)
    {
        if((str[i]<0x30)||(str[i])>0x39) {
            return 1;
        }
        data[i]=str[i]-0x30;
    }

    dev = open(FND_DEVICE, O_RDWR);
    if (dev<0) {
        __android_log_print(ANDROID_LOG_INFO, "Device Open Error", "Driver = %s", str);
	return -1;
    } else {
        __android_log_print(ANDROID_LOG_INFO, "Device Open Success", "Driver = %d", str);
        write(dev,&data,4);
	close(dev);
	return 0;
    }
}


//////////////////*fpga_fnd end*//////////////////

int fpga_buzzer(int x)
{
    int dev;
    unsigned char data;
    unsigned char retval;

    data = (char)x;

    dev = open(BUZZER_DEVICE, O_RDWR);
    if (dev<0) {
        __android_log_print(ANDROID_LOG_INFO, "Device Open Error", "Driver = %d", x);
        return -1;
    } else {
        __android_log_print(ANDROID_LOG_INFO, "Device Open Success", "Driver = %d", x);
        write(dev,&data,1);
        close(dev);
        return 0;
    }
}

int fpga_step_motor(int action, int direction, int speed)
{
    int i;
    int dev;
    int str_size;

    unsigned char motor_state[3];

    if(speed>250)
        speed = 250;
    else if(speed<5)
        speed = 5;

    memset(motor_state,0,sizeof(motor_state));

    motor_state[0] = (unsigned char)action;
    motor_state[1] = (unsigned char)direction;
    motor_state[2] = (unsigned char)speed;

    dev = open(STEP_DEVICE, O_RDWR);
    if (dev<0) {
        __android_log_print(ANDROID_LOG_INFO, "Device Open Error", "Driver = %d", dev);
    } else {
        __android_log_print(ANDROID_LOG_INFO, "Device Open Success", "Driver = %d", dev);
        write (dev, motor_state, sizeof(motor_state));
        close(dev);
    }
}

int fpga_led(int x)
{
    int dev;
    unsigned char data;
    unsigned char retval;

    //                   1    2    3    4    5    6    7    8   OFF    O
    unsigned char val[] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x00,0xFF};

    dev = open(LED_DEVICE, O_RDWR);
    if (dev<0) {
        __android_log_print(ANDROID_LOG_INFO, "Device Open Error", "Driver = %d", x);
    } else {
        __android_log_print(ANDROID_LOG_INFO, "Device Open Success", "Driver = %d", x);
        write (dev, &val[x], sizeof(unsigned char));
        close(dev);
    }
}

int fpga_text_lcd(const char* str1, const char* str2)
{
    int i;
    int dev;
    int str_size;
    int chk_size;

    unsigned char string[32];
    memset(string, 0, sizeof(string));

    dev = open(FPGA_TEXT_LCD_DEVICE, O_RDWR);
    if (dev<0) {
        __android_log_print(ANDROID_LOG_INFO, "Device Open Error", "Driver = %d", dev);
        return -1;
    }
    else {
        str_size = strlen(str1);
        if (str_size>0) {
            strncat(string, str1, str_size);
            memset(string + str_size, ' ', LINE_BUFF - str_size);
        }

        str_size = strlen(str2);
        if (str_size>0) {
            strncat(string, str2, str_size);
            memset(string + LINE_BUFF + str_size, ' ', LINE_BUFF - str_size);
        }

        write(dev, string, MAX_BUFF);

        close(dev);
    }
}

int fpga_dot(int x)
{
    int i;
    int dev;
    int str_size;

    str_size=sizeof(fpga_number[x]);

    dev = open(DOT_DEVICE, O_RDWR);
    if (dev<0) {
        __android_log_print(ANDROID_LOG_INFO, "Device Open Error", "Driver = %d", x);
    } else {
        __android_log_print(ANDROID_LOG_INFO, "Device Open Success", "Driver = %d", x);
        switch(x) {
            case 10 :
                write(dev,fpga_set_full,sizeof(fpga_set_full));
                break;
            case 11 :
                write(dev,fpga_set_blank,sizeof(fpga_set_blank));
                break;
            default :
                write(dev,fpga_number[x],str_size);
                break;
        }
        close(dev);
    }
}



jint Java_achro4_fpga_ex6_FpgaPushSwitchActivity_ReceivePushSwitchValue( JNIEnv* env,jobject thiz)  
{ 
	int retval; 
	__android_log_print(ANDROID_LOG_INFO, "FpgaPushSwitchExample", "push read");
	retval=fpga_push_switch();
}

jint Java_achro4_fpga_ex6_FpgaPushSwitchActivity_DeviceOpen( JNIEnv* env,jobject thiz)  
{ 
	int retval; 
	retval=fpga_push_open();
}

jint Java_achro4_fpga_ex6_FpgaPushSwitchActivity_DeviceClose( JNIEnv* env,jobject thiz)  
{ 
	int retval; 
	retval=fpga_push_close();
}


//////////////////*fpga_fnd start*//////////////////

JNIEXPORT jint JNICALL Java_achro4_fpga_ex6_FpgaPushSwitchActivity_ReceiveFndValue( JNIEnv* env, jobject thiz, jstring val )
{
    jint result;
    const char * str = (*env)->GetStringUTFChars(env,val,NULL);
    __android_log_print(ANDROID_LOG_INFO, "FpgaFndExample", " value = %s", str);
    result=fpga_fnd(str);
    (*env)->ReleaseStringUTFChars(env, val, str);

    return result;
}

//////////////////*fpga_fnd end*//////////////////

jint Java_achro4_fpga_ex6_FpgaPushSwitchActivity_FpgaSetBuzzerValue(JNIEnv* env, jobject thiz, jint val)
{
    jint result;
    __android_log_print(ANDROID_LOG_INFO, "FpgaBuzzerExample", " value = %d", val);
    result = fpga_buzzer(val);

    return result;
}

void Java_achro4_fpga_ex6_FpgaPushSwitchActivity_FpgaSetMotorState(JNIEnv* env, jobject thiz, jint act, jint dir, jint spd)
{
    __android_log_print(ANDROID_LOG_INFO, "FpgaStepMotorExample", "SetMotor");
    fpga_step_motor(act, dir, spd);
}

void Java_achro4_fpga_ex6_FpgaPushSwitchActivity_FpgaSetLedValue(JNIEnv* env, jobject thiz, jint val)
{
    __android_log_print(ANDROID_LOG_INFO, "FpgaLedJniExample", "led value = %d", val);
    fpga_led(val);
}

JNIEXPORT jint JNICALL Java_achro4_fpga_ex6_FpgaPushSwitchActivity_FpgaSetTextLcdValue(JNIEnv* env, jobject thiz, jstring val1, jstring val2)
{
    jint result;
    const char * pstr1 = (*env)->GetStringUTFChars(env, val1, NULL);
    __android_log_print(ANDROID_LOG_INFO, "FpgaFndExample", " value = %s", pstr1);

    const char * pstr2 = (*env)->GetStringUTFChars(env, val2, NULL);
    __android_log_print(ANDROID_LOG_INFO, "FpgaFndExample", " value = %s", pstr2);


    fpga_text_lcd(pstr1, pstr2);

    (*env)->ReleaseStringUTFChars(env, val1, pstr1);
    (*env)->ReleaseStringUTFChars(env, val2, pstr2);

    return result;
}

jstring Java_achro4_fpga_ex6_FpgaPushSwitchActivity_FpgaSetDotValue( JNIEnv* env,jobject thiz, jint val )
{
    __android_log_print(ANDROID_LOG_INFO, "FpgaDotDotExample", "dot value = %d", val);
    fpga_dot(val);
}






