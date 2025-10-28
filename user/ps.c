#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
  procdump();   // syscall que acabas de crear
  exit(0);
}

