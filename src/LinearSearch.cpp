#include "Function.cpp"
#include <climits>
#include <cstdlib>
#include <ctime>

/**
 * Class LinearSearch:
 * clase abstracta para representar las diferentes
 * busquedas lineales
 * @eps:    precision utilizada como condicion de parada
 * @MAX_IT: numero maximo de iteraciones permitidas
 * @f:      funcion utilizada para optimizar
 * @x:      valor del punto en cada iteracion
 * @gx:     valor del gradiente en cada iteracion
 * @fx:     valor de la funcion en cada iteracion
 */
class LinearSearch {
public:
    double eps;
    int MAX_IT;
    Function *f;
    vector x;
    vector gx;
    double fx;
    
    /**
     * alpha - longitud del paso
     * Parametros:
     *  @d - vector de la direccion de busqueda
     * Descripcion:
     *  Longitud del paso en el algoritmo iterativo,
     *  su valor por defecto es 1
     */
    virtual double alpha(vector &d)
    {
        return 1;
    }
    
    /**
     * dir - direccion de busqueda
     * Descripcion:
     *  Retorna la direccion de descenso
     *  del algoritmo, por defecto es la
     *  direccion opuesta al gradiente
     */
    virtual vector dir()
    {
        return -1*gx;
    }

    /**
     * linear_search
     * Parametros:
     *  @x0 - vector inicial
     * Descripcion:
     *  Algorimto de busqueda lineal,
     *  converge a la solucion que es
     *  almacenada en la variable x
     */
    int linear_search(vector x0)
    {
        vector d;
        x = x0;
        int k = 0;
        while (true) {
            k++;
            fx = f -> val(x);
            gx = f -> d(x);
            if (abs(gx) < eps || k > MAX_IT)
                break;
            d = dir();
            x = x + alpha(d)*d;
        }
        return k-1;
    }
};

/**
 * Class Cauchy
 * Subclase hija de LinearSearch
 * @al0:    paso inicial en el algoritmo de backtraking
 * gamma:   parametro del algoritmo de bactracking
 * @ro:     parametro del algoritmo de backtraking
 * @sig:    parametro que puede ser randomizado para
 *          variaciones de Cauchy, por defecto es igual a 1
 */
class Cauchy : public LinearSearch
{
public:
    double al0, gamma, ro, sig;

    /**
     * Cauchy- Constructor de la clase
     * Descripcion:
     *  Establece los parametros para el metodo de
     *  cauchy
     */
    Cauchy(double al0, double gamma, double ro, double sig = 1) :
        al0(al0), gamma(gamma), ro(ro), sig(sig) { }
    
    /**
     * Sigma - 
     * Descripcion:
     *  retorna el valor del parametro sigma
     */
    virtual double sigma()
    {
        return sig;
    }
    
    /**
     * alpha - longitud del paso 
     * Descripcion:
     *  sobrescribe la funcion de la clase padre
     *  utiliza el algoritmo de backtraking para
     *  para calcular la longitud del paso
     */
    virtual double alpha(vector &d)
    {
        double C = gamma*(gx*d), a = al0;
        while(f -> val(x + a*d) > fx + a*C){
            a = ro*a*sigma();
        }
        return a;
    }
};

/**
 * Class Cauchy
 * Subclase hija de Cauchy
 */
class Cauchy2 : public Cauchy {
public:

    /**
     * Cauchy2- Constructor de la clase
     * Descripcion:
     *  Establece los parametros para el metodo de
     *  cauchy e inicializa la semilla para generar
     *  los numeros aleatorios
     */
    Cauchy2(double al0, double gamma, double ro = 0.5, double sig = 1) :
        Cauchy(al0, gamma, ro, sig) {
            srand(time(NULL));
    }

    /**
     * sigma
     * Descripcion:
     *  sobrescribe la funcion de la clase padre
     *  utiliza el algoritmo de backtraking para
     *  para calcular la longitud del paso
     */
    virtual double sigma()
    {
        return (double) std::rand()/INT_MAX;
    }
};


/**
 * Class Newton
 * Subclase hija de Linear Search
 */
class Newton : public LinearSearch
{
    /**
     * dir - direccion de busqueda
     * Descripcion:
     *  sobrescribe la funcion de la clase padre
     *  calcular la direccion de busqueda
     */
    virtual vector dir() {
        matrix H = f -> h(x);
        H = f -> inverse(H);
        return -1*(H*gx);
    }
};

class Bfgs : public Cauchy
{
public:
    matrix Hk, Hk_inv;
    vector x_prev, s, y;

    Bfgs(double al0, double gamma, double ro, matrix H, double sig = 1) :
        Cauchy(al0, gamma, ro, sig) {
            Hk=H;
    }

    virtual vector dir(){
        return (-1*Hk_inv)*gx;
    }

    int linear_search(vector x0)
    {
        int n=Hk.size();
        Hk_inv=Hk;
        vector d;
        x = x0;
        int k = 0;
        while (true) {
            k++;
            fx = f -> val(x);
            gx = f -> d(x);
            if (abs(gx) < eps || k > MAX_IT)
                break;
            d = dir();
            x_prev = x;
            x = x + alpha(d)*d;
            s = x - x_prev;
            y = (f->d(x)) - gx;
            Hk_inv= ((identity(n) - ((1/(y*s))*producto_vectores(s,y)))*Hk_inv*(identity(n) - ((1/(y*s))*producto_vectores(y,s)))) + ((1/(y*s))*producto_vectores(s,s));
        }
        return k-1;
    }
};