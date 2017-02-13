//
// Created by shane on 13/02/2017.
//

#ifndef UNTITLED_PIPELINE_H
#define UNTITLED_PIPELINE_H

typedef struct Pipe{
    int top;
    int bottom;
    int width;
    int x;
    int speed;
}Pipe;

typedef struct pipe_list
{
    int pipeNum;
    Pipe *pipe;
    struct pipe_list *next;
}pipe_list;

pipe_list* create_list(Pipe *);
pipe_list* add_to_list(Pipe *);
pipe_list* search_in_list(int, pipe_list **);
int delete_from_list(int);
void print_list();



#endif //UNTITLED_PIPELINE_H
