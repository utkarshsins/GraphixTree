#include <iostream>
#include "betanoise.h"
#include "Debug.h"
#include "colornoise.h"
#include <algorithm>

#define M_PI 3.141592654


void swap(struct Complex * coeff1, struct Complex * coeff2)
{
	double temp = coeff1->real;
	coeff1->real = coeff2->real;
	coeff2->real = temp;

	temp = coeff1->imaginary;
	coeff1->imaginary = coeff2->imaginary;
	coeff2->imaginary = temp;
}


// Gaussian Random Function :
// Knuth Algorithm
//		Mean				= 0
//		Standard Deviation	= 1
double BetaNoise::gauss()
{
	static double V1, V2, S;
	static int phase = 0;
	double X;

	if(phase == 0) {
		do {
			double U1 = (double)rand() / RAND_MAX;
			double U2 = (double)rand() / RAND_MAX;

			V1 = 2 * U1 - 1;
			V2 = 2 * U2 - 1;
			S = V1 * V1 + V2 * V2;
			} while(S >= 1 || S==0);

		X = V1 * sqrt(-2 * log(S) / S);
	} else
		X = V2 * sqrt(-2 * log(S) / S);

	phase = 1 - phase;
	return X;
}

void BetaNoise::fastfourierinverse(int exp2, std::vector<struct Complex> coeffs)
{
	int num = coeffs.size();
	int lognum = (int) (log((double) num)/log(2.0));

	// Bit Reversal
	// Reference:
	//	http://www.codeproject.com/Articles/9388/How-to-implement-the-FFT-algorithm
	int j=0;
	for(int i=0;i<num;i+=2)
	{
		if(j>i)
		{
			swap(&coeffs[i],&coeffs[j]);
		}
		int m = num/4;
		for(;m>=2 && j>m;j-=m, m/=2);
		j+=m;
	}

	// Danielson Lanzcos
	// Reference:
	//	(1)
	//		Paul Bourke
	//		http://paulbourke.net
	//	(2)
	//		http://www.codeproject.com/Articles/9388/How-to-implement-the-FFT-algorithm

	double c1 = -1.0;
	double c2 = 0.0;
	int l2 = 1;
	for (int l=0;l<lognum;l++)
	{
      int l1 = l2;
      l2 <<= 1;
      double u1 = 1.0;
      double u2 = 0.0;
      for (j=0;j<l1;j++) {
         for (int i=j;i<num;i+=l2) {
            int i1 = i + l1;
			double t1 = u1 * coeffs[i1].real - u2 * coeffs[i1].imaginary;
			double t2 = u1 * coeffs[i1].imaginary + u2 * coeffs[i1].real;
			coeffs[i1].real = coeffs[i].real - t1;
			coeffs[i1].imaginary = coeffs[i].imaginary - t2;
			coeffs[i].real += t1;
			coeffs[i].imaginary += t2;
         }
         double z =  u1 * c1 - u2 * c2;
         u2 = u1 * c2 + u2 * c1;
         u1 = z;
      }
      c2 = sqrt((1.0 - c1) / 2.0);
      c1 = sqrt((1.0 + c1) / 2.0);
   }
}

std::vector<Complex> BetaNoise::noise(int exp2, double beta)
{
	int num = (int) pow(2.0,exp2);
	std::vector<Complex> noisearr(num,Complex());

	// Frequency Domain Coefficients using Gaussian Random Numbers
	for(int i=1; i<=num/2; i++)
	{
		double magnitude = pow(i+1.0,-beta/2.0) * gauss();
		double phase = M_PI*2.0*gauss();
		Complex coeff;
		coeff.real = magnitude*cos(phase);
		coeff.imaginary = magnitude*sin(phase);
		Complex coeffrev;
		coeffrev.real = coeff.real;
		coeffrev.imaginary = -coeff.imaginary;
		noisearr[i]=coeff;
		noisearr[num-i]=coeffrev;
	}
	noisearr[num/2].imaginary = 0;

	// Inverse Fourier Transform using Fast Fourier
	fastfourierinverse(exp2, noisearr);

	return noisearr;
}

double randomnoise(double min, double max)
{
    return min + ((double) rand() / RAND_MAX) * (max-min);
}

static bool abs_compare(double a, double b)
{
    return (fabs(a) < fabs(b));
}

std::vector<double> BetaNoise::librarynoise(int exp2, double beta)
{
	// ONE-BY-F^(BETA) IMPLEMENTATION

	std::vector<double> onefbeta;

	int seed = randomnoise(123456780,123456790);
	//int seed = 123456789;
	int num = pow(2.0,exp2);
	double * x = f_alpha(num, 1.0, beta, &seed); // f_alpha(no. of values, variance, beta, rand-seed)
	for(int i =0; i<num; i++)
		onefbeta.push_back(x[i]);

	onefbeta[0] = 0;		// Hack to establish continuity between time-periods
	onefbeta[num-1] = 0;	// Hack to establish continuity between time-periods

	double result = fabs(* std::max_element(onefbeta.begin(), onefbeta.end(), abs_compare));
	if(result!=0)
		for(int i = 0; i<num; i++)
		{
			onefbeta[i] /= result;	// Normalize
		}

	// Hack to establish smooth continuity
	for(int i = num-2; i>0; i--)
		if(abs(onefbeta[i]) < 0.1)
		{
			onefbeta[i+1] = 0;
			onefbeta.resize(i+2,0);
			num = i+2;
			break;
		}

	return onefbeta;
}

//int main(int argc, char *argv[])
//{
//	std::vector<Complex> test = BetaNoise::noise(7, 2.5);
//	for(int i=0;i<test.size(); i++)
//	{
//		std::cout << "[" << i << "]\t" << test[i].real <<std::endl;
//	}
//	int n;
//	std::cin >> n;
//
//	return 1;
//}


