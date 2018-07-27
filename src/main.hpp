/*

    The Calvin Chronicle's Game
    Copyright (C) 2018 Ethan Kelly

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef main_hpp
#define main_hpp
#pragma once

#include <math.h>
#include <algorithm>
#include <deque>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <queue>
#include <random>
#include <stdexcept>
#include <stdio.h>
#include <unistd.h>
#include <unordered_map>
#include "../include/libtcod/libtcod.hpp"
#include "../include/LightMask/LightMask.hpp"
#include "../include/cereal/cereal.hpp"
#include "rarityDict/dangerDictionary.hpp"
#include "rarityDict/rarityDictionary.hpp"
#include "color.hpp"
class Actor;
#include "camera.hpp"
#include "effects.hpp"
#include "destructible.hpp"
#include "attacker.hpp"
#include "ai.hpp"
#include "pickable.hpp"
#include "container.hpp"
#include "gui.hpp"
#include "actor.hpp"
#include "map.hpp"
#include "engine.hpp"

#endif