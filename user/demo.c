#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// Carga de CPU para que el scheduler reparta tiempo
static void
burn(unsigned long long iters)
{
  volatile unsigned long long x = 0;
  for (unsigned long long i = 0; i < iters; i++)
    x += i;
}

int
main(int argc, char *argv[])
{
  int N = 5;                               // cantidad de hijos
  unsigned long long iters = 1ULL << 26;   // carga de CPU (ajusta si quieres)

  if (argc > 1)
    N = atoi(argv[1]);

  printf("Creando %d procesos...\n", N);

  for (int i = 0; i < N; i++) {
    int pid = fork();
    if (pid < 0) {
      printf("fork failed\n");
      exit(1);
    }

    if (pid == 0) {
      // ---------- Hijo ----------
      int t = 50 * (i + 1);      // 50, 100, 150, ...
      settickets(t);

      // Pequeña pausa para que este print no se mezcle con otros
      sleep(1);
      printf("Hijo PID=%d iniciado (tickets=%d)\n", getpid(), t);

      burn(iters);               // consumir CPU para que el scheduler actúe

      // Otra pequeña pausa antes del print final
      sleep(1);
      printf("Hijo PID=%d terminado (tickets=%d)\n", getpid(), t);
      exit(0);
    }

    // ---------- Padre ----------
    // Espacio entre forks para que el hijo recién creado imprima "iniciado"
    sleep(3);
  }

  // Padre espera a todos
  for (int i = 0; i < N; i++)
    wait(0);

  printf("Todos los procesos han terminado.\n");

  // Margen para procdump (Ctrl+A, luego P) si quieres mirar slices/tickets
  sleep(30);
  exit(0);
}

