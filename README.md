# CS4115-Final-Assignment
The final graded assignment tasked to me as part of my Data Structures and Algorithms module in Second year at UL.

We were asked to take sample street data from OpenStreetMap and perform a number of operations on this data. For example, we had to represent the data internally in a more efficient structure than a regular n x n matrix, find the degree of a node, find the largest degree of any node in the data, find all neighbours that are k edges away from a node, and finding the shortest path between two nodes by total edge length or total number of edges. I accomplished this by implementing Dijkstra's Algorithm, though I could also have easily developed it into an A* algorithm using the xyz data from OpenStreetMap. 

The most significant requirement of this assignment was not just producing a program that would accomplish these tasks, but one that could do so in a more optimal manner. This demand proved to be challening but also very educational. I felt like I was truly being tested, as I pulled out all the tricks I could think of to optimise performance, such as passing objects to methods by reference to reduce costs involved with copying large lists of nodes.

Overall I felt this assignment assisted in polishing my C++ skills and my general mindset when it comes to programming. It proved to be a valuable experience where I could see the real life beneifts of what I was doing.


# How This Program Works
When you compile and run this program, it will seek input from the user. The first input is for the location of the OpenStreetMap data. You will see I've attached bel.osm.graph aand bel.osm.xyz in this repo. to pass these in pass in the location like this: 

C:\\users\deberain\Downloads\CS4115-Final-Assignment\bel

No need to append the .osm.graph or .osm.xyz. The program does this for you.

The second input it seeks is the query number. In total there are 7 different queries.

1. lists the order and size of the graph
2. gets the largest degree of any node in the list
3. gets the average degree of all nodes in the list
4. gets the immediate neighbours of a node
5. gets the neighbours k edges away from the a node
6. gets the shortest path by actual distance travelled
7. gets the shortest path by total number of edges

Queries 4 to 7 wil prompt you for a third input once the data has been read in. This input will be the parameters for the query itself.

for 4. It will be the node itself. Ex 12
for 5. it will be the node and the number of edges to travel, ex 12 5
for 6. an 7. it will be the start and end node. ex 3 390
