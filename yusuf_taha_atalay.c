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

// this method will take a keyword and a file to search the keyword occurrence count
int fileScore(char *content, char *keyword) {
    int content_length, keyword_length;
    int keyword_count = 0;
    int keyword_found = 0;  //act as a boolean
    content_length = strlen(content);
    keyword_length = strlen(keyword);

    // the index of first matching character cannot be greater than content_length-keyword_length
    for (int i = 0; i < content_length - keyword_length; i++) {
        // in this method i created a placeholder as length of search keyword and program shifts it bit by bit and increments the counter when keyword occurs

        // lets assume the keyword exists in the context
        keyword_found = 1;
        for (int j = 0; j < keyword_length; j++) {

            if (content[i + j] != keyword[j]) {
                keyword_found = 0;
                break;
            }
        }
        if (keyword_found == 1) {
            keyword_count++;
        }
    }

    return keyword_count;
}


int main() {

    char searchKeyword[30];
    printf("Type the search keyword: ");
    fgets(searchKeyword, 30, stdin);
    printf("\n %s", searchKeyword);

    int file_count = 0;  // how many files we have in the folder i will store them as a node in an array before building the heap
    DIR *folder;
    FILE *file_pointer;
    struct dirent *entry;
// program is running at cmake-build folder so i carried the files folder into that folder
    folder = opendir("files");
    if (folder == NULL) {
        puts("Unable to read directory");
        return (1);
    } else {
        puts("Directory is opened!");

        // after opening the directory lets traverse all the files in the directory
        while ((entry = readdir(folder)) != NULL) {
            // every folder in a file system contains this two directories named "." and ".."
            if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) {
                continue;
            }
            printf("file name: %s\n", entry->d_name);
            // create all per-file variables here
            int keyword_count = 0;
            char file_name[21];
            strcpy(file_name, entry->d_name);
            char file_position[23];
            sprintf(file_position, "%s%s", "files/", file_name);
            printf("%s\n", file_position);

            // i looked around a couple of files and realized that max amount of char have been more than 14k so i hope 25k char limit will do the work
            char content[25000];

            // opening the file
            file_pointer = fopen(file_position, "r");
            if (file_pointer == NULL) {
                printf("Could not open file %s\n", file_name);
                return 1;
            }
            // starts reading the file
            while (fgets(content, sizeof(content), file_pointer) != NULL) {
                //reads the file and assigns its content to the content variable until its EOF
                // since here it reads the file line by line i won't do any processing here instead i will wait it to finish
            }
            // file has been read and after this code block it will proceed to the next file
            printf("%s\n", content);

            keyword_count = fileScore(content, searchKeyword);
            printf("amount of  %s  appeared in file is: %d\n",searchKeyword ,keyword_count);
            printf("end of file\n", file_name);


        }
    }
    closedir(folder);

    return (0);


}









