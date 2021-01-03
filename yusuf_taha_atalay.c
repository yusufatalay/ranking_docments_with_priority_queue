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

    char searchKeyword[30];
    printf("Type the search keyword: ");
    fgets(searchKeyword, 30, stdin);
    printf("\n %s", searchKeyword);

}









