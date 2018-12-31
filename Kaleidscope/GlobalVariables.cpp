#include "GlobalVariables.h"
#include <string>
#include <map>

std::string g_identifierStr = "";
double g_numVal = 0;
int g_cursorToken = -1;
std::map<char, int> g_binaryOperatorPrecedences = {};
