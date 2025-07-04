#include "stdAfx.h"
#include "GConsole.h"


static std::vector<std::string> strConsoleHistory;
static int index = 0;
static std::string strPrintedLine;

static int iConsoleHistoryPos = 0;
static int iConsoleEditPos = 5 + systemEngineFont->GetSpace();
static int iConsoleLetterPos = 0;

bool openConsole = true;

double dTimeBlinkEnd = 0;
double dTimeBlinkStart = 0;

void DrawConsole(void)
{
    if (!openConsole) return;

    // get width of screen
    uint uiconsoleWidth = pViewPort->GetWidth();
    // get 3/4 view of height screen
    uint uiconsoleHeight = pViewPort->GetHeight() * 0.75;

    // draw background color console
    drawSquare(0, 0, uiconsoleWidth, uiconsoleHeight, COLOR_ADD_ALPHA(0x4F0000FFUL, 200UL));
    // draw input background color console
    drawSquare(0, uiconsoleHeight + 10, uiconsoleWidth, 10, COLOR_ADD_ALPHA(0xAA0000FFUL, 230UL));

    std::vector<std::string> strConsoleLines;

    // get lines from console
    getConsoleBufferLines(pConsoleMain, strConsoleLines);

    unsigned int numLines = strConsoleLines.size();
    int yPosLine = uiconsoleHeight - 14;

    while (1)
    {
        if (yPosLine <= 0) break;
        if (numLines == 0) break;

        drawText(systemEngineFont, 10, yPosLine, 1.0f, 0xFFFF55FFUL, strConsoleLines[numLines - 1].c_str());
        yPosLine -= systemEngineFont->charHeight;
        numLines--;
    }

    yPosLine = uiconsoleHeight + (systemEngineFont->charWidth >> 1);

    drawText(systemEngineFont, 10, yPosLine, 1.0f, COLOR_WHITE, ": ");
    drawText(systemEngineFont, 10 + systemEngineFont->GetSpace(), yPosLine, 1.0f, COLOR_WHITE, strPrintedLine.c_str());


    if (((SDL_GetTicks64() * 2) / 1000) & 1)
        drawText(systemEngineFont, iConsoleEditPos, yPosLine, 1.0f, COLOR_WHITE, "|");

    strConsoleLines.clear();
}

void AppendConsoleHistory(void)
{
    if (strPrintedLine == "") return;

    // if we have situation where count of string bigger than MAX_CONSOLE_HISTORY
    if (strConsoleHistory.size() == MAX_CONSOLE_HISTORY) 
    {     
        // just erase first element
        FOREACH_ARRAY(MAX_CONSOLE_HISTORY)
        {
            if (i == MAX_CONSOLE_HISTORY - 1) {
                strConsoleHistory[i] = strPrintedLine;
                break;
            }

            strConsoleHistory[i] = strConsoleHistory[i + 1];
        }
    }
    else 
    {
        strConsoleHistory.push_back(strPrintedLine);
    }

    iConsoleHistoryPos = strConsoleHistory.size();
}

bool ParseCMD(const char* strCMD) 
{
    if (strCMD[0] != '/')
        return false;

    if (strlen(strCMD) > 256) return false;

    char strBuffC[256];

    strcpy(strBuffC, strCMD);

    char* p = strtok(strBuffC, " ");

    if (p) 
    { 
        if (strcmp(p, "connect") == 0)
            return false;

        p = strtok(NULL, " ");

        if (!p) return false;

        free(strAddressString);

        strAddressString = stringCopy(p);

       // memset(strAddressString, 0, sizeof(strAddressString));

        //strcpy(strAddressString, p);

        PrintF("%s\n", strAddressString);

        pConsoleDataBase->executeFunc("joinGame()");
    } 
    return true;

}

