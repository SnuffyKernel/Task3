#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

void print_permissions(__mode_t mode)
{
    char perms[10];
    strcpy(perms, "---------");

    if (mode & S_IRUSR)
        perms[0] = 'r';
    if (mode & S_IWUSR)
        perms[1] = 'w';
    if (mode & S_IXUSR)
        perms[2] = 'x';
    if (mode & S_IRGRP)
        perms[3] = 'r';
    if (mode & S_IWGRP)
        perms[4] = 'w';
    if (mode & S_IXGRP)
        perms[5] = 'x';
    if (mode & S_IROTH)
        perms[6] = 'r';
    if (mode & S_IWOTH)
        perms[7] = 'w';
    if (mode & S_IXOTH)
        perms[8] = 'x';

    printf("Symbolic: %s\n", perms);
    printf("Octal: %o\n", mode & 0777);
    printf("Binary: ");
    for (int i = 8; i >= 0; i--)
    {
        printf("%c", (mode & (1 << i)) ? '1' : '0');
    }
    printf("\n");
}

void set_permissions(const char *path, __mode_t mode)
{
    if (chmod(path, mode) == -1)
    {
        perror("chmod");
        exit(EXIT_FAILURE);
    }
}

__mode_t digit_to_mode(const char *perms)
{
    __mode_t mode = 0;

    for (int i = 0, s_ir = 0400, s_iw = 0200, s_ix = 0100; i < 3; i++, s_ir >>= 3, s_iw >>= 3, s_ix >>= 3)
    {
        if (perms[i] == '1')
            mode |= s_ix;
        if (perms[i] == '2')
            mode |= s_iw;
        if (perms[i] == '3')
            mode |= s_ix | s_iw;
        if (perms[i] == '4')
            mode |= s_ir;
        if (perms[i] == '5')
            mode |= s_ir | s_ix;
        if (perms[i] == '6')
            mode |= s_ir | s_iw;
        if (perms[i] == '7')
            mode |= s_ir | s_iw | s_ix;
    }

    return mode;
}

__mode_t symbolic_to_mode(const char *perms)
{
    __mode_t mode = 0;

    if (isdigit(perms[0]))
    {
        return digit_to_mode(perms);
    }
    else
    {
        if (perms[0] == 'r')
            mode |= S_IRUSR;
        if (perms[1] == 'w')
            mode |= S_IWUSR;
        if (perms[2] == 'x')
            mode |= S_IXUSR;
        if (perms[3] == 'r')
            mode |= S_IRGRP;
        if (perms[4] == 'w')
            mode |= S_IWGRP;
        if (perms[5] == 'x')
            mode |= S_IXGRP;
        if (perms[6] == 'r')
            mode |= S_IROTH;
        if (perms[7] == 'w')
            mode |= S_IWOTH;
        if (perms[8] == 'x')
            mode |= S_IXOTH;
    }
    return mode;
}

void usage(const char *prog_name)
{
    fprintf(stderr, "Usage: %s <file>\n", prog_name);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        usage(argv[0]);
    }

    const char *file = argv[1];
    struct stat st;

    if (stat(file, &st) == -1)
    {
        perror("stat");
        exit(EXIT_FAILURE);
    }

    print_permissions(st.st_mode);

    printf("Enter new permissions: ");
    char new_perms[10];
    scanf("%9s", new_perms);

    __mode_t new_mode = symbolic_to_mode(new_perms);
    print_permissions(new_mode);

    printf("Apply changes? (y/n): ");
    char choice;
    scanf(" %c", &choice);

    if (choice == 'y')
    {
        set_permissions(file, new_mode);
    }

    return 0;
}