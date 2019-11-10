#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <cstdlib>
#include <ctime>
#include <string>
#include<sstream>

using std::cout;
using std::cin;
using std::endl;
typedef unsigned long long int UINT64;

struct Vertex
{
    int v;
    bool isTree = false;
    unsigned long long int randUINT64;
    bool isNewRand = true;
    unsigned long long int metka;
    bool isMetka = false;
    bool isBridge = false;
};
typedef std::vector< std::list<Vertex> > Graph;

const int sizeN = 100;
float c = 1.5;
int sizeM = (int)(c * (float)sizeN);
Graph graph;

Graph createGraph(int sizeN, int sizeM)
{
    Graph f_graph;
    std::list<Vertex> listVertex;
    int sizeAllM = sizeM;

    for (int i = 0; i <= sizeN - 1; i++)
    {
        f_graph.push_back(listVertex);
    }
    cout << "Size of Graph: " << f_graph.size() << " (" << sizeN << ")" << endl;

    int v = 0, u = 0;
    bool isOld;
    Vertex vertex = {0, false, 0, 0};
    while (sizeM > 0)
    {
        isOld = false;
        u = rand() % sizeN;
        v = rand() % sizeN;
        if (u == v)
        {
            continue;
        }

        //cout << "SizeM=" << sizeM << " Try vertex: u=" << u << " v=" << v << endl;
        for (auto& element : f_graph[u])
        {
            if (element.v == v)
            {
                isOld = true;
                break;
            }
        }

        if (isOld == true)
        {
            continue;
        }
        else
        {
            vertex.v = v;
            f_graph[u].push_back(vertex);
            if (v != u)
            {
                vertex.v = u;
                f_graph[v].push_back(vertex);
            }
            cout << "New vertex " << sizeAllM - sizeM <<": (" << u << "," << v << ")" << " (" << v << "," << u << ")" << endl;
        }

        sizeM -= 1;
    }

    for (int i = 0; i < f_graph.size(); i++)
    {
        cout << "Vertex " << i << ": ";
        for (auto& itemList : f_graph[i])
        {
            cout << itemList.v << " ";
        }
        cout << endl;
    }
    return f_graph;
}

void writeGraph(Graph graph, std::string name)
{
    std::ofstream myfile;
    myfile.open (name);
    for (int i = 0; i < graph.size(); i++)
    {
        for (auto& itemList : graph[i])
        {
            myfile << itemList.v << " ";
        }
        myfile << endl;
    }
    myfile.close();
}

Graph readGraph(std::string name, int sizeN)
{
    int i = 0;
    Vertex vertex = {0, false, 0, true,0, false, false};
    Graph graph;
    std::list<Vertex> listVertex;
    for (int i = 0; i <= sizeN - 1; i++)
    {
        graph.push_back(listVertex);
    }

    std::ifstream input( name );
    for( std::string line; getline( input, line ); )
    {
        std::stringstream str_strm;
        str_strm << line; //convert the string s into stringstream
        std::string temp_str;
        int temp_int;
        while(!str_strm.eof()) {
            str_strm >> temp_str; //take words into temp_str one by one
            if(std::stringstream(temp_str) >> temp_int) { //try to convert string to int
                //cout << temp_int << " ";
                vertex.v = temp_int;
                graph[i].push_back(vertex);
            }
            temp_str = ""; //clear temp string
        }
        //cout << endl;

        i++;
    }

    return graph;
}

void showGraph(Graph graph)
{
    for (int i = 0; i < graph.size(); i++)
    {
        cout << "Vertex " << i << ": ";
        for (auto& itemList : graph[i])
        {
            cout << itemList.v << " ";
        }
        cout << endl;
    }
}

UINT64 getRandom(const UINT64 &begin = 0, const UINT64 &end = 100) {
    return begin >= end ? 0 : begin + (UINT64) rand()*((end - begin)/RAND_MAX);
    //return rand()%2;
};

/* lab 1_1 */
int timer;
int enter[sizeN], ret[sizeN];
bool used[sizeN];

