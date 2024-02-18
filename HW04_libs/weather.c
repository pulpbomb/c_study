#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>
#include <cjson/cJSON_Utils.h>

int main(int argc, char* argv[]) {
        if (argc != 2) {
                printf("Usage: %s <cityname>\n", argv[0]);
                return errno;
        }
        return 0;
}