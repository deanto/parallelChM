// NG.cpp: определяет точку входа для консольного приложения.
//
#include <fstream>
#include <iomanip>

#include "math.h"
#include "timer.h"

using namespace std;

int A_V = 845;
int C_V = 2625;
int M_V = 8192;

int n = 1;
//-----------------------------------------
int my_rand()
{
	n = (n * A_V + C_V) % M_V;
	return n;
}
//-----------------------------------------
void my_srand(int s)
{
	n = s;
}
//-----------------------------------------


void quickSort(double arr[], int left, int right) 
{
    int i = left, j = right;
    double tmp;
    double pivot = arr[(left + right) / 2]; 
    /* partition */
    while (i <= j) {
        while (arr[i] < pivot)
        i++;
        while (arr[j] > pivot)
        j--;
        if (i <= j) {
            tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
            i++;
            j--;
        }
    }; 
    /* recursion */
    if (left < j)
    quickSort(arr, left, j);
    if (i < right)
    quickSort(arr, i, right);
 
}
//-----------------------------------------
double F_x(double x, double a, double b)
{
	return (x-a)/(b-a);	
}
//-----------------------------------------
int main(int argc, char* argv[])
{
	if(argc != 4)
	{
		printf("You must enter: %s [inputfile] [outputfile] [timefile]", argv[0]);
		getchar();
		return -1;
	}
	double MIN, MAX;
	int iMax;
	
	//working with input file
	ifstream in;
	in.open(argv[1]);
	in >> iMax >> MIN >> MAX;
	in.close();
		
	double old_MIN = MIN;
	double old_MAX = MAX;

	//starting of time record
	Timer timer;
    timer.start();

	//to shift the range on [0;MAX(+/-)shift]
	double shift = 0;
	int shift_direct = 0;
	if(MIN < 0) 
	{
		shift = -MIN;
		shift_direct = -1;
		MIN = 0;
		MAX += shift;
	}
	else if (MIN > 0)
	{
		shift = MIN;
		shift_direct = 1;
		MIN = 0;
		MAX -= shift;
	}
	//домножение на 10^k до целого числа
	unsigned int ten_pow = 1;
	while((double)(int)MAX - MAX != 0)
		MAX *= 10, ten_pow *= 10;
		
	my_srand(0);
	int i = 0;
	double* out_value = new double[iMax];
	double* out_value_sort = new double[iMax];
	while( i < iMax )
	{
		out_value[i] = (double)( my_rand() % (int)(MAX + 1.0) );		
		out_value[i] /= ten_pow;
		out_value[i] = out_value[i] + (double)shift_direct * shift;
		out_value_sort[i] = out_value[i];
		i++;
	}
	
	//finishing of time record
	double time_result = timer.getElapsed();

	int int_count = 11;
	//calculation of the statistical characteristics(xi_quadro)
	double int_length = (old_MAX - old_MIN) / (double)int_count;
	int* emp_frequency = new int[int_count];
	for(int i = 0; i < int_count; i++)
		emp_frequency[i] = 0;
	for(int i = 0; i < iMax; i++)
	{
		int int_index = (out_value[i] - old_MIN) / int_length;
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

	//calculation of the statistical characteristics(lambda)
	quickSort(out_value_sort, 0, iMax - 1);
	int new_out_value_num = 0;
	for(int i = 0; i < iMax - 1; i++)
		if(out_value_sort[i] != out_value_sort[i+1])
			new_out_value_num++;
	new_out_value_num++;
	double* new_out_value = new double[new_out_value_num];
	double* new_out_value_repeat = new double[new_out_value_num];
	for(int i = 0; i < new_out_value_num; i++)
		new_out_value_repeat[i] = new_out_value[i] = 0;
	new_out_value[0] = out_value_sort[0];
	new_out_value_repeat[0]++;
	int current_index = 0;
	for(int i = 1; i < iMax; i++)
	{
		if(new_out_value[current_index] != out_value_sort[i])
		{
			current_index++;
			new_out_value[current_index] = out_value_sort[i];
			new_out_value_repeat[current_index]++;
		}
		else
			new_out_value_repeat[current_index]++;
	}
	double d, dMax;
	double tmp = new_out_value_repeat[0];
	dMax = tmp/iMax - F_x(new_out_value[0], old_MIN, old_MAX);
	if(dMax < 0) dMax = -dMax;
	for(int i = 1; i < new_out_value_num; i++)
	{
		tmp += new_out_value_repeat[i];
		d = tmp/iMax - F_x(new_out_value[i], old_MIN, old_MAX);
		if(d < 0) d = -d;
		if(dMax < d) dMax = d;
	}
	double lambda = dMax;
	
	//working with timeLog file
	ofstream log;	
	log.open(argv[3]);
	log << time_result;
	log.close();

	//working with output file
	ofstream out;
	out.open(argv[2]);
	out << xi_quadro << endl << lambda << endl;
	for(int i = 0; i < iMax; i++)
		out << out_value[i] << endl;
	out.close();

	//clearing used memory
	delete[] out_value;
	delete[] emp_frequency;
	
	return 0;
}