#include "keyboard_map.h"

#define LINES 25
#define COLUMNS_IN_LINE 80
#define BYTES_FOR_EACH_ELEMENT 2
#define SCREENSIZE BYTES_FOR_EACH_ELEMENT * COLUMNS_IN_LINE * LINES
#define VIDEO_START 0xb8000

#define REG_SCREEN_CTRL 0x3D4
#define REG_SCREEN_DATA 0x3D5

/* Maintain a global location for the current video memory to write to */
static int current_loc = 0;
/* Video memory starts at 0xb8000. Make it a constant pointer to
   characters as this can improve compiler optimization since it
   is a hint that the value of the pointer won't change */
static char *const vidptr = (char *) 0xb8000;

void set_cursor(int offset) {
    offset /= 2; // Covert from cell offset to char offset.

    // This is similar to get_cursor, only now we write
    // bytes to those internal device registers.
    write_port(REG_SCREEN_CTRL, 14);
    write_port(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
    write_port(REG_SCREEN_CTRL, 15);
    write_port(REG_SCREEN_DATA, offset);
}

int get_cursor_pos() {
    return current_loc;
}

void kb_init(void) {
    /* This is a very basic keyboard initialization. The assumption is we have a
     * PS/2 keyboard and it is already in a propr state. This may not be the case
     * on real hardware. We simply enable the keyboard interupt */

    /* Get current master PIC interrupt mask */
    unsigned char curmask_master = read_port(0x21);

    /* 0xFD is 11111101 - enables only IRQ1 (keyboard) on master pic
       by clearing bit 1. bit is clear for enabled and bit is set for disabled */
    write_port(0x21, curmask_master & 0xFD);
}

void clear_screen(void) {
    unsigned int i = 0;
    while (i < SCREENSIZE) {
        vidptr[i++] = ' ';
        vidptr[i++] = 0x07;
    }
}

void print_newline(void) {
    unsigned int line_size = BYTES_FOR_EACH_ELEMENT * COLUMNS_IN_LINE;
    current_loc = current_loc + (line_size - current_loc % (line_size));
}


void print(const char *str) {
    unsigned int i = 0;
    while (str[i] != '\0') {
        if (str[i] == '\n') {
            print_newline();
            i++;
        } else {
            vidptr[current_loc++] = str[i++];
            vidptr[current_loc++] = 0x07;
        }
    }
}

void keyboard_handler(void) {
    const char *echo = "echo> ";
    signed char keycode;

    keycode = read_port(0x60);
    /* Only print characters on keydown event that have
     * a non-zero mapping */
    if (keycode >= 0 && keyboard_map[keycode]) {
        switch (keyboard_map[keycode]) {
            case '\n':
                print_newline();
                print(echo);
                break;
            case '\b':
                if (current_loc % COLUMNS_IN_LINE > 6 * 2) {
                    vidptr[--current_loc] = 0x00;
                    vidptr[--current_loc] = 0x00;
                }
                break;
            default:
                vidptr[current_loc++] = keyboard_map[keycode];
                /* Attribute 0x02 is green on black characters */
                vidptr[current_loc++] = 0x02;
        }
    }

    set_cursor(current_loc);

    /* Send End of Interrupt (EOI) to master PIC */
    write_port(0x20, 0x20);
}