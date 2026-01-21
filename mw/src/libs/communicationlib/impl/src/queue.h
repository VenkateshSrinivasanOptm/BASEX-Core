#ifndef QUEUE_H
#define QUEUE_H

#include <QQueue>
#include <QMutex>
#include <QWaitCondition>
#include <QSharedPointer>
#include <QDebug>


template<typename T> class Queue
{
public:
    Queue(): run(true){}

    virtual ~Queue()
    {
        queue.clear();
    }

    void addToQueue(QSharedPointer<T> const &obj)
    {
        queueMutex.lock();
         queue.enqueue(obj);
         condition.wakeOne();
        queueMutex.unlock();
    }

    QSharedPointer<T> removeFromQueue()
    {
        queueMutex.lock();
        if(queue.isEmpty())
        {
          if(run)condition.wait(&queueMutex);
          if(queue.isEmpty()) {
            queueMutex.unlock();
           return QSharedPointer<T>();
          }
         else
         {
           QSharedPointer<T> obj(queue.dequeue());
           queueMutex.unlock();
            return obj;
         }
       }
       else
      {
       QSharedPointer<T> obj(queue.dequeue());
       queueMutex.unlock();
       return obj;
      }
   }

   void clearQueue()
   {
       queueMutex.lock();
        qDebug() << __FUNCTION__ << ":" << __LINE__ << "clear queue ";
        queue.clear();
        run=false;
        condition.wakeAll();
       queueMutex.unlock();
   }

private:
    Q_DISABLE_COPY(Queue);
    QQueue< QSharedPointer<T> > queue;
    QMutex queueMutex;
    QWaitCondition condition;
    volatile bool run;
};

#endif // QUEUE_H
