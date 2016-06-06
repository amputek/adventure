#include <stdio.h>
#include <queue>
#include "Math.hpp"

using namespace ci;
using namespace std;
using namespace gmath;

class Graph{
  private:
    int numberOfVertices;
    list<int> *connections;
    string DFSUtil(int v, int target, bool visited[], string pathSoFar){
        visited[v] = true;
        if( v == target ) return pathSoFar + to_string(v);
        
        for (list <int>::iterator i = connections[v].begin(); i != connections[v].end(); ++i){
            if (!visited[*i]){
                string x = pathSoFar + to_string(v);
                string p = DFSUtil(*i, target, visited, x);
                if(p != "") return p;
            }
        }
        return "";
    }
  public:
    Graph(int V){
        numberOfVertices = V;
        connections = new list<int>[numberOfVertices];
    }
    void addEdge(int node, int neighbour){
        connections[node].push_back(neighbour);
    }
    
    vector<int> backtrace(int* parent, int start, int end){
        int i = end;
        vector<int> route;
        route.push_back(end);
        while( i != start ){
            i = parent[i];
            route.push_back( i );
        }
        return route;
    };
    
    vector<int> BFS(int startIndex, int targetIndex ){
        int originalStart = startIndex;
        bool* visited = new bool[numberOfVertices];
        int* parent = new int[numberOfVertices];
        for (int i = 0; i < numberOfVertices; i++) visited[i] = false;

        queue<int> q;
        q.push( startIndex );
        visited[ startIndex ] = true;
        
        while(!q.empty()){
            
            startIndex = q.front();
            if( startIndex == targetIndex ) return backtrace( parent, originalStart, targetIndex );
            q.pop();
            
            for (list<int>::iterator i = connections[startIndex].begin(); i != connections[startIndex].end(); i++){
                if (!visited[*i]){
                    parent[*i] = startIndex;
                    q.push(*i);
                    visited[*i] = true;
                }
            }
        }
        vector<int> blank;
        return blank;
    }

    string DFS(int startIndex, int targetIndex ){
        bool *visited = new bool[numberOfVertices];
        for (int i = 0; i < numberOfVertices; i++) visited[i] = false;
        return DFSUtil( startIndex, targetIndex, visited, "");
    }
};


class Node {
public:
    Node(){}
    Node( vec2 p, int n ){
        pos = p;
        name = n;
    }
    vector<Node*> neighbours;
    vec2 pos;
    int name;
};


class NavPolygon {
  public:
    NavPolygon(){}
    NavPolygon( vector<vec2> p ){
        points = p;
    }
    vec2 getCenterPos(){
        float centerX = 0;
        float centerY = 0;
        for (int i = 0; i < points.size(); i++) {
            centerX += points[i].x;
            centerY += points[i].y;
        }
        centerX /= points.size();
        centerY /= points.size();
        return vec2(centerX,centerY);
    }
    bool intersects(vec2 target){
        bool intersects = false;
        for (int i = 0, j = points.size() - 1; i < points.size(); j = i++){
            if ((((points[i].y <= target.y) && (target.y < points[j].y)) ||
                 ((points[j].y <= target.y) && (target.y < points[i].y))) &&
                (target.x < (points[j].x - points[i].x) * (target.y - points[i].y) / (points[j].y - points[i].y) + points[i].x))
                
                intersects = !intersects;
        }
        return intersects;
    }
    vector<vec2> points;
    Node * centerRef;
    vector<Node*> transitionRefs;
};



