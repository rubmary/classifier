#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;
unordered_map<float, float> datos_1;
unordered_map<float, float> datos_2;

void recta(float m, float x, float y, float apertura, int cantidad_datos){
	int corte_eje_x = (m*x-y)/m;
	int punto_x_1=corte_eje_x-apertura;
	int punto_x_2=corte_eje_x+apertura;
	bool aux=true;

	for (int i=0; i<cantidad_datos; i++){
		if (aux)
			datos_1.insert(make_pair<float,float>(i,m*(i-punto_x_1)+(rand()%100)));
		else
			datos_2.insert(make_pair<float,float>(i,m*(i-punto_x_2)-(rand()%100)));

		aux=!aux;
	}

	for ( auto it = datos_1.begin(); it != datos_1.end(); it++){
		cout<<it->first<<"\t"<<it->second<<endl;
	}

	cout<<endl<<endl;

	for ( auto it = datos_2.begin(); it != datos_2.end(); it++){
		cout<<it->first<<"\t"<<it->second<<endl;
	}

}

int main (int argc, const char **argv){
	recta(3, 1,1,100,1000);

	return 0;
}
