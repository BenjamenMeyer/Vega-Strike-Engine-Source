/*
 * coord_select.cpp
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

#include "cmd/unit_generic.h"
#include "src/star_system.h"
#include "loc_select.h"
#include "coord_select.h"
#include "root_generic/vs_globals.h"
#include "src/in_kb.h"
#include "gfx/camera.h"
#include "src/universe.h"

int CoordinateSelectChange = 0;
int CoordinateSelectmousex;
int CoordinateSelectmousey;
extern Vector MouseCoordinate(int mouseX, int mouseY);

extern KBSTATE mouseButtonState;

void CoordinateSelect::MouseMoveHandle(KBSTATE, int x, int y, int, int, int) {
    if (mouseButtonState == DOWN) {
        CoordinateSelectChange = 2;
    } else {
        CoordinateSelectChange = 1;
    }
    CoordinateSelectmousex = x;
    CoordinateSelectmousey = y;
}

CoordinateSelect::CoordinateSelect(QVector start) : LocSelAni("locationselect.ani", true, .5, MIPMAP, true),
        LocalPosition(start) {
    CrosshairSize = 2;
    CoordinateSelectmousex = configuration()->graphics.resolution_x / 2;
    CoordinateSelectmousey = configuration()->graphics.resolution_y / 2;
    CoordinateSelectChange = 1;
}

void CoordinateSelect::UpdateMouse() {
    if (CoordinateSelectChange == 1) {
        Vector CamPos;
        Vector CamQ, CamR;
        _Universe->AccessCamera()->GetPQR(CamPos, CamQ, CamR);

        Vector mousePoint(MouseCoordinate(CoordinateSelectmousex, CoordinateSelectmousey));
        float mouseDistance = mousePoint.k * mousePoint.k;
        mousePoint = Transform(CamPos, CamQ, CamR, mousePoint);
        //QVector cp;
        CamPos = _Universe->AccessCamera()->GetPosition();
        //float mouseDistance = mousePoint.Dot (CamR);
        //distance out into z...straight line...

        float distance = CamR.Dot((LocalPosition - CamPos).Cast());           //distance out into z...straight line...
        if (mouseDistance != 0) {
            LocalPosition = mousePoint * (distance / mouseDistance) + CamPos.Cast();
        } else {
            LocalPosition = 2 * CamR + CamPos.Cast();
        }
        CoordinateSelectChange = 0;
    }
    if (CoordinateSelectChange == 2) {
        Vector CamPos, CamQ, CamR;
        _Universe->AccessCamera()->GetPQR(CamPos, CamQ, CamR);
        CamPos = _Universe->AccessCamera()->GetPosition();

        LocalPosition = LocalPosition - CamPos;
        float distance = sqrt(CamR.Dot(LocalPosition));         //distance out into z...straight line...
        //make it a ratio btw top and bottom.... for near and far;
        float ratio = float(configuration()->graphics.resolution_y - CoordinateSelectmousey) / configuration()->graphics.resolution_y;
        float tmp, n, f;
        GFXGetFrustumVars(true, &tmp, &tmp, &tmp, &tmp, &n, &f);         ///unkind call :-D
        tmp = n + ratio * ratio * ratio * (f - n);           //how far n^3 law
        if (distance != 0) {
            LocalPosition = LocalPosition * (tmp / distance) + CamPos;
        } else {
            LocalPosition = CamPos + CamR * n;
        }
        CoordinateSelectChange = 0;
    }
}

void CoordinateSelect::Draw() {
    if (CoordinateSelectChange) {
        UpdateMouse();
    }
    GFXLoadIdentity(MODEL);

    GFXPushBlendMode();
    GFXBlendMode(ONE, ONE);
    LocSelAni.SetPosition(LocalPosition);
    LocSelAni.Draw();
    GFXPopBlendMode();
}

