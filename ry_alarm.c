#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "ry_alarm.h"

#ifndef SIGNAL_ARGS
#define SIGNAL_ARGS  int postgres_signal_arg
#endif

static volatile sig_atomic_t alarm_enabled = false;

#define disable_alarm() (alarm_enabled = false)
#define enable_alarm()  (alarm_enabled = true)

static void 
handle_sig_alarm(SIGNAL_ARGS)
{
    // 信号处理函数;

}

void InitializeTimeouts(void)
{
    disable_alarm();

    signal(SIGALRM, handle_sig_alarm);
}
