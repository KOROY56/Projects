from turtledemo.nim import COLOR

import win32con
import win32gui
import cv2
import pytesseract
pytesseract.pytesseract.tesseract_cmd = r"C:\Program Files\Tesseract-OCR\tesseract.exe"


# try indexes 0,1,2 until you hit DroidCam
cap = cv2.VideoCapture(0,cv2.CAP_DSHOW)  # if not, try 1 or 2


count = 1

run = True
msg = True
msg1 = True
states= []
i=0
currentstate = -1
priorstate = -1

while run :

    ret, frame = cap.read()
    if not ret:
        print("Failed to grab frame")
        run =False
        pass
        #break


    if cap.isOpened()!=True:
        print("Camera is not Open !")
    if msg :
        print("Camera is Open !")
        msg = False
    cv2.imshow("camera footage", frame)
    cv2.waitKey(5)

    # Find Footage Renderer/Window output
    hwnd = win32gui.FindWindow(None, "Camera footage")

    if(hwnd==0):
        print("Unable to find the Window")
        run = False
        break
    else :
        if(msg1 == True):
            print("Window Detected")
            msg1 = False


    placementvalue = win32gui.GetWindowPlacement(hwnd)
    renderstate = placementvalue[0]
    #print(f"Render state : {renderstate}")


    left , top , right , bottom = win32gui.GetWindowRect(hwnd)
    #print (left,top,right,bottom) #prints left top right and bottom dimensions of the device's screen
    width = right - left # Calculates the width
    height = bottom - top # Calculates the height
    #print(width, height) # prints the width and the height




    if (i==0):
        priorstate = renderstate
        i=1
    else :
        currentstate = renderstate
        i=0

    if (currentstate != priorstate):
        if(width == 1552 and height == 832):
            print("Window Fullscreen !")
        elif (left < -10000 and top < -10000):
            print("Window Minmimized !")
        else:
            print("Window Nomarlized !")
    else :
        pass











cap.release()
cv2.destroyAllWindows()



