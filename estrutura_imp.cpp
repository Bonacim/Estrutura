
#include "estrutura.h"
#include "estrutura_sdl.h"

Estrutura::Estrutura(int t_inicial, int t_largura, int height, int lenght)
{
	Inicializa(t_inicial, t_largura, height, lenght);
}

Estrutura::Estrutura()
{
	Inicializa();
}

Estrutura::~Estrutura()
{

}

void Estrutura::Inicializa(int t_inicial, int t_largura, int height, int lenght)
{
	h = height;
	l = lenght;
	Fmed.y=G;
	if (!K1)
	{
		printf( "Digite a constante elastica K1: ");
		scanf("%lf",&k1);
		printf("Digite a constante elastica K2: ");
		scanf("%lf",&k2);
		printf("Digite o parametro passo: ");
		scanf("%lf",&step);
		printf("\n");
	}
	else
	{
		k1 = K1;
		k2 = K2;
		//step = TIMESTEP*TIMESTEP;
		step = STEP;
	}
}

bool Estrutura::read_inputs() 
{
	int x, y, i;
	h = 0;
	l = 1;
	n = 1;

	FILE *in;
	in = fopen("densidades.txt", "r");

	if (in == NULL)
	{
		printf("Cannot open file.\n");
		//fclose(in);
		return 1;
	}

	while ((i = fgetc(in)) != EOF)
	{
		if (i == ' ') 
		{
			l++;
			n++;
		}
		else if (i == '\n') 
		{
			if (h >= (int)largura.capacity()) largura.resize(h + SIZE_INCR);
			largura[h] = l;
			l = 1;
			n++;
			h++;
		}
	}
	largura[h] = l;
	h++;
	rewind(in);

	printf("Elementos: %d\t Altura: %d\t Largura base: %d\n",n,h,largura[0]);

	densidade.resize(h);
	for (i = 0; i < h; i++)
		densidade[i].resize(largura[i]);

	for (y = 0; y < h; y++) 
	{
		for (x = 0; x < largura[y]; x++) 
		{
			fscanf(in, "%lf", &(densidade[y][x]));
			//printf("%lf ",densidade[y][x]);
		}
		//printf("\n");
	}

	fclose(in);
	return 0;
}

void Estrutura::prepare_environment() 
{
	int i, j;

	//Posição
	pos.resize(h);
	for (i = 0; i < h; i++)
		pos[i].resize(largura[i]);


	for (i = 0; i < h; i++)
	{
		for (j = 0; j < largura[i]; j++)
		{
			pos[i][j].x = j * ESCALA;
			pos[i][j].y = i * ESCALA;
		}
	}

	//Aceleração
	acel.resize(h);
	for (i = 0; i < h; i++)
		acel[i].resize(largura[i]);


	for (i = 0; i < h; i++)
	{
		for (j = 0; j < largura[i]; j++)
		{
			acel[i][j].x = 0;
			acel[i][j].y = 0;
		}
	}

	//Tensões
	Txx.resize(h);
	for (i = 0; i < h; i++)
		Txx[i].resize(largura[i]);

	Txy.resize(h);
	for (i = 0; i < h; i++)
		Txy[i].resize(largura[i]);

	Tyy.resize(h);
	for (i = 0; i < h; i++)
		Tyy[i].resize(largura[i]);

	//Forças axiais
	Fxp.resize(h);
	for (i = 0; i < h; i++)
		Fxp[i].resize(largura[i]);

	Fxn.resize(h);
	for (i = 0; i < h; i++)
		Fxn[i].resize(largura[i]);

	Fyp.resize(h);
	for (i = 0; i < h; i++)
		Fyp[i].resize(largura[i]);

	Fyn.resize(h);
	for (i = 0; i < h; i++)
		Fyn[i].resize(largura[i]);

	Fres.resize(h);
	for (i = 0; i < h; i++)
		Fres[i].resize(largura[i]);
}

bool Estrutura::converged() 
{
    double Fmed_norma, kmed;
    kmed = (k1+k2)/2;
    Fmed_norma=sqrt(Fmed.x*Fmed.x + Fmed.y*Fmed.y);
    //step=Fmed_norma/kmed/10;
    if (Fmed_norma>F_ZERO) return false;
    return true;
}

void Estrutura::calculate_forces() 
{
	int i, j;

	for (i = 0; i < h; i++)
		{
		for (j = 0; j < largura[i]; j++)
		{
			OlhaArredor(i, j);
		}
	}
}

void Estrutura::calculate_dispacements() 
{
	int i, j;
	for (i = 0; i < h; i++)
	{
		for (j = 0; j < largura[i]; j++)
		{
			pos[i][j].x += acel[i][j].x * step;
			pos[i][j].y += acel[i][j].y * step;
		}
	}
}

void Estrutura::print_failure() 
{
	printf("Falhou em calcular as forcas\n");
}

