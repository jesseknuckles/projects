//Jesse Knuckles, I pledge my honor that I have abided by Stevens Honor System.
#define _DEFAULT_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <errno.h>
#include <sys/wait.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>


#define BLUE "\x1b[34;1m"
#define DEFAULT "\x1b[0m"
#define GREEN   "\033[32m"

volatile int sig = 0;

void sighandler(int);
void sighandler(int sign) {
    sig = 1;
}

char* oct_to_permission(mode_t); 

char* oct_to_permission(mode_t m) {

    char* perm = (char*) malloc(9);

    mode_t temp = m;

    mode_t u = ((int)m & 448) >> 6;
    char* us = (char*) malloc(4);
    if ((int)u == 7) {
        strcat(us,"rwx");
    }
    if ((int)u == 6) {
        strcat(us,"rw-");
    }
    if ((int)u == 5) {
        strcat(us,"r-x");
    }
    if ((int)u == 4) {
        strcat(us,"r--");
    }
    if ((int)u == 3) {
        strcat(us,"-wx");
    }
    if ((int)u == 2) {
        strcat(us,"-w-");
    }
    if ((int)u == 1) {
        strcat(us,"--x");
    }
    if ((int)u == 0) {
        strcat(us,"---");
    }

    m = temp;

    mode_t u1 = ((int)m & 56) >> 3;

    char* o = (char*) malloc(4);

    if ((int)u1 == 7) {
        strcat(o,"rwx");
    }
    if ((int)u1 == 6) {
        strcat(o,"rw-");
    }
    if ((int)u1 == 5) {
        strcat(o,"r-x");
    }
    if ((int)u1 == 4) {
        strcat(o,"r--");
    }
    if ((int)u1 == 3) {
        strcat(o,"-wx");
    }
    if ((int)u1 == 2) {
        strcat(o,"-w-");
    }
    if ((int)u1 == 1) {
        strcat(o,"--x");
    }
    if ((int)u1 == 0) {
        strcat(o,"---");
    }

    m = temp;

    mode_t u2 = (int)m & 7;

    char* g = (char*) malloc(4);
    if ((int)u2 == 7) {
        strcat(g,"rwx");
    }
    if ((int)u2 == 6) {
        strcat(g,"rw-");
    }
    if ((int)u2 == 5) {
        strcat(g,"r-x");
    }
    if ((int)u2 == 4) {
        strcat(g,"r--");
    }
    if ((int)u2 == 3) {
        strcat(g,"-wx");
    }
    if ((int)u2 == 2) {
        strcat(g,"-w-");
    }
    if ((int)u2 == 1) {
        strcat(g,"--x");
    }
    if ((int)u2 == 0) {
        strcat(g,"---");
    }

    strcat(perm, us);
    strcat(perm, o);
    strcat(perm, g);
    return perm;

}


