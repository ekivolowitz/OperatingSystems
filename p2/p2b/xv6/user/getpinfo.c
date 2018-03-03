#include "types.h"
#include "stat.h"
#include "user.h"
#include "pstat.h"
//#define DEBUG
int
main(int argc, char *argv[]) {
  settickets(10);
  struct pstat p;
  getpinfo(&p);
  exit();
}
