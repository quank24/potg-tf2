#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
int toCount =0;
int kills = 0;
int assists = 0;
int damage = 0;
int quankKills = 0;


void getTime(char* line)
{
	const char *PATTERN1 = "- ";
		const char *PATTERN2 = ": ";
		
		char *target = NULL;
		char *start, *end;
		
		if (start = strstr(line, PATTERN1))
		{
			start += strlen(PATTERN1);
			if (end =strstr(start, PATTERN2))
			{
				target = (char*)malloc(end - start +1);
				memcpy(target, start, end-start);
				target[end-start] = '\0';
			}
		}
		if (target)printf("%s\n", target);
		free(target);
}

void getFrag(char* line)
{
	const char *PATTERN1 = "- ";
		const char *PATTERN2 = " killed ";
		
		char *target = NULL;
		char *start, *end;
		
		if (start = strstr(line, PATTERN1))
		{
			start += strlen(PATTERN1);
			if (end =strstr(start, PATTERN2))
			{
				target = (char*)malloc(end - start +1);
				memcpy(target, start, end-start);
				target[end-start] = '\0';
			}
		}
		
		if (target){
		printf("KILL %s\n", target);
			kills++;
		}
		free(target);
		
}
void getAssist(char* line)
{
	const char *PATTERN1 = "- ";
		const char *PATTERN2 = " triggered \"kill assist\"";
		
		char *target = NULL;
		char *start, *end;
		
		if (start = strstr(line, PATTERN1))
		{
			start += strlen(PATTERN1);
			if (end =strstr(start, PATTERN2))
			{
				target = (char*)malloc(end - start +1);
				memcpy(target, start, end-start);
				target[end-start] = '\0';
			}
		}
		if (target){
		printf("ASSIST %s\n", target);
			assists++;
		}
		free(target);
		
}

void getDamage(char* line)
{
	const char *PATTERN1 = "- ";
		const char *PATTERN2 = " triggered \"damage\"";
		
		char *target = NULL;
		char *start, *end;
		
		if (start = strstr(line, PATTERN1))
		{
			start += strlen(PATTERN1);
			if (end =strstr(start, PATTERN2))
			{
				target = (char*)malloc(end - start +1);
				memcpy(target, start, end-start);
				target[end-start] = '\0';
			}
		}
		if (target){
		//printf("DAMAGE %s\n", target);
			damage++;
		}
		free(target);
		
}

int main(int argc, char* argv[])
{
    int count = 0;
    int startPoints = 0;
    int stopPoints = 0;
    char const* const fileName = argv[1]; /* should check that argc > 1 */
    FILE* file = fopen(fileName, "r"); /* should check the result */
    char line[1024];
    

    while (fgets(line, sizeof(line), file)) {
        //printf("%s", line); 
        count++;
			
        
//----------------------------------Round start check---------------------------------//
        //if(strstr(line, "World triggered \"Round_Setup_End\"") != NULL) {
		if(strstr(line, "World triggered \"Round_Start\"") != NULL) {
			
			//startPoints();
			startPoints++;
			printf("round %d start @ ",startPoints);
			getTime(line);
			toCount = 1;
		}
//-----------------------------------Round end check----------------------------------//
		if(strstr(line, "World triggered \"Round_Win\"") != NULL)
		{
			//stopPoints();
			stopPoints++;
			printf("round %d end @ ", stopPoints);
			getTime(line);
			toCount = 0;
		}
//--------------------------------All Counters During Live Play----------------------------------------------------//
		if(toCount)getFrag(line); 
		if(toCount)getAssist(line);
		if(toCount)getDamage(line);
		
		}//while fgets end bracket
		
    fclose(file); 
    printf("\n-------------------STATS----------------------");
	printf("\nNumber of lines in log file: %d\n", count);
	printf("Number of kills: %d\n", kills);
	printf("Number of assists: %d\n", assists);
	printf("Total damage messages: %d\n", damage);
	//printf("user quank kills: %d\n", quankKills);
	printf("----------------------------------------------\n");
    return 0;
}
