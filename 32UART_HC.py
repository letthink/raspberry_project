# Author: Lishiyao
# CreatTime: 2021/2/9 23 40
# FileName: 32UART_HC 
# Description: Simple introduction of the code
#coding=utf-8
#不加上行申明，不可用中文注释

import serial
import time
import RPi.GPIO as GPIO
#引脚的设置
# Trig_pin = 20
# Echo_pin = 21
LED0 = 26
LED1 = 17
GPIO.setmode(GPIO.BCM)
GPIO.setup(LED1 ,GPIO.OUT, initial=GPIO.HIGH)
GPIO.setup(LED0 ,GPIO.OUT, initial=GPIO.HIGH)

ser = serial.Serial("/dev/ttyAMA0",115200)

ser.flushInput()

def main():
    GPIO.output(LED1, GPIO.HIGH)
    GPIO.output(LED0, GPIO.LOW)
    # time.sleep(1)
    # GPIO.output(LED1, GPIO.LOW)
    #st = '\r'

    while True:
        # GPIO.output(LED1,GPIO.HIGH)
        ser.flushInput()
        GPIO.output(LED0, GPIO.HIGH)
        time.sleep(0.5)

        RxData = ser.inWaiting()
        # print(type(RxData),",,,,",RxData)
        # # for i in RxData:
        # #     print("Every RxData:",i)
        # if RxData != 0 :
        #     rcv = ser.read(ser.in_waiting).decode("gbk")
        #     print(rcv)
        #     # print(type(rcv))
        tmp = ser.read(2)
        print("The data is :",tmp)
        #print(type(tmp),"++++",tmp)
        # for i in rcv:
        #     print("----",i,"---")
        # print('=====',st)
        if tmp == b'01': #此处注意二进制的表达方式（bytes）
            GPIO.output(LED1, GPIO.LOW)
            ser.write("1".encode("utf-8"))
        elif tmp == b'02':
            GPIO.output(LED1, GPIO.HIGH)
            ser.write("2".encode("utf-8"))
        GPIO.output(LED0, GPIO.LOW)
        time.sleep(0.1)

if __name__ == '__main__':
    main()