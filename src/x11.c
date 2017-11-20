#include <X11/Xlib.h>
#include <fakekey/fakekey.h>

void x11_type_char(const char *utf8_char) {
    Display *disp = XOpenDisplay(NULL);

    FakeKey *fk = fakekey_init(disp);
    fakekey_press(fk, (const unsigned char*)utf8_char, -1, 0);
    fakekey_release(fk);

    XSync(disp, False);
    XCloseDisplay(disp);
}
