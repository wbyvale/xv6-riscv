#  README — Implementación de mrdprotect y munrdprotect en xv6-riscv

**INFORME - Implementación de mrdprotect y munrdprotect en xv6-riscv**

**1\. Introducción**

Este informe documenta la implementación de dos nuevas llamadas al sistema en xv6-riscv:

- **mrdprotect(void \*addr, int len)**  
    Revoca el permiso de lectura de _len_ páginas consecutivas desde _addr_.
- **munrdprotect(void \*addr, int len)**  
    Restaura el permiso de lectura en las mismas páginas previamente protegidas.

Estas syscalls modifican directamente las entradas de la tabla de páginas (**PTEs**) del proceso, manipulando el bit **PTE_R**. El propósito es forzar que cualquier lectura posterior en una página protegida resulte en un **page fault**.

**2\. Archivos modificados**

**Espacio de Usuario**

| **Archivo** | **Descripción** |
| --- | --- |
| user/user.h | Agregados prototipos de mrdprotect y munrdprotect |
| user/usys.pl | Generados stubs para ambas syscalls |
| Makefile | Se agregaron los binarios de prueba \_rdprotect_test, \_rdp_ok, \_rdp_errortest |
| user/rdprotect_test.c | Test que debe producir un page fault |
| user/rdp_ok.c | Test que comprueba la restauración del permiso |
| user/rdp_errortest.c | Casos de error: syscalls deben devolver -1 |

**Espacio Kernel**

| **Archivo** | **Cambio** |
| --- | --- |
| kernel/syscall.h | Añadidos números de syscall |
| kernel/defs.h | Añadidos prototipos globales |
| kernel/syscall.c | Wrappers + entradas en syscalls\[\] |
| kernel/vm.c | Implementación central de protección/desprotección |
| usys.S | Generado automáticamente al compilar |

**3\. Diseño de la solución**

**3.1 Validaciones necesarias**

Cada syscall (mrdprotect y munrdprotect) valida:

- len > 0
- addr alineada a página:
- if (va % PGSIZE != 0) return -1;
- Todas las páginas deben tener un **PTE válido**
- Todas las páginas deben tener **PTE_U** (evita tocar memoria del kernel)
- Antes de modificar, se verifica _todo el rango_  
    → evita inconsistencias parciales

**3.2 Modificación de permisos del PTE**

**En mrdprotect:**

\*pte &= ~PTE_R; // Apaga el bit de lectura

**En munrdprotect:**

\*pte |= PTE_R; // Restaura el bit de lectura

**Limpieza de TLB:**

sfence_vma();

Permite que la CPU use los nuevos permisos.

**4\. Pruebas realizadas**

**4.1 rdprotect_test - DEBE FALLAR**

Este test:

- Reserva memoria
- Escribe → OK
- Llama mrdprotect → OK
- Intenta leer → **page fault**

**Resultado obtenido:**

Escritura inicial OK

mrdprotect OK

usertrap(): unexpected scause ...

Comportamiento correcto.

**4.2 rdp_ok - DEBE RESTAURAR PERMISO**

Este test:

- Escribe → OK
- Protege con mrdprotect
- Desprotege con munrdprotect
- Lee → NO debe fallar

**Resultado obtenido:**

Escritura inicial OK

mrdprotect OK

munrdprotect OK

Lectura despues de munrdprotect OK, c = A

Protección y desprotección funcionando.

**4.3 rdp_errortest - CASOS DE ERROR**

Este programa prueba que la syscall NO modifica permisos y devuelve -1 cuando:

- len = 0
- dirección no alineada: addr + 1
- dirección inválida/no mapeada

**Resultado obtenido:**

mrdprotect(addr,0) -> -1

munrdprotect(addr,0) -> -1

mrdprotect(addr+1,1) -> -1

munrdprotect(addr+1,1) -> -1

mrdprotect(bad_addr,1) -> -1

munrdprotect(bad_addr,1) -> -1

Fin de pruebas de error (el proceso NO debe morir aqui).

Manejo de errores correcto.

**6\. Conclusiones**

- Se implementaron correctamente las syscalls **mrdprotect** y **munrdprotect**.
- Se validaron direcciones, alineación y permisos antes de modificar cualquier PTE.
- Los tests demuestran:  
    ✔ Protección correcta → page fault  
    ✔ Restauración correcta → lectura permitida  
    ✔ Validación correcta de errores → retorno -1
- Toda la funcionalidad trabaja establemente sin comprometer la memoria del kernel.

**Implementación completada con éxito y cumpliendo todos los requisitos.**