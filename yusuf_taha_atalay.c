#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

// Yusuf Taha ATALAY

// define a struct for each node and put necessary attributes in it
typedef struct node {
    char fileName[21];
    // how many times the keyword appeared in that file
    int relevancyScore;

    // number of child nodes this node has
    int degree;
    struct node *parent;

    // pointer to its immediate right sibling
    struct node *sibling;

    // will point to the left-most child
    struct node *child;
} node;

node *NewNode(char *fileName, int relevancyScore) {
    // Arrange the space for the new node
    node *node = malloc(sizeof(*node));

    //Assign the values in the first go
    strcpy(node->fileName, fileName);
    node->relevancyScore = relevancyScore;

    // its degree is 0 it will change when i plug it into the heap
    node->degree = 0;

    // for now these values are NULL i will arrange them as we read the files
    node->parent = NULL;
    node->sibling = NULL;
    node->child = NULL;

    return node;
}


int main() {

   // char searchKeyword[30];
   // printf("Type the search keyword: ");
   // fgets(searchKeyword, 30, stdin);
   // printf("\n %s", searchKeyword);


    DIR *folder;
    FILE *file_pointer ;
    struct dirent *entry;
// program is running at cmake-build folder so i have to go one folder up in order to find the folder named "files"
    folder = opendir("../files");
    if(folder == NULL)
    {
        puts("Unable to read directory");
        return(1);
    }
    else
    {
        puts("Directory is opened!");

        // after opening the directory lets traverse all the files in the directory
        while((entry = readdir(folder)) != NULL){
            // every folder in a file system contains this two directories named "." and ".."
            if ( !strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..") )
            {
                continue;
            }
          printf("file name: %s\n", entry->d_name);
            // create all per-file variables here
            char* file_name[21];
            strcpy((char *) file_name, entry->d_name);
            // i looked around a couple of files and realized that max amount of char have been more than 14k so i hope 25k char limit will do the work
            char content[25000];

            // opening the file
            file_pointer = fopen(entry->d_name, "r");
            if(file_pointer == NULL){
                printf("Could not open file %s\n",file_name);
                return 1;
            }while(fgets(content,sizeof(content),file_pointer)!=NULL){
                printf("%s\n",content);
            }




            int word_count = 0;

        }
    }
    closedir(folder);

    return(0);










   



}









