#ifndef CONCURRENTBOUNDEDQUEUE_H
#define CONCURRENTBOUNDEDQUEUE_H

#include <mutex>
#include <queue>
#include <memory>

using namespace std;

struct cuadrante{

    int minXlimit = 0;
    int maxXlimit = 0;
    int minYlimit = 0;
    int maxYlimit = 0;

    cuadrante(int xmin, int xmax, int ymin, int ymax);

    cuadrante(){ }

};

class ConcurrentQueue{
    
    std::mutex mtx;
    std::queue<cuadrante> cbq;

    public:

        ConcurrentQueue(){ }

        // Encola el cuadrante enq
        void enqueue(cuadrante enq);

        // Desencola un cuadrante
        bool dequeue(cuadrante& frente);
};

#endif