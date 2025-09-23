#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int yo = getpid();
  int padre = getppid();

  printf("Soy %d, hijo de %d\n", yo, padre);

  printf("getancestor(0) = %d (yo)\n", getancestor(0));
  printf("getancestor(1) = %d (padre)\n", getancestor(1));
  printf("getancestor(2) = %d (abuelo)\n", getancestor(2));
  printf("getancestor(1000) = %d (invalido)\n", getancestor(1000));

  exit(0);
}
