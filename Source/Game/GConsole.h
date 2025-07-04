#pragma once

#include "GMain.h"

#define MAX_CONSOLE_HISTORY 128
#define MAX_PRINTED_LINE    512

#define CONSOLE_NONE 0
#define CONSOLE_CMD  1
#define CONSOLE_CHAT 2

extern bool openConsole;

extern byte ubConsoleType;

/* draw console */
void DrawConsole(void);
/* input text and execute command or just puts message on multiplyer chat */
void ProcessInputConsole(void);
/* prints console inputs history */
void PrintConsoleHistory(void);