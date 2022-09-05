#include<iostream>
#include<fstream>
#include"class.hpp"             // includes class definitions
#include"global_variables.hpp"  // includes extern/global variables [also used in function.cpp]
#include"function.hpp"          // includes function declarations
using namespace std;

// Vertex class constructor
Vertex :: Vertex()
{
    outgoing_edge = NULL;
}

// Halfedge class constructor
Halfedge :: Halfedge()
{
    origin = NULL;
    prev = next = twin = NULL;
    incident_face = NULL;
}

// Face class constructor
Face :: Face()
{
    incident_edge = NULL;
    centroid_x = 0;
    centroid_y = 0;
}

// function to create halfedges for the initial polygon
void halfedge_fill(class Vertex* vertex,class Halfedge* halfedge,class Face* face,int no_of_sides)
{
    int no_of_halfedges = 2 * no_of_sides;
    for(int i = 0;i < no_of_sides;i++)
    {
        // WRITE LOGIC IN README

        // filling halfedge information which belongs to face 0
        halfedge[2*i].edge_num = i;
        halfedge[2*i].origin = &vertex[i % no_of_sides];
        halfedge[2*i].next = &halfedge[(2*i + 2) % no_of_halfedges];
        halfedge[2*i].prev = &halfedge[(2*i - 2 + no_of_halfedges) % no_of_halfedges];
        halfedge[2*i].twin = &halfedge[2*i + 1];
        halfedge[2*i].incident_face = &face[face_counter];

        // pointing face 0's incident_edge to a halfedge
        if(i == 0)
        {
            face[face_counter].incident_edge = &halfedge[2*i];
        }

        // filling twins of above halfedges which dont belong to any face
        halfedge[2*i + 1].edge_num = i;
        halfedge[2*i + 1].origin = &vertex[(i + 1) % no_of_sides];
        halfedge[2*i + 1].next = &halfedge[((2*i + 1) - 2 + no_of_halfedges) % no_of_halfedges];
        halfedge[2*i + 1].prev = &halfedge[((2*i + 1) + 2) % no_of_halfedges];
        halfedge[2*i + 1].twin = &halfedge[(2*i + 1) - 1];
        edge_counter++;
    }
}

// function to create a face for inital polygon
void face_fill(class Halfedge* halfedge,class Face* face)
{
    // filling face 0 information
    face[face_counter].face_num = face_counter;
    face[face_counter].incident_edge = &halfedge[0];

    class Halfedge* temp = face[face_counter].incident_edge;
    // do-while loop to travel around inner-halfedges and point their incident_face pointer to face 0
    do
    {
        temp->incident_face = &face[face_counter];
        temp = temp->next;
    }while(temp != face[face_counter].incident_edge);
    face_counter++;
}

// function to write assignment output into output file
void ass_output(class Halfedge* halfedge,class Face* face,ofstream& f_output)
{
   // Output of Edges
        f_output << "Edges:" << endl;
        for(int i = 0;i < edge_counter;i++)
        {
            f_output << i << endl;
        }

        // Output of Halfedges
        f_output << "Halfedges:" << endl;
        for(int i = 0;i < 2*edge_counter;i++)   // iterating through all halfedges created
        {
            // printing only the edges which belong to any face
            if(halfedge[i].incident_face != NULL)
            {
                f_output << "startVertexIndex=" << halfedge[i].origin->vertex_num << " endVertexIndex=" << halfedge[i].twin->origin->vertex_num << " nextEdge=" << halfedge[i].next->edge_num << " previousEdge=" << halfedge[i].prev->edge_num << " faceIndex=" << halfedge[i].incident_face->face_num << " edgeIndex=" << halfedge[i].edge_num << endl;
            }
        }

        // Output of Faces
        f_output << "Faces:" << endl;
        for(int i = 0;i < face_counter;i++)  // iterating through all faces created
        {
            // variables used for output
            class Halfedge* start = face[i].incident_edge;
            class Halfedge* temp = start;

            f_output << "FaceIndex:" << face[i].face_num << " Edges " << temp->edge_num;
            temp = temp->next;
            int count = 0;
            while(temp != start)
            {
                f_output << "->" << temp->edge_num;
                temp = temp->next;
                count++;
            }
            f_output << endl;
        }
        f_output << endl;
}

