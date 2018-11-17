#ifndef lighting_hpp
#define lighting_hpp
#pragma once

struct light
{
    int x;
    int y;
    float intensity;
    bool isStatic;
    TCODColor color;
};

class LightingEngine
{
public:
    LightingEngine();
    ~LightingEngine();
    void setIntensity( float intensity );
    void setAmbient( float ambient );
    bool addLight( int x, int y, float intensity, bool isStatic, TCODColor color );
    bool generateStatic();
    bool calculateDynamic();
    std::vector<light> lights;
    std::vector<float> staticMask;
    std::vector<float> dynamicMask;
private:
    bool nextGeneration();
};

#endif