#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../jsmn.h"

/*
 * A small example of jsmn parsing when JSON structure is known and number of
 * tokens is predictable.
 */

//	static const char *JSON_STRING =
//	"{\"user\": \"johndoe\", \"admin\": false, \"uid\": 1000,\n  "
//	"\"groups\": [\"users\", \"wheel\", \"audio\", \"video\"]}";

static int jsoneq(
	const char *json, // 조사할 josn stirng
	 jsmntok_t *tok, // token array pointer
	  const char *s) // key값
	 {
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
			strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
} // key값이 일치하면 0 리턴 아니면 -1 리턴.

char *readJSONFILE(){
	FILE *fp;
	int count=0;
	char line[128];
	char filename[128];
	printf("원하는 파일명 입력: \n");
	scanf("%s",filename);
	fp=fopen(filename,"rt"); //파일 열기
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
// void jsonNamelist(char *JSON_STRING,jsmntok_t *t,int tokcount){
// 	int i;
// 	int count=0;
// 	printf("*********NAME LIST********\n");
// 	for(i=0;i<tokcount;i++){
// 		if (t[i+1].size >0 && t[i+1].type ==JSMN_STRING){
// 			printf("[NAME %d] %.*s \n",count+1,t[i+1].end-t[i+1].start,JSON_STRING + t[i+1].start);
// 			count++;
// 	}
// 	}
// }
void jsonNamelist2(char *JSON_STRING,jsmntok_t *t,int tokcount,int *nameTokIndex){
	int i;
	int count=0;
	for(i=1;i<tokcount+1;i++){
		if(t[0].type==JSMN_ARRAY ){
			if(t[i].size >0 && t[i].type==JSMN_STRING){
				// printf("i= %d,size: %d, parent : %d \n",i,t[i].size,t[i].parent);
				nameTokIndex[count]=i;
				count++;
			}
		}
		else{
			if(t[i].size >0 && t[i].type==JSMN_STRING &&t[t[i].parent].parent<0 ){

				if(t[i+1].type==2){ // 첫번째 name의 다음이 array이면 아래 조건문 실행(data4.json일때)
					for(i+=1;i<tokcount;i++){
					if(t[i].size >0 && t[i].type==JSMN_STRING){
					//	printf("i= %d,size: %d, parent : %d \n",i,t[i].size,t[i].parent);
						nameTokIndex[count]=i;
						count++;
					}
				}
				}
				else{
				nameTokIndex[count]=i;
				count++;
			}
		}
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

void selecObject(char *JSON_STRING, jsmntok_t *t, int tokcount, int *objectIndex){
int select=0;
int i=0;
int j=0;
int s=0;
int addsize=0;

objectIndex=(int*)malloc(sizeof(int)); // objectIndex를 int 크기만큼 동적할당 해준다.
objectIndex=0;
for(i=0;i<tokcount;i++){
	if(t[0].type==JSMN_ARRAY){
		if(t[i].type==JSMN_OBJECT && t[i-1].size==0 || t[i-1].type==2 ){
			addsize+=sizeof(int);
			objectIndex=(int*)realloc(objectIndex,addsize); // objectIndex에 int크기만큼 재할당 해준다.
			objectIndex[j]=i+1; //objectIndex값 저장. objectIndex[0]에는 i+1인 object의 첫번째 name이 저장되어있다.
			j++; // 출력된것의 번호를 알기 위한것.
		}
	}
	else{ //시작이 오브젝트일때
		if(t[i].type==JSMN_OBJECT && t[i-1].size==0){ // 토큰의 타입이 오브젝트일때 4개가 나오는데 거기서 그 전의 사이즈가 0인것이 큰 오브젝트다.
			if(t[i+2].type==2){
				for(i+=1;i<tokcount;i++){
					if(t[i].type==JSMN_OBJECT && t[i-1].size==0 || t[i-1].type==2 ){
						addsize+=sizeof(int);
						objectIndex=(int*)realloc(objectIndex,addsize); // objectIndex에 int크기만큼 재할당 해준다.
						objectIndex[j]=i+1; //objectIndex값 저장. objectIndex[0]에는 i+1인 object의 첫번째 name이 저장되어있다.
						j++; // 출력된것의 번호를 알기 위한것.
					}
				}
			}
			else{
			addsize+=sizeof(int);
			objectIndex=(int*)realloc(objectIndex,addsize); // objectIndex에 int크기만큼 재할당 해준다.
			objectIndex[j]=i+1; //objectIndex값 저장. objectIndex[0]에는 i+1인 object의 첫번째 name이 저장되어있다.
			j++; // 출력된것의 번호를 알기 위한것.
		}
	}
	}
}
i=0;
while(1){
	printf("원하는 번호 입력:  (exit:0) ");
	scanf("%d",&select); // 번호를 입력받기.
	if(select == 0) break; //0을 입력했을때 빠져나오게 함.
	s=objectIndex[select-1]; // 1번 누르면 objectIndex의 0번째부터 시작.
	printf("%.*s : %.*s \n",t[s].end-t[s].start,JSON_STRING + t[s].start,
	t[s+1].end-t[s+1].start,JSON_STRING + t[s+1].start); // 첫번째 name과 value를 출력
	if(objectIndex[select]==0)
		objectIndex[select]=sizeof(objectIndex); //만약에 object가 1개면 밑에 반복문을 어느정도 진행시키기 위한 임의값.
	for(i=objectIndex[select-1]+2;i<objectIndex[select-1]+(objectIndex[1]-objectIndex[0]);i++){ // 첫번째 object 시작+2(이미 첫번째 해서)부터 다음 object 시작 전까지 진행.
		if(t[0].type==JSMN_ARRAY){
			if(t[i].size >0 && t[i].type==JSMN_STRING){ //name값을 찾기
					printf("    [%.*s]    ",t[i].end-t[i].start,JSON_STRING + t[i].start); //name값에만 [] 넣어주기
					printf("%.*s \n",t[i+1].end-t[i+1].start,JSON_STRING + t[i+1].start); //vaule 출력.
				}
		}
		else{ // 시작값이 array로 시작하는것이 아닐때 즉 오브젝트일때
			if(t[i].size >0 && t[i].type==JSMN_STRING){ //name값을 찾기
				if(t[i+1].type==2){ // i가 첫번째 name이므로 i+1 즉 첫번째 name의 다음이 어레이면 실행.(data4.json일때)
					for(i+=1;i<tokcount;i++){
					if(t[i].size >0 && t[i].type==JSMN_STRING){
						printf("    [%.*s]    ",t[i].end-t[i].start,JSON_STRING + t[i].start); //name값에만 [] 넣어주기
						printf("%.*s \n",t[i+1].end-t[i+1].start,JSON_STRING + t[i+1].start); //vaule 출력.
						}
					}
				}
				else{ //data4.json이 아닐때
					printf("    [%.*s]    ",t[i].end-t[i].start,JSON_STRING + t[i].start); //name값에만 [] 넣어주기
					printf("%.*s \n",t[i+1].end-t[i+1].start,JSON_STRING + t[i+1].start); //vaule 출력.
				}
			}
		}
	}
}
}

int main() {
	int i;
	int r;
	char* JSON_STRING=readJSONFILE();
	char* JSON_OBJECT;

//	printf("%s",JSON_STRING);

	jsmn_parser p;
	jsmntok_t t[128]; /* We expect no more than 128 tokens */

	jsmn_init(&p);
	r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t, sizeof(t)/sizeof(t[0]));
	if (r < 0) {
		printf("Failed to parse JSON: %d\n", r);
		return 1;
	}

	/* Assume the top-level element is an object */
	//if (r < 1 || t[0].type != JSMN_OBJECT) {
	if (r < 1 ) {
		printf("Object expected\n");
		return 1;
	}
	int nameIndex[100]={0};
	int *objectIndex;  //objectIndex값을 저장하기 위한 포인터-> 배열로사용할것이다.
	int *OI;
//	jsonNamelist(JSON_STRING,t,r);
	jsonNamelist2(JSON_STRING,t,r,nameIndex);
	printNameList(JSON_STRING,t,nameIndex);
	// selecNameList(JSON_STRING,t,nameIndex);
	objectNameList(JSON_STRING,t,r,objectIndex);
	// objectNameList2(JSON_STRING,t,r,objectIndex);
	selecObject(JSON_STRING,t,r,objectIndex);
	/* Loop over all keys of the root object */
//	return EXIT_SUCCESS;
}
	/* Loop over all keys of the root object */
//	return EXIT_SUCCESS;
// for (i = 1; i < r; i++) {	//	return EXIT_SUCCESS;
// 	if (jsoneq(JSON_STRING, &t[i], "name") == 0) {
// 			/* We may use strndup() to fetch string value */
// 			printf("- name: %.*s\n", t[i+1].end-t[i+1].start,
// 					JSON_STRING + t[i+1].start);
// 			i++;
// 		} else if (jsoneq(JSON_STRING, &t[i], "keywords") == 0) {
// 			/* We may additionally check if the value is either "true" or "false" */
// 			printf("- keywords: %.*s\n", t[i+1].end-t[i+1].start,
// 					JSON_STRING + t[i+1].start);
// 			i++;
// 		} else if (jsoneq(JSON_STRING, &t[i], "description") == 0) {
// 			/* We may want to do strtol() here to get numeric value */
// 			printf("- UID: %.*s\n", t[i+1].end-t[i+1].start,
// 					JSON_STRING + t[i+1].start);
// 			i++;
// 		} else if (jsoneq(JSON_STRING, &t[i], "example") == 0) {
// 			int j;
// 			printf("- example:\n");
// 			printf("- UID: %.*s\n", t[i+2].end-t[i+2].start,
// 					JSON_STRING + t[i+2].start);
// 	}
// 	return EXIT_SUCCESS;
// }
