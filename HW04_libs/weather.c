
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>
#include <cjson/cJSON_Utils.h>

struct MemoryStruct {
        char *memory;
        size_t size;
};

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp){
        size_t realsize = size * nmemb;
        struct MemoryStruct *mem = (struct MemoryStruct *)userp;
 
        char *ptr = realloc(mem->memory, mem->size + realsize + 1);
        
        if(!ptr) {
                printf("not enough memory (realloc returned NULL)\n");
                return 0;
        }
 
        mem->memory = ptr;
        memcpy(&(mem->memory[mem->size]), contents, realsize);
        mem->size += realsize;
        mem->memory[mem->size] = 0;
 
        return realsize;
}

int main(int argc, char *argv[]){
        
        setlocale(LC_ALL, "ru_RU.UTF-8");

        if (argc != 2) {
                printf("Usage: %s <cityname>\n", argv[0]);
                return errno;
        }
        
        char *full_url = (char *)malloc(strlen("https://wttr.in/") + strlen(argv[1]) + strlen("?format=j1") + 1);
        
        if (full_url == NULL) {
                fprintf(stderr, "Memory allocation failed.\n");
        }
        
        strcpy(full_url, "https://wttr.in/");
        strcat(full_url, argv[1]);
        strcat(full_url, "?format=j1");

        printf("full_url: %s\n", full_url);
        
        CURL *curl_handle;
        CURLcode res;
        struct MemoryStruct chunk;
        chunk.memory = malloc(1);
        chunk.size = 0;
        curl_global_init(CURL_GLOBAL_ALL);
        curl_handle = curl_easy_init();
        curl_easy_setopt(curl_handle, CURLOPT_URL, full_url);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
        curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
        res = curl_easy_perform(curl_handle);
        if(res != CURLE_OK) {
                fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
        }
        //printf("%s\n", chunk.memory);

        cJSON *root = cJSON_Parse(chunk.memory);
        cJSON *current_condition = cJSON_GetObjectItem(root, "current_condition");
        cJSON *temp_C = cJSON_GetObjectItem(current_condition, "temp_C");
        printf("Температура на сегодняшний день: %d градусов Цельсия\n", temp_C->valueint);
        cJSON_Delete(root);

        curl_easy_cleanup(curl_handle);
        free(chunk.memory);
        free(full_url);
        curl_global_cleanup();
        return 0;
}