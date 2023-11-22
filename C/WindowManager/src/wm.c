#include "wm.h"
#include <X11/Xlib.h>

WM_t *wm;
XWindowAttributes current_window_attrs;
XEvent current_event;

int main(void) {
    wm = malloc(sizeof(WM_t));

    if (!(wm->dpy = XOpenDisplay(0x0))) return 1;

    XGrabKey(wm->dpy, XKeysymToKeycode(wm->dpy, XStringToKeysym("F1")), Mod1Mask,
            DefaultRootWindow(wm->dpy), True, GrabModeAsync, GrabModeAsync);
    XGrabButton(wm->dpy, 1, Mod1Mask, DefaultRootWindow(wm->dpy), True,
                ButtonPressMask | ButtonReleaseMask | PointerMotionMask,
                GrabModeAsync, GrabModeAsync, None, None);
    XGrabButton(wm->dpy, 3, Mod1Mask, DefaultRootWindow(wm->dpy), True,
                ButtonPressMask | ButtonReleaseMask | PointerMotionMask,
                GrabModeAsync, GrabModeAsync, None, None);

    wm->pointer_state.subwindow = None;
    for(;;) {
        XNextEvent(wm->dpy, &current_event);

        if (current_event.type == KeyPress && current_event.xkey.subwindow != None)
            XRaiseWindow(wm->dpy, current_event.xkey.subwindow);
        else if (current_event.type == ButtonPress && current_event.xkey.subwindow != None) {
            XGetWindowAttributes(wm->dpy, current_event.xkey.subwindow, &current_window_attrs);
            wm->pointer_state = current_event.xbutton;
        }
        else if (current_event.type == MotionNotify && wm->pointer_state.subwindow != None) {
            int xdiff = current_event.xbutton.x_root - wm->pointer_state.x_root;
            int ydiff = current_event.xbutton.y_root - wm->pointer_state.y_root;

            XMoveResizeWindow(
                wm->dpy, current_event.xbutton.subwindow,
                current_window_attrs.x + (wm->pointer_state.button == 1 ? xdiff : 0),
                current_window_attrs.y + (wm->pointer_state.button == 1 ? ydiff : 0),
                MAX(1, current_window_attrs.width + (wm->pointer_state.button == 3 ? xdiff : 0)),
                MAX(1, current_window_attrs.height + (wm->pointer_state.button == 3 ? ydiff : 0)));
        }
        else if(current_event.type == ButtonRelease) {
            wm->pointer_state.subwindow = None;
        }
    }

    free(wm);
    return 0;
}
