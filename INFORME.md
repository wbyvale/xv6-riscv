cat > INFORME.md << 'EOF'
# INFORME — T1 xv6-riscv: `getppid()` y `getancestor(n)`

## **1) Funcionamiento de las llamadas al sistema**

### **getppid(void)**
Devuelve el **PID del proceso padre** del proceso que la invoca.  
- Si el proceso no tiene padre, retorna **-1**.  
- Se toma el lock de `struct proc` para leer `parent->pid` de forma segura.  
- Complejidad: **O(1)**.

### **getancestor(int n)**
Devuelve el **PID del ancestro n** del proceso actual.  
- `n = 0` → PID del mismo proceso; `n = 1` → padre; `n = 2` → abuelo; …  
- Si `n < 0` o no existe tal ancestro, retorna **-1**.  
- Implementación: se parte de `myproc()` y se sube `n` veces por el puntero `parent`, protegiendo lecturas con locks.  
- Complejidad: **O(n)** (n = niveles ascendidos).

> **Ejemplo real de salida (programa de prueba `yosoytupadre`):**
> ```
> Soy 4, hijo de 2
> getancestor(0) = 4 (yo)
> getancestor(1) = 2 (padre)
> getancestor(2) = 1 (abuelo)
> getancestor(1000) = -1 (invalido)
> ```

---

## **2) Explicación de las modificaciones realizadas**

- **kernel/syscall.h**: Se agregaron los números de syscall para `getppid` y `getancestor`.  
- **user/user.h**: Se añadieron los prototipos visibles desde programas de usuario.  
- **user/usys.pl**: Se agregaron entradas para generar los stubs automáticamente (usys.S se regenera).  
- **kernel/syscall.c**: Se enlazaron las nuevas syscalls en la tabla de enrutamiento.  
- **kernel/sysproc.c**: Se implementó la lógica de `sys_getppid` y `sys_getancestor`.  
- **user/yosoytupadre.c**: Se creó un programa de prueba para verificar el correcto funcionamiento.  
- **Makefile**: Se agregó el binario de prueba al listado de `UPROGS` y a la regla de `fs.img`.

---

## **3) Dificultades encontradas y soluciones**

- **argint devolvía void**: El compilador daba error.  
  *Solución*: Corregir las firmas y retornos de `argint`, `argaddr` y `argstr`.  

- **Stubs desaparecían al editar `usys.S`**: Se perdían símbolos al recompilar.  
  *Solución*: Editar solo `usys.pl`, no `usys.S`, y regenerar.  

- **Error al enlazar `yosoytupadre`**: Referencias indefinidas.  
  *Solución*: Asegurar que los stubs se generen globalmente con `usys.pl`.  

- **Fallo en Makefile (`UPROGS`)**: Aparecía “No rule to make target”.  
  *Solución*: Quitar el `\` al final del bloque `UPROGS` y usar `$(UPROGS)` en la receta.  

- **Error con `mkfs/mkfs`**: “No such file or directory”.  
  *Solución*: Usar la ruta `./mkfs/mkfs` y cuidar la indentación con TAB.

