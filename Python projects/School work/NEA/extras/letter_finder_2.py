import cv2
import os
import numpy as np

path_roi = r'images\roi'
path_final = r'images\final'

img=cv2.imread(r'images\readin\sd19 (1).jpg')
imgCont=img.copy()
imgFinal=img.copy()


imggray=cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
th, imgBin = cv2.threshold(imggray, 80, 255, cv2.THRESH_OTSU)
imgcanny=cv2.Canny(imgBin,200,240)
contours,_ = cv2.findContours(imgcanny, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

rect_d=[] #store the dimensions of my bounding boxes
min_cont_area = 20
max_cont_area = 500
for cnt in contours:
    #print(cv2.contourArea(cnt))
    if cv2.contourArea(cnt)>min_cont_area and cv2.contourArea(cnt) < max_cont_area: #Limit the contours based on area?  
        box_d=cv2.boundingRect(cnt)
        x,y,w,h=box_d
        rect_d.append([x,y,w,h]) 
        cv2.rectangle(imgCont, (x,y),(x+w,y+h), (0, 0, 255),1)

i=0
roi_n=[] #store each bounding box for later
for d in range(0,len(rect_d)):    
    x=rect_d[i][0] #Find x 
    y=rect_d[i][1] #Find y 
    w=rect_d[i][2] #Find width 
    h=rect_d[i][3] #Find height 
    roi=imgFinal[y:y+h,x:x+w]
    print(roi.shape)

    (tH, tW) = roi.shape[0],roi.shape[1]
    dX = int(max(0, 28 - tW) / 2.0)
    dY = int(max(0, 28 - tH) / 2.0)
    # pad the image and force 32x32 dimensions
    padded = cv2.copyMakeBorder(roi, top=dY, bottom=dY,
        left=dX, right=dX, borderType=cv2.BORDER_CONSTANT,
        value=(0, 0, 0))
    padded = cv2.resize(padded, (28, 28))



    roi_n.append(roi)
    cv2.imwrite(os.path.join(path_roi,'Boxed_ROIs_'+str(i) + '.jpg'), roi) 
    i+=1

cv2.imshow('Final Result',imgCont)
cv2.imwrite(os.path.join(path_final,'Final Result.jpg'),imgCont)

cv2.waitKey(0)
cv2.destroyAllWindows()