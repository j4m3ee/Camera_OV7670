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
    if Err: return 'Error image.'
    tmp = image.markPoint(img)
    pData,allData = image.process(img)
    print(pData)
    printDetail(allData)
    cam.save(tmp, path + '.png')
    text = str(pData) +','+ convertListToStr(allData)
    pc2.write((text).encode())
    #pc2.write(("123456789012345678901234").encode())
    return allData

def convertListToStr(ls):
    text = ','.join(str(i) for i in ls)
    return text

def printDetail(ls):
    
    print(','.join(str(ls[i]) for i in range(0,4)),end='')
    print("   Avg =",ls[4])

    print(','.join(str(ls[i]) for i in range(5,9)),end='')
    print("   Avg =",ls[9])

    print(','.join(str(ls[i]) for i in range(10,14)),end='')
    print("   Avg =",ls[14])

    print(','.join(str(ls[i]) for i in range(15,19)),end='')
    print("   Avg =",ls[19])

    print()



def sentData(data):
    pass

opr = ['L','C','R']
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
    print('Ready to capture!!!')

    while True:
        while not pc2.inWaiting():
            time.sleep(0.1)
        order = pc2.read_until().decode('ascii')
        if(order in opr):
            tp = ""
            if order == 'L':
                tp = 'Left'
            elif order == 'C':
                tp = 'Centre'
            elif order == 'R':
                tp = 'Right'
            print('Capture : ' + tp,end=' -> ')
            lsData = capturePic(folder + order)
        elif order == 'A':
            print('PC2 : ackReceive.')
        elif order == 'w':
            print('PC2 : waitting ack.')
        elif order == 'l':
            print('PC2 : timeout.')
        elif order == 'S':
            print('PC2 : waitting order...')
        elif order == 'z':
            print('PC2 : sented.')
        elif order == 'p':
            print('PC2 : sented again.')
        else:
            print('PC2 :',order)

    
