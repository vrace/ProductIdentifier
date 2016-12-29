#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Base64Encode.h"

#define IDENTIFIER_MAX_LEN 1000
#define IDENTIFIER_BASE64_LEN BASE64_ENCODE_LEN(IDENTIFIER_MAX_LEN)

int min(int a, int b)
{
    return a < b ? a : b;
}

int PrepareIdentifierFromCommandLine(char *buffer, int maxchars, char *argv[])
{
    char *p = buffer;
    int count, i;
    
    memset(buffer, 0, maxchars);
    
    count = min(maxchars, (int)strlen(argv[1]));
    strncpy(p, argv[1], count);
    p += count;
    maxchars -= count;
    
    for (i = 0; i < 2; i++)
    {
        count = min(maxchars, 3);
        strncpy(p, "<#>", count);
        p += count;
        maxchars -= count;
        
        count = min(maxchars, (int)strlen(argv[i + 2]));
        strncpy(p, argv[i + 2], count);
        p += count;
        maxchars -= count;
    }
    
    p[maxchars - 1] = 0;
    
    return (int)strlen(buffer);
}

int AppendUserInput(char *buffer, int maxchars, const char *name)
{
    if (maxchars > 1)
    {
        int read;
        
        printf("Enter %s: ", name);
        fgets(buffer, maxchars, stdin);
        
        read = (int)strlen(buffer);
        if (buffer[read - 1] == '\n')
            buffer[--read] = '\0';
        
        return read;
    }
    else
    {
        return 0;
    }
}

int PrepareIdentifierFromUserInput(char *buffer, int maxchars)
{
    const char *names[] = { "Product System ID", "Product System Customer ID", "Product Number" };
    int bytes = 0;
    int count, i;
    
    memset(buffer, 0, maxchars);
    
    for (i = 0; i < 3; i++)
    {
        bytes += AppendUserInput(buffer + bytes, maxchars - bytes, names[i]);
        
        if (i == 0 || i == 1)
        {
            count = min(maxchars - bytes, 3);
            strncpy(buffer + bytes, "<#>", count);
            bytes += count;
        }
    }
    
    buffer[maxchars - 1] = '\0';
    
    return (int)strlen(buffer);
}

int PrepareIdentifier(char *buffer, int maxchars, int argc, char *argv[])
{
    if (argc == 4)
    {
        return PrepareIdentifierFromCommandLine(buffer, maxchars, argv);
    }
    else
    {
        return PrepareIdentifierFromUserInput(buffer, maxchars);
    }
}

int main(int argc, char *argv[])
{
    int i;
    char plainIdentifier[IDENTIFIER_MAX_LEN];
    char base64buffer[IDENTIFIER_BASE64_LEN];
    
    PrepareIdentifier(plainIdentifier, IDENTIFIER_MAX_LEN, argc, argv);
    Base64Encode(base64buffer, plainIdentifier, (int)strlen(plainIdentifier));
    
    if (argc != 4)
        printf("Product Identifier:\n");
    
    for (i = 0; i < (int)strlen(base64buffer); i++)
        printf("%02x", base64buffer[i]);
    printf("\n");
    
    if (argc != 4)
    {
        printf("Press ENTER to quit.\n");
        fgets(plainIdentifier, IDENTIFIER_MAX_LEN, stdin);
    }
    
    return 0;
}
