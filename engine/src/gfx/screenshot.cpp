/*
 * screenshot.cpp
 *
 * Vega Strike - Space Simulation, Combat and Trading
 * Copyright (C) 2001-2025 The Vega Strike Contributors:
 * Project creator: Daniel Horn
 * Original development team: As listed in the AUTHORS file
 * Current development team: Roy Falk, Benjamen R. Meyer, Stephen G. Tuggy
 *
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
 * along with Vega Strike.  If not, see <https://www.gnu.org/licenses/>.
 */


#include "src/in_kb.h"
#include "root_generic/vs_globals.h"
#include "root_generic/xml_support.h"
#include "gfx/vsimage.h"
#include "configuration/configuration.h"
// See https://github.com/vegastrike/Vega-Strike-Engine-Source/pull/851#discussion_r1589254766
#include <glut.h>

using namespace VSFileSystem;

void Screenshot(const KBData &, KBSTATE state) {
    if (state == PRESS) {
        GLint xywh[4] = {0, 0, 0, 0};
        xywh[2] = configuration()->graphics.resolution_x;
        xywh[3] = configuration()->graphics.resolution_y;
        glGetIntegerv(GL_VIEWPORT, xywh);
        unsigned char *tmp = static_cast<unsigned char *>(malloc(
            static_cast<size_t>(xywh[2]) * static_cast<size_t>(xywh[3]) * 4 * sizeof(unsigned char)));
        //memset(tmp,0x7f,xywh[2]*xywh[3]*4*sizeof(char));
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glPixelStorei(GL_PACK_ROW_LENGTH, xywh[2]);
        glFinish();
        glReadPixels(0, 0, xywh[2], xywh[3], GL_RGB, GL_UNSIGNED_BYTE, tmp);
        glPixelStorei(GL_PACK_ROW_LENGTH, 0);
        ::VSImage image;
        VSFileSystem::VSFile f;
        static int count = 0;
        std::string filename = "Screenshot" + XMLSupport::tostring(count) + ".png";
        for (;;) {
            if (f.OpenReadOnly(filename, TextureFile) <= VSFileSystem::Ok) {
                f.Close();
                filename = "Screenshot" + XMLSupport::tostring(++count) + ".png";
            } else {
                break;
            }
        }
        char *tmpchar = strdup(filename.c_str());
        image.WriteImage(tmpchar, tmp, PngImage, xywh[2], xywh[3], false, 8, TextureFile, true);
        free(tmpchar);
    }
}

