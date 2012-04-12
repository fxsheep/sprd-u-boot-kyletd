
#ifndef _DIRENT_H_
#define _DIRENT_H_


#ifndef DT_UNKNOWN
#define  DT_UNKNOWN     0
#define  DT_FIFO        1
#define  DT_CHR         2
#define  DT_DIR         4
#define  DT_BLK         6
#define  DT_REG         8
#define  DT_LNK         10
#define  DT_SOCK        12
#define  DT_WHT         14
#endif

/* the following structure is really called dirent64 by the kernel
 * headers. They also define a struct dirent, but the latter lack
 * the d_type field which is required by some libraries (e.g. hotplug)
 * who assume to be able to access it directly. sad...
 */
struct dirent {
    uint64_t         d_ino;
    int64_t          d_off;
    unsigned short   d_reclen;
    unsigned char    d_type;
    char             d_name[256];
};

typedef struct DIR  DIR;

int              getdents(unsigned int, struct dirent*, unsigned int);
DIR*             opendir(const char*  dirpath);
DIR*             fdopendir(int fd);
struct dirent*   readdir(DIR*  dirp);
int              readdir_r(DIR*  dirp, struct dirent *entry, struct dirent **result);
int              closedir(DIR*  dirp);
void             rewinddir(DIR *dirp);
int              dirfd(DIR* dirp);
int              alphasort(const void *a, const void *b);
int              scandir(const char *dir, struct dirent ***namelist,
                                 int(*filter)(const struct dirent *),
                                 int(*compar)(const struct dirent **, 
                                              const struct dirent **));
#endif /* _DIRENT_H_ */