// function to split edges in the polygon
void split(int split_start_edge,int split_end_edge,class Vertex* vertex,class Halfedge* halfedge,class Face* face)
{
    // start_vertex = new vertex 1 number, end_vertex = new vertex 2 number
    int start_vertex = vertex_counter++,end_vertex = vertex_counter++;

    // new_face = new face number
    int new_face = face_counter++;

    // new_edge_1 = new edge 1 number, new_edge_2 = new edge 2 number, new_edge_3 = new edge 3 number
    int new_edge_1 = edge_counter++,new_edge_2 = edge_counter++, new_edge_3 = edge_counter++;

    // split_start_halfedge = halfedge which is split first, split_end_halfedge = halfedge which is split second
    class Halfedge* split_start_halfedge = NULL;
    class Halfedge* split_end_halfedge = NULL;

    // if-else blocks to make sure that the splitting halfedges are on the same face
    if(halfedge[2*split_end_edge].incident_face == halfedge[2*split_start_edge].incident_face && halfedge[2*split_end_edge].incident_face != NULL && halfedge[2*split_start_edge].incident_face != NULL)
    {
        split_start_halfedge = &halfedge[2*split_start_edge];;
        split_end_halfedge = &halfedge[2*split_end_edge];
    }
    else if(halfedge[2*split_end_edge + 1].incident_face == halfedge[2*split_start_edge].incident_face && halfedge[2*split_end_edge + 1].incident_face != NULL && halfedge[2*split_start_edge].incident_face != NULL)
    {
        split_start_halfedge = &halfedge[2*split_start_edge];
        split_end_halfedge = &halfedge[2*split_end_edge + 1];
    }
    else if(halfedge[2*split_end_edge + 1].incident_face == halfedge[2*split_start_edge + 1].incident_face && halfedge[2*split_end_edge + 1].incident_face != NULL && halfedge[2*split_start_edge + 1].incident_face != NULL)
    {
        split_start_halfedge = &halfedge[2*split_start_edge + 1];
        split_end_halfedge = &halfedge[2*split_end_edge + 1];
    }
    else if(halfedge[2*split_end_edge].incident_face == halfedge[2*split_start_edge + 1].incident_face && halfedge[2*split_end_edge].incident_face != NULL && halfedge[2*split_start_edge + 1].incident_face != NULL)
    {
        split_start_halfedge = &halfedge[2*split_start_edge + 1];
        split_end_halfedge = &halfedge[2*split_end_edge];
    }

    // Step1: Creating start_vertex/new vertex 1
    vertex[start_vertex].x = (split_start_halfedge->origin->x + split_start_halfedge->twin->origin->x)/2;
    vertex[start_vertex].y = (split_start_halfedge->origin->y + split_start_halfedge->twin->origin->y)/2;
    vertex[start_vertex].vertex_num = start_vertex;

    // Step2: Creating end_vertex/new vertex 2
    vertex[end_vertex].x = (split_end_halfedge->origin->x + split_end_halfedge->twin->origin->x)/2;
    vertex[end_vertex].y = (split_end_halfedge->origin->y + split_end_halfedge->twin->origin->y)/2;
    vertex[end_vertex].vertex_num = end_vertex;

    // Step3: Creating new_edge_1 between start_vertex & end_vertex whose origin is start_vertex [Eg: For split 2,3:- this halfedge starts from vertex 4 to vertex 5]
    halfedge[2*new_edge_1].edge_num = new_edge_1;
    halfedge[2*new_edge_1].origin = &vertex[start_vertex];
    halfedge[2*new_edge_1].twin = &halfedge[2*new_edge_1 + 1];
    halfedge[2*new_edge_1].next = split_end_halfedge;
    halfedge[2*new_edge_1].prev = split_start_halfedge;
    halfedge[2*new_edge_1].incident_face = split_end_halfedge->incident_face;
    halfedge[2*new_edge_1].incident_face->incident_edge = &halfedge[2*new_edge_1];

    // Assigning new vertex 1.outgoing_edge pointer to new_edge_1
    vertex[start_vertex].outgoing_edge = &halfedge[2*new_edge_1];

    // Step4: Creating twin of new_edge_1 between start_vertex & end_vertex whose origin is start_vertex [Eg: For split 2,3:- this halfedge starts from vertex 5 to vertex 4]
    halfedge[2*new_edge_1 + 1].edge_num = new_edge_1;
    halfedge[2*new_edge_1 + 1].origin = &vertex[end_vertex];
    halfedge[2*new_edge_1 + 1].twin = &halfedge[2*new_edge_1];
    halfedge[2*new_edge_1 + 1].next = &halfedge[2*new_edge_2];
    halfedge[2*new_edge_1 + 1].prev = &halfedge[2*new_edge_3];
    halfedge[2*new_edge_1 + 1].incident_face = &face[new_face];
    
    // Assigning new vertex 2.incident_edge pointer to new_edge_1.twin
    vertex[end_vertex].outgoing_edge = &halfedge[2*new_edge_1 + 1];

    // Creating new face & assigning its properties
    face[new_face].face_num = new_face;
    face[new_face].incident_edge = &halfedge[2*new_edge_1 + 1];

    // Step5: Creating & placing new_edge_3 in it's right place
    class Halfedge* temp = split_end_halfedge->prev;

    halfedge[2*new_edge_3].edge_num = new_edge_3;
    halfedge[2*new_edge_3].origin = temp->twin->origin;
    halfedge[2*new_edge_3].twin = &halfedge[2*new_edge_3 + 1];
    halfedge[2*new_edge_3].next = &halfedge[2*new_edge_1 + 1];
    halfedge[2*new_edge_3].prev = temp;
    halfedge[2*new_edge_3].incident_face = &face[new_face];
    temp->next = &halfedge[2*new_edge_3];

    // Step6: Creating & placing new_edge_2 in it's right place
    halfedge[2*new_edge_2].edge_num = new_edge_2;
    halfedge[2*new_edge_2].origin = &vertex[start_vertex];
    halfedge[2*new_edge_2].twin = &halfedge[2*new_edge_2 + 1];
    halfedge[2*new_edge_2].next = split_start_halfedge->next;
    halfedge[2*new_edge_2].prev = &halfedge[2*new_edge_1 + 1];
    halfedge[2*new_edge_2].incident_face = &face[new_face];

    // Assigning new_edge_2's next edge's prev pointer to new_edge_2
    halfedge[2*new_edge_2].next->prev = &halfedge[2*new_edge_2];

    // Step7: Now connecting old edges which are split to new edges and changing their properties
    split_end_halfedge->origin = &vertex[end_vertex];
    split_end_halfedge->prev = &halfedge[2*new_edge_1];

    // Step8: Creating twin of new_edge_3 & placing it in it's right place
    halfedge[2*new_edge_3 + 1].edge_num = new_edge_3;
    halfedge[2*new_edge_3 + 1].origin = &vertex[end_vertex];
    halfedge[2*new_edge_3 + 1].twin = &halfedge[2*new_edge_3];
    halfedge[2*new_edge_3 + 1].prev = split_end_halfedge->twin;
    halfedge[2*new_edge_3 + 1].next = halfedge[2*new_edge_3 + 1].prev->next;
    halfedge[2*new_edge_3 + 1].incident_face = halfedge[2*new_edge_3 + 1].prev->incident_face;
   
    // Updating old splitted halfedge's next pointer
    split_start_halfedge->next = &halfedge[2*new_edge_1];

    // Step9: Creating twin of new_edge_2 & placing it in it's right place
    halfedge[2*new_edge_2 + 1].edge_num = new_edge_2;
    halfedge[2*new_edge_2 + 1].twin = &halfedge[2*new_edge_2];
    halfedge[2*new_edge_2 + 1].origin = halfedge[2*new_edge_2 + 1].twin->next->origin;
    halfedge[2*new_edge_2 + 1].next = split_start_halfedge->twin;
    halfedge[2*new_edge_2 + 1].prev = halfedge[2*new_edge_2 + 1].next->prev;
    halfedge[2*new_edge_2 + 1].incident_face = halfedge[2*new_edge_2 + 1].prev->incident_face;

    // Step10: Now connecting twins of old edges to be split and changing their properties
    halfedge[2*new_edge_2 + 1].prev->next = &halfedge[2*new_edge_2 + 1];
    split_start_halfedge->twin->origin = &vertex[start_vertex];
    split_start_halfedge->twin->prev = &halfedge[2*new_edge_2 + 1];

    split_end_halfedge->twin->next->prev = &halfedge[2*new_edge_3 + 1];
    split_end_halfedge->twin->next = &halfedge[2*new_edge_3 + 1];

    // Now, the halfedge connections have been completed

    // Step11: Assigning new face pointer to all halfedges that belong to new face
    face_update(new_face,halfedge,face);
}

