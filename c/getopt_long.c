#include <stdio.h>     /* for printf */
#include <stdlib.h>    /* for exit */
#include <getopt.h>

extern char *optarg;
extern int optind, opterr, optopt;

int option_index = 0;
static struct option long_options[] = {
                                        {"add", 1, NULL, 'a'},
                                        {"delete", 1, NULL, 'd'},
                                        {"verbose", 0, NULL, 'v'},
                                        {"create", 1, NULL, 'c'},
                                        {"file", 1, NULL, 0},
                                        {"option", 2, NULL, 0},
                                        {NULL, 0, NULL, 0}
                                      };


int main(int argc, char **argv)
{
    int c;
    int digit_optind = 0;

    while (1) 
    {
        c = getopt_long(argc, argv, "a:c:d:012v", long_options, &option_index);
        if (c == -1)
            break;

        switch (c) 
        {
            case 0:
                //option=gggg         wrong:option gggg
                printf("option %s", long_options[option_index].name);
                if (optarg)
                    printf(" with arg %s", optarg);
                printf("\n");
                break;

            case '0':
            case '1':
            case '2':
                printf("option %c\n", c);
                break;

            case 'a':
                printf("option a with value '%s'\n", optarg);
                break;

            case 'c':
                printf("option c with value '%s'\n", optarg);
                break;

            case 'd':
                printf("option d with value '%s'\n", optarg);
                break;
            case 'v':
                printf("option v\n");
                break;

            case '?':
                printf("no in option\n");
                break;

            default:
                printf("?? getopt returned character code 0%o ??\n", c);
        }
    }

    if (optind < argc) 
    {
        printf("non-option ARGV-elements: ");
        while (optind < argc)
            printf("%s ", argv[optind++]);
        printf("\n");
    }

    exit(EXIT_SUCCESS);
}

