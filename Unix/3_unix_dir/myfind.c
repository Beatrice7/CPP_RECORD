#include "apue.h"
#include <dirent.h>
#include <limits.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

//typedef定义函数指针Myfunc
typedef int Myfunc(const char *, const struct stat *, int);
//静态全局全局变量
static Myfunc myfunc, myfuncCompareContent, myfuncCompareName;
//后三个分别用来实现三种功能，第一个是程序自带的统计比例函数
static int myftw(char *, Myfunc *);
static int dopath(Myfunc *);
static long nreg, less4096, ndir, nblk, nchr, nfifo, nslink, nsock, ntot; //7+total

/*用于mufuncCompareContent() 和 myfuncCompareName()*/
static long filesize, filefindcount;
static char *filebuf, *comparebuf, *inputpath, *inputfilepath, *inputfilename;
static char *getRealPath(const char*, char*) ;                /*获取绝对路径*/
static const char* getFileName(const char *);                 /*分离目录和文件名*/

int main(int argc,  char *argv[])
{
    int ret;
    struct stat statbuf;

    if(!(argc == 2 || (argc == 4 && 0 == strcmp(argv[2], "-comp")) || (argc >= 4 && 0 == strcmp(argv[2], "-name"))))
                err_quit("usage: myfind <pathname> [-comp <filename> | -name<str>...]");

    if(lstat(argv[1], &statbuf) < 0)
        err_quit("lstat error: %s\n", argv[1]);

    if(S_ISDIR(statbuf.st_mode) == 0) //equal 0, not a directory, then use current directory
        strcpy(argv[1], ".");
    
    if(argc == 2)
    {
        ret = myftw(argv[1], myfunc);

        ntot = nreg + ndir + nblk + nfifo + nslink + nsock + nchr;
        if(ntot == 0)
            ntot = 1;/* avoid divide by 0; print 0 for all counts*/
        printf("regular files  = %7ld, %5.2f %%\n", nreg, nreg*100.0/ntot);
        printf("less than 4096 = %7ld, %5.2f %%\n", less4096, less4096*100.0/ntot);
        printf("derectories    = %7ld, %5.2f %%\n", ndir, ndir*100.0/ntot);
        printf("block special  = %7ld, %5.2f %%\n", nblk, nblk*100.0/ntot);
        printf("char special   = %7ld, %5.2f %%\n", nchr, nchr*100.0/ntot);
        printf("FIFOs          = %7ld, %5.2f %%\n", nfifo, nfifo*100.0/ntot);
        printf("symbolic links = %7ld, %5.2f %%\n", nslink, nslink*100.0/ntot);
        printf("sockets        = %7ld, %5.2f %%\n", nsock, nsock*100.0/ntot);
    }

    /*找出所有与指定文件有 相同内容 的文件*/
    if(argc == 4 && 0 == strcmp(argv[2], "-comp"))
    {
        /*输入判断，如果文件不符合，则可以直接返回错误，不用open了*/
        if(lstat(argv[3], &statbuf) < 0)
            err_quit("lstat error: %s\n", argv[3]);
        if(!S_ISREG(statbuf.st_mode))
            err_quit("not a regular file: %s\n", argv[3]);

        /*读入文件内容*/
        int fd;
        filesize = statbuf.st_size;

        if((fd = open(argv[3], O_RDONLY, FILE_MODE)) == -1)
            err_sys("can't open the file: %s\n", argv[3]);
        if((filebuf = (char*)malloc(sizeof(char) * filesize)) == NULL ||
               (comparebuf = (char*)malloc(sizeof(char) * filesize)) == NULL)
                err_sys("malloc error\n");
        if(read(fd, filebuf, filesize) != filesize)
            err_sys("read error: %s\n", argv[3]);
        close(fd);

        /*将起始目录转为绝对路径*/
        int len;
        inputpath = path_alloc(&len);
        getRealPath(argv[1], inputpath);
		char* tmp = inputpath + strlen(inputpath);
		*(tmp-1) = 0;

        //printf("inputpath_after_tmp:%s\n")
        //???
        char *ptr, *end;
        const char *split;
        split = getFileName(argv[3]);    /*分离目录和文件名*/
        
        //保存指定文件的绝对目录
        inputfilepath = path_alloc(&len);    //path_alloc返回的类型是char*
        getRealPath(argv[1], inputfilepath);
        
        end = argv[3] + strlen(argv[3]);
        ptr = inputfilepath + strlen(inputfilepath);
        while(split < end)
            *ptr++ = *split++;
        *ptr = 0;

        //开始查找
        printf("\nSame content as '%s':\n", argv[3]);
        ret = myftw(inputpath, myfuncCompareContent);
        if(filefindcount == 0)
            printf("Can't find any match file.\n");
        printf("\n");
    }

    /*找出filename相同的文件*/
    if(argc >= 4 && strcmp(argv[2], "-name") == 0)
    {
        int len, i;
        const char *split;
        inputpath = path_alloc(&len);
        inputfilename = path_alloc(&len);
        getRealPath(argv[1], inputpath);    //inputpath后面有'/'
		char* tmp = inputpath + strlen(inputpath);
		*(tmp-1) = 0;

        printf("\n");
        for(i = 3; i < argc; i++)
        {
            split = getFileName(argv[i]);
            if(split != argv[i])    /*输入的文件名包含了路径*/
                err_quit("only filename, do you include pathname?");

            strncpy(inputfilename, argv[i], len);
            filefindcount = 0;
            printf("%d. Same filename as '%s':\n", i-2, argv[i]);
            ret = myftw(inputpath, myfuncCompareName);
            if(filefindcount == 0)
                printf("Cannot find any match file\n\n");
        }
    }

    exit(ret);
}

