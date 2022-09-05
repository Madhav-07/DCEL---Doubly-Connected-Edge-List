// function to create halfedges for the initial polygon
void halfedge_fill(class Vertex*,class Halfedge*,class Face*,int);

// function to create a face for inital polygon
void face_fill(class Halfedge*,class Face*);

// function to write assignment output into output file
void ass_output(class Halfedge*,class Face*,ofstream&);

// function to split edges in the polygon
void split(int,int,class Vertex*,class Halfedge*,class Face*);

// function to update the incident_face of halfedges belonging to new face after a split
void face_update(int,class Halfedge*,class Face*);

// function to find centroid of all existing face
void find_centroid_for_face(class Face*,int);

// function that locates which face the given point is in and returns the face's number
int Id_find(class Halfedge*,class Face*,double,double,int);