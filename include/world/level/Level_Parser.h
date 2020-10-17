#pragma once

#include"world/level/Level.h"

#include"Core.h"

IC_ERROR_CODE level_create_from_file(const char *path, Level *dest);