/*
Benjamin Chen
12/3/2021
Prog5 Implementaiton - BFS and A*
*/
#include "Graph.h"
#include "List.h"
#include "PQueue.h"
#include "Dictionary.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct Point {
    int x;
    int y;
} Point;

Point *create_point(int x, int y) {
    Point *p = malloc(sizeof(Point));
    p->x = x;
    p->y = y;
    return p;
}

//Hint: The ListPtrs will store ints for bfs and a_star.
//      You need to define the dataCompare, dataPrinter and freeData functions here. 

int dataCompare(Point* x, Point* y){
    if(x->x == y->x && x->y == y->y){ //checks if points are equal
        return 0;
    }
    else{
        return -1;
    }
}

void dataPrinter(int x){
    printf("%d \n", x);
}

void freeData(Point* x){
    free(x);
}
/**
 * @brief Implements the bst algorithm to find the shortest path between 2 nodes in an unweighted graph.
 * 
 * @param g The graph to search through
 * @param vertex_start The index of the vertex in the graph from where to start the search.
 * @param vertex_end The index of the vertex in the graph that needs to be reached.
 * @param vertices_explored The value in this pointer should be set to the number of vertices that were visited by the algorithm.
 * @return ListPtr A list of the nodes that make up the path from the start to the end, including the start and end nodes.
 */
ListPtr bfs(Graph *g, int vertex_start, int vertex_end, long *vertices_explored) {
    //Perform bfs traversal. Make sure to store who the parent of each node in your traversal is.
    // Mark visited nodes to maek sure not to visit them twice
    struct Node{//creates a struct to keep track of parent node and checked nodes
        bool checked;
        int prev;
    }Node;

    struct Node x[graph_num_vertices(g)]; //array of structs

    int i;
    for(i = 0; i < graph_num_vertices(g); i++){
        x[i].checked = false;//initializes all checked to false
        x[i].prev = -1;//and all prev to -1
    }

    long explored = 0;
    ListPtr queue = list_create(dataCompare, dataPrinter, freeData);
    list_append(queue, vertex_start); //adds the first node to queue
    x[vertex_start].checked = true; //marks the first vertex checked
    bool endLoop = false;
    while(list_length(queue) != 0){ //while the queue is not empty
        int index = list_del_index(queue, 0); //gets front of queue
        ListPtr adj = graph_get_neighbors(g, index); //get all the adjacent vertices from the vertex
        int i;
        for(i = 0; i < list_length(adj); i++){ //checks all the adjacent vertices
            int curr = (*(int*)list_get_index(adj, i));
            //printf("%d\n", curr);
            if(x[curr].checked == false){ //if the vertex has not been checked yet
                x[curr].checked = true;
                x[curr].prev = index; //saves the previous vertex of the node
                list_append(queue, curr); //adds the vertex to the queue to be checked again
                
                explored++; //adds to number of explored vertices
                if(curr == vertex_end){ //if the front of queue is the end point, break out of loop
                    endLoop = true;
                    break;
                }
            } //if vertex has been checked, pass
        }
        if(endLoop){
            break;
        }

    }
    ListPtr path = list_create(dataCompare, dataPrinter, freeData);
    int v = vertex_end;
    while(v != -1){ //creates new list and adds all paths from array of struct created.
        list_insert(path, 0, v);
        v = x[v].prev;
    }
    if(list_length(path) != 0){
        *vertices_explored = explored; //sets explored and then returns list
        return path;
    }
    else{
        return NULL; //if list is empty return null
    }
}

/**
 * @brief Implements the A* search algorithm to find the shortest path between 2 nodes in an unweighted graph.
 * 
 * @param g The graph to search through
 * @param nodeArr Array of positions of the graph vertices on a 2d plane. Note this is a pointer to a contiguous array, where nodeArr[i] should correspond to node i in the graph. 
 * @param vertex_start The index of the vertex in the graph from where to start the search.
 * @param vertex_end The index of the vertex in the graph that needs to be reached.
 * @param vertices_explored The value in this pointer should be set to the number of vertices that were visited by the algorithm.
 * @return ListPtr A list of the nodes that make up the path from the start to the end, including the start and end nodes.
 */
ListPtr a_star(Graph *g, Point *nodeArr, int vertex_start, int vertex_end, long *vertices_explored) {
    //Perform a star traversal. Make sure to store who the parent of each node in your traversal is, and the path cost up to that node.

    return NULL;
}


int main(int argc, char* argv[]) {
    // argv[1] will tell you if to run bfs or a_star
    int algorithm = (strcmp(argv[1], "bfs") == 0) ? 0 : 1;
    // stdin will contain your input
    
    ListPtr nodes = list_create(dataCompare, dataPrinter, freeData);
    char s[1000];
    char *data;
    while(fgets(s, 1000, stdin) != NULL){ //gets input and stores it in a point in a list
        if(strstr(s, "Nodes")!= NULL){
            fgets(s, 1000, stdin);
        }
        if(strstr(s, "Edges") != NULL){
            break;
        }
        data = strtok(s, " ");
        while(data != NULL){
            int x = atoi(data);
            //printf("%d ", x);
            data = strtok(NULL, " ");
            int y = atoi(data);
            //printf("%d\n", y);
            Point* p1 = create_point(x,y);
            list_append(nodes, p1);
            data = strtok(NULL, " ");
        }
    }
    //list_print(nodes);
    Graph* g = graph_create(list_length(nodes)); // Create graph
    //graph_add_edge(g, 1, edge2);
    //Parse node locations + count number of nodes
    while(fgets(s, 1000, stdin) != NULL){ //gets all edges
        if(strstr(s, "paths") != NULL){
            break;
        }
        //printf("%s", s);
        data = strtok(s, " ");
        while(data != NULL){
            int edge1 = atoi(data);
            data = strtok(NULL, " ");
            int edge2 = atoi(data);
            data = strtok(NULL, " ");
            //printf("%d %d\n", edge1, edge2);
            if(graph_add_edge(g, edge1, edge2) == false){printf("error\n"); } //adds those edges to the graph
        }
    }
    //printf("%d",graph_num_edges(g));
    while(fgets(s, 1000, stdin) != NULL){
        //printf("%s", s);
        data = strtok(s, " ");
        while(data != NULL){ //gets info for BFS and A*
            int edge1 = atoi(data);
            data = strtok(NULL, " ");
            int edge2 = atoi(data);
            long vertices_explored = 0;
            ListPtr bf = bfs(g, edge1,edge2, &vertices_explored); //calls bfs and stores it in a list
            int left = 0;
            int right = 0;
            int up = 0;
            int down = 0;
            for(int i = 1; i < list_length(bf); i++){
                Point* p1 = list_get_index(nodes, list_get_index(bf, i - 1));//checks each of the points in BFS
                Point* p2 = list_get_index(nodes, list_get_index(bf, i));
                if(p1->x > p2 ->x){ //incremenets depending on which direction each point is in.
                    left++;
                }
                else if(p1->x < p2->x){
                    right++;
                }
                else if(p1->y > p2->y){
                    up++;
                }
                else if(p1->y < p2->y){
                    down++;
                }
            }
            printf("%ld %d %d %d %d %d\n", vertices_explored, list_length(bf)-1, left, right, up, down); //output
            data = strtok(NULL, " ");
        }
    }

    //free Data structures
    return 0;
    
   
}