void Estrutura::OlhaArredor(int y, int x)
{
	Coordenada ri, rj, delta_r, F;
	double l, l0, theta, Fg;
	bool is_x;
	int delta;

	l0 = ESCALA;
	ri.x = pos[y][x].x;
	ri.y = pos[y][x].y;

	//Eixo X positivo
	if (x + 1 < largura[y])
	{
		is_x=true;
		delta=1;
		F=SetForce(y,x,is_x,ri,l0,delta);

		Fxp[y][x].x = F.x;
		Fxp[y][x].y = F.y;
	}
	else //Ponto na lateral direita
	{
		Fxp[y][x].x = 0;
		Fxp[y][x].y = 0;
	}

	//Eixo X negativo
	if (x > 0)
	{
		is_x=true;
		delta=-1;
		F=SetForce(y,x,is_x,ri,l0,delta);

		Fxn[y][x].x = F.x;
		Fxn[y][x].y = F.y;

	}
	else //Encostado na parede
	{
		rj.x = -ESCALA;
		rj.y = y * ESCALA;

		delta_r.x = ri.x - rj.x;
		delta_r.y = ri.y - rj.y;

		l = sqrt(delta_r.x*delta_r.x + delta_r.y*delta_r.y);
		theta = atan2(delta_r.y, delta_r.x);

		Fxn[y][x].x = -k1 * (l - l0) * cos(theta);
		Fxn[y][x].y = -k1 * (l - l0) * sin(theta) - k2 * delta_r.y;
	}

	//Eixo Y positivo
	if (y + 1 < h)
	{
		is_x=false;
		delta=1;
		F=SetForce(y,x,is_x,ri,l0,delta);

		Fyp[y][x].x = F.x;
		Fyp[y][x].y = F.y;
	}
	else //Ponto no topo
	{
		Fyp[y][x].x = 0;
		Fyp[y][x].y = 0;
	}

	//Eixo Y negativo
	if (y > 0)
	{
		is_x=false;
		delta=-1;
		F=SetForce(y,x,is_x,ri,l0,delta);

		Fyn[y][x].x = F.x;
		Fyn[y][x].y = F.y;
	}
	else //Ponto na base
	{
		Fyn[y][x].x = 0;
		Fyn[y][x].y = 0;
	}

	Fg = densidade[y][x] * VOLUME * -G;
	Fres[y][x].x = Fxp[y][x].x + Fxn[y][x].x + Fyp[y][x].x + Fyn[y][x].x;
	Fres[y][x].y = Fxp[y][x].y + Fxn[y][x].y + Fyp[y][x].y + Fyn[y][x].y + Fg;

	acel[y][x].x = Fres[y][x].x / (densidade[y][x] * VOLUME);
	acel[y][x].y = Fres[y][x].y / (densidade[y][x] * VOLUME);

	Fmed.x += Fres[y][x].x/n;
	Fmed.y += Fres[y][x].y/n;
}

Coordenada Estrutura::SetForce (int y, int x, bool is_x, Coordenada ri, double l0, int delta)
{
	Coordenada rj, delta_r, Force;
	double l, theta;

	if (is_x)
	{
		rj.x = pos[y][x + delta].x;
		rj.y = pos[y][x + delta].y;

		delta_r.x = ri.x - rj.x;
		delta_r.y = ri.y - rj.y;

		l = sqrt(delta_r.x*delta_r.x + delta_r.y*delta_r.y);
		theta = atan2(delta_r.y, delta_r.x);


		Force.x = -k1 * (l - l0) * cos(theta);
		Force.y = -k1 * (l - l0) * sin(theta) - k2 * delta_r.y;
	}
	else
	{
		rj.x = pos[y + delta][x].x;
		rj.y = pos[y + delta][x].y;

		delta_r.x = ri.x - rj.x;
		delta_r.y = ri.y - rj.y;

		l = sqrt(delta_r.x*delta_r.x + delta_r.y*delta_r.y);
		theta = atan2(delta_r.y, delta_r.x);

		Force.x = -k1 * (l - l0) * cos(theta) - k2 * delta_r.x;
		Force.y = -k1 * (l - l0) * sin(theta);
	}
	return Force;
}

void Estrutura::SalvaT()
{
	int x, y;
	FILE *out;

	//Salva TXX
	out = fopen("Txx.txt", "wt");
	for (y = 0; y < h; y++)
	{
		for (x = 0; x < largura[y]; x++)
		{
			fprintf(out, "%.17lf ", Txx[y][x]);
		}
		fprintf(out, "\n");
	}
	fclose(out);

	//Salva TXY
	out = fopen("Txy.txt", "wt");
	for (y = 0; y < h; y++)
	{
		for (x = 0; x < largura[y]; x++)
		{
			fprintf(out, "%.17lf ", Txy[y][x]);
		}
		fprintf(out, "\n");
	}
	fclose(out);

	//Salva TYY
	out = fopen("Tyy.txt", "wt");
	for (y = 0; y < h; y++)
	{
		for (x = 0; x < largura[y]; x++)
		{
			fprintf(out, "%.17lf ", Tyy[y][x]);
		}
		fprintf(out, "\n");
	}
	fclose(out);

}


void Estrutura::CalculaT()
{
	int x, y;
	double area;
	area = ESCALA * ESCALA;
	for (y = 0; y < h; y++)
	{
		for (x = 0; x < largura[y]; x++)
		{
			Txx[y][x] = (Fxp[y][x].x + Fxn[y][x].x) / area;

			Txy[y][x] = (Fxp[y][x].y + Fxn[y][x].y) / area;

			Tyy[y][x] = (Fyp[y][x].y + Fyn[y][x].y) / area;
		}
	}
}

bool Estrutura::Executar()
{
	if (read_inputs()) return 1;     // le os arquivos da estrutura, e arquivo de input
    prepare_environment();  // alocacao de memória, matrizes, inicializa o que precisar
    int iteration = 0;
    bool quit=false;
    create_window();
    while( !converged() ) {
        calculate_forces();
        calculate_dispacements();
        render_window(&quit,this);
        if (quit)
        {
        	printf("Janela fechada\n");
        	return 0;
        }
        iteration++;

		//printf("Pressione ENTER para continuar...\n");
        //getchar();

        if(iteration > max_iteration) 
        {
            	print_failure();
            	printf("%.17lf %.17lf\n",pos[0][0].x, pos[0][0].y);
            	printf("Pressione ENTER para continuar...\n");
        		getchar();
            	return 1;
        }
    }
    CalculaT();
    SalvaT();
    return 0;
}