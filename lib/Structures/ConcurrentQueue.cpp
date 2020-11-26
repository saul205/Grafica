#include "ConcurrentQueue.hpp"

cuadrante::cuadrante(int xmin, int xmax, int ymin, int ymax)
{
    minXlimit = xmin;
    maxXlimit = xmax;
    minYlimit = ymin;
    maxYlimit = ymax;
}

void ConcurrentQueue::enqueue(cuadrante enq)
{
    std::unique_lock<std::mutex> lck(mtx, std::defer_lock);
    lck.lock();
    cbq.push(enq);
    lck.unlock();
};

bool ConcurrentQueue::dequeue(cuadrante &frente)
{
    bool extraido = false;
    std::unique_lock<std::mutex> lck(mtx, std::defer_lock);
    lck.lock();
    if (!cbq.empty())
    {
        frente = cbq.front();
        extraido = true;
        cbq.pop();
    }
    lck.unlock();
    return extraido;
}