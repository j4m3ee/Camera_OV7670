import numpy as np
import cv2

xPos = [30,45,75,90]
yPos = [40,60,100,120]

def markPoint(img):
    tmpImg = []
    tmpImg = np.copy(img)
    for x in xPos:
        for y in yPos:
            tmpImg[y,x] = 255
    cv2.destroyWindow('camera output')
    cv2.imshow('camera output', tmpImg)
    cv2.waitKey(1)
    return tmpImg

def getValue(img):
    data = []
    tmpImg = np.copy(img)
    for x in xPos:
        for y in yPos:
            #print('Value x = {},y = {} is {}'.format(x,y,tmpImg[y,x]))
            data.append(tmpImg[y,x][0])
    return data 

def process(img):
    data = getValue(img)
    mean = (max(data)-min(data))//2
    q1 = [data[0],data[4],data[1],data[5]]
    q2 = [data[8],data[12],data[9],data[13]]
    q3 = [data[2],data[6],data[3],data[7]]
    q4 = [data[10],data[14],data[11],data[15]]
    qAv= [0,0,0,0]
    bitData = [-1,-1,-1,-1]
    for i in q1:
        qAv[0] += i
    qAv[0] //= 4
    for i in q2:
        qAv[1] += i
    qAv[1] //= 4
    for i in q3:
        qAv[2] += i
    qAv[2] //= 4
    for i in q4:
        qAv[3] += i
    qAv[3] //= 4
    #print(qAv)
    for i in range(4):
        if qAv[i] < mean:
            bitData[i] = 0
        else:
            bitData[i] = 1
    strBit = ''.join(str(i) for i in bitData)
    packList = list(q1 + [qAv[0]] + q2 + [qAv[1]] + q3 + [qAv[2]] + q4 +[qAv[3]])
    return (strBit,packList)
    #return (strBit + " : " + str(int(strBit,2)),packList)


    
    