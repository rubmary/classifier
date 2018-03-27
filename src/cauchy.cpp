#include "LinearSearch.cpp"
#include <iostream>
#include <fstream>
#include <ctime>
#include <climits>
#include <cstdlib>
#define EPS 1e-4


void make_data(matrix &X, vector &D){
	vector x(3, 1);
	int M;
	std::cin >> M;
	D = vector(M);
	for (int i = 0; i < M; i++) {
		std::cin >> x[1] >> x[2] >> D[i];
		X.push_back(x);
	}

	double miuX = 0, varX = 0, miuY = 0, varY = 0;
	for (int i = 0; i < M; i++) {
		miuX += X[i][1];
		miuY += X[i][2];
	}
	miuX /= M;
	miuY /= M;
	for (int i = 0; i < M; i++) {
		varX += sq(X[i][1] - miuX);
		varY += sq(X[i][2] - miuY);
	}
	varX /= (M-1);
	varY /= (M-1);
	for (int i = 0; i < M; i++) {
		X[i][1] = (X[i][1] - miuX)/varX;
		X[i][2] = (X[i][2] - miuY)/varY;
	}
}

void make_graphics(vector w, matrix X, vector D){
	double C = w[0], B = w[2], A = w[1];
	std::ofstream line("line.txt"), c1("c1.txt"), c2("c2.txt");
	int N = X.size();
	for (int i = 0; i < N; i++)
		line << X[i][1] << ' ' << -(C + A*X[i][1])/B << std::endl;
	for (int i = 0; i < N; i++) {
		if(D[i] == 1)
			c1 << X[i][1] << ' ' << X[i][2] << std::endl;
		else
			c2 << X[i][1] << ' ' << X[i][2] << std::endl;
	}
}

std::vector <int> results(vector w, matrix X, vector D) {
	int correct = 0, wrong = 0;
	int M = X.size();
	for (int i = 0; i < M; i++){
		double cross = w*X[i];
		if ((cross > 0 && D[i] > 0) || (cross < 0 && D[i] < 0))
			correct++;
		else
			wrong++;
	}
	return std::vector<int>({correct, wrong});
}

double get_rand(double a, double b){
	double r = (double) std::rand()/INT_MAX;
	return r*(b - a) - a;
}

int main() {
	matrix X;
	vector D;
	make_data(X, D);

	ET *f = new ET(3, X, D, 0.5);
	vector  w0(3, 2);
	srand(time(NULL));
	for (int i = 0; i < w0.size(); i++)
		w0[i] = get_rand(-1, 1);
	
	int k;
	clock_t t1, t2;
	double ro = 0.3;
	Cauchy *cauchy = new Cauchy(10, 1e-4, ro);
	cauchy -> eps = EPS;
	cauchy -> MAX_IT = 100000;
	cauchy -> f = f;
	
	t1 = clock();
	k = cauchy -> linear_search(w0);
	t2 = clock();

	vector w = cauchy -> x;
	std::vector<int> r = results(w, X, D);
	make_graphics(w, X, D);

	std::cout << "Valor de rho: " << ro << std::endl;
	std::cout << "Total de llamadas a: "<< std::endl;
	std::cout << "\tFuncion:   " << f -> total << std::endl;
	std::cout << "\tDerivadas: " << f -> total_d << std::endl;
	std::cout << "\tHessiano:  " << f -> total_h << std::endl;
	std::cout << "\tInversa:   " << f -> total_i << std::endl;
	std::cout << "Norma del gradiente:  " << abs(cauchy -> gx) << std::endl; 
	std::cout << "Total de iteraciones: " << k << std::endl;
	std::cout << "Resultados: " << std::endl;
	std::cout << "\tDatos bien clasificados: " << r[0] << std::endl;
	std::cout << "\tDatos mal  clasificados: " << r[1] << std::endl;
	std::cout << "\tPrecision:               " << 100.00*r[0]/(r[0]+r[1]) << "%" << std::endl;
	std::cout << "Tiempo utilizado: " << (double) (t2 - t1)/CLOCKS_PER_SEC << std::endl;
}