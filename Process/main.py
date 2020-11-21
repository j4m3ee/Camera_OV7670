import time 
from camera import Camera
import image
import serial
import os
import shutil

pc2 = serial.Serial()
pc2.port = 'COM11'
pc2.baudrate = 115200
pc2.bytesize = serial.EIGHTBITS
pc2.stopbits = serial.STOPBITS_ONE
pc2.parity = serial.PARITY_NONE
pc2.timeout = 0.4
pc2.open()

cam = Camera('COM5')
cam.connect()
time.sleep(0.2)

folder = './out/'

def capturePic(path):
    img, Err = cam.getImg()
    tmp = image.markPoint(img)
    pData,allData = image.process(img)
    print(pData,allData)
    cam.save(tmp, path + '.png')
    pc2.write([ord('C')])
    return allData

def firstCapture():
    ls = ['L','C','R']
    for i in ls:
        print('Capture : ' + i,end=' -> ')
        pc2.write([ord(i)])
        time.sleep(1)
        capturePic(folder + i)
    time.sleep(1)

def sentData(data):
    pass

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
    while True:
        while not pc2.inWaiting():
            time.sleep(0.1)
        order = pc2.read_until().decode('ascii')
        order = order[0].capitalize()
        if order == 'T':
            firstCapture()
        elif order == 'C': #Take picture at center
            print('Capture : ',order,end=' -> ')
            pc2.write([ord(order)])
            time.sleep(1)
            lsData = capturePic(folder + order)
            pc2.write(bytearray(lsData))
        elif order == 'L': #Take picture at left
            print('Capture : ' + order,end=' -> ')
            pc2.write([ord(order)])
            time.sleep(1)
            lsData = capturePic(folder + order)
            pc2.write(bytearray(lsData))
        elif order == 'R': #Take picture at right 
            print('Capture : ' + order,end=' -> ')
            pc2.write([ord(order)])
            time.sleep(1)
            lsData = capturePic(folder + order)
            pc2.write(bytearray(lsData))