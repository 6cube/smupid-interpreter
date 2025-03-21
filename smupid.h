#pragma once

#ifndef _INC_STDLIB

#include <stdlib.h>

#endif
#ifndef _INC_STDIO

#include <stdio.h>

#endif
#ifndef _INC_STRING

static int strchr(const char* str, char chr) // returns NULL if not found (failure point btw)
{
    for (int i = 0; i < sizeof(str); i++)
    {
        if (str[i] == chr)
            return i;
    }
    return -1;
}

#endif

#define NOT_FOUND       -1
#define ZIP             1
#define LOOP_FORWARD    2
#define LOOP_BACK       3
#define COMMENT         4

static unsigned char* mem;
static unsigned short ptr = 0;
static char jumpMode = 0;
static short loopNum = 0;
static unsigned char inComment = 0;
static unsigned char eq;

unsigned char* smupid_setup()
{
    mem = (unsigned char*)calloc(65536, 1);
    return mem;
}

void smupid_cleanup()
{
    free(mem);
}

char smupid(const char* term)
{
    char c = term[0];

    if (jumpMode != 0)
    {
        switch (jumpMode)
        {
        case ZIP:
            if (c == '{' && inComment == 0)
                loopNum++;
            else if (c == '}' && inComment == 0)
                loopNum--;
            else if (strchr(term, ')') != -1)
                inComment = 0;
            else if (c == '(')
            {
                inComment = 1;
                return 0;
            }
            else if (c == 'z')
            {
                if (loopNum < 1 && inComment == 0)
                {
                    jumpMode = 0;
                    loopNum = 0;
                }
            }
            break;
        case LOOP_FORWARD:
            if (c == '{' && inComment == 0)
                loopNum++;
            else if (strchr(term, ')') != NOT_FOUND)
                inComment = 0;
            else if (c == '(')
            {
                inComment = 1;
                return 0;
            }
            else if (c == '}' && inComment == 0)
            {
                loopNum--;
                if (loopNum < 0 && inComment == 0)
                {
                    jumpMode = 0;
                    loopNum = 0;
                }
            }
            break;
        case LOOP_BACK:
            if (c == '{' && inComment == 0)
            {
                loopNum--;
                if (loopNum < 0 && inComment == 0)
                {
                    jumpMode = 0;
                    loopNum = 0;
                    return 0;
                }
            }
            else if (strchr(term, ')') != NOT_FOUND)
            {
                inComment = 1;
                return 0;
            }
            else if (c == '(')
                inComment = 0;
            else if (c == '}' && inComment == 0)
                loopNum++;

            return -1;
        
        case COMMENT:
            if (strchr(term, ')') != NOT_FOUND)
                jumpMode = 0;
            break;
        }

        return 1;
    }

    int k = atoi(term + 1);

    switch (c)
    {
    case 'p':
        ptr = 0;
        break;
    case 'd':
        ptr += k;
        break;
    case 'a':
        mem[ptr] += k;
        break;
    case 'i':
        mem[ptr] = getchar();
        break;
    case 'o':
        putchar(mem[ptr]);
        break;
    case 'z':
        jumpMode = ZIP;
        break;
    case '=':
        eq = k;
        break;
    case '{':
        if (mem[ptr] == eq)
            jumpMode = LOOP_FORWARD;
        break;
    case '}':
        jumpMode = LOOP_BACK;
        return -1;
        break;
    case '(':
        jumpMode = COMMENT;
        return 0;
        break;
    }
    return 1;
}
