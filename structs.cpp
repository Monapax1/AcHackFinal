#include <cmath>
#include "structs.h"
#include "globals.h"


Entity entity[32];
Entity myself;
Offsets offsets;
Color color;

// Aktualisiert die Informationen aller Entitäten
void UpdateEntInfo()
{
    for (int i = 1; i < gNumberOfPlayers; i++)
    {
        if (entity[i].bChecked)
        {
            if (entity[i].xHeadCoord && entity[i].yHeadCoord && entity[i].zHeadCoord &&
                myself.xHeadCoord && myself.yHeadCoord && myself.zHeadCoord)
            {
                // Berechne die relativen Koordinaten
                entity[i].relativeX = (*entity[i].xHeadCoord - *myself.xHeadCoord);
                entity[i].relativeY = (*entity[i].yHeadCoord - *myself.yHeadCoord);
                entity[i].relativeZ = (*entity[i].zHeadCoord - *myself.zHeadCoord);

                // Berechne die Distanz von mir zu dem Gegner
                entity[i].distanceFromMe = sqrt((entity[i].relativeX * entity[i].relativeX) +
                    (entity[i].relativeY * entity[i].relativeY) +
                    (entity[i].relativeZ * entity[i].relativeZ));

               
            }
        }
    }
}
