#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
  // 1. Reservar una página nueva en el heap
  char *addr = sbrk(0);  // dirección actual del break
  if (sbrk(4096) == (char*)-1) {
    printf("sbrk fallo\n");
    exit(1);
  }

  printf("addr = %p\n", addr);

  // 2. Escribir algo
  addr[0] = 'X';
  printf("Escritura inicial OK\n");

  // 3. Proteger contra lectura
  if (mrdprotect(addr, 1) < 0) {
    printf("mrdprotect fallo\n");
    exit(1);
  }
  printf("mrdprotect OK\n");

  // 4. Seguir escribiendo (deberia estar permitido)
  addr[0] = 'Y';
  printf("Escritura despues de mrdprotect OK\n");

  // 5. Intentar leer (esto deberia causar un page fault y matar el proceso)
  printf("Intentando leer (deberia fallar)...\n");
  char c = addr[0];   // <- aquí deberia romperse
  printf("NO deberias ver esto, c = %c\n", c);

  // 6. Revertir la protección (si el proceso siguiera vivo)
  if (munrdprotect(addr, 1) < 0) {
    printf("munrdprotect fallo\n");
    exit(1);
  }
  printf("munrdprotect OK\n");

  printf("Fin normal (esto tampoco deberias verlo si hay page fault)\n");
  exit(0);
}
