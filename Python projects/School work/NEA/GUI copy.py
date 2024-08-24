from msilib.schema import Directory
import os
import shutil
from tkinter import *
from tkinter import filedialog
from PIL import ImageTk, Image
from functools import partial

import numpy as np

from Letter_finder import letter_finder_copy
import model.model as model

class Main:
    def __init__(self) -> None:
        self.resultPath = r'result'

        self.window = Tk()
        self.window.title('Window')
        self.Frame = Frame(self.window, relief=RIDGE)
        self.Frame.grid(row=0,column=0)
        self.buttons()

    def buttons(self):
        width, height = 30, 1
        buttonFrame = Frame(self.Frame, relief=RIDGE)

        def fileSelect():
            # Finding file
            self.window.filename = filedialog.askopenfilename(initialdir='images')
            self.filename = self.window.filename
            #Opening the file
            openFile = partial(self.fileViewer, self.window.filename, 0, 1)
            fileViewButton = Button(buttonFrame, text=os.path.basename(self.window.filename), width=width, height=height, anchor=CENTER, command=openFile)
            fileViewButton.grid(row=1,column=0)
        
        addFile = Button(buttonFrame, text='Add File', width=width, height=height, anchor=CENTER, command=fileSelect)
        charExtr = Button(buttonFrame, text='OCR', width=width, height=height, anchor=CENTER, command=self.characterExtraction)
        quit = Button(buttonFrame, text='Quit', width=width, height=height, anchor=CENTER, command=self.quit)

        addFile.grid(row=0,column=0)
        charExtr.grid(row=2,column=0)
        quit.grid(row=4,column=0)
        buttonFrame.grid(row=0,column=0, rowspan=1)

    # Show the image
    def fileViewer(self, path, row, column):
        # Frame 
        frameImg = Frame(self.Frame, relief=RIDGE)
        frameImg.grid(row=0, column=1,rowspan=10)

        # Open image
        image = Image.open(path)
        image = ImageTk.PhotoImage(image)
        fileView = Label(frameImg, image=image, anchor=CENTER)
        fileView.image = image
        fileView.grid(row=row, column=column, rowspan=10)

    def characterExtraction(self):
        self.finalPath, self.roi_n = letter_finder_copy.character_extraction(self.filename)
        self.fileViewer(os.path.join(self.finalPath,'Final Result.jpg'), 0, 2)
        #print(self.roiPath)
        self.modelApplication()

    def modelApplication(self):

        file = open(os.path.join(self.resultPath,'text.txt'))
        for roi in self.roi_n:
            print(np.shape(roi))
            np.reshape(roi, (1, 28, 28, 1))
            text = model.applyModel(roi)
            file.write(text)
        file.close()

    def quit(self):
        try:
            shutil.rmtree(self.path_roi)
            shutil.rmtree(self.path_final)
            self.window.destroy()

        except AttributeError:
            self.window.destroy()
            
if __name__ == '__main__':
    program = Main()
    program.window.mainloop()