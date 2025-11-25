#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
  char *addr = sbrk(0);
  if (sbrk(4096) == (char*)-1) {
    printf("sbrk fallo\n");
    exit(1);
  }

  printf("addr = %p\n", addr);

  // escribir
  addr[0] = 'A';
  printf("Escritura inicial OK\n");

  // proteger
  if (mrdprotect(addr, 1) < 0) {
    printf("mrdprotect fallo\n");
    exit(1);
  }
  printf("mrdprotect OK\n");

  // desproteger
  if (munrdprotect(addr, 1) < 0) {
    printf("munrdprotect fallo\n");
    exit(1);
  }
  printf("munrdprotect OK\n");

  // leer de nuevo (ya no debería fallar)
  char c = addr[0];
  printf("Lectura despues de munrdprotect OK, c = %c\n", c);

  exit(0);
}