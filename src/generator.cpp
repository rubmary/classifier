#include <iostream>
#include <climits>
#include <cstdlib>
#include <ctime>
using namespace std;

double get_rand(double a, double b){
	double r = (double) std::rand()/INT_MAX;
	return r*(b - a) + a;
}

int main() {
	double A, B, C;
	srand(time(NULL));
	A = get_rand(-3, 3);
	B = get_rand(-3, 3);
	C = get_rand(-3, 3);
	int N;
	cin >> N;
	cout << N << endl;
	for (int i = 0; i < N; i++) {
		double X, Y;
		X = get_rand(-10, 10);
		Y = get_rand(-10, 10);
		cout << X << ' ' << Y << ' ';
		if (A*X + B*Y + C > 0)
			cout << 1;
		else
			cout << -1;
		cout << endl;
	}
}