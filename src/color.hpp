#ifndef color_hpp
#define color_hpp

    static const TCODColor worldEvents( 102, 102, 102 );
    static const TCODColor goodThing( 51, 153, 255 );
    static const TCODColor badThing( 255, 102, 255 );
    static const TCODColor magicalThing( 128, 0, 255 );
    static const TCODColor oddThing( 0, 0, 217 );
    static const TCODColor guiBackground( 0, 51, 102 );
    static const TCODColor guiForeground( 0, 134, 178 );

    static const TCODColor dungeon1Background( 20, 20, 20 );
    static const TCODColor dungeon1Wall( 50, 140, 50 );
    static const TCODColor dungeon1Floor( 60, 60, 60 );

    static const TCODColor calvin1( TCODColor::lightestSepia );
    static const TCODColor orc( TCODColor::desaturatedGreen );
    static const TCODColor troll( TCODColor::darkerGreen );
    static const TCODColor demon( TCODColor::darkerRed );
    static const TCODColor bob( TCODColor::darkBlue );
    static const TCODColor corpse( 140, 0, 0 );
    static const TCODColor cockroach( TCODColor::darkerSepia );

    static const TCODColor ration( 30, 175, 30 );
    static const TCODColor scroll( TCODColor::lightYellow );

    TCODColor returnUnseen( const TCODColor &col );

#endif