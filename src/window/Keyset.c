#include"window/Keyset.h"
#include"util/Utils.h"
#include"Log.h"

#include<stdlib.h>

#ifdef IC_DEBUG

static const char *const KEYSET_IDENTIFIER_NAMES[] =
{
    "forward", "backward", "left", "right", "up/jump", "down", "shoot", "perspective"
};

static const char *const KEYSET_OPTION_NAMES[1] =
{
    "key"
};

static void __keyset_check_required_options_impl(IC_OPTIONS_32 options, uint8_t num_options, const char *part_name, const char *const *option_names, uint32_t line, const char *path, IC_ERROR_CODE *error_code)
{
    for (uint8_t i = 0; i < num_options; ++i)
    {
        if (!IC_DEBUG_OPTION_IS_SET(options, i))
        {
            log_trace("Error on line %u at '%s': Option '%s' of %s must be set", line + 1, path, option_names[i], part_name);
            if (error_code) *error_code = IC_KEYSET_INCOMPLETE_ERROR;
        }
    }
}

#endif // IC_DEBUG

#ifdef IC_DEBUG
    #define __check_required_options(options, num_options, part_name, option_names, line, path, error_code) __keyset_check_required_options_impl(options, num_options, part_name, option_names, line, path, error_code)
#else
    #define __check_required_options(options, num_options, part_name, option_names, line, path, error_code)
#endif // IC_DEBUG

struct _Key_Map_Element
{
    const char *key_name;
    IC_KEY key_value;
};

typedef struct _Key_Map_Element Key_Map_Element;

static const Key_Map_Element KEY_MAP[] =
{
    { "A", IC_KEY_A },
    { "B", IC_KEY_B },
    { "C", IC_KEY_C },
    { "D", IC_KEY_D },
    { "E", IC_KEY_E },
    { "F", IC_KEY_F },
    { "G", IC_KEY_G },
    { "H", IC_KEY_H },
    { "I", IC_KEY_I },
    { "J", IC_KEY_J },
    { "K", IC_KEY_K },
    { "L", IC_KEY_L },
    { "M", IC_KEY_M },
    { "N", IC_KEY_N },
    { "O", IC_KEY_O },
    { "P", IC_KEY_P },
    { "Q", IC_KEY_Q },
    { "R", IC_KEY_R },
    { "S", IC_KEY_S },
    { "T", IC_KEY_T },
    { "U", IC_KEY_U },
    { "V", IC_KEY_V },
    { "W", IC_KEY_W },
    { "X", IC_KEY_X },
    { "Y", IC_KEY_Y },
    { "Z", IC_KEY_Z },
    { "Left", IC_KEY_LEFT },
    { "Right", IC_KEY_RIGHT },
    { "Up", IC_KEY_UP },
    { "Down", IC_KEY_DOWN },
    { "Space", IC_KEY_SPACE },
    { "Shift", IC_KEY_SHIFT },
    { "Enter", IC_KEY_ENTER }
};

static const uint32_t KEY_MAP_LENGTH = sizeof(KEY_MAP) / sizeof(Key_Map_Element);

static void __keyset_check_key_impl(uint32_t num_parts, String_View *parts, Keyset *res, uint32_t line, const char *path, IC_ERROR_CODE *error_code, const char *key_name, IC_KEY *key_dest)
{
    IC_DEBUG_OPTIONS_32(options)

    for (uint32_t i = 1; i < num_parts; ++i)
    {
        uint32_t num_arg_pair;
        String_View *arg_pair = string_view_split(&parts[i], '=', &num_arg_pair);

        if (num_arg_pair == 0) // Incorrect syntax
        {
            log_trace("Warning on line %u at '%s': '%.*s' is incorrect syntax", line + 1, path, parts[i].length, parts[i].c_str);
            free(arg_pair);
            if (error_code) *error_code = IC_KEYSET_SYNTAX_ERROR;
            continue;
        }

        if (string_view_equals_c_str(&arg_pair[0], "key"))
        {
            *key_dest = (IC_KEY)-1;
            const Key_Map_Element *itr = KEY_MAP;
            for (uint32_t i = 0; i < KEY_MAP_LENGTH; ++i)
            {
                if (string_view_equals_c_str(&arg_pair[1], itr->key_name))
                {
                    *key_dest = itr->key_value;
                    break;
                }

                ++itr;
            }

            log_assert(*key_dest != -1, "Error setting forward key: Unknown key '%.*s'", arg_pair[1].length, arg_pair[1].c_str);
            IC_DEBUG_OPTION_SET(options, 0);
        }
        else
        {
            log_trace("Warning on line %u at '%s': Unrecognized option '%.*s' for '%s'", line + 1, path, arg_pair[0].length, arg_pair[0].c_str, key_name);
            if (error_code) *error_code = IC_KEYSET_SYNTAX_ERROR;
        }

        free(arg_pair);
    }

    __check_required_options(options, 1, "forward", KEYSET_OPTION_NAMES, line + 1, path, error_code);
}

