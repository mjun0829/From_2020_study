#include <dirent.h>
#include <fcntl.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

void fileMode(const struct stat *fileInfo);
void mTime(const struct stat *fileInfo);

int main(int argc, char const *argv[]) {
    struct stat fileInfo;
    DIR *dirp;
    struct dirent *dirInfo;
    if (argc != 2) {
        printf("Usage: ./main.out [pathname]\n");
        exit(-1);
    } else {
        printf("Directory to search: %s\n", argv[1]);
        if (chdir(argv[1]) == -1) {
            printf("%s does not exist or is not a directory.\n", argv[1]);
            exit(-1);
        }
        dirp = opendir(argv[1]);
        while ((dirInfo = readdir(dirp)) != NULL) {

            stat(dirInfo->d_name, &fileInfo);
            fileMode(&fileInfo);
            printf("%s ", (getpwuid(fileInfo.st_uid))->pw_name);
            printf("%9ld", fileInfo.st_size);
            mTime(&fileInfo);
            printf(" %s\n", dirInfo->d_name);
        }
    }
    return 0;
}

void fileMode(const struct stat *fileInfo) {
    if (S_ISDIR(fileInfo->st_mode)) {
        printf("d");
    } else {
        printf("-");
    }

    //	User
    if (S_IRUSR & fileInfo->st_mode) {
        printf("r");
    } else {
        printf("-");
    }
    if (S_IWUSR & fileInfo->st_mode) {
        printf("w");
    } else {
        printf("-");
    }
    if (S_IXUSR & fileInfo->st_mode) {
        printf("x");
    } else {
        printf("-");
    }
    //	Group
    if (S_IRGRP & fileInfo->st_mode) {
        printf("r");
    } else {
        printf("-");
    }
    if (S_IWGRP & fileInfo->st_mode) {
        printf("w");
    } else {
        printf("-");
    }
    if (S_IXGRP & fileInfo->st_mode) {
        printf("x");
    } else {
        printf("-");
    }
    //	Others
    if (S_IROTH & fileInfo->st_mode) {
        printf("r");
    } else {
        printf("-");
    }
    if (S_IWOTH & fileInfo->st_mode) {
        printf("w");
    } else {
        printf("-");
    }
    if (S_IXOTH & fileInfo->st_mode) {
        printf("x");
    } else {
        printf("-");
    }
    printf("\t");
}

void mTime(const struct stat *fileInfo) {

    time_t seconds = fileInfo->st_mtime;
    time(&seconds);
    printf(" %ld", seconds);
}
