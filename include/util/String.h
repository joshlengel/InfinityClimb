#pragma once

#include"Core.h"

#include<stdint.h>

struct _String
{
    char *c_str;
    uint32_t length;
};

struct _String_View
{
    const char *c_str;
    uint32_t length;
};

typedef struct _String String;
typedef struct _String_View String_View;

void string_create(String *dest, const char *str);
void string_destroy(const String *string);

void string_view_create_s(String_View *dest, const String *string, uint32_t from_index, uint32_t to_index);
void string_view_create_c_str(String_View *dest, const char *str, uint32_t from_index, uint32_t to_index);

String_View *string_view_split(const String_View *string, char delim, uint32_t *splits);
String_View string_view_trim(const String_View *string);

IC_BOOL string_equals(const String *str1, const String *str2);
IC_BOOL string_equals_c_str(const String *str1, const char *str2);
IC_BOOL string_view_equals(const String_View *str1, const String_View *str2);
IC_BOOL string_view_equals_c_str(const String_View *str1, const char *str2);