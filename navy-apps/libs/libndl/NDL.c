#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

static int evtdev = -1;
static int fbdev = -1;
static int screen_w = 0, screen_h = 0;
static int event_fd = -1;

// 以毫秒为单位返回系统时间
uint32_t NDL_GetTicks() {
	struct timeval current_time;
	gettimeofday(&current_time, NULL);
//	printf("tv_sec = %ld, us = %ld\n", current_time.tv_sec, current_time.tv_usec);
	uint32_t ms = (uint32_t)(current_time.tv_sec * 1000 + current_time.tv_usec/1000);
//	printf("ms = %u\n", ms);
  return ms;
}

int NDL_PollEvent(char *buf, int len) {
	int retval = read(event_fd, buf, len);
//	fseek(event_fp, 0, SEEK_SET);
	return retval;
}

void NDL_OpenCanvas(int *w, int *h) {
	// get the size of screen -- start
	char buf[128];
	int dispinfo_fd = open("/proc/dispinfo", "r");
	read(dispinfo_fd, buf, -1);
	sscanf(buf, "WIDTH\t: %d\nHEIGHT\t: %d\n", &screen_w, &screen_h);
	printf("by yinhua, screen_w = %d, screen_h = %d\n", screen_w, screen_h);
	close(dispinfo_fd);
	// get the size of screen -- end
  if (getenv("NWM_APP")) {
    int fbctl = 4;
    fbdev = 5;
    screen_w = *w; screen_h = *h;
    char buf[64];
    int len = sprintf(buf, "%d %d", screen_w, screen_h);
    // let NWM resize the window and create the frame buffer
    write(fbctl, buf, len);
    while (1) {
      // 3 = evtdev
      int nread = read(3, buf, sizeof(buf) - 1);
      if (nread <= 0) continue;
      buf[nread] = '\0';
      if (strcmp(buf, "mmap ok") == 0) break;
    }
    close(fbctl);
  }
}

void NDL_DrawRect(uint32_t *pixels, int x, int y, int w, int h) {
}

void NDL_OpenAudio(int freq, int channels, int samples) {
}

void NDL_CloseAudio() {
}

int NDL_PlayAudio(void *buf, int len) {
  return 0;
}

int NDL_QueryAudio() {
  return 0;
}

int NDL_Init(uint32_t flags) {
  if (getenv("NWM_APP")) {
    evtdev = 3;
  }
	event_fd = open("/dev/events", "r");
  return 0;
}

void NDL_Quit() {
	close(event_fd);
}
