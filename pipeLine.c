#include <stdio.h>
#include <malloc.h>
#include <stdbool.h>
#include "pipeLine.h"

pipe_list *head = NULL;
pipe_list *curr = NULL;


pipe_list* create_list(Pipe *pipe) {
    pipe_list *ptr = (pipe_list*)malloc(sizeof(pipe_list));
    if(ptr == NULL) {
        printf("failed \n");
        return NULL;
    }

    ptr->pipeNum = 1;
    ptr->pipe = pipe;
    ptr->next = NULL;

    head = curr = ptr;
    return ptr;
}

pipe_list* add_to_list(Pipe *pipe) {
    if(NULL == head) {
        return (create_list(pipe));
    }

    pipe_list *ptr = (pipe_list*)malloc(sizeof(pipe_list));
    if(ptr == NULL) {
        printf(" failed \n");
        return NULL;
    }
    ptr->pipe = pipe;
    ptr->pipeNum = head->pipeNum+1;
    ptr->next = NULL;

    curr->next = ptr;
    curr = ptr;

    return ptr;
}

pipe_list* search_in_list(int pipeNum, pipe_list **prev) {
    pipe_list *ptr = head;
    pipe_list *tmp = NULL;
    bool found = false;


    while(ptr != NULL) {
        if(ptr->pipeNum == pipeNum) {
            found = true;
            break;
        } else {
            tmp = ptr;
            ptr = ptr->next;
        }
    }

    if(found == true) {
        if(prev)
            *prev = tmp;
        return ptr;
    } else {
        return NULL;
    }
}

int delete_from_list(int pipeNum) {

    pipe_list *prev = NULL;
    pipe_list *del = NULL;


    del = search_in_list(pipeNum,&prev);
    if(del == NULL) {
        return -1;
    } else {
        if(prev != NULL)
            prev->next = del->next;

        if(del == curr) {
            curr = prev;
        } else if(del == head) {
            head = del->next;
        }
    }

    free(del);
    del = NULL;
    return 0;
}

void print_list(void) {
   pipe_list *ptr = head;

    printf("\nStart\n");
    while(ptr != NULL) {
        printf("\n [%d] \n",ptr->pipeNum);
        printf("\n [%d] \n",ptr->pipe->width);
        ptr = ptr->next;
    }
    printf("\nEnd\n");
    return;
}



int main() {
    Pipe pip = {12,12,12,12,12};
    Pipe pip2 = {15,16,17,18,19};
    add_to_list(&pip);
    print_list();
    add_to_list(&pip2);
    print_list();
    delete_from_list(1);
    print_list();
}