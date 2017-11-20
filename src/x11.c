#include <assert.h>
#include <X11/Xlib.h>
#include <fakekey/fakekey.h>
#include "dbg.h"

void x11_type_char(const char *utf8_char) {
    Display *disp = XOpenDisplay(NULL);
    assert(disp);

    FakeKey *fk = fakekey_init(disp);
    assert(fk);

    fakekey_press(fk, (const unsigned char*)utf8_char, -1, 0);
    fakekey_release(fk);

    XSync(disp, False);
    XCloseDisplay(disp);
}
