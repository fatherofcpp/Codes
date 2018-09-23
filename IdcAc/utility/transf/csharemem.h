#pragma once

#include "report_data.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

int shm_write(st_ac_reg *p_reg);
int shm_init(void);



