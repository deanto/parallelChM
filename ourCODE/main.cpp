#include <math.h>
#include <fstream>
#include "timer.h"

using namespace std;

// Линейный конгруэнтный генератор
int x;

int a=845,c=2625,m=8192; // из презентации значения
// int a=845,c=2625,m=1024;

int GET_rund ()
{
	x=(a*x+c)%m;  // даст число от 0 до m=8192
	return x;
}

void SET_x0_rund (int x0)
{
	x=x0;
}

double rundforBoxMuller()
{
	// выдает равномерно распределенную случайную величину на отрезке [-1,1]
	  double t= ((double)(GET_rund()-m/2))/(m/2);

	  return t;
}
// _______________________________Линейный конгруэнтный генератор



int main(int argc, char* argv[])
{
	
	/*if(argc != 4)
	{
		printf("wrong input format (program inputfile outputfile timefile)");
		getchar();
		return -1;
	}*/
	
	// чтение из файла

	int count;
	double mu, sigquad; //количество чисел, мю, сигма в квадрате
	
	ifstream F;
	F.open("n2500.txt",ios::in);
	F>>count>>mu>>sigquad;
	F.close();
	
	// ________________чтение из файла


	Timer timer;
	timer.reset();
    timer.start();

// используем  Преобразование Бокса — Мюллера


	double *sequence = new double[count+1];// создали массив

	double x,y,s,BM;

	SET_x0_rund(0);

	int i=0;
	while (i<count)
	{
		do 
		{
			x=rundforBoxMuller();
			y=rundforBoxMuller();
			s=x*x+y*y;
		} while (s >= 1 || s == 0);

		
		BM= sqrt(-2.0 * log(s) / s);

		sequence[i]=x*BM*sqrt(sigquad)+mu;
		sequence[i+1]=y*BM*sqrt(sigquad)+mu;

		i+=2;
	}

	timer.stop();
	double time_result = timer.getElapsed();

// посчитали время работы


	// теперь критерии
// пирсон

	int a=(int)(mu-3*sqrt(sigquad));
	int b=(int)(mu+3*sqrt(sigquad));
	int N = 11;
	int* frequency = new int[N];
	for(int i = 0; i < N; i++) frequency[i] = 0;

	double interval = (b - a) / (double)N;

	for(int i = 0; i < N; i++)
	{
		int tmp = (sequence[i] - a) / interval;
		if(tmp == N) tmp--;
		frequency[tmp]++;
	}

	double xi = 0;
	double f = (double)b / (double)N;

	for(int i = 0; i < N; i++)
	{
		double tmp = ((double)frequency[i] - f);
		xi += tmp * tmp;
	}

	xi /= f;
	// пирсона посчитали



// колмагоров
	



	double colmagorov=0;
	//колмагорова посчитали 

	

	/// запишем файлики и все

	ofstream T;
	T.open(argv[2]);
	T << xi << endl << colmagorov << endl;
	for(int i = 0; i < N; i++) T << sequence[i] << endl;
	T.close();

	ofstream O;	
	O.open(argv[3]);
	O << time_result;
	O.close();

	delete[] sequence;
	return 0;
}
