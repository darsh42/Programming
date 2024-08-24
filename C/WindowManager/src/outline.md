# Guide lines

Creating a window manager, these are regular Xclients

Description:

    - make sure no other winow manger is running
    - manage windows using 2 X mechanisms, properties and events

    Window Heirarchy:
      - Root window is invisible and takes up entire screen 
      - Each rectangle within each window is lower down the heirarchy

    - Window manager maneges top-level windows, that is direct children of the root window

    Substructure redirection:
      - intercepting top-level window signals, moving, resizing, showing and hiding
      - if a program registers for redirection, any modification of the window will be redirected to 
        the registered program. This however only affects direct children of the window.
      - Xserver only allows for 1 program to register for substructure redirection at any given level
        this is the reason there can only be 1 window manager
    
    Reparenting:
      - Framing, Window managers can create frames which contain each application
      - The frames usually have title bars, resize, minimize and exit options bound to them
      - The frame then becomes the child window of the root window
      - ISSUE: changes to the child of the parent window (the started application) will not be registered by
               the window manager, meaning the parent window also needs to register for substructure redirection
    
    Functionality:
        - Button presses (GUI buttons)
        - Creating new windows
        - Destroying windows
        - Entering a new window
        - Focus on a window
        - Key presses (Keyboard keys)
        - Reparenting windows
        

    
