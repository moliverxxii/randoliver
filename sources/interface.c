/*
 * interface.c
 *
 *  Created on: 8 févr. 2024
 *      Author: moliver
 */

#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include "interface.h"

static const char* const COLOUR_ESCAPE_COMMAND[COLOUR_ESCAPE_COUNT] =
{
    "0",   //RESET
    "1",   //BOLD
    "2",   //FAINT
    "3",   //ITALIC
    "4",   //UNDERLINE
    "5",   //SLOW_BLINK
    "6",   //RAPID_BLINK
    "7",   //REVERSE_VIDEO
    "8",   //HIDE
    "9",   //STRIKE
    "10",  //PRIMARY_FONT
    "20",  //GOTHIC
    "21",  //UNDERLINE_DOUBLE
    "22",  //NORMAL
    "23",  //NITALIC_NBLACK
    "24",  //NUNDERLINE
    "25",  //NBLINK
    "27",  //NREVERSE
    "28",  //REVEAL
    "29",  //NSTRIKE
    "30",  //FOREGROUND_BLACK
    "31",  //FOREGROUND_RED
    "32",  //FOREGROUND_GREEN
    "33",  //FOREGROUND_YELLOW
    "34",  //FOREGROUND_BLUE
    "35",  //FOREGROUND_MAGENTA
    "36",  //FOREGROUND_CYAN
    "37",  //FOREGROUND_WHITE
    "38",  //FOREGROUND_COLOUR
    "39",  //DEFAULT_FOREGROUND_COLOUR
    "40",  //BACKGROUND_BLACK
    "41",  //BACKGROUND_RED
    "42",  //BACKGROUND_GREEN
    "43",  //BACKGROUND_YELLOW
    "44",  //BACKGROUND_BLUE
    "45",  //BACKGROUND_MAGENTA
    "46",  //BACKGROUND_CYAN
    "47",  //BACKGROUND_WHITE
    "48",  //BACKGROUND_COLOUR
    "49",  //DEFAULT_BACKGROUND_COLOUR
    "55",  //NOVERLINED
    "73",  //SUPERSCRIPT
    "74",  //SUBSCRIPT
    "75",  //NSUBSCRIPT_NSUPERSCRIPT
    "90",  //FOREGROUND_BRIGHT_BLACK
    "91",  //FOREGROUND_BRIGHT_RED
    "92",  //FOREGROUND_BRIGHT_GREEN
    "93",  //FOREGROUND_BRIGHT_YELLOW
    "94",  //FOREGROUND_BRIGHT_BLUE
    "95",  //FOREGROUND_BRIGHT_MAGENTA
    "96",  //FOREGROUND_BRIGHT_CYAN
    "97",  //FOREGROUND_BRIGHT_WHITE
    "100", //BACKGROUND_BRIGHT_BLACK
    "101", //BACKGROUND_BRIGHT_RED
    "102", //BACKGROUND_BRIGHT_GREEN
    "103", //BACKGROUND_BRIGHT_YELLOW
    "104", //BACKGROUND_BRIGHT_BLUE
    "105", //BACKGROUND_BRIGHT_MAGENTA
    "106", //BACKGROUND_BRIGHT_CYAN
    "107"  //BACKGROUND_BRIGHT_WHITE
};

static const char* const CURSOR_ESCAPE_COMMAND[CURSOR_ESCAPE_COUNT] =
{
        "%dA"   , //CURSOR_UP,
        "%dB"   , //CURSOR_DOWN,
        "%dC"   , //CURSOR_FORWARD,
        "%dD"   , //CURSOR_BACK,
        "%dE"   , //CURSOR_NEXT_LINE,
        "%dF"   , //CURSOR_PREVIOUS_LINE,
        "%dG"   , //CURSOR_HORIZONTAL_ABSOLUTE,
        "%d;%dH", //CURSOR_POSITION,
        "%dJ"   , //ERASE_IN_DISPLAY,
        "%dK"   , //ERASE_IN_LINE,
        "%dS"   , //SCROLL_UP,
        "%dT"   , //SCROLL_DOWN,
        "%d;%df", //HORIZONTAL_VERTICAL_POSITION,
        "5i"    , //AUX_PORT_ON,
        "4i"    , //AUX_PORT_OFF,
        "6n"    , //DEVICE_STATUS_REPORT,
        "s"     , //SAVE_CURRENT_CURSOR_POSITION,
        "u"       //RESTORE_SAVED_CURSOR_POSITION,
};

