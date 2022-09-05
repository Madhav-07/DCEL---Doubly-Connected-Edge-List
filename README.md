INPUT FILE FORMAT:-
1) Vertex file: Takes a pair of 2 numbers for each line. For eg: 1 2
                                                                 7 2
                                                                 7 8
                                                                 1 8

2) Split file: Takes a pair a string = "Split" then a pair of 2 number per line. For eg: Split 2 3
                                                                                         Split 0 1
               Then takes a 3 char(s) = Id:, then 3 numbers.                     For eg: Id:1 2 2
                                                                                         Id:2 3 3

FILE HANDLING:-
Used C++ File Handling

LOGIC:
My program uses arrays to access each halfedge,vertex,face objects of the polygon. But we can also travel using double linked lists as the connections between each and every objects have been created and assigned. The only advantage is we can access any halfedge,face or vertex object from their respective arrays.

The number of halfedges, vertices & faces is calculated beforehand by counting the no_of_vertices provided & no_of_splits as input.
So this number of halfedges, vertices & faces represents the size of vertex_array, halfedge_array, face_array.

So for:
1) Vertex: "vertex 0" is accessed from vertex_array[0], "vertex 1" is accessed from vertex_array[1] ...
2) Face: "face 0" is accessed from face_array[0], "face 1" is accessed from face_array[1] ...
3) Halfedges: Here we need twice the number of edges, ie 2 halfedges for 1 edge. So my implementation is such that, halfedge_array[2*i] & halfedge[2*i + 1] are twins. For eg: halfedge_array[0] & halfedge_array[1] are twins. Another convention followed is that upto creating only the square(or polygon), halfedge_array[even_number] are inner-halfedges which belong to face 0 & halfedge_array[odd_number] are outer-halfedges which don't belong to any face.

Split Function: First, i make connections between all halfedges correctly. Then use another function called "face_update" to update the incident_face pointer of all halfedges in the new face.

ID Function: Here i added to new data members in class Face, called centroid_x,centroid_y, which are the coordinates of the centroid of the face. Firstly i call function ["find_centroid_for_face"] which calculates the centroid of every existing face. Next call another function ["Id_find"] which finds to which face the given point belongs and returns the face_number or face_index.

SAMPLE INPUT FILES:
vertex.in : provides input for coordinates of vertices
split.in : provides input for split & Id commands

OUTPUT FORMAT:-
For Edges: I have kept a Edge_counter whose value denotes the next new edge's number. So output is printed until i = Edge_counter - 1;

For Halfedges: I have used a for(int i=0;i<2*Edge_counter;i++) meaning there are last halfedge in halfedge_array till halfedge_array[2*Edgecounter + 1]
               & only printing those halfedges which belong any face (ie. their incident face is not null)
               FORMAT: halfedge_array[0] ie edgeIndex = 0/2 = 0
                       halfedge_array[1] ie edgeIndex = 1/2 = 0
                       halfedge_array[2] ie edgeIndex = 2/2 = 1       
                       ...                                      (only if their incident face is not null, then prints their properties)
                       ...
                       ...
                       ...

For Faces: I have put for(int i=0;i<Face_counter;i++) which access each face from face_array[], so prints from face 0 & its edges, face 1 & its edges, face 2 & its edges,...

For Id query: Writes into the file in the form:- Id:<query-Id> <face_number>

Also I have left a line between every iteration of output written ie. 1st forming polygon, next for every split, then one line space after which Id's are printed every next line. I have also attached my "output.out" file for reference

ROBUSTNESS OF THE CODE:-
I can proudly say that this code can work for infinite splits as long as there is memory in the stack (because i have used static memory allocation for objects using arrays), so please feel free to give input of several splits provided the splits are done to the edges which belong to the same face (ie. no splitting of edges which belong to different faces). I have provided "split.in" file where i have given 7 splits and verified its output as well by drawing on paper.