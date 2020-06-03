#ifndef EINA_PIPE_H
#define EINA_PIPE_H

# ifdef _WIN32
#  define pipe_write(fd, buffer, size) send((fd), (char *)(buffer), size, 0)
#  define pipe_read(fd, buffer, size)  recv((fd), (char *)(buffer), size, 0)
#  define pipe_close(fd)               closesocket(fd)
#  define PIPE_FD_ERROR   SOCKET_ERROR
# else
#  include <unistd.h>
#  define pipe_write(fd, buffer, size) write((fd), buffer, size)
#  define pipe_read(fd, buffer, size)  read((fd), buffer, size)
#  define pipe_close(fd)               close(fd)
#  define PIPE_FD_ERROR   -1
# endif // _WIN32

#endif /* EINA_PIPE_H */
