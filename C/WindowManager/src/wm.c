#include "wm.h"
#include <X11/X.h>
#include <X11/Xlib.h>

struct Manager {
    Display *display;

    Window root;
};

/* -------------Function Declarations---------- */

/* Auxiliary functions */
int handle_error(struct _XDisplay *display, XErrorEvent *err);
int handle_WM_detected(struct _XDisplay *display, XErrorEvent *err);

/* Manager functions */
static int manager_init();
static int manager_kill();
static void manager_run();

/* Event functions
 * button press          -> processes any GUI button clicks on frame window          *
 * client message        ->
 * configure request     -> used to re-configure size, position border and stacking  *
 * create notify         -> create new root window child created, new process etc.   *
 * destroy notify        -> destroy root window child, exit process etc.             *
 * enter notify          -> cursor enters new window                                 *
 * expose                ->
 * focus in              -> focus on different window                                *
 * key press             -> process any keyboard input                               *
 * mapping notify        ->                                                          *
 * mapping request       -> make the window visible, map the window to the display   *
 * motion notify         ->
 * property notfiy       ->
 * reparent notify       -> re-renders the reconfigured frame and child              *
 * unmap notify          -> hide the window, un-map the window from the display      */
static void event_button_press(XButtonPressedEvent event);
static void event_client_message(XClientMessageEvent event);
static void event_configure_request(XConfigureRequestEvent event);
static void event_create_notify(XCreateWindowEvent event);
static void event_destroy_notify(XDestroyWindowEvent event);
static void event_enter_notify(XEnterWindowEvent event);
static void event_expose(XExposeEvent event);
static void event_focus_in(XFocusInEvent event);
static void event_key_press(XKeyPressedEvent event);
static void event_mapping_notify(XMappingEvent event);
static void event_map_request(XMapRequestEvent event);
static void event_motion_notify(XMotionEvent event);
static void event_property_notify(XPropertyEvent event);
static void event_reparent_notify(XReparentEvent event);
static void event_unmap_notify(XUnmapEvent event);


/* Variables */
static bool running = true;
static bool WM_detected = false;

static struct Manager manager;

void event_button_press(XButtonPressedEvent event) {
}
void event_client_message(XClientMessageEvent event) {
}
void event_configure_request(XConfigureRequestEvent event) {
    XWindowChanges c;

    c.x = event.x;
    c.y = event.y;
    c.width = event.width;
    c.height = event.height;
    c.sibling = event.above;
    c.stack_mode = event.detail;
    c.border_width = event.border_width;

    XConfigureWindow(manager.display, event.window, event.value_mask, &c);
}
void event_create_notify(XCreateWindowEvent event) {

}
void event_destroy_notify(XDestroyWindowEvent event) {
}
void event_enter_notify(XEnterWindowEvent event) {
}
void event_expose(XExposeEvent event) {
}
void event_focus_in(XFocusInEvent event) {
}
void event_key_press(XKeyPressedEvent event) {
}
void event_mapping_notify(XMappingEvent event) {
}
void event_map_request(XMapRequestEvent event) {

}
void event_motion_notify(XMotionEvent event) {
}
void event_property_notify(XPropertyEvent event) {
}
void event_reparent_notify(XReparentEvent event) {
}
void event_unmap_notify(XUnmapEvent event) {
}

int handle_WM_detected(struct _XDisplay *display, XErrorEvent *err) {
    fprintf(stderr, "[Error] wm.c: Another Window Manager is running\n");
    WM_detected = true;
    return 0;
}

int handle_XError(struct _XDisplay *display, XErrorEvent *err) {
    char exception = ' ';
    switch (err->request_code) {
        case X_SetInputFocus:      exception = BadMatch; break;
        case X_PolyText8:          exception = BadDrawable; break;
        case X_PolyFillRectangle:  exception = BadDrawable; break;
        case X_PolySegment:        exception = BadDrawable; break;
        case X_ConfigureWindow:    exception = BadMatch; break;
        case X_GrabButton:         exception = BadAccess; break;
        case X_GrabKey:            exception = BadAccess; break;
        case X_CopyArea:           exception = BadDrawable; break;
        default: break;
    }

    if (err->error_code == exception)
        return 0;

    fprintf(stderr, "[Error] wm.c: request code=%d, error code=%d\n", err->request_code, err->error_code);
    return 0;
}

int manager_init() {
    if ((manager.display = XOpenDisplay(NULL)) == NULL) {

        return -1;
    }

    /* Check for current running wm instance */
    XSetErrorHandler(&handle_WM_detected);
    XSelectInput(manager.display, manager.root, SubstructureRedirectMask | SubstructureNotifyMask);
    if (WM_detected) return -1;

    // Clear current XEventList and set to generic error handler
    XSync(manager.display, 0);
    XSetErrorHandler(&handle_XError);
    XSync(manager.display, 0);

    return 0;
}


int manager_create_notif(XCreateWindowEvent event) {


    return 0;
}

int manager_kill() {
    XCloseDisplay(manager.display);
    return 0;
}

void manager_run() {
    while (running) {
        XEvent current_event;

        XNextEvent(manager.display, &current_event);

        switch(current_event.type) {
            case      ButtonPress: event_button_press(current_event.xbutton); break;
            case    ClientMessage: event_client_message(current_event.xclient); break;
            case ConfigureRequest: event_configure_request(current_event.xconfigurerequest); break;
            case     CreateNotify: event_create_notify(current_event.xcreatewindow); break;
            case    DestroyNotify: event_destroy_notify(current_event.xdestroywindow); break;
            case      EnterNotify: event_enter_notify(current_event.xcrossing); break;
            case           Expose: event_expose(current_event.xexpose); break;
            case          FocusIn: event_focus_in(current_event.xfocus); break;
            case         KeyPress: event_key_press(current_event.xkey); break;
            case    MappingNotify: event_mapping_notify(current_event.xmapping); break;
            case       MapRequest: event_mapping_request(current_event.xmaprequest); break;
            case     MotionNotify: event_motion_notify(current_event.xmotion); break;
            case   PropertyNotify: event_property_notify(current_event.xproperty); break;
            case   ReparentNotify: event_reparent_notify(current_event.xreparent); break;
            case      UnmapNotify: event_unmap_notify(current_event.xunmap); break;
            default: break;
        }
    }
}

int main(int argc, char **argv) {
    manager_init();
    manager_run();
    manager_kill();
}
