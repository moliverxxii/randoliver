/*
 * interface.c
 *
 *  Created on: 8 févr. 2024
 *      Author: moliver
 */

#include <stdio.h>
#include <stdint.h>
#include "interface.h"

static const char* const COLOUR_ESCAPE_COMMAND[COLOUR_ESCAPE_COUNT] =
{
    "0",  //RESET
    "1",  //BOLD
    "2",  //FAINT
    "3",  //ITALIC
    "4",  //UNDERLINE
    "5",  //SLOW_BLINK
    "6",  //RAPID_BLINK
    "7",  //REVERSE_VIDEO
    "8",  //HIDE
    "9",  //STRIKE
    "10", //PRIMARY_FONT
    "20", //GOTHIC
    "21", //UNDERLINE_DOUBLE
    "22", //NORMAL
    "23", //NITALIC_NBLACK
    "24", //NUNDERLINE
    "25", //NBLINK
    "27", //NREVERSE
    "28", //REVEAL
    "29", //NSTRIKE
    "30", //FOREGROUND_BLACK
    "31", //FOREGROUND_RED
    "32", //FOREGROUND_GREEN
    "33", //FOREGROUND_YELLOW
    "34", //FOREGROUND_BLUE
    "35", //FOREGROUND_MAGENTA
    "36", //FOREGROUND_CYAN
    "37", //FOREGROUND_WHITE
    "38", //FOREGROUND_COLOUR
    "39", //DEFAULT_FOREGROUND_COLOUR
    "40", //BACKGROUND_BLACK
    "41", //BACKGROUND_RED
    "42", //BACKGROUND_GREEN
    "43", //BACKGROUND_YELLOW
    "44", //BACKGROUND_BLUE
    "45", //BACKGROUND_MAGENTA
    "46", //BACKGROUND_CYAN
    "47", //BACKGROUND_WHITE
    "48", //BACKGROUND_COLOUR
    "49", //DEFAULT_BACKGROUND_COLOUR
    "55", //NOVERLINED
    "73", //SUPERSCRIPT
    "74", //SUBSCRIPT
    "75"  //NSUBSCRIPT_NSUPERSCRIPT
};

static const char* const CURSOR_ESCAPE_COMMAND[CURSOR_ESCAPE_COUNT] =
{
        "n A"    , //CURSOR_UP,
        "n B"    , //CURSOR_DOWN,
        "n C"    , //CURSOR_FORWARD,
        "n D"    , //CURSOR_BACK,
        "n E"    , //CURSOR_NEXT_LINE,
        "n F"    , //CURSOR_PREVIOUS_LINE,
        "n G"    , //CURSOR_HORIZONTAL_ABSOLUTE,
        "n ; m H", //CURSOR_POSITION,
        "n J"    , //ERASE_IN_DISPLAY,
        "n K"    , //ERASE_IN_LINE,
        "n S"    , //SCROLL_UP,
        "n T"    , //SCROLL_DOWN,
        "n ; m f", //HORIZONTAL_VERTICAL_POSITION,
        "n m"    , //SELECT_GRAPHIC_RENDITION,
        "5i"     , //AUX_PORT_ON,
        "4i"     , //AUX_PORT_OFF,
        "6n"     , //DEVICE_STATUS_REPORT,
        "s"      , //SAVE_CURRENT_CURSOR_POSITION,
        "u"        //RESTORE_SAVED_CURSOR_POSITION,
};

static const char* const ESCAPE_START = "\033[";
static const char* const ESCAPE_END   = "m";

static const char* get_escape_sequence(colour_escape_t colour);
static const char* get_cursor_escape(cursor_escape_t command);

void set_colour_escape(colour_escape_t colour);

void
init_interface()
{
    set_colour_escape(FOREGROUND_GREEN);
    set_colour_escape(BACKGROUND_MAGENTA);
    printf("COLOUR!!!\n");
    set_colour_escape(RESET);
}


static const char*
get_escape_sequence(colour_escape_t colour)
{
    return COLOUR_ESCAPE_COMMAND[colour];
}

void
set_colour_escape(colour_escape_t colour)
{
    printf("%s%s%s", ESCAPE_START, get_escape_sequence(colour), ESCAPE_END);
}
