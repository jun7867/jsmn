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

void jsonNamelist(char *JSON_STRING,jsmntok_t *t,int tokcount,int *nameTokIndex){
	int i;
	int count=0;
	for(i=1;i<tokcount+1;i++){
			if(t[i].size >0 && t[i].type==JSMN_STRING){
				nameTokIndex[count]=i;
				count++;
			}
	}
}
void printNameList(char *JSON_STRING,jsmntok_t *t,int *nameTokIndex){
	printf("********NAME LIST******\n");
	int i=0;
	int s=0;
	while(1){
		if(nameTokIndex[i]==0) break;
		s=nameTokIndex[i];
		printf("[NAME %d] %.*s \n",i+1,t[s].end-t[s].start,JSON_STRING + t[s].start);
		i++;
	}
}

// void selecNameList(char *JSON_STRING,jsmntok_t *t,int *nameTokIndex){
// 	int i=0;
// 	int select;
// 	int s=0;
// 	while(1){
// 	printf("Select Name's no : (exit : 0) \n");
// 	scanf("%d",&select);
// 	if(select==0) break;
// 	s=nameTokIndex[select-1];
// 	printf("[NAME %d] %.*s \n",select,t[s].end-t[s].start,JSON_STRING + t[s].start);
// 	printf(" %.*s \n",t[s+1].end-t[s+1].start,JSON_STRING + t[s+1].start);
//
// }
// }
void objectNameList(char *JSON_STRING,jsmntok_t *t,int tokcount,int *objectIndex)
{
	int i=0;
	int j=0;
	printf("********OBJECT LIST******\n");
	for(i=0;i<tokcount;i++){
		if(t[0].type==JSMN_ARRAY){ //type이 array로 시작하면 아래식 진행.
			if(t[i].type==JSMN_OBJECT && t[i-1].size==0 || t[i-1].type==2 ){
				printf("[NAME %d] %.*s \n",j+1,t[i+2].end-t[i+2].start,JSON_STRING + t[i+2].start); // i+2인 이유는 object에서 +2인것이 value이기 떄문에.
				j++; // 출력된것의 번호를 알기 위한것.
			}
		}
		else{ //시작이 array가 아닐때
			if(t[i].type==JSMN_OBJECT && t[i-1].size==0 && t[t[i+1].parent].parent<0){ // 토큰의 타입이 오브젝트일때 4개가 나오는데 거기서 그 전의 사이즈가 0인것이 큰 오브젝트다.

				if(t[i+2].type==2){ // i가 {로 시작하는값이므로 그것의 +2 즉 첫번째 name의 다음이 어레이면 실행.(data4.json일때)
					for(i+=1;i<tokcount;i++){
					if(t[i].type==JSMN_OBJECT && t[i-1].size==0 || t[i-1].type==2){
						printf("[NAME %d] %.*s \n",j+1,t[i+2].end-t[i+2].start,JSON_STRING + t[i+2].start); // i+2인 이유는 object에서 +2인것이 value이기 떄문에.
						j++; // 출력된것의 번호를 알기 위한것.
						}
					}
				}
				else{
					if(t[i].type==JSMN_OBJECT && t[i-1].size==0 && t[t[i+1].parent].parent<0){
						printf("[NAME %d] %.*s \n",j+1,t[i+2].end-t[i+2].start,JSON_STRING + t[i+2].start); // i+2인 이유는 object에서 +2인것이 value이기 떄문에.
						j++; // 출력된것의 번호를 알기 위한것.
				}
			}

				// printf("[NAME %d] %.*s \n",j+1,t[i+2].end-t[i+2].start,JSON_STRING + t[i+2].start); // i+2인 이유는 object에서 +2인것이 value이기 떄문에.
				// j++; // 출력된것의 번호를 알기 위한것.
			}
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
	int nameIndex[100]={0};
	int objectIndex[100]={0};

	// jsonNamelist(JSON_STRING,t,r,nameIndex);
	// printNameList(JSON_STRING,t,nameIndex);
	// selecNameList(JSON_STRING,t,nameIndex);
	objectNameList(JSON_STRING,t,r,objectIndex);
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