void dfs_d (int v, int p = -1)
{
    used[v] = true;
    enter[v] = ret[v] = timer++;
    for (auto& item : graph[v])
    {
        int to = item.v;
        if ( to == p )
            continue;
        if ( used[to] )
            ret[v] = std::min(ret[v],enter[to]);
        else
        {
            dfs_d (to, v);
            ret[v] = std::min(ret[v], ret[to]);
            if ( ret[to] > enter[v] )
                cout << "New bridge: (" << v << ", " << to << ")" << endl;
        }
    }
}
void findBridges_d()
{
    timer = 0;
    for(int i = 0; i < sizeN; ++i)
    {
        used[i] = false;
    }
    for(int i = 0; i < sizeN; ++i)
    {
        if(!used[i]) dfs_d(i,-1);
    }
}
/* End of lab 1_1 */

/* lab 1_2 */
void dfs_r(int v, int p = -1, int initV = 0)
{
    //cout << "Vertex tree: " << v << " -> " << endl;
    used[v] = true;

    for (auto& item : graph[v])
    {
        int to = item.v;
        if ( to == p )
            continue;
        if (!used[to])
        {
            item.isTree = true;
            for (auto& vertex : graph[to])
            {
                if (vertex.v == v)
                    vertex.isTree = true;
            }

            dfs_r (to, v, initV);
        }
    }

    UINT64 sum = 0;
    for (auto& item : graph[v])
    {
        int to = item.v;
        if (to != p) // its not metka
        {
            //cout << "try rand" << endl;
            if (item.isMetka == true)
            {
                sum ^= item.metka;
            }
            else if (item.isNewRand == true)
            {
                item.randUINT64 = getRandom(0, ULLONG_MAX);
                item.isNewRand = false;
                for (auto& vertex : graph[to])
                {
                    if (vertex.v == v)
                    {
                        vertex.randUINT64 = item.randUINT64;
                        vertex.isNewRand = false;
                    }
                }
                //cout << "Rand (" << v << ", " << to << ") = " << item.randUINT64 << endl;
            }
            sum ^= item.randUINT64;
        }

    }

    for (auto& item : graph[v])
    {
        int to = item.v;
        if (to == p)
        {
            if (item.isTree == true)
            {
                if (to == initV) {
                    item.metka = getRandom(0, ULLONG_MAX);
                    //cout << "Rand ";
                }
                else {
                    item.metka = sum;
                }
                item.isMetka = true;
                for (auto& vertex : graph[to])
                {
                    if (vertex.v == v)
                    {
                        vertex.isMetka = true;
                        vertex.metka = item.metka;
                        break;
                    }
                }
                //cout << "Metka (" << v << ", " << to << ") = " << item.metka << endl;
                if (item.metka == 0)
                {
                    item.isBridge = true;
                    for (auto& vertex : graph[to])
                    {
                        if (vertex.v == v)
                        {
                            vertex.isBridge = true;
                            break;
                        }
                    }
                }
                break;
            }
        }
    }
}
void findBridges_r()
{
    for(int i = 0; i < sizeN; ++i)
    {
        used[i] = false;
    }
    for(int i = 0; i < sizeN; ++i)
    {
        if(!used[i]) dfs_r(i, -1, i);
    }


    for (int i = 0; i < sizeN; i++)
    {
        for (auto& item : graph[i])
        {
            if (item.isBridge == true)
            {
                cout << "New bridge: (" << i << ", " << item.v << "); sum = " << item.metka << endl;
            }
            for (auto& vertex : graph[item.v])
            {
                if (vertex.v == i)
                {
                    vertex.isBridge = false;
                }
            }
        }
    }
}
/* End of lab 1_2 */


int main() {
    std::srand(unsigned(std::time(0)));

    //graph = createGraph(sizeN,sizeM);
    //writeGraph(graph, "graph8_12.txt");
    graph = readGraph("graph100_200.txt", sizeN);
    //showGraph(graph);

    findBridges_d();
    cout << endl;

    findBridges_r();
    cout << endl;

    
    return 0;
}