static const char* const COLOUR_PARAMETER_LIST[COLOUR_PARMETER_COUNT] =
{
    "",            //COLOUR_PARAMETER_NONE
    ";5;%d",       //COLOUR_PARAMETER_1B
    ";2;%d;%d;%d"  //COLOUR_PARAMETER_3B
};

static const char* const ESCAPE_START = "\033[";
static const char* const ESCAPE_END   = "m";

static const char* get_escape_sequence(colour_escape_t colour);
static const char* get_cursor_escape(cursor_escape_t command);

void
init_interface()
{
    set_colour_escape(FOREGROUND_BRIGHT_WHITE);
    printf("COLOUR!!!\n");
    set_colour_escape(BOLD);
    printf("COLOUR!!!\n");
    set_colour_escape(RESET);
    set_colour_escape(DEFAULT_BACKGROUND_COLOUR);
}

void reset_line()
{
    set_cursor_escape(CURSOR_UP, 1);
    set_cursor_escape(ERASE_IN_LINE, 0);
}



static const char*
get_escape_sequence(colour_escape_t colour)
{
    return COLOUR_ESCAPE_COMMAND[colour];
}

static const char*
get_colour_parameter(colour_parameter_count_t count)
{
    return COLOUR_PARAMETER_LIST[count];
}

static const char*
get_cursor_escape(cursor_escape_t command)
{
    return CURSOR_ESCAPE_COMMAND[command];
}


void
set_colour_escape(colour_escape_t colour,
                  ...)
{
    va_list args;
    va_start(args, colour);

    colour_parameter_count_t parameter_count;
    switch(colour)
    {
    case FOREGROUND_COLOUR: //Set colour    Next arguments are 5;n or 2;r;g;b
    case BACKGROUND_COLOUR:
        parameter_count = va_arg(args, colour_parameter_count_t);
        break;
    default:
        parameter_count = COLOUR_PARAMETER_NONE;
    }


    printf("%s%s", ESCAPE_START, get_escape_sequence(colour));
    int colour_1b;
    int red;
    int green;
    int blue;
    switch(parameter_count)
    {
    case COLOUR_PARAMETER_NONE:
    default:
        printf("%s", get_colour_parameter(parameter_count));
        break;
    case COLOUR_PARAMETER_1B:
        colour_1b = va_arg(args, int);
        printf(get_colour_parameter(parameter_count), colour_1b);
        break;
    case COLOUR_PARAMETER_3B:
        red   = va_arg(args, int);
        green = va_arg(args, int);
        blue  = va_arg(args, int);
        printf(get_colour_parameter(parameter_count), red, green, blue);
        break;
    }
    va_end(args);
    printf(ESCAPE_END);
}

void
set_cursor_escape(cursor_escape_t command, ...)
{
    va_list args;
    va_start(args, command);
    int n;
    int m;
    printf(ESCAPE_START);
    switch(command)
    {
    case CURSOR_UP:
    case CURSOR_DOWN:
    case CURSOR_FORWARD:
    case CURSOR_BACK:
    case CURSOR_NEXT_LINE:
    case CURSOR_PREVIOUS_LINE:
    case CURSOR_HORIZONTAL_ABSOLUTE:
    case ERASE_IN_DISPLAY:
    case ERASE_IN_LINE:
    case SCROLL_UP:
    case SCROLL_DOWN:
        n = va_arg(args, int);
        printf(get_cursor_escape(command), n);
        break;
    case CURSOR_POSITION:
    case HORIZONTAL_VERTICAL_POSITION:
        n = va_arg(args, int);
        m = va_arg(args, int);
        printf(get_cursor_escape(command), n, m);
        break;
    case AUX_PORT_ON:
    case AUX_PORT_OFF:
    case DEVICE_STATUS_REPORT:
    case SAVE_CURRENT_CURSOR_POSITION:
    case RESTORE_SAVED_CURSOR_POSITION:
    default:
        printf("%s", get_cursor_escape(command));
        break;
    }
    va_end(args);


}
