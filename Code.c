/*                                                  Advance Operating System Assignment1
                                                       
						       Submitted to:- 
                                                                       Sapna Varshney
                                                       
						       Submitted by:-
                                                                        Ankit Sardana(04)
                                                                        
									Rishabh Joshi(27)
									
									Prabal Partap(18)
                                                                 


Problem Statement  – File System
Write a program (using appropriate Unix / Linux system calls) that visits every directory, starting
with the current directory. It should be able to handle loops in the directory hierarchy.
The program should read the directory and print the following information for all files in the
directory – owner, file types, access permissions, and access times.

TO RUN: 
$ gcc assignment.c
$ ./a.out /path/to/directory/




  Structure used to read directory entries of a Directory
 
  struct dirent {
               ino_t          d_ino;       /* Inode number
               off_t          d_off;       /* Not an offset; see below
               unsigned short d_reclen;    /* Length of this record
               unsigned char  d_type;      /* Type of file; not supported
                                              by all filesystem types
               char           d_name[256]; /* Null-terminated filename
           };


  Structure use to read status of the entries
 
  struct stat {
               dev_t     st_dev;         /* ID of device containing file
               ino_t     st_ino;         /* Inode number
               mode_t    st_mode;        /* File type and mode
               nlink_t   st_nlink;       /* Number of hard links
               uid_t     st_uid;         /* User ID of owner
               gid_t     st_gid;         /* Group ID of owner
               dev_t     st_rdev;        /* Device ID (if special file)
               off_t     st_size;        /* Total size, in bytes
               blksize_t st_blksize;     /* Block size for filesystem I/O
               blkcnt_t  st_blocks;      /* Number of 512B blocks allocated

               /* Since Linux 2.6, the kernel supports nanosecond
                  precision for the following timestamp fields.
                  For the details before Linux 2.6, see NOTES.

               struct timespec st_atim;  /* Time of last access
               struct timespec st_mtim;  /* Time of last modification
               struct timespec st_ctim;  /* Time of last status change

           #define st_atime st_atim.tv_sec      /* Backward compatibility
           #define st_mtime st_mtim.tv_sec
           #define st_ctime st_ctim.tv_sec
           };
*/




#include<dirent.h>		 //header file to use directory related variables and functions                                        	   
#include<stdio.h>                //header file to use standard I/O functions and file related functions                         	   
#include<sys/stat.h>             //header file to use stat() function                         	                                
#include<string.h>		 //header file for strcpy and strcat function				 
#include<time.h>		 //header file to use of ctime() function
#include <pwd.h>	         //header file provides a definition for struct passwd
#include<stdlib.h>		 //header file malloc() function
#include "link.h"		 //header file to linked list functions
#include<unistd.h>		 //header file to use chdir() function


struct stat sb; 		 			//structure stat to store information of each file
struct dirent *dir; 		 			//structure dirent to store info related to directory
char directory[50];		 			//for taking user input for directory


void listfiles(char *path,FILE *fp) ; 			//recursive function to search for directories and files in directory



