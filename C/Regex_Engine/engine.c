#include "engine.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main( void )
{
    return 0;
}

char *regex(char *s, char *pattern)
{
    /* construct pattern matching functions */
    char c;
    while ((c = *s++))
    {
        char p = *pattern;

        switch ((enum general) p)
        {
            case CHARACTER_START:       break;
            case CHARACTER_END:         break;
            case INPUT_START:           break;
            case INPUT_END:             break;
            case ASSOCIATION_START:     break;
            case ASSOCIATION_END:       break;
            case QUANTIFIER_START:      break;
            case QUANTIFIER_END:        break;
            case WILDCARD_ZERO_OR_MORE: break;
            case WILDCARD_ONE_OR_MORE:  break;
            case WILDCARD_ZERO_OR_ONE:  break;
            default:
                if (p
                break;
        }
    }
}
