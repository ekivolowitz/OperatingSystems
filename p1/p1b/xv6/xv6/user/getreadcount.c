#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  int retValue = getreadcount();
  printf(1, "%d\n", retValue);
  exit();
}
