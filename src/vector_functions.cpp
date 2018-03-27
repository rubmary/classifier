#include <vector>
#include <cmath>
#define sq(X) (X)*(X)
#define cub(X) (X)*(X)*(X)
typedef std::vector<std::vector<double> > matrix;
typedef std::vector<double> vector;

matrix cross(vector A, vector B)
{
	int n = A.size(), m = B.size();
	matrix C = std::vector<vector>(n, vector(m, 0));
	for (int i = 0; i < n; i++) 
	{
		for (int j = 0; j < m; j++)
			C[i][j] = A[i]*B[j];
	}
	return C;
}
matrix operator + (matrix &A, matrix &B)
{
	int n = A.size(), m = A[0].size();
	matrix C(n, vector(m));
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
			C[i][j] = A[i][j] + B[i][j];
	}
	return C;
}
vector operator * (matrix &A, vector &B)
{
	int n = A.size(), m = B.size();
	vector C(n, 0);

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
			C[i] += A[i][j]*B[j];
	}

	return C;
}
vector operator + (vector A, vector B)
{
	int n = A.size();
	vector C(n);
	for (int i = 0; i < n; i++)
		C[i] = A[i] + B[i];
	return C;
}
vector operator * (double eta, vector A)
{
	int n = A.size();
	vector ans(n);
	for (int i = 0; i < n; i++)
		ans[i] = eta*A[i];
	return ans;
}
vector operator - (vector A, vector B)
{
	int n = A.size();
	vector C(n);
	for (int i = 0; i < n; i++)
		C[i] = A[i] - B[i];
	return C;
}
double operator * (vector A, vector B)
{
	int n = A.size();
	double ans = 0;
	for (int i = 0; i < n; i++)
		ans += A[i]*B[i];
	return ans;
}

matrix operator * (matrix A, matrix B)
{
	int n = A.size(), m = B.size(), t = B[0].size();
	matrix C(n, vector(t));
	for (int i = 0; i < n; i++){
		for (int j = 0; j < t; j++) {
			C[i][j] = 0;
			for (int k = 0; k < m; k++)
				C[i][j] += A[i][k]*B[k][j];
		}
	}
	return C;
}

matrix t(matrix A) {
	int n = A.size(), m = A[0].size();
	matrix B(m, vector(n));
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			B[j][i] = A[i][j];
	return B;
}
double abs(vector &x) { return std::sqrt(x*x); }
