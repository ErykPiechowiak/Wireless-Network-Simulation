#ifndef GENERATOR_H_
#define GENERATOR_H_
#include <math.h>
class Generator
{
public:
	Generator(int kernel);
	~Generator();
	double Rand(); //Draws numbers between <0,1>
	double Rand(int min, int max);
	double RndExp(double lambda);
	int RndZeroOne(double p);
	int get_kernel();
	void set_seed(int seed);
private:
	int kernel_;
	const double kM = 2147483647.0;
	static const int kA = 16807;
	static const int kQ = 127773;
	static const int kR = 2836;
};
#endif // !GENERATOR_H_