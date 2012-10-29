#include "wind.h"

Wind::Wind()
{
    wind_x_t = 5;
    wind_y_t = 5;
    wind_z_t = 5;
    //ctor
}

Wind::Wind(double wx, double wy, double wz)
{
    wind_x_t = wx;
    wind_y_t = wy;
    wind_z_t = wz;
    //ctor
}

double Wind :: force_at(double t, int coord, double value)
{
    switch(coord)
    {
        case 0: //x
            return wind_x_t;
        case 1: //y
            return wind_y_t;
        case 2: //z
            return wind_z_t;
    }
}
