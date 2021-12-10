/*
Benjamin Chen
12/3/2021
Graph Implementation
*/
#include <stdlib.h>
#include <stdio.h>
#include "List.h"
#include "Graph.h"

typedef struct Graph{
    int num_vertices;
    ListPtr *adjacency_list; // Is an array of ListPtrs for faster access
} Graph;

// Hints: You will need to implement a ListPtr DataCompare, DataPrinter and 
//        freeData for the lists used in the adjacency list. These will be ints.
//        You can copy this from the ones in prog5.c, but they should have different name to avoid a namesapce collision.
//TODO: Implement all functions from the header file.

int DataCompare1(int x, int x2){ //dataCompare for regular lists
    if(x2 == x)
        return 0;
    if(x > x2)
        return 1;
    if (x < x2)
        return -1;
}

int DataCompare2(int* x, int* y){ //dataCompare for list of int pointers
    int a = *((int *)x);
    int b = *((int *)y);
    if(a == b){
        return 0;
    }
    if(a > b){
        return 1;
    }
    if(a < b){
        return -1;
    }

}
void DataPrinter2(int* x){ //dataPrinter for list of int pointers
    int a = *((int *)x);
    printf("%d\n", a);
}

void DataPrinter1(int x){ //dataPrinter for regular list
    printf("%d\n", x);
}

void freeData1(ListPtr x){
    list_destroy(x, true);
}

Graph *graph_create(int numVertices){
    Graph *graph = (Graph*)malloc(sizeof(Graph));
    graph->num_vertices = numVertices;
    graph->adjacency_list = (ListPtr*)malloc(sizeof(ListPtr)* numVertices);
    int i = 0;
    for(i=0;i < numVertices; i++){ //creates a list for each of the vertices
        graph->adjacency_list[i] = list_create(DataCompare1, DataPrinter1, freeData1);
    }
    return graph;
}
/*
deconstructor
*/
void graph_destroy(Graph *g){
    int i;
    for(i=0;i < g->num_vertices; i++){
        list_destroy(g->adjacency_list[i], true);
    }
    free(g);
}

/**
 * @brief Adds a new undirected, unweighted edge to the graph between 2 nodes
 * 
 * @param g The graph to add the edge to
 * @param v1 The vertex to add the edge to
 * @param v2 The second vertex to add the edge to
 * @return true If the operation was succesful
 * @return false If the operation failed. E.g. node out of bounds, edge already exists
 */
bool graph_add_edge(Graph *g, int v1, int v2){
    bool x = true;
    if(list_find_element(g->adjacency_list[v1], v2) != -1 || list_find_element(g->adjacency_list[v2], v1) != -1 || v1 > g->num_vertices || v2 > g->num_vertices || v1 == v2){
        return false;
    }
    else{
        x = list_append(g->adjacency_list[v1], v2); //appends into each list
        x = list_append(g->adjacency_list[v2], v1);
    }
    return x;
}

/**
 * @brief Resize graph to add more vertices.
 * 
 * @param g The graph to add vertices to
 * @param numVertices The number of new vertices to add
 * @return true If the operation was succesful
 * @return false If the operation failed. E.g realloc failed
 */
bool graph_add_nodes(Graph *g, int numVertices){
    int i;
    ListPtr *new_adjecency_list = (ListPtr*)realloc(g->adjacency_list, numVertices * sizeof(ListPtr));
    if(new_adjecency_list !=NULL){ //adds new nodes to existing adj_list
        g->adjacency_list = new_adjecency_list;
        for(i = g->num_vertices; i < numVertices + g->num_vertices; i++){
            g->adjacency_list[i] = list_create(DataCompare1, DataPrinter1, freeData1);//mallocs each new list
        }
        g->num_vertices = numVertices + g->num_vertices; //update num of vertices
    }
}

/**
 * @brief Removes an edge from the graph
 * 
 * @param g The graph to remove the edge from
 * @param v1 One vertex of the edge
 * @param v2 The second vertex of the edge
 * @return true If the operation was succesful
 * @return false If the operation failed. E.g. edge does not exist, vertices not in graph
 */
bool graph_remove_edge(Graph *g, int v1, int v2){
    
    if(list_del_index(g->adjacency_list[v2], list_find_element(g->adjacency_list[v2], v1)) != NULL&& list_del_index(g->adjacency_list[v1], list_find_element(g->adjacency_list[v1], v2)) !=NULL){
        return true; //if delete is successful, return true
    }
    return false; //else return false
}

//returns true if graph has edge, false if not
bool graph_has_edge(Graph *g, int v1, int v2){
    if(v1 < 0 || v2 < 0 || v1 == v2){
        return false;
    }
    int x = list_find_element(g->adjacency_list[v1], v2); //checks list if it has the edge
    return x!= -1; //if has edge, returns not negative 1
}


//returns a list of all the adj nodes of a vertex in the form of a int pointer list
ListPtr graph_get_neighbors(Graph *g, int vertex){
    if(vertex < 0 || g->num_vertices < vertex || g->adjacency_list[vertex] == NULL){
        return NULL;
    }
    ListPtr temp = list_create(DataCompare2, DataPrinter2, freeData1);
    //creates a list of int pointers that point to an index
    int i;
    for(i = 0; i < list_length(g->adjacency_list[vertex]); i++){
        int* y = (int*) malloc(sizeof(int));
        int x = ((int)list_get_index(g->adjacency_list[vertex], i)); //gets the value from the adj_list
        *y = x; //sets the pointer equal to the value
        list_append(temp, y);//adds pointer to the list
    }
    return temp;
}
//returns number of vertices in graph
int graph_num_vertices(Graph *g){
    return g->num_vertices;
}


//returns number of edges in graph
int graph_num_edges(Graph *g){
    int i;
    int counter;
    int num = 0;
    for(i = 0; g->num_vertices > i; i++){ //iterate through the list and get total number of edges
        counter = 0;
        if(list_length(g->adjacency_list[i]) != -1){
            num = num + list_length(g->adjacency_list[i]);
        }
    }
    return num/2; //divided by two because each edge is added into the graph twice
}