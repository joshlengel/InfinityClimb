#pragma once

#include"window/Input.h"
#include"Core.h"

struct _Keyset
{
    IC_KEY forward;
    IC_KEY backward;
    IC_KEY left;
    IC_KEY right;
    IC_KEY up;
    IC_KEY down;
    IC_KEY shoot;

    IC_KEY switch_perspectives;
};

typedef struct _Keyset Keyset;

Keyset keyset_load(const char *path, IC_ERROR_CODE *error_code);
void keyset_save(const char *path, const Keyset *keyset);