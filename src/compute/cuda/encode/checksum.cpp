#include <cstdio>
#include <cstdint>
#include <string>
#include "../kernels/module.fatbin.h"
///
int32_t main()
{
  char* s = (char*)module_fatbin;
  uint32_t check = 7;
  while (*s) {
    check = check * 101 + *s++;
  }
	printf("Module: %u\n", check);
  return 0;
}
