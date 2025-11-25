#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
  char *addr;
  char *bad_addr;
  int r;

  // Reservar una página para tener una dirección válida y alineada
  addr = sbrk(0);
  if (sbrk(4096) == (char*)-1) {
    printf("sbrk fallo\n");
    exit(1);
  }

  printf("addr (valida, alineada) = %p\n", addr);

  // -------------------------------
  // 1) len <= 0
  // -------------------------------
  r = mrdprotect(addr, 0);
  printf("mrdprotect(addr, 0) -> %d (esperado -1)\n", r);

  r = munrdprotect(addr, 0);
  printf("munrdprotect(addr, 0) -> %d (esperado -1)\n", r);

  // -------------------------------
  // 2) addr no alineada (addr+1)
  // -------------------------------
  r = mrdprotect(addr + 1, 1);
  printf("mrdprotect(addr+1, 1) -> %d (esperado -1)\n", r);

  r = munrdprotect(addr + 1, 1);
  printf("munrdprotect(addr+1, 1) -> %d (esperado -1)\n", r);

  // -------------------------------
  // 3) Pagina que no esta mapeada
  //    (elige una direccion "lejana" que seguro no tengas mapeada)
  // -------------------------------
  bad_addr = (char*)0x80000000UL; // fuera del espacio usual de usuario

  r = mrdprotect(bad_addr, 1);
  printf("mrdprotect(bad_addr, 1) -> %d (esperado -1)\n", r);

  r = munrdprotect(bad_addr, 1);
  printf("munrdprotect(bad_addr, 1) -> %d (esperado -1)\n", r);

  printf("\nFin de pruebas de error (el proceso NO debe morir aqui).\n");
  exit(0);
}