Keyset keyset_load(const char *path, IC_ERROR_CODE *error_code)
{
    if (error_code) *error_code = IC_NO_ERROR;

    Keyset res = {0};
    
    IC_ERROR_CODE ec;
    String source_str = read_source(path, &ec);
    if (ec != IC_NO_ERROR)
    {
        if (error_code) *error_code = ec;
        return res;
    }

    String_View source;
    string_view_create_s(&source, &source_str, 0, UINT32_MAX);

    uint32_t num_lines;
    String_View *lines = string_view_split(&source, '\n', &num_lines);

    IC_DEBUG_OPTIONS_32(keyset_options);

    for (uint32_t l = 0; l < num_lines; ++l)
    {
        String_View line = lines[l];
        line = string_view_trim(&line);

        uint32_t num_parts;
        String_View *parts = string_view_split(&line, ' ', &num_parts);

        if (num_parts == 0) // Not really an error
        {
            free(parts);
            log_trace("Warning on line %u at '%s': Empty line with whitespace found. This is not critical, but leads to inefficiencies", l + 1, path);
            if (error_code) *error_code = IC_KEYSET_WHITESPACE_INEFFICIENCY;
            continue;
        }

        if (string_view_equals_c_str(&parts[0], "forward"))
        {
            __keyset_check_key_impl(num_parts, parts, &res, l, path, error_code, "forward", &res.forward);

            IC_DEBUG_OPTION_SET(keyset_options, 0);
        }
        else if (string_view_equals_c_str(&parts[0], "backward"))
        {
            __keyset_check_key_impl(num_parts, parts, &res, l, path, error_code, "backward", &res.backward);

            IC_DEBUG_OPTION_SET(keyset_options, 1);
        }
        else if (string_view_equals_c_str(&parts[0], "left"))
        {
            __keyset_check_key_impl(num_parts, parts, &res, l, path, error_code, "left", &res.left);

            IC_DEBUG_OPTION_SET(keyset_options, 2);
        }
        else if (string_view_equals_c_str(&parts[0], "right"))
        {
            __keyset_check_key_impl(num_parts, parts, &res, l, path, error_code, "right", &res.right);

            IC_DEBUG_OPTION_SET(keyset_options, 3);
        }
        else if (string_view_equals_c_str(&parts[0], "up/jump"))
        {
            __keyset_check_key_impl(num_parts, parts, &res, l, path, error_code, "up/jump", &res.up);

            IC_DEBUG_OPTION_SET(keyset_options, 4);
        }
        else if (string_view_equals_c_str(&parts[0], "down"))
        {
            __keyset_check_key_impl(num_parts, parts, &res, l, path, error_code, "down", &res.down);

            IC_DEBUG_OPTION_SET(keyset_options, 5);
        }
        else if (string_view_equals_c_str(&parts[0], "shoot"))
        {
            __keyset_check_key_impl(num_parts, parts, &res, l, path, error_code, "shoot", &res.shoot);

            IC_DEBUG_OPTION_SET(keyset_options, 6);
        }
        else if (string_view_equals_c_str(&parts[0], "perspective"))
        {
            __keyset_check_key_impl(num_parts, parts, &res, l, path, error_code, "perspective", &res.switch_perspectives);

            IC_DEBUG_OPTION_SET(keyset_options, 7);
        }
        else
        {
            log_trace("Warning on line %u at '%s': Unrecognized identifier '%.*s'", l + 1, path, parts[0].length, parts[0].c_str);
            if (error_code) *error_code = IC_LEVEL_SYNTAX_ERROR;
        }

        free(parts);
    }

    free(lines);
    string_destroy(&source_str);

    // Check to make sure all required identifiers are present
#ifdef IC_DEBUG
    for (uint8_t i = 0; i < 8; ++i)
    {
        if (!IC_DEBUG_OPTION_IS_SET(keyset_options, i))
        {
            log_trace("Error in level file at '%s': Missing options of identifier '%s'", path, KEYSET_IDENTIFIER_NAMES[i]);
            if (error_code) *error_code = IC_KEYSET_INCOMPLETE_ERROR;
        }
    }
#endif // IC_DEBUG

    return res;
}

void keyset_save(const char *path, const Keyset *keyset)
{
    // TODO
}