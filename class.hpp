#include<iostream>
using namespace std;

#ifndef CLASS_H
#define CLASS_H

class Vertex
{
    public:
    double x,y;                     // (x,y) = coordinates
    int vertex_num;                 // vertex_num = vertex number
    class Halfedge *outgoing_edge;  // outgoing_edge = pointer to a halfedge with origin as this vertex

    Vertex();
};

class Halfedge
{
    public:
    int edge_num;                       // edge_num = edge number
    class Vertex *origin;               // origin = vertex from which this halfedge is originating from
    class Halfedge *prev,*next,*twin;   // prev = pointer to previous halfedge, next = pointer to next halfedge, twin = pointer to twin halfedge
    class Face *incident_face;          // incident_face = pointer to the face this halfedge belongs to

    Halfedge();
};

class Face
{
    public:
    int face_num;                   // face_num = facce number
    class Halfedge *incident_edge;  // incident_edge = pointer to one of the halfedges in this face
    double centroid_x,centroid_y;   // centroid coordinates for required for "Id" query

    Face();
};

#endif