int get_file(char *path,FILE *fp,struct dirent *dir)	//to get attributes of files & directories                                      
{		
                                        
		stat(path,&sb);                         //storing attributes of file in  stat structure
		
		fprintf(fp,"%30s\t\t",dir->d_name);
                
		switch(dir->d_type)			//switch to check type of file
		
		{
		 case DT_BLK :  
				fprintf(fp,"BLOCK\t\t"); 
				break;
		 case DT_CHR :  
				fprintf(fp,"CHARACTER\t\t"); 
				break;
		 case DT_DIR :  
				fprintf(fp,"DIR\t\t"); 
				insertFirst(sb.st_ino);
				break;
                 case DT_FIFO :  
				fprintf(fp,"N_PIPE\t\t"); 
				break;
		 case DT_LNK :  
				fprintf(fp,"LINK\t\t"); 
				break;
		 case DT_REG :  
				fprintf(fp,"REGULAR\t\t"); 
				break;
		 case DT_SOCK :  
				fprintf(fp,"SOCKET\t\t"); 
				break;
		
		 case DT_UNKNOWN:  
				fprintf(fp,"DKNOW\t\t"); 
				break;
		}
			
    
		   /* DT_BLK      This is a block device.

		      DT_CHR      This is a character device.

		      DT_DIR      This is a directory.

		      DT_FIFO     This is a named pipe (FIFO).

		      DT_LNK      This is a symbolic link.

		      DT_REG      This is a regular file.

		      DT_SOCK     This is a UNIX domain socket.

		      DT_UNKNOWN  The file type could not be determined.*/             	

	  	struct passwd *pw = getpwuid(sb.st_uid);  			//storing the uid in structure passwd
		fprintf(fp,"%8s\t",pw->pw_name);				//printing the owner name
		fprintf(fp,"%10lo (octal)\t",(unsigned long) sb.st_mode);	//printing the permission
		fprintf(fp,"%24s\n", ctime(&sb.st_atime));			//printing last accessed time 
										
		if(dir->d_type==DT_LNK && (sb.st_mode & S_IFMT)==S_IFDIR) 	//checking if there is a symbolic link to directory
		{	int j;
			
			int check=find(sb.st_ino); // storing the return value from find function
			if(check==-1)		   //checking if inode number is in list or not
			{  insertFirst(sb.st_ino);	//inserting the inode in the linked list
			   listfiles(path,fp);
			   return 0;
			}
			return 1;
			
			
		} 
			
}

void listfiles(char *path,FILE *fp)        // recursive function to search for directories and files in directory                      
{
	DIR *di;                           // descriptor of directory   
	char temp[1024];		   // a string to get the path of directries in a directory					
	di=opendir(path);                  //opening directory with path specified in string path                     
	if(di==NULL)			   //checking if directory opened or not						
	{
		printf("permission denied\n");
	}
	else
	{	
	        while((dir=readdir(di))!=NULL)		//reading files and directories in the directory			
		{	
			int len = snprintf(temp, sizeof(temp)-1, "%s/%s", path, dir->d_name);	//getting path of the directory
			temp[len] = 0;	

			
			
			if(strcmp(dir->d_name,".")==0 || strcmp(dir->d_name,"..")==0)	//checking if directory is "." or ".."
				continue;
			
			else if(dir->d_type==DT_DIR ) 	//checking if a directory d_type is a directory 
														
			{	
				get_file(temp,fp,dir);  //calling the function to print the attribute
				listfiles(temp,fp);	//calling the function recursively to open new directory
				
			}
			
			else
			{
				get_file(temp,fp,dir);   //calling the function to print the attribute              
			}
		}
	}
	closedir(di);					//closing the descriptor
	
	
}


// agrv[1] = directory
int main(int argc, char *argv[]){
     	char *fname;int l; 
	
        printf("\tenter the name of output file:\t");	//asking for name of output file
	scanf("%s",fname);					
	FILE *fp=fopen(fname,"w");			//opening the output file
        
	fprintf(fp,"%30s\t\t%10s\t%8s\t%15s\t%24s\t\n\n","filename","file_type","owner","permission","access_time");	
    	
	if(argv[1] != NULL)				//checking if the directory is given as command line argument
	{
        	printf("Looking in directory: %s\n", argv[1]);
		stat(argv[1],&sb);
                insertFirst(sb.st_ino);			//inserting the inode number of starting directory in linked list
 

        	chdir(argv[1]); 			// switch directory. needed.
        	listfiles(argv[1],fp);
    	}
	else 						// fallback if user didn't enter directory before execution
	{
        printf("%s", "Enter the directory: ");	
        scanf("%s", directory); 
        printf("Looking in directory: %s\n", directory);
	stat(directory,&sb);
	insertFirst(sb.st_ino);				//inserting the inode number of starting directory in linked list 
        chdir(directory); 				// switch directory. needed.
        listfiles(directory,fp);
    	}
	fclose(fp);
    return 0;
}
