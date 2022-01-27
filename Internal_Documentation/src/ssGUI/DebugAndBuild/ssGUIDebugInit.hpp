#ifndef SSGUI_DEBUG_INIT
#define SSGUI_DEBUG_INIT

#include <string>
#include <stack>
int TabSpace = 0;
std::stack<std::string> FuncNameStack = std::stack<std::string>();
#endif