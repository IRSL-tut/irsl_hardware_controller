#ifndef __irsl_display_base_h__
#define __irsl_display_base_h__

#include <iostream>

// forground
#define DISP_COLOR_BLACK   "\033[30m"
#define DISP_COLOR_RED     "\033[31m"
#define DISP_COLOR_GREEN   "\033[32m"
#define DISP_COLOR_YELLOW  "\033[33m"
#define DISP_COLOR_BLUE    "\033[34m"
#define DISP_COLOR_MAGENTA "\033[35m"
#define DISP_COLOR_CYAN    "\033[36m"
#define DISP_COLOR_WHITE   "\033[37m"
// #define DISP_COLOR_BLACK      "\033[38m"
#define DISP_COLOR_DEFAULT "\033[39m"

#define DISP_BRIGHT_COLOR_BLACK   "\033[90m"
#define DISP_BRIGHT_COLOR_RED     "\033[91m"
#define DISP_BRIGHT_COLOR_GREEN   "\033[92m"
#define DISP_BRIGHT_COLOR_YELLOW  "\033[93m"
#define DISP_BRIGHT_COLOR_BLUE    "\033[94m"
#define DISP_BRIGHT_COLOR_MAGENTA "\033[95m"
#define DISP_BRIGHT_COLOR_CYAN    "\033[96m"
#define DISP_BRIGHT_COLOR_WHITE   "\033[97m"

// background
#define DISP_BG_COLOR_BLACK   "\033[40m"
#define DISP_BG_COLOR_RED     "\033[41m"
#define DISP_BG_COLOR_GREEN   "\033[42m"
#define DISP_BG_COLOR_YELLOW  "\033[43m"
#define DISP_BG_COLOR_BLUE    "\033[44m"
#define DISP_BG_COLOR_MAGENTA "\033[45m"
#define DISP_BG_COLOR_CYAN    "\033[46m"
#define DISP_BG_COLOR_WHITE   "\033[47m"
// #define DISP_BG_COLOR_BLACK      "\033[38m"
#define DISP_BG_COLOR_DEFAULT "\033[49m"

#define DISP_BG_BRIGHT_COLOR_BLACK   "\033[100m"
#define DISP_BG_BRIGHT_COLOR_RED     "\033[101m"
#define DISP_BG_BRIGHT_COLOR_GREEN   "\033[102m"
#define DISP_BG_BRIGHT_COLOR_YELLOW  "\033[103m"
#define DISP_BG_BRIGHT_COLOR_BLUE    "\033[104m"
#define DISP_BG_BRIGHT_COLOR_MAGENTA "\033[105m"
#define DISP_BG_BRIGHT_COLOR_CYAN    "\033[106m"
#define DISP_BG_BRIGHT_COLOR_WHITE   "\033[107m"

#nf 0
//cursor up(int i)
//"\x1b[%dA", i

//cursor_down(int i)
//"\x1b[%dB", i

//cursor_right(int i)
//"\x1b[%dC", i

//cursor_left(int i)
// "\x1b[%dD", i

//locate_cursor to (int i, int j)
//"\x1b[%d;%dH", i, j

//clear_screen
//"\x1b[2J"
//"\x1b[1;1H"

// https://stackoverflow.com/questions/4842424/list-of-ansi-color-escape-sequences
#endif

namespace irsl_common_utils {

class DisplayBase
{

public:
    // printf for display
    virtual void printf( char* format, ...) = 0;
    virtual void printf( const char* format, ...) = 0;
    // perror for display
    virtual void perror( const char* msg ) = 0;
    // add list_display
    virtual void output( unsigned int index, char* format, ...) = 0;
    // add list_display (stream version)
    virtual std::ostream& output_out( unsigned int index ) = 0;
    // add list_display
    virtual unsigned int addOutput() = 0;
    // force drawing
    virtual void flush() = 0;
};

}
#endif
