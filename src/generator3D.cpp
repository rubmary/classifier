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

int main() {
	double A, B, C, D;
	srand(time(NULL));
	A = get_rand(-3, 3);
	B = get_rand(-3, 3);
	C = get_rand(-3, 3);
	D = get_rand(-3, 3);

	int N;
	double distance, error;
	cin >> N >> distance >> error;
	cout << N << endl;

	int i = 0;
	while (i < N) {
		double X, Y, Z;
		X = get_rand(-10, 10);
		Y = get_rand(-10, 10);
		Z = get_rand(-10, 10);
	
		if (abs(A*X + B*Y + C*Z + D) < distance)
			continue;

		cout << X + get_rand(-error/2, error/2) << ' ' << Y+ get_rand(-error/2, error/2) << ' ' << Z+get_rand(-error/2, error/2) << ' ';
		if (A*X + B*Y + C*Z + D > 0)
			cout <<  1;
		else
			cout << -1;
		cout << endl;
		i++;
	}
}