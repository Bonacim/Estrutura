#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>

using namespace std;
const int max_iteration = 1000;
typedef vector < vector < double > > vec_dbl;
const double G = 9.81;
const double ESCALA = 1e-3;
const double VOLUME = ESCALA * ESCALA * ESCALA;
const int SIZE_INCR = 100;
const double K1 = 10e3;
const double K2 = 10e3;
const double STEP = 5e-11;
const double F_ZERO = 1e-12;

typedef struct
{
	double x, y;
} Coordenada;
typedef vector < vector <Coordenada> > vec_coord;

class Estrutura
{
private:
	vec_dbl densidade, Txx, Tyy, Txy;
	vector<int> largura;
	vec_coord pos, acel;
	int h, l, n;
	double k1,k2;
	double step;
	Coordenada Fmed;	
	vec_coord Fxp, Fxn, Fyp, Fyn, Fres;


public:
		Estrutura(int t_inicial, int t_largura, int height = 0, int lenght = 0);
		Estrutura();
		~Estrutura();

		void Inicializa(int t_inicial=1000, int t_largura=1000,  int height = 0, int lenght = 0);
		void SalvaT();
		void CalculaT();
		void OlhaArredor(int y, int x);
		bool Executar();

		Coordenada SetForce (int y, int x, bool is_x, Coordenada ri, double l0, int delta);



		bool read_inputs();
		void prepare_environment();
		bool converged();
		void calculate_forces();
		void calculate_dispacements();
		void render_window();
		void print_failure();
};