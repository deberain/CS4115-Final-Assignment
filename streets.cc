#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <cmath>
#include <list>
#include <vector>
#include <algorithm>
#include <float.h>

using namespace std;

class Vertex {  // Represents a node on the graph
private:
    int number;  // Node number
    float x;
    float y;
    float z;
    list<int> neighbours;  // stores index of each neighbour
public:
    int getNumber() { return number; }
    int getDegree() { return neighbours.size(); }  // Gets degree of the node
    const list<int> getNeighbours() { return neighbours; }
    void setNumber(int number) { this->number = number; }
    void addNbr(int nbr) { neighbours.push_back(nbr); }
    float calculateDistance(const Vertex& b);  // calculates length of edge connecting two vertices
    friend istream& operator>>(istream &in, Vertex& v)
    {
        in >> v.x;
        in >> v.y;
        in >> v.z;
        return in;
    }
    friend bool operator==(const Vertex& a, const Vertex& b)
    {
        return a.number == b.number;  // Two vertices are the same if they share the same number
    }
};

void printOrderAndSize(int n, int m);
void getLargestDegree(const vector<Vertex>& nodes);  // All of these pass nodes by reference.
void getAverageDegree(const vector<Vertex>& nodes);  // Otherwise there would be a heavy performance
void getNeighbours(const vector<Vertex>& nodes);     // hit from copying nodes each time.
void getKthNeighbours(const vector<Vertex>& nodes);
void shortestPathDistance(const vector<Vertex>& nodes, bool inEdges);
int findMinDist(const list<Vertex>& PQ, const vector<float>& dist);



int main()
{
    string country;
    string graphLocation, xyzLocation;
    string tempVert, tempCoord;
    bool orderObtained = false;
    int queryNum;
    int order, size;  // Number of nodes, number of edges
    int count = 1;
    vector<Vertex> nodes;

    // STEP 1 - READ IN THE NAME, OR FILE PATH, OF THE TARGET COUNTRY
    getline(cin, country);
    cin >> queryNum;

    graphLocation = country + ".osm.graph";
    xyzLocation = country + ".osm.xyz";

    // STEP 2 - READ IN THE DATA FROM THESE FILES
    ifstream vertices(graphLocation);
    ifstream coords(xyzLocation);

    while(!orderObtained)
    {
        getline(vertices, tempVert);

        if(isdigit(tempVert[0]))
        {
            istringstream lstream(tempVert);
            lstream >> order;
            lstream >> size;
            orderObtained = true;
        }
    }

    while(getline(vertices, tempVert) && getline(coords, tempCoord)) // Read until end of these files
    {
        istringstream lstreamV(tempVert);
        Vertex v;
        int nbr;

        v.setNumber(count);
        count++;

        while(lstreamV >> nbr) {
            v.addNbr(nbr);
        }

        istringstream lstreamC(tempCoord);
        lstreamC >> v;  // Read in x, y, z data to node

        nodes.push_back(v);
    }

    // STEP 3 - GRAPH QUERIES
    switch(queryNum) {
        case 1:
            printOrderAndSize(order, size);
        break;
        case 2:
            getLargestDegree(nodes);
        break;
        case 3:
            getAverageDegree(nodes);
        break;
        case 4:
            getNeighbours(nodes);
        break;
        case 5:
            getKthNeighbours(nodes);
        break;
        case 6:
            shortestPathDistance(nodes, false);  // Shotest path by pure distance travelled
        break;
        case 7:
            shortestPathDistance(nodes, true);  // Shortest path by least amount of edges
        break;
        default:
        cout << "Unknown query; exiting." << endl;
    }

    return 0;
}

/*
 *  Gets the order and size of the graph.
 */
void printOrderAndSize(int n, int m)
{
    cout << "n= " << n << "; m= " << m << "." << endl;
}

/*
 *  Gets the largest degree of any node and the number of that node
 */
