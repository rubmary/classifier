#include "vector_functions.cpp"
#include <iostream>

/**
 * Class Fuction:
 * clase abstracta para representar las funciones que
 * seran pasadas como parametro a la busqueda lineal
 * @N:          dimension del vector de entrada
 * @total:      numero de llamadas a la funcion
 * @total_d:    numero de llamadas a la derivada de la funcion
 * @total_h:    numero de llamadas al hessiando de la funcion
 * @total_i:    numero de veces que se calcula la inversa de una matriz
 */
class Function {
public:
    int N;
    int total;
    int total_d;
    int total_h;
    int total_i;
    /**
     * val - valor de la funcion evualuada en un punto
     * Parametros:
     *  x - vector en el que se evalua la funcion
     * Descripcion:
     *  retorna el valor de la funcion,
     *  por defecto es la funcion constate
     *  (igual a 1)
     */
    virtual double val(vector x){
        return 1;
    }

    /**
     * d - derivada
     * Parametros:
     *  x - vector, punto a evaluar la derivada
     * Descripcion:
     *  retorna el valor de la derivada en el 
     *  punto dado
     */
    virtual vector d(vector x) {
        return vector(0);
    }
    /**
     * h - hessiano
     * Parametros:
     *  x - vector, punto a evaluar el hessiano
     *  retorna el valor del hessiano en el 
     *  punto dado
     */
    virtual matrix h(vector x) {
        return matrix(0);
    }
    /**
     * inverse - inversa
     * Parametros:
     *  A - matriz de entrada
     * Descripcion
     *  retorna la inversa de la 
     *  matriz dada como parametro
     */
    virtual matrix inverse(matrix &A){
        return matrix(0);
    }
};

class ET : public Function{
public:    
    int M;
    matrix X;
    vector V, PHI, y, D;
    double rho;

    ET(int N0, matrix X, vector D, double rho = 1) :
        X(X), D(D), rho(rho) {
        N = N0;
        M = X.size();
        y = vector(M);
        PHI = vector(M);
    }
    double phi(double x) {
        x *= rho;
        return (exp(x) - exp(-x))/(exp(x) + exp(-x));
    }
    double dphi(double x) {
        x *= rho;
        return rho*sq(2/(exp(x) + exp(-x)));
    }
    double ddphi(double x) {
        return -2*rho*phi(x)*dphi(x);
    }
    void precalculations(vector w) {
        V = X*w;
        for (int k = 0; k < M; k++)
            y[k] = phi(V[k]);
        for (int k = 0; k < M; k++)
            PHI[k] = (y[k] - D[k])*dphi(V[k]);       
    }
    virtual double val(vector w) {
        total++;
        precalculations(w);
        double e = 0;
        for (int k = 0; k< M; k++)
            e += sq(y[k]-D[k]);
        return e/2;
    }
    virtual vector d(vector w) {
        total_d++;
        matrix Xt = t(X);
        return Xt*PHI;
    }
    virtual matrix h(vector w) {
        total_h++;
        matrix H(N, vector(N));
        for (int i = 0; i < N; i++) {
            for (int j = i; j < N; j++){
                H[i][j] = 0;
                for (int k = 0; k < M; k++)
                    H[i][j] += X[k][i]*X[k][j]*(sq(dphi(V[k])) + ddphi(V[k])*(y[k]-D[k]));
                H[j][i] = H[i][j];
            }
        }
        return H;
    }
    double det_adj(int i, int j, matrix &M) {
        matrix A(2, vector(2));
        int i2 = 0, j2 = 0;
        for (int i1 = 0; i1 < N; i1++) {
            j2 = 0;
            if (i1 == i) continue;
            for (int j1 = 0; j1 < N; j1++){
                if (j1 == j) continue;
                A[i2][j2] = M[i1][j1];
                j2++;
            }
            i2++;
        }
        return A[0][0]*A[1][1]-A[1][0]*A[0][1];
    }
    virtual matrix inverse(matrix &M) {
        total_i++;
        matrix X = matrix(N, vector(N));
        double det =    M[0][0]*(M[1][1]*M[2][2] - M[1][2]*M[2][1])
                      - M[0][1]*(M[1][0]*M[2][2] - M[1][2]*M[2][0])
                      + M[0][2]*(M[1][0]*M[2][1] - M[1][1]*M[2][0]);
        matrix Mt = t(M);
        for (int i = 0; i < N; i++){
            for (int j = 0; j < N; j++){
                X[i][j] = det_adj(i, j, Mt)*(((i+j)&1) ? -1 : 1);
                X[i][j] /= det;
            }
        }
        return X;
    }
};
