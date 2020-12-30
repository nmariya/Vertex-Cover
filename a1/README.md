# Creating Street Database 
This part is to create a database with all the coordinate information of the streets. It is used to create a undirectional graph.

#  Sample Input
The  input  comprises  lines  each  of  which  speci es  a  command.   There  are  4  kinds  of  commands.(1) add a street,  (2) modify a street,  (3) remove a street,  and,  (4) generate a graph.  Here is anexample of how your program should work.  Visualizing this example using the Cartesian coordinatesystem may help you understand what's going on.

add "Weber Street" (2,-1) (2,2) (5,5) (5,6) (3,8)

add "King Street S" (4,2) (4,8)

add "Davenport Road" (1,4) (5,8)

gg

# Commands
•add is used to add a street.  It is speci ed as:  \add "Street Name" (x1,y1) (x2,y2). . .(xn,yn)".  Each(xi,yi)is a GPS coordinate.  We interpret the coordinates as a poly-line segment.That is, we draw a line segment from(xi,yi)to(xi+1,yi+1).  You are allowed to assume thateachxiandyiis an integer.  (Note, however, that the coordinates of an intersection may notbe integers.)
•mod is used to modify the speci cation of a street.  Its format is the same as foradd.  It is anew speci cation for a street you've speci ed before.
•rm is used to remove a street.  It is speci ed as \rm "Street Name"".
•ggc auses the program to output the corresponding graph
