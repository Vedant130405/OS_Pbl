#include "../include/print.h"

char *vgaBuff = (char *)0xb8000; // VGA text buffer
int vgaBuffPos = 0;               // Position in VGA buffer

void clear_screen(void)
{
    int screen_size = (VGA_WIDTH * 2) * VGA_HEIGHT;
    for (int i = 0; i < screen_size; i += 2) {
        vgaBuff[i] = ' ';      // blank character
        vgaBuff[i + 1] = 0x30; // background only (teal)
    }
    vgaBuffPos = 0; // Reset cursor
}

void handle_next_line(void) {
    // Move cursor to next line
    vgaBuffPos = ((vgaBuffPos / (VGA_WIDTH*2)) + 1) * (VGA_WIDTH*2);
}

void print_msg(char* msg)
{
    int i = 0;

    while (msg[i] != '\0')
    {
        if (msg[i] == '\n')
        {
            handle_next_line();
            i++;
            continue;
        }

        vgaBuff[vgaBuffPos] = msg[i];      // ASCII character
        vgaBuff[vgaBuffPos + 1] = 0x3E;    // Yellow text on teal background
        vgaBuffPos += 2;
        i++;
    }
}

