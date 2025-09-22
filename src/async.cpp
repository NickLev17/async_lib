#include "../include/async.h"
#include "../include/Parser.h"

extern "C"
{

    Parser *connect(size_t sizeBlock)
    {
        Parser *pars = new Parser(sizeBlock);

        return pars;
    }

    void recieve(const char *data, size_t size, Parser *parser)
    {

        parser->parsing(data, size);
    }

    void disconnect(Parser *p)
    {

        delete p;
        }
}