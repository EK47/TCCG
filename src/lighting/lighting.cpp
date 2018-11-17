#include "../main.hpp"

LightingEngine::LightingEngine()
{
    
}

LightingEngine::~LightingEngine()
{

}

bool LightingEngine::addLight( int x, int y, float intensity, bool isStatic, TCODColor color )
{
    lights.emplace_back( x, y, intensity, isStatic, color );
    return true;
}

// This generates the static lightmap.
bool LightingEngine::generateStatic()
{
}