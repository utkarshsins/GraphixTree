#ifndef WIND_H
#define WIND_H

#include <vector>
#include <cmath>
#include <iostream>

#define WIND_TYPE_CONSTANT 0
#define WIND_TYPE_PULSE 1
#define WIND_TYPE_SINSQUARE 2

#define TOTAL_WIND_TYPES 3
#define WIND_AMPLITUDE 5

#define WIND_TIME_PERIOD 60.0

#define M_PI 3.143

class Wind
{
    public:
        Wind();
		bool change_type();
        double force_at(int, long long);
		int type;
		std::vector<double> windx, windz;		
		bool calculate_wind(long long);
};

#endif // WIND_H
