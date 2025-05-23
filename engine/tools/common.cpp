/*
 * Copyright (C) 2001-2022 Daniel Horn, Konstantinos Arvanitis,
 * pyramid3d, Stephen G. Tuggy, and other Vega Strike contributors.
 *
 * https://github.com/vegastrike/Vega-Strike-Engine-Source
 *
 * This file is part of Vega Strike.
 *
 * Vega Strike is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Vega Strike is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Vega Strike. If not, see <https://www.gnu.org/licenses/>.
 */

/** @file common.cpp Common code for the VSR package file tools.

  @todo Proper documentation.
 */

#include "tools/common.h"
#include <errno.h>
#include <vector>

FILEHandle::FILEHandle(const std::string &fname, const char perms[]) {
    file = fopen(fname.c_str(), perms);
    if (!file) {
        throw errno;
    }
}
