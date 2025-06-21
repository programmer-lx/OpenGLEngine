#include "AppMain.h"

#include "Misc/StringUtil.h"

#include <vector>
#include <string>

#include <Windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow)
{
    // parse cmdLine
    std::string str(pCmdLine);

    std::vector<std::string> cmdLine = StringUtil::split(str, {' '});

    return AppMain::run(cmdLine);
}