#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define N 1000000
#define OP *
#define INIT 1.0    //如果OP操作定义为加法，INIT为0.0; 如果OP为乘法，INIT为1.0


int get_value(double a[]){
	int value = a[0];
	return value;
}

void production1 (double a[], double *dest){
	for(int i = 0; i < get_value(a); i++)
		*dest = *dest OP a[i];	
}

void production2 (double a[], double *dest){ //去掉了不必要函数调用
	int length = get_value(a);
	for(int i = 0; i < length; i++)
		*dest = *dest OP a[i];
}

void production3 (double a[], double *dest){   //去掉不必要的内存访问
	int length = get_value(a);
	double acc = INIT;
	for(int i = 0; i < length; i++){
		acc = acc OP a[i];
	}
	*dest = acc;
}

void production4 (double a[], double *dest){    //循环展开
	int i;
	int length = get_value(a);
	double acc = INIT;
	int limit = length - 1;
	for(i = 0; i < limit; i += 2)
		acc = (acc OP a[i]) OP a[i+1];
	for(; i < length; i++)
		acc = acc OP a[i];
	*dest = acc;
}

void production5 (double a[], double *dest){ //加强并行性的循环展开
    int i;
    int length = get_value(a);
    double acc0 = INIT;
	double acc1 = INIT;
    int limit = length - 1;
    for(i = 0; i < limit; i += 2){
        acc0 = acc0 OP a[i];
		acc1 = acc1 OP a[i+1];
	}
    for(; i < length; i++)
        acc0 = acc0 OP a[i];
    *dest = acc0 OP acc1;
}

void production6 (double a[], double *dest){//修改了结合的顺序
    int i;
    int length = get_value(a);
    double acc = INIT;
    int limit = length - 1;
    for(i = 0; i < limit; i += 2)
        acc = acc OP (a[i] OP a[i+1]);
    for(; i < length; i++)
        acc = acc OP a[i];
    *dest = acc;
}

void main(){
	double result = 0.0;
	double *dest = &result;
	double a[N];
	for(int i = 0; i < N; i++){
		a[i] = (double)rand();
	}
	clock_t start, stop;
	a[0] = (double)(sizeof(a)/sizeof(double)); 
	printf("开始测试！\n");
	
	start = clock();
	production1(a, dest);
	stop = clock();
	double duration1 = (double)(stop - start);
	
	start = clock();
	production2(a, dest);
    stop = clock();
    double duration2 = (double)(stop - start);

	start = clock();
    production3(a, dest);
    stop = clock();
    double duration3 = (double)(stop - start);
	
	start = clock();
    production4(a, dest);
    stop = clock();
    double duration4 = (double)(stop - start);

	start = clock();
    production5(a, dest);
    stop = clock();
    double duration5 = (double)(stop - start);

	start = clock();
    production6(a, dest);
    stop = clock();
    double duration6 = (double)(stop - start);

	printf("production1 : %lf clocks\n",duration1);
	printf("production2 : %lf clocks\n",duration2);
	printf("production3 : %lf clocks\n",duration3);
	printf("production4 : %lf clocks\n",duration4);
	printf("production5 : %lf clocks\n",duration5);
	printf("production6 : %lf clocks\n",duration6);
}
