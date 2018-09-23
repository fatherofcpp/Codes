#ifndef INCLUDE_ALL_H
#define INCLUDE_ALL_H

//linux

using namespace std;
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <vector>
#include <map>
#include <string>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <netdb.h>
#include <termios.h>

#include <sys/stat.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define Sleep(x) usleep(x*1000)

//qt
#include "qstring.h"
#include <qlocalsocket.h>
#endif // INCLUDE_ALL_H
