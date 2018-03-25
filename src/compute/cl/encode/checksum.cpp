#include <cstdio>
#include <cstdint>
#include <string>
#include "../kernels/program.cl.h"
///
int32_t main()
{
  char* s = (char*)program.c_str();
  uint32_t check = 7;
  while (*s) {
    check = check * 101 + *s++;
  }
	printf("Program: %u\n", check);
  return 0;
}
