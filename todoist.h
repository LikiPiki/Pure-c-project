#ifndef TODOIST
#define TODOIST

struct todoistProject {
	char *name;
	long id;
};

char *TOKEN;

typedef struct todoistProject todoproject;

struct projectArray {
	todoproject *projects;
	int size;
};
typedef struct projectArray projectarray;

void requestToFile(char *filename, char *url);

projectarray getProjects(char *token, char *filename);
char* readFileByName(char *filename);

projectarray parseJsonProject(char *pureJsonFile);

#endif