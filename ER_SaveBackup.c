#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void copy_file(const char *source, const char *destination) {
    FILE *src = fopen(source, "rb");
    FILE *dest = fopen(destination, "wb");
    if (src == NULL || dest == NULL) {
        perror("Error opening file");
        return;
    }
    char buffer[1024];
    size_t bytes;
    while ((bytes = fread(buffer, 1, sizeof(buffer), src)) > 0) {
        fwrite(buffer, 1, bytes, dest);
    }
    fclose(src);
    fclose(dest);
}

int main() {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char short_date[100];
    snprintf(short_date, sizeof(short_date), "%02d-%02d-%04d %02d.%02d.%02d", 
             tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, 
             tm.tm_hour, tm.tm_min, tm.tm_sec);

    char windows_user[256];
    snprintf(windows_user, sizeof(windows_user), "%s", getenv("USERNAME"));

    char dir_path[512];
    snprintf(dir_path, sizeof(dir_path), "C:\\Users\\%s\\AppData\\Roaming\\EldenRing", windows_user);

    struct dirent *entry;
    DIR *dp = opendir(dir_path);
    if (dp == NULL) {
        perror("opendir");
        return 1;
    }

    while ((entry = readdir(dp))) {
        if (entry->d_type == DT_DIR) {
            char steam_num[256];
            snprintf(steam_num, sizeof(steam_num), "%s", entry->d_name);
            if (strcmp(steam_num, ".") != 0 && strcmp(steam_num, "..") != 0) {
                char path_input[512];
                snprintf(path_input, sizeof(path_input), "C:\\Users\\%s\\AppData\\Roaming\\EldenRing\\%s\\ER0000.sl2", windows_user, steam_num);
                
                char path_output[512];
                snprintf(path_output, sizeof(path_output), "C:\\Users\\%s\\AppData\\Roaming\\EldenRing\\%s\\BACKUP %s ER0000.sl2", windows_user, steam_num, short_date);
                
                copy_file(path_input, path_output);
            }
        }
    }
    closedir(dp);

    sleep(1);
    system("\"C:\\Program Files (x86)\\Steam\\steamapps\\common\\ELDEN RING\\Game\\eldenring.exe\"");

    return 0;
}