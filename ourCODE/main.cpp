#include <math.h>
#include <fstream>
#include "timer.h"

using namespace std;

// Линейный конгруэнтный генератор
int x;

int a=845,c=2625,m=8192; // из презентации
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
	// ПОКА НЕ РАБОТАЕТ
	///////////////////*if(argc != 4)
	//////////////////{
	//////////////////	printf("wrong input format (program inputfile outputfile timefile)");
	//////////////////	getchar();
	//////////////////	return -1;
	//////////////////}*/
	





	
	// чтение из файла

	int count;
	double mu, sigquad; //количество чисел, мю, сигма в квадрате
	

	// ПОКА НЕ РАБОТАЕТ
	///////////////////*ifstream f;
	//////////////////f.open(argv[1]);
	//////////////////f >> count >> mu >> sigquad;
	//////////////////f.close();*/
	

	count = 1000;
	mu =1 ;
	sigquad = 1;
	
	
	// ________________чтение из файла


// начинаем считать время
	// ПОКА НЕ РАБОТАЕТ

	Timer timer;
	timer.reset();
    timer.start();

// используем  Преобразование Бокса — Мюллера


	double *values = new double[count+1];// создали массив

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

		values[i]=x*BM*sqrt(sigquad)+mu;
		values[i+1]=y*BM*sqrt(sigquad)+mu;

		i+=2;
	}

	double time_result = timer.getElapsed();
// посчитали время работы




	
	

	// два критерия посчитаем...



	int iMin=(int)(mu-3*sqrt(sigquad));
	int iMax=(int)(mu+3*sqrt(sigquad));
	int int_count = 11;
	//calculation of the statistical characteristics(xi_quadro)
	double int_length = (iMax - iMin) / (double)int_count;
	int* emp_frequency = new int[int_count];
	for(int i = 0; i < int_count; i++)
		emp_frequency[i] = 0;
	for(int i = 0; i < iMax; i++)
	{
		int int_index = (values[i] - iMin) / int_length;
		if(int_index == int_count) int_index--;
		emp_frequency[int_index]++;
	}
	double xi_quadro = 0;
	double N_p = (double)iMax / (double)int_count;
	for(int i = 0; i < int_count; i++)
	{
		double tmp = ((double)emp_frequency[i] - N_p);
		xi_quadro += tmp * tmp;
	}
	xi_quadro /= N_p;
	// первый посчитали





	// файлики запишем...





	delete[] values;
	return 0;
}