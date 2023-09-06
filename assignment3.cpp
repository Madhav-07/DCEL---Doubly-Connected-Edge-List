#include<iostream>
#include<fstream>
#include"class.hpp"             // includes class definitions
#include"global_variables.hpp"  // includes extern/global variables [also used in function.cpp]
#include"function.hpp"          // includes function declarations
using namespace std;

// extern/global variables initialisation from "global_variables.hpp"
int vertex_counter = 0; // counter to keep track of next new vertex number
int edge_counter = 0;   // counter to keep track of next new edge number
int face_counter = 0;   // counter to keep track of next new face number

int main(int argc,char* argv[])
{
    cout << "HI" << endl;
    ifstream f_vertices;        // file object for reading vertex file
    ifstream f_split;           // file output for reading split file
    ofstream f_output;          // file object for output file
    f_vertices.open(argv[1]);   // opening the vertex file 
    f_split.open(argv[2]);      // opening the split file
    f_output.open(argv[3]);     // opening the output file

    int length[6];      // array containing max number of [vertices, halfedges, faces, no_of_splits, total input lines, polygon sides, no of points]
    #define vertice_length 0        
    #define halfedge_length 1
    #define face_length 2           // #define for above elements for array "length"
    #define no_of_splits 3
    #define polygon_sides 4
    #define no_of_points 5

    length[polygon_sides] = 0;
    length[no_of_splits] = 0;
    length[no_of_points] = 0;

    // while loop to compute number of initial sides in polygon
    while(!f_vertices.eof())
    {
        string line;
        getline(f_vertices,line);
        length[polygon_sides]++;
    }
    f_vertices.seekg(0,ios::beg);   // moving vertex file object cursor to start of file

    // while loop to compute number of splits in split file
    while (!f_split.eof())
    {
        string line;
        getline(f_split,line);
        if(line[0] == 'S' || line[0] == 's')
        {
            length[no_of_splits]++;
        }
        else if(line[0] == 'I' || line[0] == 'i')
        {
            length[no_of_points]++;
        }
    }
    f_split.seekg(0,ios::beg);      // moving split file object cursor to start of file    

    // calculation of max number in array "lenght" elements to assign array sizes
    length[vertice_length] = length[polygon_sides] + 2*length[no_of_splits];
    length[halfedge_length] = 2 * (length[vertice_length] + length[no_of_splits]);
    length[face_length] = 1 + length[no_of_splits];

    // defining arrays of ...
    class Halfedge halfedge[length[halfedge_length]];   // array of halfedges
    class Vertex vertex[length[vertice_length]];        // array of vertices
    class Face face[length[face_length]];               // array of faces

    // taking vertex input
    for(int i = 0;i < length[polygon_sides];i++)
    {
        f_vertices >> vertex[i].x >> vertex[i].y;   // taking input from reading file object
        vertex[i].vertex_num = i;                   // assigning vertex number to vertex
        vertex_counter++;                           // updating vertex counter
    }

/*-----------------------------------------------------------------------------Assignment1------------------------------------------------------------------------------*/

    halfedge_fill(vertex,halfedge,face,length[polygon_sides]);  // calling function to create halfedges for initial polygon
    face_fill(halfedge,face);                                   // calling function to create & assign faces to halfedges
    ass_output(halfedge,face,f_output);                         // calling function to write output in output file

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------Assignment2------------------------------------------------------------------------------*/

    string spl;                           // variable to take input of "Split"
    int split_start_edge,split_end_edge;    // split_start_edge = first edge to be split & split_end_edge = second edge to be split

    // taking input for splits & splitting edges & writing output
    for(int i = 0;i < length[no_of_splits];i++)
    {
        f_split >> spl >> split_start_edge >> split_end_edge;         // taking input from split file
        split(split_start_edge,split_end_edge,vertex,halfedge,face);  // calling split function to make splits in
        ass_output(halfedge,face,f_output);                           // calling function to write output in output file
    }

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------Assignment3------------------------------------------------------------------------------*/

    find_centroid_for_face(face,length[face_length]);

    char ch[3];     // char array for taking string "Id:"
    int point_num;  // query number
    double x,y;     // coordiantes of given point

    // for loop to take query input and write face number it belongs to into the output file 
    for(int i = 0;i < length[no_of_points];i++)
    {
        f_split >> ch[0] >> ch[1] >> ch[2] >> point_num >> x >> y;  // taking "Id" query input from split file
        f_output << "Id:" << point_num << " " << Id_find(halfedge,face,x,y,length[face_length]) << endl;    // writing face number into output file
    } 

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

    // closing all opened file objects
    f_vertices.close();
    f_split.close();
    f_output.close();

    return 0;
}