void print_cur_dir();
void print_cur_dir() {
    char* curr_path = getcwd(NULL, 0);
    if (curr_path == NULL) {
        fprintf(stderr, "Error: Cannot get current working directory. %s.\n", strerror(errno));
    }
    fprintf(stdout, "%s[%s]%s> ", BLUE, curr_path, DEFAULT); 
    free(curr_path);
}
void stat_print(char* filename, struct stat* sta);
void stat_print(char* filename, struct stat* sta) {
    struct passwd *pws;
    pws = getpwuid(sta->st_uid);
    char filetype[20] = "";
    if (S_ISREG(sta->st_mode)) {
        strcat(filetype, "regular file");
    }
    if (S_ISDIR(sta->st_mode)) {
        strcat(filetype, "directory");
    }
    if (S_ISCHR(sta->st_mode)) {
        strcat(filetype, "character device");
    }
    if (S_ISBLK(sta->st_mode)) {
        strcat(filetype, "block device");
    }
    if(S_ISFIFO(sta->st_mode)) {
        strcat(filetype, "FIFO");
    }

    char* perm = oct_to_permission((int)(sta->st_mode) & 511);

    fprintf(stdout, "File: %s\n", filename);
    fprintf(stdout, "Size: %ld     ", sta->st_size);
    fprintf(stdout, "Blocks: %ld     ", sta->st_blocks);
    fprintf(stdout, "IO Block: %d     ", sta->st_blksize);
    fprintf(stdout, "%s\n", filetype);
    fprintf(stdout, "Device: %loh/%ldd     ", sta->st_dev, sta->st_dev);
    fprintf(stdout, "Inode: %ld     ", sta->st_ino);
    fprintf(stdout, "Links: %d\n", sta->st_nlink);
    fprintf(stdout, "Access: %d %s    ", sta->st_mode & 4095, perm);
    fprintf(stdout, "Uid: ( %d/ %s)     ", sta->st_uid, pws->pw_name);
    fprintf(stdout, "Gid: ( %d/ %s)\n", sta->st_gid, pws->pw_name);
    fprintf(stdout, "Access: %ld nanoseconds\n", sta->st_atime);
    fprintf(stdout, "Modify: %ld nanoseconds \n", sta->st_mtime);
    fprintf(stdout, "Change: %ld nanoseconds \n", sta->st_ctime);
    fprintf(stdout, "Birth: -\n");
    free(perm);

}

