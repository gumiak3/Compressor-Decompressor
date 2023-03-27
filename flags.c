#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

int main(int argc, char *argv[]) {
    int opt;
    int option_index = 0;
    int version_8 = 0;
    int version_12 = 0;
    int version_16 = 0;
    int help_needed = 0;
    int info_needed = 0;
    int compress_mode = 0;
    int decompress_mode = 0;

    static struct option long_options[] = {
        {"8", no_argument, NULL, 1},
        {"12", no_argument, NULL, 2},
        {"16", no_argument, NULL, 3},
        {"help", no_argument, NULL, 4},
        {"v", no_argument, NULL, 5},
        {"c", no_argument, NULL, 6},
        {"x", no_argument, NULL, 7},
        {0, 0, 0, 0}
    };

    while((opt = getopt_long(argc, argv, "", long_options, &option_index)) != -1) {
        switch(opt) {
            case 1:
                version_8 = 1;
                break;
            case 2:
                version_12 = 1;
                break;
            case 3:
                version_16 = 1;
                break;
            case 4:
                help_needed = 1;
                break;
            case 5:
                info_needed = 1;
                break;
            case 6:
                compress_mode = 1;
                break;
            case 7:
                decompress_mode = 1;
                break;
            default:
                printf("Invalid flag.\n");
                return 1;
        }
    }
    if(help_needed) {
        printf("This is file compressor/decompressor.\n"
                "<tutaj jakis sposob jak go odpalic>\n"
                "Possible flags:\n"
                "--8, --12, --16 - choose version of algorithm to compress\n"
                "--x, --c - choose mode of program (compress or decompress)\n"
                "-v - show info about compress ratio\n"
                "--help - show this help\n");
        return 0;
    }
    int versionsum = version_8 + version_12 + version_16;
    if(versionsum > 1) {
        printf("Only one version flag can be used.\n"
                "To see help use --help flag.\n");
        return 1;
    }
    if(compress_mode + decompress_mode > 1) {
        printf("Only one mode flag can be used.\n"
                "To see help use --help flag.\n");
        return 1;
    }
    if(decompress_mode == 1 && versionsum > 0) {
        printf("Version flag is used only in compress mode.\n"
                "To see help use --help flag.\n");
        return 1;
    }
    if(versionsum == 0) {
        version_8 = 1;
    }

    return 0;
}
