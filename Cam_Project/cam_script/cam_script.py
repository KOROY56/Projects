from asyncio.windows_events import NULL
from turtledemo.nim import COLOR

import numpy 
import time
import win32con
import win32gui
import cv2
import os
import pytesseract
from paddleocr import PaddleOCR

pytesseract.pytesseract.tesseract_cmd = r"C:\Program Files\Tesseract-OCR\tesseract.exe"

os.environ['PADDLE_PDX_DISABLE_MODEL_SOURCE_CHECK'] = 'True'
ocr = PaddleOCR(use_textline_orientation=True,lang='en')





    
    
 #Camera Footage Class        

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
        self.available_cameras = 0
        

    #Setters 

    def set_window_name(self):
        return self.window_name

    def set_window_hotkey_press(self):
        return self.keypress


    def Window_Creation (self,window_name):
        self.window_name = window_name
        cv2.namedWindow(self.window_name,cv2.WINDOW_NORMAL)
        print("Window Created Successfully ! ")

    def Camera_Connection (self,num) : 
        self.camera_option = num
        self.Available_Cameras_Count()

        print(f"There are {self.available_cameras} cameras available !")

        print("Receiving Camera Footage ...")

        
        if (self.camera_option == None or self.camera_option > self.available_cameras ):
            self.cam_cap = cv2.VideoCapture(self.default_camera_option)
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


    def Available_Cameras_Count(self):
        
        for i in range(0,10):
            cap = cv2.VideoCapture(i)
            if (cap.isOpened()):
                ret  = cap.grab()
                if ret :
                    self.available_cameras += 1
                    print(f"The available camera is camera : {i}")
                   
            cap.release()

        return self.available_cameras



    def Footage(self):
        
        keystorage = [0,0]
        message = True
        first = True
        
        
        #while (self.Camera_Frame_Capture()) :
        if message:
            print("Attempting to Run Camera ... ")
            print("Camera is Running !")
            message = False

        cv2.imshow(self.window_name,self.frame)
        
        if (first):
            self.key_press = cv2.waitKey(1)
            keystorage[0].append(self.keypress)  
            first = False
        else:
            self.keypress = cv2.waitKey(1)
            keystorage[1].append(self.keypress)
            first = True

        print(f"Just pressed {self.key_press}")

           
            

   
    
    def Window_Keyboard_Hotkey_Check():
        pass        
    


    def release_camera(self):
        self.cam_cap.release()

    def close_window(self):
        cv2.destroyWindow(self.window_name)

    def close_all_windows(self):
        cv2.destroyAllWindows()
        




#Window Manager (Mainly for window HOTKEYS and not 'KEYBOARD KEYS')

class Window_Manager : 
    
    def __init__ (self,cam_object) :

        self.camera = cam_object #Which camera is it , each object that it is created has different ID value 
                                 #Therefore each camera object that we create has a different value a uniquee
                                 #So that the program would know which one we are referring we must add the object's name
                                 #within the parethensis when we call the Camera_Footage class's funtion to send Object Data

        self.window_id = None
        self.window_name = "" #Opencv Window Name from class Camera_Footage 
        self.window_state = "" #To check if the window is minimized or maximized or normal or restored
        self.window_hotkey_state = None #Which state has the user pressed the window to be in (maximum,minimum,restored,closed)
                                        #This variables can take values either of win32con.SW_SHOWNORMAL , win32con.SW_SHOWMINIMIZE and SW.SHOWMAXIMIZE
                                        #each of the values represent what they mean by their naming . Plus SHOWNORMAL stands for Restore and Normal state

        self.cvhotkey_press = None # this variable is simply added because from waitKey function can be received a hotkey value  
                                       # to a variable and if waitKey is called the previous hotkey value might lost 

    def get_window_name(self):
        self.window_name = self.camera.set_window_name()

    def get_window_id(self):
        self.window_id = win32gui.FindWindow(None,self.window_name)
    
    
    def get_cvhotkey_press(self,key):
        self.cvhotkey_press = self.camera.set_window_hotkey_press()


    def Hotkey_Handle(self):

        show1 = True
        show2 = True
        show3 = True

        try:   

            if not win32gui.IsWindow(self.window_id):
                print("Window Closed !")
                Camera_Footage.release_camera()
                Camera_Footage.close_widow()
            placement = win32gui.GetWindowPlacement(self.window_id)  
            self.window_hotkey_state = placement[1]

            if(win32con.IsZoomed(self.window_id)):
                win32gui.ShowWindow(self.window_name,win32con.SW_MAXIMIZE)
                self.window_state = "MAXIMIZED"
                if self.window_state == "MAXIMIZED" :
                    if show1:
                        print("Window MAXIMIZED !")
                        show1 = False
                        show2 = True
                        show3 = True

            elif(win32con.IsIconic(self.window_id)):
                win32gui.ShowWindow(self.window_name,win32con.SW_MINIMIZE)
                self.window_state = "MINIMIZED"
                if (self.window_state == "MINIMIZED"):
                    if (show2):
                        print("Window MINIMIZED ! ")
                        show1 = True
                        show2 = False
                        show3 = True

            else :
                self.window_state = "NORMAL"
                if self.window_state == "NORMAL" :
                    if show3:
                        print("Window NORMAL !")
                        show1 = True
                        show2 = True
                        show3 = False


        except Exception as  e:  
            print(f"Error : {e} ")
            print("On line 200 ")



window_name = str(input("Type a window name : "))
cam_num = int(input("Type in which camera you want to use : "))

cam01 = Camera_Footage() 

cam01.Window_Creation(window_name)
cam01.Camera_Connection(cam_num)
cam01.Camera_Check()

wm_cam01 = Window_Manager(cam01)

while(cam01.Camera_Frame_Capture()):


    cam01.Footage()
    

    wm_cam01.get_window_name()
    wm_cam01.get_window_id()
    wm_cam01.Hotkey_Handle()
   