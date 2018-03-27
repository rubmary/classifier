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

/**
 * Class RosenbrockFuction:
 * Subclase hija de la clase Function, rescribe
 * las funciones val, d y h, a sus valores correspondientes
 * @a - double, parametro de la funcion de Rosenbrock
 * @b - double, parametro de la funcion de Rosenbrock
 */
class RosenbrockFunction : public Function{
    double a, b;
public:

    /**
     * RosenbrockFunction - Constructor de la clase
     * Descripcion:
     *  Crea la funcion con los parametros dados, 
     *  iguala la dimension a 2 e inicia los contadores
     */
    RosenbrockFunction(double a, double b) : a(a), b(b)
    {
        N = 2;
        total = 0;
        total_d = 0;
        total_h = 0;
    }

    /**
     * val - valor de la funcion
     * Descripcion:
     *  sobrescribe la funcion val de la clase padre
     *  returna la funcion de Rosenbrock
     */
    virtual double val(vector x)
    {
        total++;
        return sq(a - x[0]) + b*sq( x[1] - sq(x[0]) );
    }

    /**
     * d - derivada
     * Descripcion:
     *  sobrescribe la funcion d de la clase padre
     *  retorna la derivada de la funcion de Rosenbrock
     *  en un punto dado
     */
    virtual vector d(vector x)
    {
        total_d++;
        double dx, dy;
        dx = -2*a + 4*b*cub(x[0]) - 4*b*x[0]*x[1] + 2*x[0];
        dy = 2*b*(x[1] - sq(x[0]));
        return vector ({dx, dy});
    }

    /**
     * h - hessiano
     * Descripcion:
     *  sobrescribe la funcion h de la clase padre
     *  retorna  el hessiado de la funcion de Rosenbrock
     *  para un punto dado
     */
    virtual matrix h(vector x) {
        total_h++;
        matrix H = matrix(2, vector(2));
        H[0][0] = 12*b*sq(x[0]) - 4*b*x[1] + 2;
        H[0][1] = -4*b*x[0];
        H[1][0] = -4*b*x[0];
        H[1][1] = 2*b;
        return H;
    }

    /**
     * inverse - inversa
     * Description:
     *  sobrescribe la funcion inverse de la clase padre
     *  retorna  la inversa de una matriz 2x2
     */
    virtual matrix inverse(matrix &A) {
        total_i++;
        matrix IA  = matrix(2, vector(2));
        double D =  A[0][0]*A[1][1] - A[0][1]*A[1][0];
        IA[0][0] =  A[1][1]/D;
        IA[0][1] = -A[1][0]/D;
        IA[1][0] = -A[0][1]/D;
        IA[1][1] =  A[0][0]/D;
        return IA;
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

    void precalculations(vector w) {
        V = X*w;
        for (int k = 0; k < M; k++)
            y[k] = phi(V[k]);
        for (int k = 0; k < M; k++)
            PHI[k] = (y[k] - D[k])*dphi(V[k]);       
    }

    virtual double val(vector w) {
        precalculations(w);
        double e = 0;
        for (int k = 0; k< M; k++)
            e += sq(y[k]-D[k]);
        return e/2;
    }
    
    virtual vector d(vector w) {
        matrix Xt = t(X);
        return Xt*PHI;
    }
};
