# Python program to find the file to letter extract

#imports
from re import T
import tkinter as tk 
import math

from tkinter import CENTER, ttk
from tkinter import filedialog as fd
from tkinter.messagebox import showinfo
from turtle import bgcolor, color


# Open files functions
def select_files():
    filetypes = (
        ('image files (jpg)', '*.jpg'),
        ('image files (png)', '*.png'),
        ('All files', '*.*')
    )

    filenames = fd.askopenfilenames(
        title='Open files',
        initialdir='/',
        filetypes=filetypes)

    showinfo(
        title='Selected Files',
        message=filenames
    )


# Create the window
root = tk.Tk()
root.title('Alfa-numeric recognition')
root.geometry('800x600')

def files_to_ocr():
    class newFile:
        def __init__():

    identifier = ttk.Label(root, text='Files to OCR', width=45, anchor='center',background='gray')
    identifier.grid(row=0,column=0)

    addFile = ttk.Button(root, text='Add File', command=)

files_to_ocr()
button_quit = ttk.Button(root, text='Exit', command=root.quit)
button_quit.grid(row=20,column=20)
root.mainloop()