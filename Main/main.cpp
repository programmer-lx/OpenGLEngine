#include "AppMain.h"

#include <vector>
#include <string>

int main(int argc, char* argv[])
{
    // parse cmdLine
    std::vector<std::string> cmdLine(argc - 1); // ignore argv[0]

    for (int i = 1; i < argc; ++i)
    {
        cmdLine[i - 1] = std::string(argv[i]);
    }

    return AppMain::run(cmdLine);
}