void getLargestDegree(const vector<Vertex>& nodes)
{
    int largestDegree = 0;
    int largestLocation = 0;
    for (Vertex v : nodes)
    {
        if(v.getDegree() > largestDegree)
        {
            largestDegree = v.getDegree();
            largestLocation = v.getNumber();
        }
    }
    cout.precision(6);
    cout << "v= " << largestLocation << "; |N(v)|= " << largestDegree << "." << endl;
}

/*
 *  Gets the average degree of all nodes
 */
void getAverageDegree(const vector<Vertex>& nodes)
{
    float sum = 0;
    for(Vertex v : nodes)
    {
        sum += v.getDegree();
    }
    sum /= nodes.size();
    cout << "avg |N(v)|= " << fixed << sum << "." << endl;
}

/*
 *  gets the immediate neighbours of a node
 */
void getNeighbours(const vector<Vertex>& nodes)
{
    int node;
    cin >> node;
    Vertex v = nodes[node - 1];

    cout << "N(" << node << ")=";
    for(int nbr : v.getNeighbours() )
    {
        cout << " " << nbr;
    }
    cout << "." << endl;
}

/*
 *  Gets all neighbours that are k edges from the node.
 */
void getKthNeighbours(const vector<Vertex>& nodes)
{
    int node;
    int k;
    Vertex init;
    vector<int> validNbrs;
    vector<int> dist(nodes.size());
    vector<int> pred(nodes.size());
    list<Vertex> queue;

    // Read in node number and edge count
    cin >> node;
    cin >> k;

    for(unsigned long long i = 0; i < nodes.size(); i++)
    {
        dist[i] = 0;
        pred[i] = -1;
    }

    // Add initial vertex to queue
    init = nodes[node - 1];
    queue.push_front(init);

    // Repeat until no Vertices left in queue
    while(!queue.empty())
    {
        Vertex v = queue.front(); // Get first vertex
        int currentNodeNum = v.getNumber();

        // if k is even, any node that is of distance k/2 means you can loop from that node back to itself, but also to init.
        if((k % 2 == 0 && dist[currentNodeNum - 1] == k/2))
        {
            validNbrs.push_back(node); // so we add the current node.
            validNbrs.push_back(v.getNumber()); // and we add init, as it is technically k edges away from itself
        }

        if(dist[currentNodeNum - 1] == k - 1) // if current distance is k - 1, all neighbours of this node are valid Kth neighbours
        {
            for(int i : v.getNeighbours())
            {
                validNbrs.push_back(i); // mark node as valid solution

            }
        } else {
            for(int i : v.getNeighbours())
            {
                if (pred[currentNodeNum - 1] != i) // if vertex not visited
                {
                    Vertex temp = nodes[i - 1]; // get ith node
                    dist[i - 1] = dist[currentNodeNum - 1] + 1; // set distance of node from init up 1
                    pred[i - 1] = currentNodeNum;

                    queue.push_front(temp); // add node to queue
                }
            }
        }
        queue.remove(v); // remove it from queue -> move to next vertex
    }

    sort(validNbrs.begin(), validNbrs.end()); // sort in ascending order
    validNbrs.erase( unique( validNbrs.begin(), validNbrs.end() ), validNbrs.end() );

    // Now print results
    cout << "N(" << node << "," << k << ")=";
    for(int i : validNbrs)
    {
        cout << " " << i;
    }
    cout << "." << endl;
}

/*
 * Implementation based on Dijkstra's algortihm, tho can be optimised into A*
 */
