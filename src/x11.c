#include <stdlib.h>
#include <assert.h>
#include <X11/Xlib.h>
#include <fakekey/fakekey.h>
#include "dbg.h"

void x11_type_char(const char *utf8_char) {
    Display *disp = XOpenDisplay(NULL);
    assert(disp);

    FakeKey *fk = fakekey_init(disp);
    assert(fk);

    /*
        Workaround: we need to release Shift keys to get the "capitalize on
        shift" feature working. If we don't do that, we will get incorrect
        character instead of the desired (capitalized) one. It happens because
        with the Shift key pressed the keycode gets altered.
    */
    KeyCode shift_l = XKeysymToKeycode(disp, XK_Shift_L);
    KeyCode shift_r = XKeysymToKeycode(disp, XK_Shift_R);
    fakekey_send_keyevent(fk, shift_l, False, 0);
    fakekey_send_keyevent(fk, shift_r, False, 0);

    fakekey_press(fk, (const unsigned char*)utf8_char, -1, 0);
    fakekey_release(fk);

    free(fk);
    XSync(disp, False);
    XCloseDisplay(disp);
}
