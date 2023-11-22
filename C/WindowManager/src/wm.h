#ifndef WM_H_INCLUDED
#define WM_H_INCLUDED

#include <X11/Xlib.h>
#include <stdlib.h>

#define MAX(a, b) ((a > b) ? (a): (b))

typedef struct WindowManager {
    Display *dpy;
    XButtonEvent pointer_state;

} WM_t;


#endif // WM_H_INCLUDED