void shortestPathDistance(const vector<Vertex>& nodes, bool inEdges)
{
    int node;
    int destNode;
    Vertex init;
    vector<int> pred(nodes.size());  // Predecessor to current node
    vector<float> dist(nodes.size());  // Distance from init to current node
    list<Vertex> PQ;  // Priority queue
    list<Vertex> explored;  // Explored nodes

    // Read in node and destination node
    cin >> node;
    cin >> destNode;

    init = nodes[node - 1];

    // Initialise Dist and pred
    for(unsigned long long i = 0; i < nodes.size(); i++)
    {
        pred[i] = -1;
        dist[i] = FLT_MAX;
    }

    dist[init.getNumber() - 1] = 0;
    PQ.insert(PQ.begin(), init);

    while(!PQ.empty())
    {
        int posLowestDistance = findMinDist(PQ, dist); // Obtain the position of the lowest distance member in the queue
        auto it = PQ.begin(); // Iterator to beginning of PQ
        advance(it, posLowestDistance); // advance iterator to position of lowest dist member
        int currentNodeNum = (*it).getNumber();

        for(int i : (*it).getNeighbours())
        {
            if(pred[currentNodeNum - 1] != i) // Avoid doing same node we came from
            {
                float distToVert;
                if(!inEdges) // travel by shortest distance or least number of roads?
                {
                    distToVert = dist[currentNodeNum - 1] + (*it).calculateDistance(nodes[i - 1]); // Calculate total distance from init to i
                } else {
                    distToVert = dist[currentNodeNum - 1] + 1; // Calculate total distance from init to i
                }

                if(distToVert < dist[i - 1])  // if this is a shorter path to i than we already knew
                {
                    dist[i - 1] = distToVert;  // then set dist for this node to new dist
                    pred[i - 1] = currentNodeNum;  // update predecessor of neighbour to this node
                }

                // If the node is already on the priority queue, don't put it on again
                if(find(explored.begin(), explored.end(), nodes[i - 1]) == explored.end())
                {
                    PQ.push_back(nodes[i - 1]); // Add neighbour to PQ
                }

            }

        }
        if(currentNodeNum != destNode) // Have we reached and explored the destination node?
        {
            explored.push_back(*it);
            PQ.remove(*it); // current vertex has been explored, remove it.
        } else {
            break; // Nothing more to consider after exploring the destination node fully.
        }
    }


    if(!inEdges)
    {
        if(dist[destNode - 1] != FLT_MAX)
        {
            list<int> path;
            int nodeNum = destNode;

            while(nodeNum != node)
            {
                path.push_back(nodeNum);
                nodeNum = pred[nodeNum - 1];
            }
            path.reverse();

            cout << "Path: " << node;

            for(int i : path)
            {
                cout << " - " << i;
            }
            cout << " // optional output" <<endl;


            cout << "d(" << node << "," << destNode << ")=  " << fixed <<  dist[destNode - 1] << "." << endl;
        } else {
            cout << "No path from " << node << " to " << destNode << endl;
            cout << "d(" << node << "," << destNode << ")=  -1.000000." << endl;
        }
    } else {
        if(dist[destNode - 1] != FLT_MAX)
        {
            list<int> path;
            int nodeNum = destNode;

            while(nodeNum != node)
            {
                path.push_back(nodeNum);
                nodeNum = pred[nodeNum - 1];
            }
            path.reverse();

            cout << "Path: " << node;

            for(int i : path)
            {
                cout << " - " << i;
            }
            cout << " // optional output" << endl;


            cout << "ed(" << node << "," << destNode << ")= " << (int) dist[destNode - 1] << "." << endl;
        } else {
            cout << "No path from " << node << " to " << destNode << endl;
            cout << "ed(" << node << "," << destNode << ")= -1." << endl;
        }
    }
}

/*
 *  RETURN POSITION OF QUEUE ELEMENT WITH SMALLEST DISTANCE
 */
int findMinDist(const list<Vertex>& PQ, const vector<float>& dist)
{
    Vertex first = PQ.front();
    float lowestDistance = dist[first.getNumber() - 1];
    int posLowestDistance = 0;
    int currentPosition = 0;

    for(Vertex v : PQ)
    {
        int vNum = v.getNumber();
        if(dist[vNum - 1] < lowestDistance)
        {
            lowestDistance = dist[vNum - 1];
            posLowestDistance = currentPosition;
        }
        currentPosition++;
    }
    return posLowestDistance;
}

/*
 *  CALCULATE LENGTH OF EDGE FROM CURRENT VERTEX TO NEXT VERTEX
 */
float Vertex::calculateDistance(const Vertex& b)
{
    float dx = b.x - this->x;
    float dy = b.y - this->y;
    dx = pow(dx, 2);
    dy = pow(dy, 2);

    return sqrt(dx + dy);

}
