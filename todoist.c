
#include "todoist.h"

#include <curl/curl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "parson/parson.h"

#define MAX_SIZE 300


void requestToFile(char *filename, char *url) {
    CURL *curl;
    puts(url);
    FILE *fp;
    CURLcode res;
    curl = curl_easy_init();                                                                                                                                                                                                                                                           
    if (curl)
    {   
        fp = fopen(filename,"wb");
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(fp);
    }   
}

projectarray parseJsonProject(char *pureJsonFile) {
    JSON_Value *root_value;
    JSON_Object *rootObj;
    JSON_Object *project;
    JSON_Array *listProjects;

    char *projectName;
    long projectId;

    root_value = json_parse_file(pureJsonFile);
    rootObj = json_value_get_object(root_value);
    listProjects = json_object_get_array(rootObj, "projects");
    int projectLen = json_array_get_count(listProjects);
    projectarray mas;
    todoproject *projects = (todoproject *) malloc(sizeof(todoproject) * projectLen);

    for (int i = 0; i < json_array_get_count(listProjects); i++) {
        todoproject projectItem;
        project = json_array_get_object(listProjects, i);
        projectName = (char *) json_object_get_string(project, "name");
        projectId = json_object_get_number(project, "id");
        // printf("%s %ld\n", projectName, projectId);
        projectItem.name = projectName;
        projectItem.id = projectId;
        projects[i] = projectItem;
    }
    mas.projects = projects;
    mas.size = projectLen;
    printf("SIZE = %d\n", mas.size);
    return mas;
}

projectarray getProjects(char *token, char *filename) {
	char *beginrequest = "https://todoist.com/API/v7/sync?token=";
    char *endrequest  = "&sync_token=\'*\'&resource_types=[\"projects\"]";
    int strLen = strlen(token) + strlen(beginrequest) + strlen(endrequest);
    char target[strLen];
    strcat(target, beginrequest);
    strcat(target, token);
    strcat(target, endrequest);
    char *str = target;
    printf("%s\n", str);
    // requestToFile(filename, str);
    requestToFile(filename, "https://todoist.com/API/v7/sync?token=11698c95ca5b3426b88e0ce4a5822d7cd43ecbe4&sync_token=\'*\'&resource_types=[\"projects\"]");
    return parseJsonProject(filename);
}
