#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#include <X11/keysym.h>

void x11_simulate_paste(void) {
    Display *disp = XOpenDisplay(NULL);

    KeyCode shift_code = XKeysymToKeycode(disp, XK_Shift_L);
    KeyCode insert_code = XKeysymToKeycode(disp, XK_Insert);

    XTestFakeKeyEvent(disp, shift_code, True, CurrentTime);
    XTestFakeKeyEvent(disp, insert_code, True, CurrentTime);
    XTestFakeKeyEvent(disp, insert_code, False, CurrentTime);
    XTestFakeKeyEvent(disp, shift_code, False, CurrentTime);

    XFlush(disp);
    XCloseDisplay(disp);
}
