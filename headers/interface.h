/*
 * interface.h
 *
 *  Created on: 8 févr. 2024
 *      Author: moliver
 */

#ifndef HEADERS_INTERFACE_H_
#define HEADERS_INTERFACE_H_

typedef enum
{
    RESET                     = 0, //Reset or normal All attributes become turned off
    BOLD                      , //Bold or increased intensity As with faint, the color change is a PC (SCO / CGA) invention.[25][better source needed]
    FAINT                     , //Faint, decreased intensity, or dim  May be implemented as a light font weight like bold.[26]
    ITALIC                    , //Italic  Not widely supported. Sometimes treated as inverse or blink.[25]
    UNDERLINE                 , //Underline   Style extensions exist for Kitty, VTE, mintty, iTerm2 and Konsole.[27][28][29]
    SLOW_BLINK                , //Slow blink  Sets blinking to less than 150 times per minute
    RAPID_BLINK               , //Rapid blink MS-DOS ANSI.SYS, 150+ per minute; not widely supported
    REVERSE_VIDEO             , //Reverse video or invert Swap foreground and background colors; inconsistent emulation[30][dubious – discuss]
    HIDE                      , //Conceal or hide Not widely supported.
    STRIKE                    , //Crossed-out, or strike  Characters legible but marked as if for deletion. Not supported in Terminal.app.
    PRIMARY_FONT              , //Primary (default) font
                                //11–19, Alternative font    Select alternative font n − 10
    GOTHIC                    , //Fraktur (Gothic)    Rarely supported
    UNDERLINE_DOUBLE          , //Doubly underlined; or: not bold Double-underline per ECMA-48,[5]: 8.3.117  but instead disables bold intensity on several terminals, including in the Linux kernel's console before version 4.17.[31]
    NORMAL                    , //Normal intensity    Neither bold nor faint; color changes where intensity is implemented as such.
    NITALIC_NBLACK            , //Neither italic, nor blackletter
    NUNDERLINE                , //Not underlined  Neither singly nor doubly underlined
    NBLINK                    , //Not blinking    Turn blinking off
                                //26,   Proportional spacing    ITU T.61 and T.416, not known to be used on terminals
    NREVERSE                  , //Not reversed
    REVEAL                    , //Reveal  Not concealed
    NSTRIKE                   , //Not crossed out
    FOREGROUND_BLACK          , //Set foreground color
    FOREGROUND_RED            ,
    FOREGROUND_GREEN          ,
    FOREGROUND_YELLOW         ,
    FOREGROUND_BLUE           ,
    FOREGROUND_MAGENTA        ,
    FOREGROUND_CYAN           ,
    FOREGROUND_WHITE          ,
    FOREGROUND_COLOUR         , //Set foreground color    Next arguments are 5;n or 2;r;g;b
    DEFAULT_FOREGROUND_COLOUR , //Default foreground color    Implementation defined (according to standard)
    BACKGROUND_BLACK          , //Set foreground color
    BACKGROUND_RED            ,
    BACKGROUND_GREEN          ,
    BACKGROUND_YELLOW         ,
    BACKGROUND_BLUE           ,
    BACKGROUND_MAGENTA        ,
    BACKGROUND_CYAN           ,
    BACKGROUND_WHITE          ,
    BACKGROUND_COLOUR         , //Set background color    Next arguments are 5;n or 2;r;g;b
    DEFAULT_BACKGROUND_COLOUR , //Default background color    Implementation defined (according to standard)
                                //50,   Disable proportional spacing    T.61 and T.416
                                //51,   Framed  Implemented as "emoji variation selector" in mintty.[32]
                                //52,   Encircled
                                //53,   Overlined   Not supported in Terminal.app
                                //54,   Neither framed nor encircled
    NOVERLINED                , //Not overlined
                                //58  Set underline color Not in standard; implemented in Kitty, VTE, mintty, and iTerm2.[27][28] Next arguments are 5;n or 2;r;g;b.
                                //59  Default underline color Not in standard; implemented in Kitty, VTE, mintty, and iTerm2.[27][28]
                                //60  Ideogram underline or right side line   Rarely supported
                                //61  Ideogram double underline, or double line on the right side
                                //62  Ideogram overline or left side line
                                //63  Ideogram double overline, or double line on the left side
                                //64  Ideogram stress marking
                                //65  No ideogram attributes  Reset the effects of all of 60–64
    SUPERSCRIPT               , //Superscript Implemented only in mintty[32]
    SUBSCRIPT                 , //Subscript
    NSUBSCRIPT_NSUPERSCRIPT   , //Neither superscript nor subscript
                                //90–97   Set bright foreground color Not in standard; originally implemented by aixterm[16]
                                //100–107 Set bright background color
    COLOUR_ESCAPE_COUNT
} colour_escape_t;

typedef enum
{
    CURSOR_UP = 0,                   //Moves the cursor n (default 1) cells in the given direction. If the cursor is already at the edge of the screen, this has no effect.
    CURSOR_DOWN,
    CURSOR_FORWARD,
    CURSOR_BACK,
    CURSOR_NEXT_LINE,                //Moves cursor to beginning of the line n (default 1) lines down. (not ANSI.SYS)
    CURSOR_PREVIOUS_LINE,            //Moves cursor to beginning of the line n (default 1) lines up. (not ANSI.SYS)
    CURSOR_HORIZONTAL_ABSOLUTE,      //Moves the cursor to column n (default 1). (not ANSI.SYS)
    CURSOR_POSITION,                 //Moves the cursor to row n, column m. The values are 1-based, and default to 1 (top left corner) if omitted. A sequence such as CSI ;5H is a synonym for CSI 1;5H as well as CSI 17;H is the same as CSI 17H and CSI 17;1H
    ERASE_IN_DISPLAY,                //Clears part of the screen. If n is 0 (or missing), clear from cursor to end of screen. If n is 1, clear from cursor to beginning of the screen. If n is 2, clear entire screen (and moves cursor to upper left on DOS ANSI.SYS). If n is 3, clear entire screen and delete all lines saved in the scrollback buffer (this feature was added for xterm and is supported by other terminal applications).
    ERASE_IN_LINE,                   //Erases part of the line. If n is 0 (or missing), clear from cursor to the end of the line. If n is 1, clear from cursor to beginning of the line. If n is 2, clear entire line. Cursor position does not change.
    SCROLL_UP,                       //Scroll whole page up by n (default 1) lines. New lines are added at the bottom. (not ANSI.SYS)
    SCROLL_DOWN,                     //Scroll whole page down by n (default 1) lines. New lines are added at the top. (not ANSI.SYS)
    HORIZONTAL_VERTICAL_POSITION,    //Same as CUP, but counts as a format effector function (like CR or LF) rather than an editor function (like CUD or CNL). This can lead to different handling in certain terminal modes.[5]: Annex A
    AUX_PORT_ON,                     //Enable aux serial port usually for local serial printer
    AUX_PORT_OFF,                    //Disable aux serial port usually for local serial printer
    DEVICE_STATUS_REPORT,            //Reports the cursor position (CPR) by transmitting ESC[n;mR, where n is the row and m is the column.
    SAVE_CURRENT_CURSOR_POSITION,    //Saves the cursor position/state in SCO console mode.[18] In vertical split screen mode, instead used to set (as CSI n ; n s) or reset left and right margins.[19]
    RESTORE_SAVED_CURSOR_POSITION,   //Restores the cursor position/state in SCO console mode.[20]
    CURSOR_ESCAPE_COUNT
} cursor_escape_t;


void init_interface();
void reset_line();

#endif /* HEADERS_INTERFACE_H_ */
