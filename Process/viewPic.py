import time 
from camera import Camera
import image
import serial
import os
import shutil

servo = serial.Serial()
servo.port = 'COM11'
servo.baudrate = 115200
servo.bytesize = serial.EIGHTBITS
servo.stopbits = serial.STOPBITS_ONE
servo.parity = serial.PARITY_NONE
servo.timeout = 0.4
servo.open()

cam = Camera('COM5')
cam.connect()
time.sleep(0.2)

folder = './out/'

def inputSheet():
    print('='*20)
    print('>>> Input list <<<')
    print('T : Recapture.')
    print('C : Center capture.')
    print('L : Left capture.')
    print('R : right capture.')
    print('Q : Quit/Exit.')
    print('='*20)

def capturePic(path):
    img, Err = cam.getImg()
    tmp = image.markPoint(img)
    pData,alldata = image.process(img)
    print(pData,alldata)
    cam.save(tmp, path + '.png')
    servo.write([ord('C')])
    return alldata

def firstCapture():
    ls = ['L','C','R']
    for i in ls:
        print('Capture : ' + i,end=' -> ')
        servo.write([ord(i)])
        time.sleep(1)
        capturePic(folder + i)
    time.sleep(1)

if __name__ == "__main__":
    for file in os.listdir(folder):
        Path = os.path.join(folder,file)
        try:
            if os.path.isfile(Path):
                os.unlink(Path)
            elif os.path.isdir(Path):
                shutil.rmtree(Path)
        except Exception as e:
            print('Failed to delete file {}. Cause {}'.format(Path,e))
    firstCapture()
    inputSheet()
    while True:
        inp = input("input order : ")
        inp = inp.capitalize()
        if inp == 'T':
            firstCapture()
        elif inp == 'C': #Take picture at center
            print('Capture : ',inp,end=' -> ')
            servo.write([ord(inp)])
            time.sleep(1)
            capturePic(folder + inp)
            
        elif inp == 'L': #Take picture at left
            print('Capture : ' + inp,end=' -> ')
            servo.write([ord(inp)])
            time.sleep(1)
            capturePic(folder + inp)
        elif inp == 'R': #Take picture at right 
            print('Capture : ' + inp,end=' -> ')
            servo.write([ord(inp)])
            time.sleep(1)
            capturePic(folder + inp)
        elif inp == 'Q':
            break
        else:
            print('Not macth any choice!')