// function to update the incident_face of halfedges belonging to new face after a split
void face_update(int face_num,class Halfedge* halfedge,class Face* face)
{
    class Halfedge* start = face[face_num].incident_edge;
    do
    {
        start->incident_face = &face[face_num];
        start = start->next;
    } while (start != face[face_num].incident_edge);
}

// function to find centroid of all existing face
void find_centroid_for_face(class Face* face,int no_of_faces)
{
    // for loop to calculate centroid coordiantes of all existing faces
    for(int i = 0;i < no_of_faces;i++)
    {
        int count = 0;  // counter to keep track of number of halfedges in a face
        class Halfedge* start = face[i].incident_edge;
        class Halfedge* temp = start;

        // do-while loop to calculate centroid coordinates of a face by traversing to all halfedges of a face once
        do
        {
            face[i].centroid_x += temp->origin->x;
            face[i].centroid_y += temp->origin->y;
            count++;
            temp = temp->next;
        }while(temp != start);
        face[i].centroid_x /= count;
        face[i].centroid_y /= count;
    }
}

// function that locates which face the given point is in and returns the face's number
int Id_find(class Halfedge* halfedge,class Face* face,double p_x,double p_y,int no_of_faces)
{
    // for loop to check whether point is present in a face
    for(int i = 0;i < no_of_faces;i++)
    {
        double centroid_x = face[i].centroid_x;
        double centroid_y = face[i].centroid_y;
        double check;   // value of upon checking for 2 point's position wrt to a line
        int status = 0; // status = 0 when a point is inside the face & status = 1 when a point is not inside the face
        class Halfedge* temp = face[i].incident_edge;
        double x1,y1,x2,y2; // coordinates of endpoints of an halfedge
        
        // do-while loop to compute whether a point lies inside a face or not
        do
        {
            // (x1,y1) & (x2,y2) are endpoints of halfedge
            x1 = temp->origin->x;
            y1 = temp->origin->y;
            x2 = temp->twin->origin->x;
            y2 = temp->twin->origin->y;

            // expression to check 2 point's position wrt to a line
            double check = ((centroid_y - y1) * (x2 - x1) - (y2 - y1) * (centroid_x - x1)) * ((p_y - y1) * (x2 - x1) - (y2 - y1) * (p_x - x1));
            
            if(check < 0)   // if given point and centroid of face lie on different sides of a halfedge (ie when check is negative)
            {
                status = 1;
                break;
            }
            temp = temp->next;
        }while(temp != face[i].incident_edge);

        if(status == 0)
        {
            return face[i].face_num;    // returns face number if given point lies in it
        }
    }
    return -1;  // returns -1 when given point doesn't belong to any face
}