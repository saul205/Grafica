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

    cuadrante(int xmin, int xmax, int ymin, int ymax){
        minXlimit = xmin;
        maxXlimit = xmax;
        minYlimit = ymin;
        maxYlimit = ymax;
    }

    cuadrante(){ }

};

class ConcurrentBoundedQueue{
    
    std::mutex mtx;
    std::queue<cuadrante> cbq;

    public:

        ConcurrentBoundedQueue(){ }

        void enqueue(cuadrante enq) {
            std::unique_lock<std::mutex> lck (mtx,std::defer_lock);
            lck.lock();
            cbq.push(enq);
            lck.unlock();
        };


        bool dequeue(cuadrante& frente) {
            bool extraido = false;
	        std::unique_lock<std::mutex> lck (mtx,std::defer_lock);
            lck.lock();
            if(!cbq.empty()){
                frente = cbq.front();
                extraido = true;
                cbq.pop();
            } 
            lck.unlock();
            return extraido;
        }

};

#endif