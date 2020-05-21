
#include <stdio.h>
#include <string>


static const char DOT_TOKEN_C = '.';

int main(int argc, char const *argv[])
{

    std::string str("DT.count");

    size_t delPos = str.rfind(DOT_TOKEN_C); // search backwards from end of string
    delPos = str.rfind(DOT_TOKEN_C, str.length() - 0); // similar operation // search backwards from end of string
    printf("delPos = %u\n", delPos);

    if (delPos != std::string::npos)\
    {
        printf("OK\n");
    }
    else
    {
        printf("Error\n");   
    }

    return 0;
}