#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>


void main (void)
{
    char buffer[80];
    int flags;

    flags = fcntl (fileno (stdin), F_GETFL);
    fcntl (fileno (stdin), F_SETFL, flags | O_NONBLOCK);
    
    while (1)
    {
        void *ptr;
        
        ptr = fgets (buffer, sizeof (buffer), stdin);

        if (ptr != 0)
            fputs (ptr, stderr);
        else
            fputs ("FOO\n", stderr);
    }
}
        
        
    
