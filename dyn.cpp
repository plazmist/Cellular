#include <iostream>
 
int main() {
  // dimensions
  int N = 3;
  int M = 3;
 
  // dynamic allocation
  int** ary = new int*[N];
  for(int i = 0; i < N; ++i)
      ary[i] = new int[M];
 
  // fill
  for(int i = 0; i < N; ++i)
    for(int j = 0; j < M; ++j)
      ary[i][j] = i;
 
  // print
  for(int i = 0; i < N; ++i)
    for(int j = 0; j < M; ++j)
      std::cout << ary[i][j] << "\n";
 
  // free
  for(int i = 0; i < N; ++i)
    delete [] ary[i];
  delete [] ary;
 
  return 0;
}