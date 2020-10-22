#include"util/String.h"
#include"util/Vector.h"

#include<stdlib.h>
#include<string.h>
#include<assert.h>

void string_create(String *dest, const char *str)
{
    dest->length = (uint32_t)strlen(str);
    dest->c_str = malloc(sizeof(char) * ((size_t)dest->length + 1));
    memcpy(dest->c_str, str, (size_t)dest->length + 1); // can use +1 here because strlen requires string to be null-terminated, so we copy this character as well
}

void string_create_sv(String *dest, const String_View *str)
{
    dest->length = str->length;
    dest->c_str = malloc(sizeof(char) * ((size_t)dest->length + 1));
    memcpy(dest->c_str, str->c_str, dest->length);
    dest->c_str[dest->length] = '\0';
}

void string_destroy(const String *string)
{
    free((void*)string->c_str);
}

String string_concat_s(const String *str1, const String *str2)
{
    String res;
    res.length = str1->length + str2->length;
    res.c_str = malloc(sizeof(char) * ((size_t)res.length + 1));
    memcpy(res.c_str, str1->c_str, str1->length);
    memcpy(res.c_str + str1->length, str2->c_str, str2->length);
    res.c_str[res.length] = '\0';

    return res;
}

String string_concat_sv(const String_View *str1, const String_View *str2)
{
    String res;
    res.length = str1->length + str2->length;
    res.c_str = malloc(sizeof(char) * ((size_t)res.length + 1));
    memcpy(res.c_str, str1->c_str, str1->length);
    memcpy(res.c_str + str1->length, str2->c_str, str2->length);
    res.c_str[res.length] = '\0';

    return res;
}

void string_view_create_s(String_View *dest, const String *string, uint32_t from_index, uint32_t to_index)
{
    assert(from_index < string->length && to_index >= from_index);

    if (to_index > string->length) to_index = string->length;

    dest->length = to_index - from_index;
    dest->c_str = string->c_str + from_index;
}

void string_view_create_c_str(String_View *dest, const char *str, uint32_t from_index, uint32_t to_index)
{
    uint32_t length = (uint32_t)strlen(str);
    assert(from_index < length && to_index >= from_index);

    if (to_index > length) to_index = length;

    dest->length = to_index - from_index;
    dest->c_str = str + from_index;
}

String_View *string_view_split(const String_View *string, char delim, uint32_t *splits)
{
    Vector strings;
    strings.elem_size = sizeof(String_View);
    vector_create(&strings, 10);

    uint32_t index = 0;
    const char *str = string->c_str;

    uint32_t sub_str_len = 0;
    const char *sub_str = string->c_str;
    
    while (index < string->length)
    {
        if (*str == delim)
        {
            if (sub_str_len > 0)
            {
                String_View sub_str_view;
                string_view_create_c_str(&sub_str_view, sub_str, 0, sub_str_len);

                vector_add(&strings, &sub_str_view);
                sub_str_len = 0;
            }

            sub_str = ++str;
            ++index;
        }
        else
        {
            ++str;
            ++sub_str_len;  
            ++index; 
        }
    }

    if (sub_str_len > 0)
    {
        String_View sub_str_view;
        string_view_create_c_str(&sub_str_view, sub_str, 0, sub_str_len);

        vector_add(&strings, &sub_str_view);
    }

    *splits = strings.size;
    return (String_View*)strings.arr;
}

String_View string_view_trim(const String_View *string)
{
    uint32_t index = 0;
    const char *start = string->c_str;
    
    while (*start == ' ')
    {
        ++start;
        ++index;
    }

    const char *end = start;
    const char *end_temp = start;

    while (index < string->length)
    {
        end = end_temp;

        while (*end != ' ' && index < string->length)
        {
            ++end;
            ++index;
        }

        end_temp = end;
        while (*end_temp == ' ') ++end_temp;
    }

    String_View res;
    string_view_create_c_str(&res, start, (uint32_t)((start - string->c_str) / sizeof(char)), index);
    return res;
}

IC_BOOL string_equals(const String *str1, const String *str2)
{
    if (str1->length != str2->length) return IC_FALSE;

    const char *itr1 = str1->c_str;
    const char *itr2 = str2->c_str;

    uint32_t index = 0;

    while (index++ < str1->length)
    {
        if (*(itr1++) != *(itr2++)) return IC_FALSE;
    }

    return IC_TRUE;
}

IC_BOOL string_equals_c_str(const String *str1, const char *str2)
{
    uint32_t str2_length = (uint32_t)strlen(str2);
    if (str1->length != str2_length) return IC_FALSE;

    const char *itr1 = str1->c_str;
    const char *itr2 = str2;

    uint32_t index = 0;

    while (index++ < str1->length)
    {
        if (*(itr1++) != *(itr2++)) return IC_FALSE;
    }

    return IC_TRUE;
}

IC_BOOL string_view_equals(const String_View *str1, const String_View *str2)
{
    if (str1->length != str2->length) return IC_FALSE;

    const char *itr1 = str1->c_str;
    const char *itr2 = str2->c_str;

    uint32_t index = 0;

    while (index++ < str1->length)
    {
        if (*(itr1++) != *(itr2++)) return IC_FALSE;
    }

    return IC_TRUE;
}

IC_BOOL string_view_equals_c_str(const String_View *str1, const char *str2)
{
    uint32_t str2_length = (uint32_t)strlen(str2);
    if (str1->length != str2_length) return IC_FALSE;

    const char *itr1 = str1->c_str;
    const char *itr2 = str2;

    uint32_t index = 0;

    while (index++ < str1->length)
    {
        if (*(itr1++) != *(itr2++)) return IC_FALSE;
    }

    return IC_TRUE;
}