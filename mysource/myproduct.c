#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../myproduct.h"

char *readJSONFILE(){
	FILE *fp;
	int count=0;
	char line[128];

	fp=fopen("myproduct.json","rt"); //파일 열기
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

int makeProduct(const char *JSON_STRING, jsmntok_t *t, int tokcount, product_t * p[]){
	int i;
	int key;
	int count=0;
	int j=0;
	char s1[20];
	int keyInfo[20]={0};

	for(i=0;i<tokcount;i++){
			if(t[i].type==JSMN_OBJECT && t[i-1].size==0){
				if(t[i+2].type==JSMN_ARRAY){
					for(i+=1;i<tokcount;i++){
						if(t[i].type==JSMN_OBJECT && t[i-1].size==0 || t[i-1].type==JSMN_ARRAY ){
							keyInfo[j]=i+1; //object의 첫 name index 저장.
							j++; //
						}
					}
				}
		}
	}

	for(i=0; i<j;i++){
	key=keyInfo[i];
	p[i]=(product_t*)malloc(sizeof(product_t));
	sprintf(s1,"%.*s",t[key+1].end-t[key+1].start,JSON_STRING + t[key+1].start);
	if(!strcmp(s1,"ADIDAS"))	p[i]->company=ADIDAS;
	else if(!strcmp(s1,"CONVERSE"))	p[i]->company=CONVERSE;
	else if(!strcmp(s1,"VANS"))	p[i]->company=VANS;
	else if(!strcmp(s1,"FILA"))	p[i]->company=FILA;
	sprintf(p[i]->name,"%.*s",t[key+3].end-t[key+3].start,JSON_STRING + t[key+3].start);
	sprintf(p[i]->price,"%.*s",t[key+5].end-t[key+5].start,JSON_STRING + t[key+5].start);
	sprintf(p[i]->color,"%.*s",t[key+7].end-t[key+7].start,JSON_STRING + t[key+7].start);
	count++;
	}
	return count;
}

void printProduct(product_t *p[],int pcount){
	int i;
	char temp[20];
	printf("********************************************************\n");
	printf("번호    제품명        제조사         가격         색상     \n");
	printf("********************************************************\n");
	for(i=0;i<pcount;i++){
		if(p[i]->company==0)
			sprintf(temp,"%s","ADIDAS");
		else if(p[i]->company==1)
			sprintf(temp,"%s","CONVERSE");
		else if(p[i]->company==2)
			sprintf(temp,"%s","VANS");
		else if(p[i]->company==3)
			sprintf(temp,"%s","FILA");
		printf("%-5d  %-11s  %-16s %-13s %-10s   \n",i+1,p[i]->name,temp,p[i]->price,p[i]->color);
	}
}

int main() {
	int i;
	int r;
	int pcount;
	char* JSON_STRING=readJSONFILE();
	char* JSON_OBJECT;

	jsmn_parser p;
	jsmntok_t t[128]; /* We expect no more than 128 tokens */
	product_t *shoeslist[20];
	jsmn_init(&p);
	r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t, sizeof(t)/sizeof(t[0]));
	if (r < 0) {
		printf("Failed to parse JSON: %d\n", r);
		return 1;
	}

	if (r < 1 || t[0].type != JSMN_OBJECT) {
		printf("Object expected\n");
		return 1;
	}
	pcount=makeProduct(JSON_STRING,t,r,shoeslist);
	printProduct(shoeslist,pcount);
}
