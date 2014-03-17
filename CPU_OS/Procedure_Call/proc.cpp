/* Output format(getticks overhead included):
 * procedure call with 0 parameters overhead
 * procedure call with 1 parameters overhead
 * procedure call with 2 parameters overhead
 * procedure call with 3 parameters overhead
 * procedure call with 4 parameters overhead
 * procedure call with 5 parameters overhead
 * procedure call with 6 parameters overhead
 * procedure call with 7 parameters overhead
 * !Note!: since there may be an memory read overhead,
 * the proc call with 0 parameter may not be the correct
 * value. It is recommended to clean and make before
 * executing the binary code to eliminate this overhead.
 */
#include <stdio.h>
#include <iostream>
#include "measure.h"
using namespace std;

void func0(){ }
void func1(int a){ }
void func2(int a, int b){ }
void func3(int a, int b, int c){ }
void func4(int a, int b, int c, int d){ }
void func5(int a, int b, int c, int d, int e){ }
void func6(int a, int b, int c, int d, int e, int f){ }
void func7(int a, int b, int c, int d, int e, int f, int g){ }

double proc_call_overhead0(int times){ MEASURE(times, func0()); }
double proc_call_overhead1(int times){ MEASURE(times, func1(1)); }
double proc_call_overhead2(int times){ MEASURE(times, func2(1,2)); }
double proc_call_overhead3(int times){ MEASURE(times, func3(1,2,3)); }
double proc_call_overhead4(int times){ MEASURE(times, func4(1,2,3,4)); } 
double proc_call_overhead5(int times){ MEASURE(times, func5(1,2,3,4,5)); }
double proc_call_overhead6(int times){ MEASURE(times, func6(1,2,3,4,5,6)); }
double proc_call_overhead7(int times){ MEASURE(times, func7(1,2,3,4,5,6,7)); }

void exec_procedure_call_exp(int iteration_times){
  cout << proc_call_overhead0(iteration_times) << endl;
  cout << proc_call_overhead1(iteration_times) << endl;
  cout << proc_call_overhead2(iteration_times) << endl;
  cout << proc_call_overhead3(iteration_times) << endl;
  cout << proc_call_overhead4(iteration_times) << endl;
  cout << proc_call_overhead5(iteration_times) << endl;
  cout << proc_call_overhead6(iteration_times) << endl;
  cout << proc_call_overhead7(iteration_times) << endl;
}

int main(int argc, const char* argv[]){
  exec_procedure_call_exp(10000);
}