#define FTW_F 1 //file
#define FTW_D 2 //directory
#define FTW_DNR 3
#define FTW_NS 4

static char *fullpath;  //何用？不是和pathname一样么

static int myftw(char* pathname, Myfunc *func)
{
    int len;
    fullpath = path_alloc(&len); //PATH_MAX + 1 bytes
    strncpy(fullpath, pathname, len);
    fullpath[len-1] = 0;
    //printf("fullpath__________%s\n", fullpath);//same as pathname, with '/' in the end
    return (dopath(func));
}

static int dopath(Myfunc* func)
{
    struct stat statbuf;
    struct dirent *dirp;
    DIR *dp;
    int ret;
    char *ptr;
    
    if(lstat(fullpath, &statbuf) < 0)
        return (func(fullpath, &statbuf, FTW_NS));
    if(S_ISDIR(statbuf.st_mode) == 0)//not a dir
        return (func(fullpath, &statbuf, FTW_F));
    //dir
    if((ret = func(fullpath, &statbuf, FTW_D)) != 0)
        return (ret);
	
    //printf("%s_______fullpath_in_dopath\n", fullpath);
    ptr = fullpath + strlen(fullpath);//point to end of path
    *ptr++ = '/';//这里不需要了，fullpath=realpath,realpath后面有已经有'/'
	//printf("after_/_fullpath______%s\n", fullpath);
    *ptr = 0;
    //if can't read dir
    if ((dp = opendir(fullpath)) == NULL)
        return (func(fullpath, &statbuf, FTW_DNR));
    //read
    while((dirp = readdir(dp)) != NULL)
    {
        if(strcmp(dirp->d_name, ".") == 0 || strcmp(dirp->d_name, "..") == 0)
            continue;
        strcpy(ptr, dirp->d_name);//append dname after '/'
        
        if((ret = dopath(func)) != 0)//recursive 递归，循环
            break;//time to leave
    }

    //ptr[-1] = 0;//erase everything from slash onwards  ???
    if(closedir(dp) < 0)
        err_ret("can't close dir %s", fullpath);
    return (ret);
}


/*第一个功能的实现函数myfunc()*/
static int myfunc(const char *pathname, const struct stat *statptr, int type)
{
    switch(type){
        case FTW_F:
            switch(statptr->st_mode & S_IFMT){
                case S_IFREG: 
                    nreg++;
                    if(statptr->st_size <= 4096)
                        less4096++;
                    break;
                case S_IFBLK: nblk++;break;
                case S_IFCHR: nchr++;break;
                case S_IFIFO: nfifo++;break;
                case S_IFLNK: nslink++;break;
                case S_IFSOCK: nsock++;break;
                case S_IFDIR: 
                              err_dump("for S_IFDIR for %s", pathname);
            }
            break;
        case FTW_D:
            ndir++;
            break;
        case FTW_DNR:
            err_ret("can't read directory %s", pathname);
            break;
        case FTW_NS:
            err_ret("stat error for %s", pathname);
            break;
        default:
            err_dump("unknown type %d for pathname %s", type, pathname);
    }
    return(0);//if correct, myfunc return 0 finally.
}


/*第二个功能的实现函数myfuncCompareContent()*/
static int myfuncCompareContent(const char *pathname, const struct stat *statptr, int type)
{
    /*只有类型，大小都相符的regular文件，才比较内容*/
    if(type == FTW_F && (statptr->st_mode & S_IFMT) == S_IFREG && statptr->st_size == filesize)  
    {
        int fd;
        if((fd = open(pathname, O_RDONLY, FILE_MODE)) == -1)
            return 0;    /*不输出异常，直接返回*/
        if(read(fd, comparebuf, filesize) != filesize)
            err_sys("read error:%s\n", pathname);
        close(fd);

        if(strcmp(filebuf, comparebuf) == 0)
        {
            if(strcmp(inputfilepath, pathname) != 0)
            {
                //???
                filefindcount++;
                printf("%s\n", pathname);
            }
        }
    }   

    return(0);
}


/*找出与指定文件有  相同文件名  的文件*/
static int myfuncCompareName(const char *pathname, const struct stat *statptr, int type)
{
    if(type == FTW_F)
    {
        const char *split;
        split = getFileName(pathname);

        if(strcmp(inputfilename, split) == 0)
        {
            filefindcount++;
            printf("%s\n", pathname);
        }
    }
    return 0;
}


/*获得指定文件的绝对路径*/
static char* getRealPath(const char *pathname, char* realpath)
{
    int len;
    char *dirpath, *ptr;

    dirpath = path_alloc(&len);    /*记录原来的目录*/
    if(getcwd(dirpath, len) == NULL)
        err_sys("getcwd failed\n");
        
//    printf("dirpath_____________%s\n", dirpath);
    if(chdir(pathname) < 0)
        err_sys("chdir failed\n");
    if(getcwd(realpath, len) == NULL)    /*记录指定目录的绝对路径*/
        err_sys("getcwd failed\n");
    
    ptr = realpath + strlen(realpath);
    if(*(ptr-1) != '/')    /*保证目录的绝对路径以斜杠结尾*/
    {
        *ptr++ = '/';
        *ptr = 0;
    }

    if(chdir(dirpath) < 0)    /*回到之前记录的原来目录*/
        err_sys("chdir failed\n");
    //printf("realpath_____________%s\n", realpath);
    return realpath;
}

/*分离目录和文件名*/
/*因为pathname是const char* 类型，不能赋值给char*类型
 * 所以直接返回类型定为const char*类型 */
static const char* getFileName(const char* pathname)
{
    const char *split;
    split = pathname + strlen(pathname);
    while(--split >= pathname && *split != '/') {}
    split++;
    return split;
}

