#include "wind.h"
#include "Debug.h"

Wind::Wind()
{
	windx = std::vector<double>(60,0);
	windz = std::vector<double>(60,0);
	type = WIND_TYPE_ZERO;
	/*for(int i =0; i<windx.size(); i++)
		std::cout << windx[i] << std::endl;*/
}

double Wind :: force_at(int coord, long long now)
{
	double index = (windx.size()-1)*std::fmod((long double) now / 1000.0, (long double)WIND_TIME_PERIOD) / WIND_TIME_PERIOD;
	//std::cout << "INDEX : " << index << "\t\t\t\t" << windx[index] << std::endl;
	if(coord == 0)
		return windx[std::floor(index)]*(1.0-std::fmod(index,1.0)) + windx[std::ceil(index)]*std::fmod(index,1.0);
	else
		return windz[std::floor(index)]*(1.0-std::fmod(index,1.0)) + windz[std::ceil(index)]*std::fmod(index,1.0);
}

bool Wind :: change_type()
{
	type = (type + 1) % TOTAL_WIND_TYPES;
	double size = windx.size();
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
			for(int i = 0; i<size; i++)
			{
				windx[i] = WIND_AMPLITUDE * ((i*2*(int) WIND_PULSE_COUNT/(int) size)%2);
				windz[i] = 0;
			}
			break;
		case WIND_TYPE_ZERO:
			for(int i = 0; i<size; i++)
			{
				windx[i] = 0;
				windz[i] = 0;
			}
			break;
		case WIND_TYPE_SINSQUARE:
			for(int i=0; i<size; i++)
			{
				windx[i] = WIND_AMPLITUDE * std::sin(M_PI*((double) i)*WIND_PULSE_COUNT/size) * std::sin(M_PI*((double) i)*WIND_PULSE_COUNT/size);
				windz[i] = 0;
			}
			windx[size-1] = 0;
			break;
	}
	return true;
}

bool Wind :: reverse_type()
{
	type -= 2;
	return change_type();
}

bool Wind :: calculate_wind(long long now)
{
	double index = (windx.size()-1)*std::fmod((long double) now / 1000.0, (long double)WIND_TIME_PERIOD) / WIND_TIME_PERIOD;
	if(windx[std::floor(index)] == windx[std::ceil(index)] && windz[std::floor(index)] == windz[std::ceil(index)])
		return false;
	return true;
}