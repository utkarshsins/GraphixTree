#ifndef WIND_H
#define WIND_H


class Wind
{
    public:
        double wind_x_t, wind_y_t, wind_z_t;
        Wind();
        Wind(double , double , double );
        double force_at(double , int , double );

};

#endif // WIND_H
