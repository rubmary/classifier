#include <iostream>
#include <climits>
#include <cstdlib>
#include <ctime>
#include <algorithm>
using namespace std;

double get_rand(double a, double b){
	double r = (double) std::rand()/INT_MAX;
	return r*(b - a) + a;
}

vector <double> rand_vector(double r) {
	double x = get_rand(-1, 1), y = get_rand(-1, 1), z = get_rand(-1, 1);
	double norm = sqrt(x*x + y*y + z*z);
	return vector <double>({x*r/norm, y*r/norm, z*r/norm});
}

int main() {

	srand(time(NULL));
	int N;
	double distance, var;
	cin >> N >> distance >> var;
	default_random_engine generator;
 	normal_distribution<double> distribution(0, var);
	vector <double> c1, c2;
	c1 = rand_vector(1);
	c2 = rand_vector(distance);
	
	for (int i = 0; i < 3; i++)
		c2[i] += c1[i];

	cout << N << endl;
	
	for (int i = 0; i < N; i++){
		double X1, Y1, Z1, X2, Y2, Z2, r1, r2;
		
		vector <double> d1, d2;
		r1 = distribution(generator);
		r2 = distribution(generator);
		while (r1 > 1.5*var)
			r1 = distribution(generator);
		while (r2 > 1.5*var)
			r2 = distribution(generator);

		d1 = rand_vector(r1);
		d2 = rand_vector(r2);

		X1 = c1[0] + d1[0];
		Y1 = c1[1] + d1[1];
		Z1 = c1[2] + d1[2];

		X2 = c2[0] + d2[0];
		Y2 = c2[1] + d2[1];
		Z2 = c2[2] + d2[2];

		cout << X1 << ' ' << Y1 << ' ' << Z1 << ' ' <<  1 << endl;
		cout << X2 << ' ' << Y2 << ' ' << Z2 << ' ' << -1 << endl;
	}
}