#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>

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
//TODO:
// We need to tokenize the given content string according to whitespaces it contains
// Use stricmp for comparing the keyword and token because its cane insensitive
// Consider other occurrence of the keyword like keyword, and keyword.
// Now we need to traverse through the array and increment the counter every time we encounter the keyword in the array


    char delimiter[] = " "; // delimiter is single white space
    char *token;
    int counter = 0;

    token = strtok(content, delimiter);  //getting the first token


    // if the keyword exist in the content and have an postfix like "," , "." or "s"  we need to tell the code that include them too.
    char option1[strlen(keyword) + 1];
    char option2[strlen(keyword) + 1];
    char option3[strlen(keyword) + 1];
    strcpy(option1, keyword);
    strcpy(option2, keyword);
    strcpy(option3, keyword);
    strncat(option1, ",", 1);
    strncat(option2, ".", 1);
    strncat(option3, "s", 1);

    while (token != NULL) {

        if ((stricmp(token, keyword) == 0) || (stricmp(token, option1) == 0) ||
            (stricmp(token, option2) == 0) || (stricmp(token, option3) == 0)) {   //"stricmp" is case insensitive;
            counter++;
        }

        token = strtok(NULL, delimiter);  //continue to the next  token;
    }
    return counter;


}


int main() {

    char searchKeyword[30];
    printf("Type the search keyword: ");
    fgets(searchKeyword, 30, stdin);
    searchKeyword[strlen(searchKeyword) -
                  1] = '\0';  // last character was a new line character so this is how i mitigate
    // let see how many files in the folder

    int file_count = 0;    // how many files we have in the folder i will store them as a node in an array before building the heap
    DIR *dirp;
    struct dirent *each_file;

    dirp = opendir("files"); /* There should be error handling after this */
    while ((each_file = readdir(dirp)) != NULL) {
        file_count++;
    }
    closedir(dirp);

    //this is where i will hold the file nodes later append them to the binomial tree  minus 2 is for escaping the ./ and ../ directories
    node fileNodes[file_count - 2];


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
        int node_index = 0; //i will use this as a index number for appending the nodes to the node array

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
            //  printf("%s\n", content);
            keyword_count = fileScore(content, searchKeyword);
            printf("amount of  %s  appeared in file is: %d\n  ", searchKeyword, keyword_count);
            printf("end of file: %s\n", file_name);


            //create a node about relevant file  and append that node to  the array
            fileNodes[node_index] = *NewNode(file_name, keyword_count);
            node_index++;

        }
    }
    closedir(folder);


    for (int i = 0; i < file_count - 2; i++) {
        printf("file name is : %s   relevancyScore is : %d  \n", fileNodes[i].fileName, fileNodes[i].relevancyScore);
    }

    return (0);
}









