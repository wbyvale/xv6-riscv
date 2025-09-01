# INFORME — Tarea 0 (xv6)

## Entorno
- SO / versión: _(pegar salida de `lsb_release -sd`)_
- QEMU: _(pegar 1ª línea de `qemu-system-riscv64 --version`)_
- Toolchain: _(pegar 1ª línea de `riscv64-unknown-elf-gcc --version`)_

## Pasos seguidos para instalar xv6
1. Instalé dependencias: `build-essential`, `git`, `make`, `gdb-multiarch`, `qemu-system-misc`, `bc`.
2. Cloné el repo y trabajé en mi rama `grupo_P`.
3. Compilé y ejecuté con `make qemu` (mi Makefile no traía `qemu-nox`).
4. Probé en la shell de xv6: `ls`, `echo "hola xv6"`, `cat README`.

## Problemas encontrados y soluciones
- **bc: not found / [: Illegal number:** faltaba `bc`. Solución: `sudo apt install -y bc` y recompilar.
- **No existía la diana `qemu-nox`:** usé `make qemu` (o `QEMUOPTS='-nographic -serial stdio -monitor none'`).
- **Monitor `(qemu)` en vez de la shell:** alterné a la consola (Ctrl+a, c) o arranqué sin monitor.
- **ERROR: Need qemu version >= 7.2:** actualicé QEMU a una versión reciente y verifiqué con `qemu-system-riscv64 --version`.
- **Pantalla rosa de debconf (reinicio de servicios):** elegí **Yes** para continuar automáticamente.

## Confirmación de funcionamiento (xv6)
En el prompt `$` de xv6 ejecuté:
ls
echo "hola xv6"
cat README
Luego me entregó devuelta lo que subí en las capturas
