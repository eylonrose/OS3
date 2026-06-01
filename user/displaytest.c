#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define SCREEN_W 640
#define SCREEN_H 480
#define FB_BYTES (SCREEN_W * SCREEN_H * 4)

static void
check(int ok, const char *msg)
{
  if(!ok){
    fprintf(2, "displaytest: %s failed\n", msg);
    exit(1);
  }
  printf("displaytest: %s ok\n", msg);
}

int
main(void)
{
  void *map1;
  void *map2;
  void *fb;

  check(flip_display(0) < 0, "flip rejects null buffer");
  check(flip_display((void *)1) < 0, "flip rejects unaligned buffer");
  check(flip_display((void *)0x40000000) < 0, "flip rejects unmapped buffer");

  map1 = map_display(0);
  check(map1 != 0 && map1 != (void *)-1, "map_display auto maps framebuffer");

  map2 = map_display(0);
  check(map2 == map1, "map_display repeated call returns same address");

  fb = sbrk(FB_BYTES);
  check(fb != (void *)-1, "sbrk framebuffer allocation");
  check(((uint64)fb % 4096) == 0, "framebuffer is page aligned");

  memset(fb, 0, FB_BYTES);
  check(flip_display(fb) == 0, "flip accepts valid framebuffer");

  printf("displaytest: all checks passed\n");
  exit(0);
}