class NavMesh{
  public:
    vector<NavPolygon> polys;
    vector<Node*> nodes;
    vector< vector<vec2> > shapes;
    vector<vec2> points;
    NavMesh(){}
    bool edgesAreSame( vec2 a1, vec2 a2, vec2 b1, vec2 b2 ){
        if( a1 == b1 && a2 == b2 ) return true;
        if( a1 == b2 && a2 == b1 ) return true;
        return false;
    }
    NavMesh( vector<NavPolygon> p ){
        polys = p;
        
        for(int i = 0; i < polys.size(); i++){
            Node * centerNode = new Node(polys[i].getCenterPos(), nodes.size());
            nodes.push_back( centerNode );
            polys[i].centerRef = centerNode;
            
            shapes.push_back( polys[i].points );
        }
        
        for( int i = 0; i < polys.size(); i++ ){
            for( int j = i+1; j < polys.size(); j++ ){
                NavPolygon p1 = polys[i];
                NavPolygon p2 = polys[j];
                for( int m = 0; m < p1.points.size() - 1; m++ ){
                    for( int n = 0; n < p2.points.size() - 1; n++ ){

                        if( edgesAreSame(p1.points[m], p1.points[m+1], p2.points[n], p2.points[n+1]) ){
                            float x = (p1.points[m].x + p1.points[m+1].x) / 2;
                            float y = (p1.points[m].y + p1.points[m+1].y) / 2;
                            Node * node = new Node( vec2(x,y), nodes.size() );
                            
                            node->neighbours.push_back( polys[i].centerRef );
                            node->neighbours.push_back( polys[j].centerRef );
                            
                            for( int k = 0; k < polys[i].transitionRefs.size(); k++ ){
                                node->neighbours.push_back( polys[i].transitionRefs[k] );
                                polys[i].transitionRefs[k]->neighbours.push_back( node );
                            }
                            for( int k = 0; k < polys[j].transitionRefs.size(); k++ ){
                                node->neighbours.push_back( polys[j].transitionRefs[k] );
                                polys[j].transitionRefs[k]->neighbours.push_back( node );
                            }
                            
                            polys[i].transitionRefs.push_back( node );
                            polys[j].transitionRefs.push_back( node );
                            
                            nodes.push_back( node );
                            
                            polys[i].centerRef->neighbours.push_back( node );
                            polys[j].centerRef->neighbours.push_back( node );
                        }
                    }
                }
            }
        }
        
        for(int i = 0; i < nodes.size(); i++){
            points.push_back( nodes[i]->pos );
        }
    }
    
    NavPolygon * insidePoly( vec2 a ){
        for(int i = 0; i < polys.size(); i++){
            if(polys[i].intersects( a )){
                return &polys[i];
            }
        }
        return nullptr;
    }
    
    bool pointInsideNavMesh( vec2 a ){
        return insidePoly(a) != nullptr;
    }
    
    vector<vec2> generateRoute( vec2 atarget, vec2 playerpos ){
        
        if( distance( atarget, playerpos ) < 10.0f ){
            vector<vec2> a(0);
            return a;
        }
        
        NavPolygon * targetPoly = insidePoly( atarget );
        NavPolygon * playerPoly = insidePoly( playerpos );
        
        if( targetPoly != NULL && playerPoly != NULL ){
            
            int playerIndex = nodes.size();
            int targetIndex = nodes.size() + 1;
            
            Graph graph( nodes.size() + 2 );
            
            for (int j = 0; j < nodes.size(); j++ ){
                for (int k = 0; k < nodes[j]->neighbours.size(); k++ ){
                    graph.addEdge( nodes[j]->name, nodes[j]->neighbours[k]->name );
                }
            }
            
            
            if( targetPoly == playerPoly ) graph.addEdge( playerIndex, targetIndex );
            
            graph.addEdge( playerIndex, playerPoly->centerRef->name );
            graph.addEdge( playerPoly->centerRef->name, playerIndex );
            graph.addEdge( targetIndex, playerPoly->centerRef->name );
            graph.addEdge( targetPoly->centerRef->name, targetIndex );
            
            for (int j = 0; j < playerPoly->transitionRefs.size(); j++ ){
                graph.addEdge( playerIndex, playerPoly->transitionRefs[j]->name );
                graph.addEdge( playerPoly->transitionRefs[j]->name, playerIndex );
            }
            for (int j = 0; j < targetPoly->transitionRefs.size(); j++ ){
                graph.addEdge( targetIndex, targetPoly->transitionRefs[j]->name );
                graph.addEdge( targetPoly->transitionRefs[j]->name, targetIndex );
            }
            
            vector<int> route = graph.BFS( playerIndex, targetIndex );
            
            vector<vec2> currentRoute;
            currentRoute.push_back( playerpos );
            for (int i = route.size()-2; i > 0; i--) {
                Node * a = nodes[ route[i] ];
                currentRoute.push_back( a->pos );
            }
            currentRoute.push_back( atarget );
            
            return currentRoute;
        }
        
        vector<vec2> a(0);
        return a;
    }
    
};
