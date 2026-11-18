#include "path.h"

#include <stdlib.h>
#include <string.h>

char* get_config_filepath() {
    char* conf_dir_base = getenv("XDG_CONFIG_HOME");
    if (conf_dir_base == NULL || strcmp(conf_dir_base, "")) {
        char* home_dir = getenv("SUDO_HOME");
        char* temp[(sizeof home_dir + sizeof ".config/") * sizeof(char)] = {0};
        strcpy((char*) temp, home_dir);
        
        conf_dir_base = strdup(strcat((char*) temp, "/.config"));
    }

    char* config_path = malloc((strlen(conf_dir_base) + sizeof "/scimitar_se_remapper/binds.conf") * sizeof(char));
    strcpy(config_path, conf_dir_base);
    strcat(config_path, "/scimitar_se_remapper/binds.conf");
    free(conf_dir_base);

    return config_path;
}