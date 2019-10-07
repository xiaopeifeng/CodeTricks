#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/syscall.h>
#include <pthread.h>
#include <unistd.h>


#define gettid syscall(SYS_gettid)

sigjmp_buf jmp_ctx;
static int number = 10;

void
alarm_func(int n) {
  printf("%ld enter alarm_func\n", gettid);
  number++;
  siglongjmp(jmp_ctx, 0);
}

void
longtimetask(int time_used) {
  printf("%ld long time task running\n", gettid);
  sleep(time_used);
  alarm(0);
}

void*
worker(void* args) {
  printf("enter worker thread: %ld\n", gettid);
  signal(SIGALRM, alarm_func);
  alarm(5);

  int res = sigsetjmp(jmp_ctx, 0);
  printf("tid: %ld sigsetjmp return value: %d\n", gettid, res);
  printf("tid: %ld number: %d\n", gettid, number);
  if (res) {
    goto fail;
  }

  longtimetask(atoi((char*)args));
  goto ok;

fail:
  printf("tid: %ld worker fail\n", gettid);
  return NULL;

ok:
  printf("tid: %ld worker ok\n", gettid);
  return;
}

void
main_jobs() {
  long count = 30;
  while (count--) {
    printf("tid: %ld, count %ld\n", gettid, count);
    sleep(1);
  }
}

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("usage: %s time\n", argv[0]);
    exit(0);
  }

  printf("start worker thread!\n");

  pthread_t th;
  pthread_create(&th, NULL, worker, argv[1]);

  main_jobs();

  pthread_join(th, NULL);

  printf("main thread exit!\n");
}

