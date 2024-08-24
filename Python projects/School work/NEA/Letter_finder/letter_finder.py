from msilib.schema import Directory
import cv2
import os
import shutil
import numpy as np

def character_extraction(img_path):
    parent_path, roi_directory, final_directory= r'D:\My folder\Programming\Python projects\School work\NEA\images', 'roi', 'final'
    path_roi = os.path.join(parent_path,roi_directory)
    path_final = os.path.join(parent_path, final_directory)

    #creating the Region Of Intrest and Final directory
    if not os.path.exists(path=path_roi):
        os.mkdir(path_roi)
    
    if not os.path.exists(path_final):
        os.mkdir(path_final)


    img=cv2.imread(img_path)
    imgCont=img.copy()
    imgFinal=img.copy()

    imggray=cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
    th, imgBin = cv2.threshold(imggray, 80, 255, cv2.THRESH_OTSU)
    imgcanny=cv2.Canny(imgBin,200,240)
    contours,_ = cv2.findContours(imgcanny, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    rect_d=[] #store the dimensions of my bounding boxes
    min_cont_height, max_cont_height = 3, 50
    min_cont_width, max_cont_width = 10, 200000
    for cnt in contours:
        #print(cv2.contourArea(cnt))
        box_d=cv2.boundingRect(cnt)
        x,y,w,h=box_d
        if min_cont_width <= w <= max_cont_width and min_cont_height <= h <= max_cont_height:
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
        #print(roi.shape)

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
    cv2.imwrite(os.path.join(path_final,'Final Result.jpg'),imgCont)

    return path_roi, path_final


if __name__ == '__main__':
    path_roi, path_final = character_extraction(r'images\readin\default_text_img.jpg')
    cv2.imshow('Final Result',path_final)    
    cv2.waitKey(0)
    cv2.destroyAllWindows()