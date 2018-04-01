#include "LinearSearch.cpp"
#include <iostream>
#include <fstream>
#include <ctime>
#include <climits>
#include <cstdlib>
#include <algorithm>
#define EPS 1e-3

int N;
void make_data(matrix &X, vector &D){
	vector x(4, 1);
	int M;
	std::cin >> M;
	D = vector(M);
	for (int i = 0; i < M; i++) {
		std::cin >> x[1] >> x[2] >> x[3];
		std::cin >> D[i];
		X.push_back(x);
	}

	double miuX = 0, varX = 0;
	double miuY = 0, varY = 0;
	double miuZ = 0, varZ = 0;

	for (int i = 0; i < M; i++) {
		miuX += X[i][1];
		miuY += X[i][2];
		miuZ += X[i][3];
	}
	miuX /= M;
	miuY /= M;
	miuZ /=M;

	for (int i = 0; i < M; i++) {
		varX += sq(X[i][1] - miuX);
		varY += sq(X[i][2] - miuY);
		varZ += sq(X[i][3] - miuZ);
	}

	varX /= (M-1);
	varY /= (M-1);
	varZ /= (M-1);

	for (int i = 0; i < M; i++) {
		X[i][1] = (X[i][1] - miuX)/varX;
		X[i][2] = (X[i][2] - miuY)/varY;
		X[i][3] = (X[i][3] - miuZ)/varZ;
	}
}

void make_graphics(vector w, matrix X, vector D){
	double D1 = w[0], A = w[1], B = w[2], C = w[3];
	std::ofstream plane("plane.txt"), c1("c1.txt"), c2("c2.txt");
	int N = X.size();
	
	double minX = 1e100, maxX = -1e100, minY = 1e100, maxY = 1e-100, minZ = 1e100, maxZ = 1e-100;

	for (int i = 0; i < N; i++){
		minX = std::min(minX, X[i][1]);
		maxX = std::max(maxX, X[i][1]);
		minY = std::min(minY, X[i][2]);
		maxY = std::max(maxY, X[i][2]);
		minZ = std::min(minZ, X[i][3]);
		maxZ = std::max(maxZ, X[i][3]);
	}

	minX -= 0.1;
	minY -= 0.1;
	minZ -= 0.1;
	maxX += 0.1;
	maxY += 0.1;
	maxZ += 0.1;

	int total = 50;
	for (int i = 1; i <= total; i++) {
		for (int j = 1; j <= total; j++) {
			double x0 = i*(maxX - minX)/total + minX;
			double y0 = j*(maxY - minY)/total + minY;
			double z0 = -(D1 + A*x0 + B*y0)/C;
			if (minZ <= z0 && z0 <=maxZ)
				plane << x0 << ' ' << y0 << ' ' << z0 << std::endl;
		}
	}

	for (int j = 1; j <= total; j++) {
		for (int i = 1; i <= total; i++) {
			double x0 = i*(maxX - minX)/total + minX;
			double y0 = j*(maxY - minY)/total + minY;
			double z0 = -(D1 + A*x0 + B*y0)/C;
			if (minZ <= z0 && z0 <=maxZ)
				plane << x0 << ' ' << y0 << ' ' << z0 << std::endl;
		}
	}

	for (int i = 0; i < N; i++) {
		if(D[i] == 1)
			c1 << X[i][1] << ' ' << X[i][2] << ' ' << X[i][3] << std::endl;
		else
			c2 << X[i][1] << ' ' << X[i][2] << ' ' << X[i][3] << std::endl;
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
	matrix X, I;
	vector D;
	make_data(X, D);

	ET *f = new ET(4, X, D, 0.5);
	vector  w0(4, 0);
	srand(time(NULL));
	
	int k;
	clock_t t1, t2;
	double ro = 0.5;

	I=identity(4);

	Bfgs *bfgs = new Bfgs(10, 1e-4, ro, I); 
	bfgs -> eps=EPS;
	bfgs -> MAX_IT = 10000;
	bfgs -> f = f;

	t1 = clock();
	k = bfgs -> linear_search(w0);
	t2 = clock();
	vector w = bfgs -> x;
	std::vector<int> r = results(w, X, D);
	make_graphics(w, X, D);
	std::cout << "Valor de rho: " << ro << std::endl;
	std::cout << "Total de llamadas a: "<< std::endl;
	std::cout << "\tFuncion:   " << f -> total << std::endl;
	std::cout << "\tDerivadas: " << f -> total_d << std::endl;
	std::cout << "\tHessiano:  " << f -> total_h << std::endl;
	std::cout << "\tInversa:   " << f -> total_i << std::endl;
	std::cout << "Norma del gradiente:  " << abs(bfgs -> gx) << std::endl; 
	std::cout << "Total de iteraciones: " << k << std::endl;
	std::cout << "Resultados: " << std::endl;
	std::cout << "\tDatos bien clasificados: " << r[0] << std::endl;
	std::cout << "\tDatos mal  clasificados: " << r[1] << std::endl;
	std::cout << "\tPrecision:               " << 100.00*r[0]/(r[0]+r[1]) << "%" << std::endl;
	std::cout << "Tiempo utilizado: " << (double) (t2 - t1)/CLOCKS_PER_SEC << std::endl;
}