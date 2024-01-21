//
//

#include "../h/Scheduler.hpp"
#include "../tests/printing.hpp"
List<TCB> Scheduler::readyThreadQueue;

TCB *Scheduler::get()
{
    return readyThreadQueue.removeFirst();
}

void Scheduler::put(TCB *tcb)
{

    readyThreadQueue.addLast(tcb);
}

bool Scheduler::isEmpty()
{
    if(!readyThreadQueue.peekFirst())
        return true;
    return false;
}
