#include <pl32-memory.h>
#include <pl32-shell.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

plarray_t* plListDir(char* path);
size_t plGetFileSize(char* path);
int plDeletePath(char* path, bool recursive);
int plOpenWith(char* path, char* program);
