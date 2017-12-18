#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>
#include <glib.h>
#include "dbg.h"

static KeySym utf8_to_keysym(gunichar utf8_char);
static KeyCode keysym_to_keycode(Display *disp, KeySym ks);
static KeyCode get_unused_keycode(KeySym *kss, int kc_cnt, int ks_per_kc);
static bool is_keycode_unused(KeySym *kss, int ks_per_kc, int kc);
static int get_current_group(Display *disp);

void x11_type_char(const char *utf8_str) {
    Display *disp = XOpenDisplay(NULL);
    assert(disp);

    /*
        Workaround: we need to release Shift keys to get the "capitalize on
        shift" feature working. If we don't do that, we will get incorrect
        character instead of the desired (capitalized) one. It happens because
        with the Shift key pressed the keycode gets altered.
    */
    KeyCode shift_l = XKeysymToKeycode(disp, XK_Shift_L);
    KeyCode shift_r = XKeysymToKeycode(disp, XK_Shift_R);
    XTestFakeKeyEvent(disp, shift_l, False, CurrentTime);
    XTestFakeKeyEvent(disp, shift_r, False, CurrentTime);

    gunichar utf8_char = g_utf8_get_char(utf8_str);
    KeySym char_ks = utf8_to_keysym(utf8_char);

    KeyCode char_kc = keysym_to_keycode(disp, char_ks);
    if (!char_kc) {
        fprintf(stderr, "No free keycodes left, aboting\n");
        exit(1);
    }

    XTestFakeKeyEvent(disp, char_kc, True, CurrentTime);
    XTestFakeKeyEvent(disp, char_kc, False, CurrentTime);

    XSync(disp, False);
    XCloseDisplay(disp);
}



static KeySym utf8_to_keysym(gunichar utf8_char) {
    if (utf8_char > 0xFF) {
        return (utf8_char | 0x01000000);
    }

    return utf8_char;
}

static KeyCode keysym_to_keycode(Display *disp, KeySym ks) {
    int kc_min, kc_max;
    XDisplayKeycodes(disp, &kc_min, &kc_max);

    int kc_cnt = kc_max - kc_min + 1;
    int ks_per_kc;
    KeySym *kss = XGetKeyboardMapping(disp, kc_min, kc_cnt, &ks_per_kc);

    KeyCode kc = XKeysymToKeycode(disp, ks);
    int cur_grp = get_current_group(disp);
    bool is_keysym_primary = (XkbKeycodeToKeysym(disp, kc, cur_grp, 0) == ks);
    if (!kc || !is_keysym_primary) {
        kc = kc_min + get_unused_keycode(kss, kc_cnt, ks_per_kc);
        int index = (kc - kc_min) * ks_per_kc;
        kss[index] = ks;
        XChangeKeyboardMapping(disp, kc_min, ks_per_kc, kss, kc_cnt);
        XSync(disp, False);
    }

    XFree(kss);
    return kc;
}

static KeyCode get_unused_keycode(KeySym *kss, int kc_cnt, int ks_per_kc) {
    /*
        It's more likely that unused keycodes are near the end of the map, so
        we start from there to not waste our time.
    */
    for (int i = kc_cnt - 1; i >= 0; i--) {
        if (is_keycode_unused(kss, ks_per_kc, i)) {
            return i;
        }
    }

    return 0;
}

static bool is_keycode_unused(KeySym *kss, int ks_per_kc, int kc) {
    for (int i = 0; i < ks_per_kc; i++) {
        int index = kc * ks_per_kc + i;
        if (kss[index] != NoSymbol) {
            return false;
        }
    }

    return true;
}

static int get_current_group(Display *disp) {
    XkbStateRec state;
    memset(&state, 0, sizeof(state));
    XkbGetState(disp, XkbUseCoreKbd, &state);
    return state.group;
}