void ProcessInputConsole(void)
{
    if (!openConsole) return;  

    SDL_StartTextInput();

    SDL_Event event;

    int length = strConsoleHistory.size();
   
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_KEYDOWN)
        {
            // close console
            if (event.key.keysym.sym == SDLK_BACKQUOTE) {
                openConsole = false;
                break;
            }
            // accept command or puts into the network chat
            if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER) 
            {     
                AppendConsoleHistory();

                if (!ParseCMD(strPrintedLine.c_str()))
                    pConsoleDataBase->execute(strPrintedLine.c_str());

                strPrintedLine.clear();
                strPrintedLine = "";
                iConsoleLetterPos = 0;
                iConsoleEditPos = 5 + systemEngineFont->GetSpace();
            }
            // move to last printed line in console history
            if (event.key.keysym.sym == SDLK_UP)
            {
                if (strConsoleHistory.empty())
                    continue;

                iConsoleHistoryPos--;                
                iConsoleHistoryPos = Clamp(iConsoleHistoryPos, 0, MAX_CONSOLE_HISTORY - 1);
                strPrintedLine = strConsoleHistory[iConsoleHistoryPos];
                iConsoleLetterPos = strPrintedLine.size();

                iConsoleEditPos = (systemEngineFont->GetSpace() * (strPrintedLine.size() + 1)) + 5;
            }
            // move to first printed line in console history
            if (event.key.keysym.sym == SDLK_DOWN)
            {
                if (strConsoleHistory.empty())
                    continue;

                iConsoleHistoryPos++;
                iConsoleHistoryPos = Clamp(iConsoleHistoryPos, 0, int(strConsoleHistory.size() - 1));
                strPrintedLine = strConsoleHistory[iConsoleHistoryPos];
                iConsoleLetterPos = strPrintedLine.size();

                iConsoleEditPos = (systemEngineFont->GetSpace() * (strPrintedLine.size() + 1)) + 5;
            }
            // move edit cursor to left
            if (event.key.keysym.sym == SDLK_LEFT)
            {
                if (strPrintedLine.empty()) continue;
                iConsoleLetterPos--;
                iConsoleLetterPos = Clamp(iConsoleLetterPos, 0, int(strPrintedLine.size()));

                iConsoleEditPos -= systemEngineFont->GetSpace();
                iConsoleEditPos = Clamp(iConsoleEditPos, int(5 + systemEngineFont->GetSpace()), int((strPrintedLine.length() + 1) * systemEngineFont->GetSpace() + 5));
                //strPrintedLine = strConsoleHistory[iConsoleHistoryPos];
            }
            // move edit cursor to right
            if (event.key.keysym.sym == SDLK_RIGHT) 
            {
                if (strPrintedLine.empty()) continue;
                iConsoleLetterPos++;
                iConsoleLetterPos = Clamp(iConsoleLetterPos, 0, int(strPrintedLine.size()));

                iConsoleEditPos += systemEngineFont->GetSpace();
                iConsoleEditPos = Clamp(iConsoleEditPos, int(5 + systemEngineFont->GetSpace()), int((strPrintedLine.length() + 1) * systemEngineFont->GetSpace() + 5));
            }

            // delete symbol from left
            if (event.key.keysym.sym == SDLK_BACKSPACE) 
            {
                if (strPrintedLine.empty()) continue;

                strPrintedLine.erase(strPrintedLine.begin() + iConsoleLetterPos - 1);
                iConsoleLetterPos--;
                iConsoleLetterPos = Clamp(iConsoleLetterPos, 0, int(strPrintedLine.size()));

                iConsoleEditPos -= systemEngineFont->GetSpace();
                iConsoleEditPos = Clamp(iConsoleEditPos, int(5 + systemEngineFont->GetSpace()), int((strPrintedLine.length() + 1) * systemEngineFont->GetSpace() + 5));
            }

            // delete symbol from right
            if (event.key.keysym.sym == SDLK_DELETE)
            {
                if (strPrintedLine.empty()) continue;

                strPrintedLine.erase(strPrintedLine.begin() + iConsoleLetterPos);

                //iConsoleLetterPos++;
                iConsoleLetterPos = Clamp(iConsoleLetterPos, 0, int(strPrintedLine.size()));

                //iConsoleEditPos += systemEngineFont->GetSpace();
                iConsoleEditPos = Clamp(iConsoleEditPos, int(5 + systemEngineFont->GetSpace()), int((strPrintedLine.length() + 1) * systemEngineFont->GetSpace() + 5));
            }

            // delete symbol from right
            if (event.key.keysym.sym == SDLK_TAB)
            {

                strPrintedLine = "PrintAllSymbols()";

                //iConsoleLetterPos++;
                iConsoleLetterPos = strPrintedLine.size();

                //iConsoleEditPos += systemEngineFont->GetSpace();
                iConsoleEditPos = (systemEngineFont->GetSpace() * (strPrintedLine.size() + 1)) + 5;
            }

        }
        /* text input */
        if (event.type == SDL_TEXTINPUT)
        {
            strPrintedLine.insert(iConsoleLetterPos, event.text.text);
            iConsoleLetterPos++;

            iConsoleEditPos += systemEngineFont->GetSpace();
        }
    }

    SDL_StopTextInput();
}


//void EnableConsole()
//{
//    ProcessInputConsole();

//    DrawConsole();
//}


void PrintConsoleHistory(void) 
{
	PrintF("Console input history ----------------\n");

	unsigned iLength = strConsoleHistory.size();

	FOREACH_ARRAY(iLength)
        PrintF("> %s;\n", strConsoleHistory[i].c_str());

    PrintF("---------------------------------------------------------------\n");
}

void consoleExecute()
{
	strPrintedLine.clear();

	

}