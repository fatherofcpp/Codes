#include "csharemem.h"
#include "Log.h"

static int s_semid;
static int s_shmid;
static char *s_shmptr;

static int _p(void)
{
    struct sembuf sem_p;

    sem_p.sem_num = 0;
    sem_p.sem_op = -1;

    if(semop(s_semid, &sem_p, 1) < 0)
    {
        gLog.Add("%s semop", __FUNCTION__);
        return -1;
    }
    return 0;
}

static int _v(void)
{
    struct sembuf sem_v;

    sem_v.sem_num = 0;
    sem_v.sem_op = 1;

    if(semop(s_semid, &sem_v, 1) < 0)
    {
        gLog.Add("%s semop", __FUNCTION__);
        return -1;
    }
    return 0;
}

int shm_init(void)
{
    key_t sem_key;
    key_t shm_key;
    const char f_path[32] = "/root/col_app";
    sem_key = ftok(f_path, 1);
    if (sem_key < 0)
    {
        gLog.Add("%s sem_key", __FUNCTION__);
        return -1;
    }

    shm_key = ftok(f_path, 2);
    if (shm_key < 0)
    {
        gLog.Add("%s shm_key", __FUNCTION__);
        return -2;
    }

    s_semid = semget(sem_key, 0, 0666);
    if (s_semid < 0)
    {
        gLog.Add("%s semid", __FUNCTION__);
        return -3;
    }

    s_shmid = shmget(shm_key, 0, 0666);
    if (s_shmid < 0)
    {
        gLog.Add("%s shmid", __FUNCTION__);
        return -4;
    }

    s_shmptr = (char*)shmat(s_shmid, 0, 0);
    if ((MS_INT32)s_shmptr < 0)
    {
        gLog.Add("%s shmat", __FUNCTION__);
        return -5;
    }
    return 0;
}

int shm_deinit(void)
{
    int ret = shmdt(s_shmptr);
    return ret;
}

int shm_write(st_ac_reg *p_reg)
{
    int ret = 0;
    ret = _p();
    if(ret != 0)
    {
        return ret;
    }
    memcpy(s_shmptr, p_reg, sizeof(*p_reg));
    ret = _v();
    if(ret != 0)
    {
        return ret;
    }
    return 0;
}


