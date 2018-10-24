# About
Any character you like without having to memorize and perform Compose sequences

# Dependencies
1. `meson`
1. `gtk3`
1. `xlib`
1. `libxtst`

# Compiling & installing
1. `meson build`
1. `ninja -C build`
1. `ninja -C build install` (you may need root permissions)

# Configuring
`fastchar` reads configuration from `$XDG_CONFIG_HOME/fastchar/config` file.
The configuration is a UTF-8 string. On fastchar run, each character in that
string will appear as a menu entry.

# Usage
1. Add characters you like to the config file
1. Focus a text field where you want to type in a character
1. Execute `fastchar` and choose any character from the menu appeared. Holding
`Shift` while choosing will enable you to type in capitalized versions of letter
characters.
