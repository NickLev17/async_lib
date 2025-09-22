#ifndef ASYNC_H
#define ASYNC_H
#include "cstddef"
class Parser;

#ifdef __cplusplus
extern "C"
{
#endif

    Parser *connect(size_t sizeBlock);
    void recieve(const char *data, size_t size, Parser *parser);
    void disconnect(Parser *p);

#ifdef __cplusplus
}
#endif

#endif // ASYNC_H