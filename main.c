#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
int match_found=0;

int keyword_file_lookup(char *path1, char *keyword1, int size_path, int size_keyword);
int match(char path1[],char keyword1[],int size_path,int size_keyword);
void find_path(char *name,char *keyword);
void find_inside_file(char *name,char *keyword);


/*main*/
int main(int argc, char *argv[]) 
{
		
	if(argc != 4)
	{
		printf("Error! Arguments Not Correct, please refer to Project Manual\n");
		return 0;
	}

	if(strlen(argv[3])<3)
	{
		printf("Error! Keyword length too short\n");
		return 0;
	}

	if(strcmp(argv[1],"-a")==0)
	{	
		find_path(argv[2],argv[3]);
		printf("\n*Number of matches found = %d\n\n",match_found);
	}
	else if(strcmp(argv[1],"-b")==0)
	{
		find_inside_file(argv[2],argv[3]);
		printf("\n*Number of matches found = %d\n\n",match_found);
	}
	else
	{
		printf("Error! No Such Option as: %s\n", argv[1]);
	}

    return 0;
}


/*function looksup if keyword is present in file of given filepath or not*/
int keyword_file_lookup(char *path1, char *keyword1, int size_path, int size_keyword)
{	
	char file_name[size_path], keyword[size_keyword];
	
	strcpy(file_name,path1);
	strcpy(keyword,keyword1);	
	
	char ch;
	FILE *fptr;
	int keyword_size=strlen(keyword);     
     	//printf("Size: %d\n",keyword_size);
	fptr = fopen(file_name, "r");

	if (fptr == NULL)
	{
		//perror("Cannot Open File!\n");
		//exit(EXIT_FAILURE);
		return 0;
	}
          
	int i=0;
 	while((ch = fgetc(fptr)) != EOF)
   	{
		if(keyword[i]==ch)
		{
			i++;
			if(i==keyword_size)
			{
				//printf("all matched!\n");
				return 1;
			}
			
		}
		else
		{
			i=0;
		}	
	}
     
   	fclose(fptr);
	return 0;
}


/*function that finds if keyword is present in the path or not*/
int match(char path1[],char keyword1[],int size_path,int size_keyword)
{
	char path[size_path], keyword[size_keyword+1];
	
	strcpy(path,path1);
	strcpy(keyword,keyword1);	
		
	int i, count=0,count1=0, j,flag=0;

	for(i=0; i<sizeof(path)-1; i++)
	{
		
		if(keyword[0]==path[i])
		{
		
			for(j=0; j<sizeof(keyword)-1; j++)
			{
				//printf("%c %c\n", keyword[j],path[i+j] );
				if(keyword[j]==path[i+j])
				{
					count++;
				}
			}
			if(count==sizeof(keyword)-1)
			{
				
				count1++;
				//printf("matched!\n");
			}
			count=0;
			//printf("\n");	
		}
	}
	
	return count1;

}


/*function that passes list of all files in dir (including those in SubDirs) to match function*/
void find_path(char *name,char *keyword)
{
    DIR *dir;
    char s[1000];
    struct dirent *entry;

    if (!(dir = opendir(name)))
        {
		printf("Path Entered not correct!\n");
		return;
	}

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            char path[1000];
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
             continue;

            snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);
            find_path(path,keyword);
        } 
	else {
		snprintf(s, sizeof(s), "%s/%s", name, entry->d_name);
		
		int check=0;
		check=match(s,keyword,strlen(s),strlen(keyword));
		
		if(check>0)
		{
			match_found++;
			printf("%s\n",s);
		}


	      }
    }
	
    closedir(dir);
}

/*function that passes list of all files in dir (including those in SubDirs) to the keyword_file_lookup*/
void find_inside_file(char *name,char *keyword)
{
    DIR *dir;
    char s[1000];
    struct dirent *entry;
	int check_res=0;

    if (!(dir = opendir(name)))
        {
		printf("Path Entered not correct!\n");
		return;
	}

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            char path[1000];
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
             continue;

            snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);
            find_inside_file(path,keyword);
        } 
	else {
		snprintf(s, sizeof(s), "%s/%s", name, entry->d_name);
				
		check_res=0;
		check_res=keyword_file_lookup(s,keyword,strlen(s),strlen(keyword));
		//printf("%d\n",check_res);
		if(check_res==1)
		{
			//printf("Keyword matched in filepath\n");
			match_found++;
			printf("%s\n",s);
			check_res=0;
		}
		
	      }
    }
	
	
    closedir(dir);
}

