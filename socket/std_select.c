#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include <errno.h>

int main(void)
{

    // 尚未绑定本地端口;;
    int maxfd = 0;
    fd_set readmask;
    int ret  = 0 ;
    char buf[BUFSIZ];


    FD_ZERO(&readmask);
    maxfd = 0;

    FD_SET(0, &readmask);


    while ( select(maxfd + 1,  &readmask, NULL, NULL, 0) >= 0 )
    {

        if ( ret < 0 )
        {
            if (errno != EINTR && errno != EWOULDBLOCK)
                return 0;
        }

        ret = read(0, buf, sizeof(buf));

        write(1, buf, strlen(buf));

        if (buf[0] == 'q' )
            return 0;

    }


    return 0;
}
