#include "wind.h"

Wind::Wind()
{
	windx = std::vector<double>(60,WIND_AMPLITUDE);
	windz = std::vector<double>(60,0);
}

Wind::Wind(double wx, double wy, double wz)
{
	windx = std::vector<double>(60,WIND_AMPLITUDE);
	windz = std::vector<double>(60,0);
}

double Wind :: force_at(double t, int coord, double now)
{
	double index = (windx.size()-1)*std::fmod((long double) now / 1000.0, (long double)WIND_TIME_PERIOD) / WIND_TIME_PERIOD;
	if(coord == 0)
		return windx[std::floor(index)]*(1.0-std::fmod(index,1.0)) + windx[std::ceil(index)]*std::fmod(index,1.0);
	else
		return windz[std::floor(index)]*(1.0-std::fmod(index,1.0)) + windz[std::ceil(index)]*std::fmod(index,1.0);
}

bool Wind :: change_type(long long now)
{
	type = (type + 1) % TOTAL_WIND_TYPES;
	switch(type)
	{
		case WIND_TYPE_CONSTANT:
			for(int i = 0; i<windx.size(); i++)
			{
				windx[i] = WIND_AMPLITUDE;
				windz[i] = 0;
			}
			break;
		case WIND_TYPE_PULSE:
			for(int i = 0; i<windx.size(); i++)
			{
				windx[i] = WIND_AMPLITUDE * (i%2);
				windz[i] = 0;
			}
			break;
		case WIND_TYPE_SINSQUARE:
			double size = windx.size();
			for(int i=0; i<size; i++)
			{
				windx[i] = WIND_AMPLITUDE * std::sin(M_PI*((double) i)/size) * std::sin(M_PI*((double) i)/size);
				windz[i] = 0;
			}
			break;
	}
	return true;
}

bool Wind :: calculate_wind(long long now)
{
	double index = (windx.size()-1)*std::fmod((long double) now / 1000.0, (long double)WIND_TIME_PERIOD) / WIND_TIME_PERIOD;
	if(windx[std::floor(index)] == windx[std::ceil(index)] && windz[std::floor(index)] == windx[std::ceil(index)])
		return false;
	return true;
}