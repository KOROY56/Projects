from asyncio.windows_events import NULL
from turtledemo.nim import COLOR

import numpy 
import win32con
import win32gui
import cv2
import os
import pytesseract

pytesseract.pytesseract.tesseract_cmd = r"C:\Program Files\Tesseract-OCR\tesseract.exe"

default_camera_option = 0 

class Camera_Footage :
    #Default Camera Option in case the user adds something that would create a bug
    default_camera_option = 0
    
    def __init__ (self):
        self.window_name = None
        self.camera_option = None
        self.cam_cap = None
        self.suc = None
        self.frame = None
        self.cam_read = None
        self.key_press = None
        

    def Window_Creation (self,window_name):
        self.window_name = window_name
        cv2.namedWindow(self.window_name)
        print("Window Created Successfully ! ")

    def Camera_Connection (self,num) : 
        self.camera_option = num
        print("Receiving Camera Footage ...")
        if (self.camera_option is None):
            self.cam_cap = cv2.VideoCapture(default_camera_option)
        else:
            self.cam_cap = cv2.VideoCapture(self.camera_option)


    def Camera_Check(self):
        if (self.cam_cap.isOpened()):
            print("Camera Footage Received !")
   
        else:
            print("Camera Footage Troubleshooting !")
   
        return self.cam_cap.isOpened()

    def Camera_Frame_Capture(self):
        self.suc = self.cam_cap.grab()
        
        if self.suc :
            self.suc , self.frame = self.cam_cap.retrieve()
       
            
        return self.suc 
           
    
    def Footage(self):
        print("Camera Running ! ")
        while (self.Camera_Frame_Capture()) :
            cv2.imshow(self.window_name,self.frame)
            self.key_press = cv2.waitKey(1)
            print(f"Just pressed {self.key_press}")

    def Window_Hotkey_Check():
        pass
    
        
        


window_name = str(input("Type a window name : "))
cam_num = int(input("Type in which camera you want to use : "))

cam01 = Camera_Footage() 

cam01.Window_Creation(window_name)
cam01.Camera_Connection(cam_num)
cam01.Camera_Check()
cam01.Camera_Frame_Capture()
cam01.Footage()

        
   