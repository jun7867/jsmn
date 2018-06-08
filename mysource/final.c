#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../jsmn.h"

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
			strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}

char *readJSONFILE(){
	FILE *fp;
	int count=0;
	char line[128];
	char filename[128];
	fp=fopen("data.json","rt"); //파일 열기
	if(fp==NULL){
		printf("Error file not open \n");
		return NULL;
	}
	static char *JSON_STRING;
	JSON_STRING=(char *)malloc(sizeof(line));

	while(1){
		fgets(line,sizeof(line),fp);
		if(feof(fp)) break;
		count +=strlen(line);
		JSON_STRING=(char *)realloc(JSON_STRING,count+1);
		strcat(JSON_STRING,line);
	}
	fclose(fp);
  return JSON_STRING;
}

void jsonNamelist(char *JSON_STRING,jsmntok_t *t,int tokcount){
	int i;
	int count=0;
	printf("*********NAME LIST********\n");
	for(i=0;i<tokcount;i++){
		if (t[i].size >0 && t[i].type ==JSMN_STRING){
			printf("[NAME %d] %.*s \n",count+1,t[i].end-t[i].start,JSON_STRING + t[i].start);
			count++;
	}
	}
}

int main() {
	int i;
	int r;
	char* JSON_STRING=readJSONFILE();
	jsmn_parser p;
	jsmntok_t t[128]; /* We expect no more than 128 tokens */

	jsmn_init(&p);
	r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t, sizeof(t)/sizeof(t[0]));
	if (r < 0) {
		printf("Failed to parse JSON: %d\n", r);
		return 1;
	}

	/* Assume the top-level element is an object */
	if (r < 1 || t[0].type != JSMN_OBJECT) {
		printf("Object expected\n");
		return 1;
	}
	jsonNamelist(JSON_STRING,t,r);
	/* Loop over all keys of the root object */
	// for (i = 1; i < r; i++) {
	// 	if (jsoneq(JSON_STRING, &t[i], "name") == 0) {
	// 		/* We may use strndup() to fetch string value */
	// 		printf("- User: %.*s\n", t[i+1].end-t[i+1].start,
	// 				JSON_STRING + t[i+1].start);
	// 		i++;
	// 	} else if (jsoneq(JSON_STRING, &t[i], "keywords") == 0) {
	// 		/* We may additionally check if the value is either "true" or "false" */
	// 		printf("- Admin: %.*s\n", t[i+1].end-t[i+1].start,
	// 				JSON_STRING + t[i+1].start);
	// 		i++;
	// 	} else if (jsoneq(JSON_STRING, &t[i], "description") == 0) {
	// 		/* We may want to do strtol() here to get numeric value */
	// 		printf("- UID: %.*s\n", t[i+1].end-t[i+1].start,
	// 				JSON_STRING + t[i+1].start);
	// 		i++;
	// 	} else if (jsoneq(JSON_STRING, &t[i], "examples") == 0) {
	// 		int j;
	// 		if (t[i+1].type != JSMN_ARRAY) {
	// 			continue; /* We expect groups to be an array of strings */
	// 		}
	// 		for (j = 0; j < t[i+1].size; j++) {
	// 			jsmntok_t *g = &t[i+j+2];
	// 			printf("  * %.*s\n", g->end - g->start, JSON_STRING + g->start);
	// 		}
	// 		i += t[i+1].size + 1;
	// 	}
	// }
	return EXIT_SUCCESS;
}