int main(int argc, char** argv) {

    struct sigaction signal;
    signal.sa_handler = sighandler;

    if (sigaction(SIGINT, &signal, NULL) == -1) {
        fprintf(stdout, "Error: Cannot register signal handler. %s.\n", strerror(errno));
        exit(errno);
    }

    char user_input[1024];
    size_t len = 0;


    while (1) {
        if (sig == 1) {
            sig = 0;
            fprintf(stdout,"\n");
            continue;
        }
        sig = 0;
        print_cur_dir(); //may be error to check.
        if (fgets(user_input, 1024, stdin) == NULL) {
            if (sig == 1) {
                sig = 0;
                fprintf(stdout, "\n");
                continue;
            }
            fprintf(stderr, "Error: Failed to read from stdin. %s.\n", strerror(errno));
            continue;
        }
        if (strcmp(user_input,"exit\n") == 0) {
            exit(EXIT_SUCCESS);
        }

        if (strcmp(user_input, "cd\n") == 0 || strcmp(user_input, "cd ~\n") == 0) {
            uid_t user = getuid();
            struct passwd* pwd = getpwuid(user);
            char* curr_dir = pwd->pw_dir;
            if (pwd == NULL) {
                fprintf(stderr, "Error: Cannot get passwd entry. %s.\n", strerror(errno));
                continue;
            }
            if (chdir(curr_dir) == -1) {
                fprintf(stderr,"Error: Cannot change directory to %s. %s.\n", pwd->pw_dir, strerror(errno));
                continue;
            }
        }
        else if (user_input[0] == 'c' && user_input[1] == 'd') {
            if (sig == 1) {
                continue;
            }
            int count_cd_arg = 0;
            int cd_index = 0;
            while(user_input[cd_index] != '\n') {
                if (user_input[cd_index] == ' ') {
                    count_cd_arg++;
                }
                cd_index++;
            }
            if (count_cd_arg > 1) { 
                fprintf(stderr, "Error: Too many arguments to cd.\n");
                continue;
            }
            user_input[strlen(user_input)-1] = 0;
            if (chdir(&user_input[3]) == -1) { //wont let me into any directories

                fprintf(stderr,"Error: Cannot change directory to '%s'. %s.\n", &user_input[3], strerror(errno));
                continue;
            }
        }
        else if (user_input[0] == 'l' && user_input[1] == 's' && (user_input[2] == '\n' || user_input[3] != '-')) {
            DIR* dir;
            struct dirent* dirp;

            struct stat sta;

            char* curr_dir = getcwd(NULL, 0);
            if (curr_dir == NULL) {
                fprintf(stderr, "Error: Cannot get current working directory. %s.\n", strerror(errno));
                continue;
            }

            if (user_input[2] == '\n') {
                dir = opendir(curr_dir);
                while ((dirp = readdir(dir)) != NULL) {
                    if ((dirp->d_name)[0] == '.') {
                        continue;
                    }
                    stat(dirp->d_name, &sta);
                    if (S_ISDIR(sta.st_mode)) {
                        if (strcmp(dirp->d_name, ".") == 0|| strcmp(dirp->d_name, "..")==0) {
                            continue;
                        }
                        else {
                        fprintf(stdout, "%s%s%s\n", GREEN, dirp->d_name, DEFAULT); }
                    }
                    else {
                        fprintf(stdout, "%s\n", dirp->d_name);
                    }
                }
                closedir(dir);
                free(curr_dir);
            }
            else {
                struct stat sta;
                int count_cd_arg = 0;
                int cd_index = 0;
                while(user_input[cd_index] != '\n') {
                    if (user_input[cd_index] == ' ') {
                        count_cd_arg++;
                    }
                    cd_index++;
                }
                if (count_cd_arg > 1) { 
                    fprintf(stderr, "Error: Too many arguments to ls.\n");
                    continue;
                }
                user_input[strlen(user_input)-1] = 0;
                dir = opendir(&user_input[3]);
                if (dir == NULL) {
                    fprintf(stderr, "Error: cannot open directory. %s.\n", strerror(errno));
                    continue;
                }
                while ((dirp = readdir(dir)) != NULL) {
                    if ((dirp->d_name)[0] == '.') {
                        continue;
                    }
                    stat(dirp->d_name, &sta);
                    if (S_ISDIR(sta.st_mode)) {
                        if (strcmp(dirp->d_name, ".") == 0|| strcmp(dirp->d_name, "..")==0) {
                            continue;
                        }
                        else {
                        fprintf(stdout, "%s%s%s\n", GREEN, dirp->d_name, DEFAULT); }
                    }
                    else {
                        fprintf(stdout, "%s\n", dirp->d_name);
                    }
                }
                closedir(dir);
                free(curr_dir);
            }
        }
        else if (user_input[0] == 's' && user_input[1] == 't' && user_input[2] == 'a' && user_input[3] == 't'){
                struct stat sta;
                int j = 0;
                int* arg_locations = (int*) malloc(4 * ((strlen(user_input)/2) + 1)); //catch error.
                    int count_arg = 0;
                    arg_locations[count_arg] = 0;
                    while (user_input[j] != '\n') {
                        if (user_input[j] == ' ') {
                            user_input[j] = 0;
                            j++;
                            count_arg++;
                            arg_locations[count_arg] = j;
                        }
                        j++;
                    }
                    if (count_arg > 2) {
                        fprintf(stderr, "Error: Too many arguments to stat.\n");
                        continue;
                    }
                    user_input[j] = 0;
                    char* arg_list[count_arg];
                    for (int z = 0; z <= count_arg; z++) {
                        arg_list[z] = &user_input[arg_locations[z]];
                    }
                    if (count_arg == 1) {
                        if (stat(arg_list[1], &sta) != 0) {
                            fprintf(stderr, "Error: %s is not a file. %s.\n", arg_list[1], strerror(errno));
                            continue;
                        }
                        stat_print(arg_list[1], &sta);
                    }
                    else if (count_arg == 2) {
                        char filename[strlen(arg_list[1]) + 1 + strlen(arg_list[2])];
                        strcat(filename, arg_list[1]);
                        strcat(filename, "/");
                        strcat(filename, arg_list[2]);
                        if (stat(filename, &sta) != 0) {
                            fprintf(stderr, "Error: %s is not a file. %s.\n", filename, strerror(errno));
                            continue;
                        }
                        stat_print(arg_list[2], &sta);
                    }
        }
        else if (user_input[0] == 'f' && user_input[1] == 'i' && user_input[2] == 'n' && user_input[3] == 'd') { 
            DIR* dir;
            struct dirent* dirp;
            char* curr_dir = getcwd(NULL, 0);
            if (curr_dir == NULL) {
                fprintf(stderr, "Error: Cannot get current working directory. %s.\n", strerror(errno));
                continue;
            }
                int j = 0;
                int* arg_locations = (int*) malloc(4 * ((strlen(user_input)/2) + 1)); //catch error.
                    int count_arg = 0;
                    arg_locations[count_arg] = 0;
                    while (user_input[j] != '\n') {
                        if (user_input[j] == ' ') {
                            user_input[j] = 0;
                            j++;
                            count_arg++;
                            arg_locations[count_arg] = j;
                        }
                        j++;
                    }
                    if (count_arg > 2) {
                        fprintf(stderr, "Error: Too many arguments to find.\n");
                        continue;
                    }
                    user_input[j] = 0;
                    char* arg_list[count_arg];
                    for (int z = 0; z <= count_arg; z++) {
                        arg_list[z] = &user_input[arg_locations[z]];
                    }
                    if (count_arg == 1) {
                        dir = opendir(curr_dir);
                        if (dir == NULL) {
                    fprintf(stderr, "Error: cannot open directory. %s.\n", strerror(errno));
                    continue;
                }
                int found = 0;
                while ((dirp = readdir(dir)) != NULL) {
                    if (strcmp(dirp->d_name,arg_list[1]) == 0) {
                        fprintf(stdout, "%s\n", dirp->d_name);
                        closedir(dir);
                        found = 1;
                    }
                    }
                if (found == 0) {
                fprintf(stderr, "find: '%s': No such file or directory\n", arg_list[1]);
                }
                continue;
                }
                else {
                    dir = opendir(arg_list[1]);
                    if (dir == NULL) {
                    fprintf(stderr, "Error: cannot open directory. %s.\n", strerror(errno));
                    continue;
                }
                int found = 0;
                while ((dirp = readdir(dir)) != NULL) {
                    if (strcmp(dirp->d_name,arg_list[2]) == 0) {
                        fprintf(stdout, "%s\n", dirp->d_name);
                        closedir(dir);
                        found = 1;
                    }
                    }
                if (found == 0) {
                fprintf(stderr, "find: '%s': No such file or directory\n", arg_list[2]);
                }
                continue;
                }
        }
        else { 
            pid_t pid;
            int wait_res;
            if ((pid = fork()) == 0) {
                int j = 0;
                int* arg_locations = (int*) malloc(4 * ((strlen(user_input)/2) + 1)); //catch error.
                    int count_arg = 0;
                    arg_locations[count_arg] = 0;
                    while (user_input[j] != '\n') {
                        if (user_input[j] == ' ') {
                            user_input[j] = 0;
                            j++;
                            count_arg++;
                            arg_locations[count_arg] = j;
                        }
                        j++;
                    }
                    user_input[j] = 0;
                    char* arg_list[count_arg + 1];
                    for (int z = 0; z <= count_arg; z++) {
                        arg_list[z] = &user_input[arg_locations[z]];
                    }
                    arg_list[count_arg + 1] = NULL;
                    char path[5 + strlen(arg_list[0])];
                    if(arg_list[0][0] == '.' && arg_list[0][1] == '/') {
                        strcat(path, arg_list[0]);
                    }
                    else {
                    strcat(path, "/bin/");
                    strcat(path, arg_list[0]); }
                    free(arg_locations);
                    if (execv(path, arg_list) == -1) {
                        fprintf(stderr, "Error: exec() failed. %s.\n", strerror(errno));
                        exit(errno);
                    }
                }
            else if (pid < 0) {
                fprintf(stderr, "Error: fork() failed. %s.\n", strerror(errno));
                continue;
            }
            else {
                if (wait(&wait_res) == -1 && sig ==0) {
                    fprintf(stderr, "Error: wait() failed. %s.\n", strerror(errno));
                    continue;
                }
            }
        }
    }
    return 0;
    
}