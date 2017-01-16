#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "curl/curl.h"
#include "myhtml/api.h"

#include "todoist.h"

char* load_file(char const* path)
{
    char* buffer = 0;
    long length;
    FILE * f = fopen (path, "rb");

    if (f)
    {
      fseek (f, 0, SEEK_END);
      length = ftell (f);
      fseek (f, 0, SEEK_SET);
      buffer = (char*)malloc ((length+1)*sizeof(char));
      if (buffer)
      {
        fread (buffer, sizeof(char), length, f);
      }
      fclose (f);
    }
    buffer[length] = '\0';

    return buffer;
}

void loadWebPageToFile(char *url, char *filename) {
    CURL *curl;
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

struct hrefItem
{
    char *text;
    char *link;
};

int main(int argc, char const *argv[])
{
    loadWebPageToFile("https://habrahabr.ru/", "page.html");

    char *html = load_file("page.html");
    char *attr_value = "post__title_link";
    char *token = "11698c95ca5b3426b88e0ce4a5822d7cd43ecbe4";
    char *TOKEN = "11698c95ca5b3426b88e0ce4a5822d7cd43ecbe4";
    char *filename = "todo.txt";


    myhtml_t* myhtml = myhtml_create();
    myhtml_init(myhtml, MyHTML_OPTIONS_DEFAULT, 1, 0);

    myhtml_tree_t* tree = myhtml_tree_create();
    myhtml_tree_init(tree, myhtml);

    myhtml_parse(tree, MyHTML_ENCODING_UTF_8, html, strlen(html));

    myhtml_collection_t *collection = NULL;


    collection = myhtml_get_nodes_by_attribute_value(tree, NULL, NULL, false,
                                                             "class", strlen("class"),
                                                             attr_value, strlen(attr_value), NULL);

    if(collection) {
        for(size_t i = 0; i < collection->length; i++) {
            myhtml_tree_attr_t *gets_attr = myhtml_attribute_by_key(collection->list[i], "href", strlen("href"));
            const char *attr_char = myhtml_attribute_value(gets_attr, NULL);
            collection->list[i] = myhtml_node_child(collection->list[i]);
            const char *text = myhtml_node_text(collection->list[i], NULL);
            printf("%s  %s\n", attr_char, text);
        }
            
        printf("Total found: %zu\n", collection->length);
    }
    myhtml_collection_destroy(collection);
    myhtml_tree_destroy(tree);
    myhtml_destroy(myhtml);

    // todoist here
    projectarray proj;
    proj = getProjects("11698c95ca5b3426b88e0ce4a5822d7cd43ecbe4", "todo.txt");
    todoproject *projects = proj.projects;
    for (int i = 0; i < proj.size; i++)
    {
        printf("%s %ld\n", projects[i].name, projects[i].id);
    }

    free(proj.projects);
    return 0;
}