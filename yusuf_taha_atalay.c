#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>

// Yusuf Taha ATALAY
// 150119040
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

// struct of the heap
typedef struct heap {
    node *head;
} heap;


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

heap *CreateHeap() {
    // EXPERIMENTAL
    heap *heap_header;
    heap_header = (heap *) malloc(sizeof(heap));
    heap_header->head = NULL;
    return heap_header;
}

node *FindMin(heap *heap_header) {
    // EXPERIMENTAL
    node *next = NULL;
    node *current = heap_header->head;
    int min = INT_MAX; // maximum integer value possible in C to represent infinity
    while (current != NULL) {
        if (current->relevancyScore < min) {
            min = current->relevancyScore;
            next = current;
        }
        current = current->sibling;
    }
    return next;
}

void Link(node *node1, node *node2) {
    // links two B(k-1) heads and obtains a B(k) and root of it is node2
    node1->parent = node2;  //make them arrangements
    node1->sibling = node2->child;
    node2->child = node1;
    node2->degree++;  // since it gains a new child we should increment its degree
}

node *MergeRootList(heap *heap1, heap *heap2) {
    //if any of these are empty return the other one
    if (heap1->head == NULL) {
        return heap2->head;
    } else if (heap2->head == NULL) {
        return heap1->head;
    } else {
        node *head;
        node *tail;
        node *heap1Next = heap1->head;
        node *heap2Next = heap2->head;

        if (heap1->head->degree <= heap2->head->degree) {
            head = heap1->head;
            heap1Next = heap2Next->sibling;
        } else {
            head = heap2->head;
            heap2Next = heap2Next->sibling;
        }
        tail = head;


        while (heap1Next != NULL && heap2Next != NULL) {
            if (heap1Next->degree == heap2Next->degree) {
                tail->sibling = heap1Next;
                heap1Next = heap1Next->sibling;
            } else {
                tail->sibling = heap2Next;
                heap2Next = heap2Next->sibling;
            }
            tail = tail->sibling;
        }
        if (heap1Next != NULL) {
            tail->sibling = heap1Next;
        } else {
            tail->sibling = heap2Next;
        }
        return head;
    }

}

heap *Merge(heap *heap1, heap *heap2) {
    //merges two heap
    heap *header = CreateHeap();
    header->head = MergeRootList(heap1, heap2);
    if (header->head == NULL) {
        return header;
    }
    node *previous = NULL;
    node *current = header->head;
    node *next = current->sibling;

    while (next != NULL) {
        if (next->degree != current->degree || (next->sibling != NULL && next->sibling->degree == current->degree)) {
            previous = current;
            current = next;
        } else if (current->relevancyScore <= next->relevancyScore) {
            current->sibling = next->sibling;
            Link(next, current);
        } else {
            if (previous == NULL) {
                header->head = next;
            } else {
                previous->sibling = next;
            }
            Link(current, next);
            current = next;
        }
        next = current->sibling;
    }

    return header;
}

void Insert(heap *heap, node *file) {
    struct heap *heap1 = CreateHeap();
    heap->head = file;
    struct heap *newHeap = Merge(heap, heap1);
    heap->head = newHeap->head;
    //node *toInsert = NewNode(fileName, relevancyScore);
    //toInsert->parent = toInsert->child = toInsert->sibling = NULL;
    //toInsert->degree = 0;
    //heap1->head = toInsert;
    //heap = Merge(heap1, heap);
}

node *ExtractMin(heap *heap) {
    if (heap->head == NULL) {
        return NULL;
    }


    node *temp = heap->head;
    node *current = temp->sibling;
    node *previous = temp;
    node *tempPrev = NULL;

    while (current != NULL) {
        if (current->relevancyScore < temp->relevancyScore) {
            temp = current;
            tempPrev = previous;
        }
        previous = current;
        current = current->sibling;
    }

    if (temp == heap->head) {
        heap->head = temp->sibling;
    } else {
        tempPrev->sibling = temp->sibling;
    }


    struct heap *holder = CreateHeap();

    node *z = previous->child;
    while (z != NULL) {
        node *next = z->sibling;
        z->sibling = holder->head;
        holder->head = z;
        z = next;
    }
    struct heap *newHeap = Merge(heap, holder);
    heap->head = newHeap->head;
    return temp;


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
            // printf("file name: %s\n", entry->d_name);
            // create all per-file variables here
            int keyword_count;
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
    heap *fileHeap = CreateHeap();


    for (int i = 0; i < file_count - 2; i++) {
        Insert(fileHeap, &fileNodes[i]);
    }


    for (int i = 0; i < 41; i++) {
        node *removedOne = ExtractMin(fileHeap);
        printf("removed file is %s  and its score is %d \n", removedOne->fileName, removedOne->relevancyScore);

    }
    return (0);
}









