
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
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
                fprintf(stderr, "not enough memory (realloc returned NULL)\n");
                return errno;
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
                fprintf(stderr, "Usage: %s <cityname>\n", argv[0]);
                return errno;
        }
        char *full_url = (char *)malloc(strlen("https://wttr.in/") + strlen(argv[1]) + strlen("?format=j1") + 1);
        if (full_url == NULL) {
                fprintf(stderr, "Memory allocation failed.\n");
                return errno;
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

        cJSON *root = cJSON_Parse(chunk.memory);
        if (root != NULL) {
                cJSON *request = cJSON_GetObjectItem(root, "request");
                cJSON *query = cJSON_GetObjectItem(cJSON_GetArrayItem(request, 0), "query");
                const char *queryStr = query->valuestring;
                if (strstr(queryStr, "Thot Not") != NULL) {
                        fprintf(stderr, "Bad location name. Try city name. Exit.\n");
                        return errno;
                }
                cJSON *current_condition = cJSON_GetObjectItem(root, "current_condition");
                if (current_condition != NULL && cJSON_IsArray(current_condition)) {
                        cJSON *first_condition = cJSON_GetArrayItem(current_condition, 0);
                        if (first_condition != NULL) {
                                cJSON *temp_C = cJSON_GetObjectItem(first_condition, "temp_C");
                                cJSON *winddir16Point = cJSON_GetObjectItem(first_condition, "winddir16Point");
                                cJSON *windspeedKmph = cJSON_GetObjectItem(first_condition, "windspeedKmph");
                                cJSON *weatherDesc = cJSON_GetObjectItem(first_condition, "weatherDesc");
                                cJSON *value = cJSON_GetObjectItem(cJSON_GetArrayItem(weatherDesc, 0), "value");
                                if (cJSON_IsString(temp_C) && cJSON_IsString(winddir16Point) && cJSON_IsString(windspeedKmph) && cJSON_IsString(value)) {
                                        printf("Temperature in %s: %s degrees Celsius\n", argv[1], temp_C->valuestring);
                                        printf("Weather is: %s\n", value->valuestring);
                                        printf("Wind dirrection: %s\n", winddir16Point->valuestring);
                                        printf("Wind speed: %s Kmph\n", windspeedKmph->valuestring);
                                } else {
                                printf("Some data in 'current_condition' not string.\n");
                                }
                        } else {
                                printf("Cant get first element from 'current_condition' array.\n");
                        }
                } else {
                        printf("'current_condition' field are not an array.\n");
                }
                cJSON_Delete(root);
        } else {
                printf("Error parsing JSON.\n");
        }

        curl_easy_cleanup(curl_handle);
        free(chunk.memory);
        free(full_url);
        curl_global_cleanup();
        return 0;
}