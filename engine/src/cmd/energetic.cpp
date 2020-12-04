/**
 * energetic.cpp
 *
 * Copyright (C) Daniel Horn
 * Copyright (C) 2020 pyramid3d, Stephen G. Tuggy, and other Vega Strike
 * contributors
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

#include "energetic.h"

#include "game_config.h"
#include "damageable.h"
#include "vegastrike.h"

/* This class provides all energy generation methods to unit types -
 * ships, space installations, missiles, drones, etc. */

Energetic::Energetic()
{

}

void Energetic::decreaseWarpEnergy( bool insys, float time )
{
    static float bleedfactor = GameConfig::GetVariable( "physics", "warpbleed", 2.0f );
    static bool  WCfuelhack  = GameConfig::GetVariable( "physics", "fuel_equals_warp", false );
    if (WCfuelhack)
        this->warpenergy = this->fuel;
    this->warpenergy -= (insys ? jump.insysenergy/bleedfactor : jump.energy)*time;
    if (this->warpenergy < 0)
        this->warpenergy = 0;
    if (WCfuelhack)
        this->fuel = this->warpenergy;
}

float Energetic::energyData() const
{
    const Damageable *damageable = reinterpret_cast<const Damageable*>(this);
    const Shield shield = damageable->shield;

    static bool max_shield_lowers_capacitance =
        GameConfig::GetVariable( "physics", "max_shield_lowers_capacitance", false );
    if (max_shield_lowers_capacitance) {
        if ( maxenergy <= damageable->totalShieldEnergyCapacitance( shield ) )
            return 0;
        return ( (float) energy )/( maxenergy-damageable->totalShieldEnergyCapacitance( shield ) );
    } else {
        return ( (float) energy )/maxenergy;
    }
}

//Returns the current ammt of energy left
float Energetic::energyRechargeData() const
{
    return recharge;
}

float Energetic::fuelData() const
{
    return fuel;
}

float Energetic::getFuelUsage( bool afterburner )
{
    static float normalfuelusage = GameConfig::GetVariable( "physics", "FuelUsage", 1.0f );
    static float abfuelusage     = GameConfig::GetVariable( "physics", "AfterburnerFuelUsage", 4.0f );
    if (afterburner)
        return abfuelusage;
    return normalfuelusage;
}

float Energetic::getWarpEnergy() const
{
    return warpenergy;
}

void Energetic::increaseWarpEnergy( bool insys, float time )
{
    static bool WCfuelhack = GameConfig::GetVariable( "physics", "fuel_equals_warp", false );
    if (WCfuelhack)
        this->warpenergy = this->fuel;
    this->warpenergy += (insys ? jump.insysenergy : jump.energy)*time;
    if (this->warpenergy > this->maxwarpenergy)
        this->warpenergy = this->maxwarpenergy;
    if (WCfuelhack)
        this->fuel = this->warpenergy;
}

float Energetic::maxEnergyData() const
{
    return maxenergy;
}

void Energetic::rechargeEnergy()
{
    static bool reactor_uses_fuel = GameConfig::GetVariable( "physics", "reactor_uses_fuel", false );
    if ( (!reactor_uses_fuel) || (fuel > 0) )
        energy += recharge * simulation_atom_var;
}

bool Energetic::refillWarpEnergy()
{
    static bool WCfuelhack = GameConfig::GetVariable( "physics", "fuel_equals_warp", false );
    if (WCfuelhack)
        this->warpenergy = this->fuel;
    float tmp = this->maxwarpenergy;
    if (tmp < this->jump.energy)
        tmp = this->jump.energy;
    if (tmp > this->warpenergy) {
        this->warpenergy = tmp;
        if (WCfuelhack)
            this->fuel = this->warpenergy;
        return true;
    }
    return false;
}

void Energetic::setAfterburnerEnergy( float aft )
{
    afterburnenergy = aft;
}

void Energetic::setEnergyRecharge( float enrech )
{
    recharge = enrech;
}

void Energetic::setFuel( float f )
{
    fuel = f;
}



void Energetic::setMaxEnergy( float maxen )
{
    maxenergy = maxen;
}

float Energetic::shieldRechargeData() const
{
    const Damageable *damageable = reinterpret_cast<const Damageable*>(this);
    const Shield shield = damageable->shield;
    return shield.recharge;
}



float Energetic::warpCapData() const
{
    return maxwarpenergy;
}


float Energetic::warpEnergyData() const
{
    if (maxwarpenergy > 0)
        return ( (float) warpenergy )/( (float) maxwarpenergy );
    if (jump.energy > 0)
        return ( (float) warpenergy )/( (float) jump.energy );
    return 0.0f;
}


















