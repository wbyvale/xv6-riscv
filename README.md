# Tarea 2 — Lottery Scheduling en xv6-riscv
## 1. Implementación
- **Campos nuevos**: `tickets`, `cpu_slices` en `kernel/proc.h`.
- **Inicialización**: `tickets=100`, `cpu_slices=0` en `allocproc()` (`kernel/proc.c`).
- **Syscall**: `settickets(int)`:
  - `kernel/syscall.h`: `#define SYS_settickets <N>`
  - `kernel/syscall.c`: `extern uint64 sys_settickets(void);` + `[SYS_settickets] = sys_settickets`
  - `kernel/sysproc.c`: implementación de `sys_settickets`.
  - `user/user.h`: `int settickets(int);`
  - `user/usys.pl`: agregado `settickets` (se genera `usys.S`).
- **PRNG**: `krand()` / `kseed()` en `kernel/proc.c` (LCG simple).
- **Scheduler**: reemplazo por Lottery Scheduling en `scheduler()`:
  - Sumatoria de tickets de `RUNNABLE`
  - Sorteo `r = krand()%total + 1`
  - Selección por acumulación
  - `cpu_slices++` en el proceso elegido
  - Robustez: `tickets >= 1`, `total==0` → continuar
- **Monitoreo**: `procdump()` imprime `tickets` y `slices`.
## 2. Archivos modificados
- `kernel/proc.h`
- `kernel/proc.c`
- `kernel/syscall.h`
- `kernel/syscall.c`
- `kernel/sysproc.c`
- `user/usys.pl` (no editar `usys.S` a mano)
- `user/user.h`
- `user/demo.c`
- `Makefile` (`_demo` en `UPROGS`)
## 3. Cómo compilar y ejecutar
```bash
make clean
make qemu
# en xv6:
demo        # crea N procesos con distintos tickets
# (opcional) Ctrl+A, luego P para procdump
