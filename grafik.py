# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""
from sys import argv
import cv2 as cv 
import numpy as np
import sys
import os

def getImage(file, rows, cols):
    fd = open(file, 'rb')
    f = np.fromfile(fd, dtype=np.uint8,count=rows*cols)
    im = f.reshape((rows, cols)) 
    
    im=np.dstack([im, im , im])
    fd.close()
    return im


def drawLines(mat, cords):
    print len(cords)
    for i in np.arange(0, len(cords)-1, 1):
        print cords[i]
        cv.line(mat, cords[i], cords[i+1], (0 , 255, 0), 1)
        
    return mat

def main():
    if len(argv)<2:
        print "Too few arguments"
        sys.exit(1)
    print len(argv)
    imgLoc=argv[2]
    resLoc=argv[1]
    imgRows=int(argv[3])
    imgCols=int(argv[4])
    imgRaw=getImage(imgLoc, imgRows, imgCols)
    if len(argv)<=5:
        img=getImage(imgLoc, imgRows, imgCols)
        img=cv.resize(img, (imgCols*50, imgRows*50), interpolation=cv.INTER_NEAREST)
        if cv.imwrite(os.path.join(os.getcwd(), resLoc+".jpeg"), img)==0:
            print "Greska"
            sys.exit(1)
        else:
            cordList=[]

        for i in np.arange(5, len(argv)-1, 2):
            cordList.append((int(argv[i]), int(argv[i+1])))
        img=getImage(imgLoc, imgRows, imgCols)
        img=drawLines(img, cordList)
        print os.path.join(os.path.join(os.getcwd(), resLoc), ".jpeg")
        img=cv.resize(img, (imgCols*50, imgRows*50), interpolation=cv.INTER_NEAREST)
        if cv.imwrite(os.path.join(os.getcwd(), resLoc+".jpeg"), img)==0:
            print "Greska"
            exit(1)


if __name__=="__main__":
    main()
