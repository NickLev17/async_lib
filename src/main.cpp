#include "../include/Parser.h"
#include <iostream>
#include "../include/async.h"

using namespace std;

int main()
{

    Parser *parser_1 = connect(5);
    Parser *parser_2 = connect(5);
    Parser *parser_3 = connect(5);

    const char *commands = "cmd1{cmd3,cmd4}cmd5,cmd6";
    size_t len = std::strlen(commands);
    recieve(commands, len, parser_1);

    const char *commands5 = "cmd10000{cmd30000, }cmd5000,cmd6000";
    size_t len5 = std::strlen(commands5);
    recieve(commands5, len5, parser_2);
    recieve(commands, len, parser_3);
    const char *commands2 = "cmd44,cmd22,cmd33,cmd44,cmd55";
    size_t len2 = std::strlen(commands2);

    recieve(commands2, len2, parser_1);

    const char *commands3 = "cmd100,cmd200,cmd300, ,cmd500";
    size_t len3 = std::strlen(commands3);

    recieve(commands3, len3, parser_1);

    disconnect(parser_1);
    disconnect(parser_2);
    return 0;
}
