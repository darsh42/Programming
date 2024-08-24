#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 50
#define BUFNAME 15

#define FSMAX 70000000
#define UPDATESIZE 30000000

typedef struct file {
    char name[BUFNAME];
    int size;
} file_t;

typedef struct dir {
    char name[BUFNAME];
    file_t *files;
    int num_files;
    struct dir *subdirs;
    int num_subdirs;
} dir_t;

int mkdir(dir_t *pwd, char *path, char *name);
int mkfile(dir_t *top, char *path, char *line);

int change_dir(char *path, char *name);

/* calculates values for questions */
int dir_size(dir_t dir, int *counter);
int delete(dir_t dir, int min_size, int *file_size);
/* misc */
void get_name(char *line, char name[BUFNAME]);

int main(int argc, char* argv[]) {
    if (argc != 2) {printf("specify input!\n"); return -1;}

    char line[BUFSIZE];
    FILE *fp = fopen(argv[1], "r");

    dir_t top = {"/", NULL, 0, NULL, 0};
    char path[100];
    while (fgets(line, BUFSIZE, fp) != NULL) {
        size_t len = strlen(line);

        switch(line[0]) {
            case '$':
                // terminal command
                char cmd[BUFNAME];
                for (int i = 0; line[i] != '\0'; i++)
                    if (line[i] == ' ') {strncpy(cmd, &line[i+1], len - i - 1); break;}

                switch (cmd[0]) {
                    case 'l':
                        break;
                    case 'c':
                        char name[BUFNAME];
                        get_name(line, name);
                        change_dir(path, name);
                        break;
                }

                break;
            case 'd':
                // check if dir exists, if not create
                char name[BUFNAME];
                get_name(line, name);
                mkdir(&top, path, name);
                break;
            default:
                mkfile(&top, path, line);
                break;
        }
    }

    fclose(fp);
    int part1 = 0;
    int part2 = FSMAX;
    int FStotal = dir_size(top, &part1);

    delete(top, UPDATESIZE - (FSMAX - FStotal), &part2);

    printf("part 1: %d\n", part1);
    printf("part 2: %d\n", part2);


    return 0;
}

void get_name(char *line, char name[BUFNAME]) {
    size_t len = strlen(line);

    int end = 0;
    for (int i = len; i >= 0; i--)
        if (line[i] == ' ') {
            strncpy(name, &line[i+1], len - i);
            end = len - i - 2;
            break;
        }

    name[end] = '\0';

    return;
}

dir_t* find_pwd(dir_t *top, char* path);

/* Creates and adds new directory in pwd */
int mkdir(dir_t *top, char* path, char *name) {

    dir_t *pwd = NULL;
    pwd = find_pwd(top, path);

    /* checks if there is a pre-existing directory */
    for (int i = 0; i < pwd->num_subdirs; i++) {
        /* comparing names of current sub-directories and name of new sub-directory */
        if (strcmp(pwd->subdirs[i].name, name) == 0) return 1;
    }

    if (pwd->num_subdirs == 0) {
        pwd->num_subdirs++;
        pwd->subdirs = (dir_t *) malloc(pwd->num_subdirs * sizeof(dir_t));
    } else {
        pwd->num_subdirs++;
        pwd->subdirs = (dir_t *) realloc(pwd->subdirs, pwd->num_subdirs * sizeof(dir_t));
    }


    /* bad error */
    if (pwd->subdirs == NULL) return -1;

    dir_t newdir;
    strncpy(newdir.name, name, strlen(name)+1);
    newdir.num_subdirs = 0;
    newdir.num_files = 0;

    pwd->subdirs[pwd->num_subdirs-1] = newdir;
    return 0;
}

int mkfile(dir_t *top, char *path, char *line) {
    int size = atoi(&line[0]);
    char name[BUFNAME];
    get_name(line, name);
    dir_t *pwd = find_pwd(top, path);
    if (pwd->num_files == 0)  {
        pwd->num_files++;
        pwd->files = (file_t *) malloc(pwd->num_files * sizeof(file_t));
    } else {
        pwd->num_files++;
        pwd->files = (file_t *) realloc(pwd->files, pwd->num_files * sizeof(file_t));
    }

    if (pwd->files == NULL) return -1;

    file_t newfile;
    strncpy(newfile.name, name, strlen(name)+1);
    newfile.size = size;

    pwd->files[pwd->num_files - 1] = newfile;
    return 0;
}


int change_dir(char *path, char *name) {

    // case "cd ../"
    if (name[0] == '.' && name[1] == '.') {
        size_t path_len = strlen(path);
        for (size_t i = path_len; i >=0; i--) {
            if (path[i] == '/' && i != path_len - 1) {
                path[i+1] = '\0';
                return 0;
            }
        }
    }

    if (path[0] != '/') {
        strncpy(path, name, strlen(name));
    } else {
        size_t name_len = strlen(name);
        name[name_len] = '/';
        name[name_len+1] = '\0';

        strcat(path, name);
    }
    return 0;
}

dir_t* find_pwd(dir_t *top, char* path) {
    int index = 0;
    char next_dir[15];

    dir_t *tmp = top;

    for (int i = 0; path[i] != '\0' && index < 15; i++) {
        if (i == 0 && path[i] == '/')
            continue;

        if (path[i] == '/') {
            next_dir[index++] = '\0';
            for (int j = 0; j < tmp->num_subdirs; j++) {
                // if the name found in the subdirs is the same as the next directory
                if (strcmp(tmp->subdirs[j].name, next_dir) == 0) {tmp = &tmp->subdirs[j]; break;}
            }

            index = 0;
            continue;
        }

        next_dir[index++] = path[i];
    }

    return tmp;
}

int dir_size(dir_t dir, int *counter) {
    int total = 0;

    for (int i = 0; i < dir.num_files; i++)
        total += dir.files[i].size;

    for (int i = 0; i < dir.num_subdirs; i++)
        total += dir_size(dir.subdirs[i], counter);

    if (total <= 100000) {
        (*counter)+= total;
    }

    return total;
}

int delete(dir_t dir, int min_size, int *file_size) {
    int total = 0;

    for (int i = 0; i < dir.num_files; i++)
        total += dir.files[i].size;

    for (int i = 0; i < dir.num_subdirs; i++)
        total += delete(dir.subdirs[i], min_size, file_size);

    if (total >= min_size && total < *file_size) {
        (*file_size) = total;
    }

    return total;
}
