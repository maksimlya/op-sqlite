#ifndef ThreadPool_h
#define ThreadPool_h

#include <condition_variable>
#include <exception>
#include <mutex>
#include <queue>
#include <stdio.h>
#include <thread>
#include <vector>

namespace opsqlite {

class Delegate {
public:
    // Default constructor
    Delegate() : callable(nullptr) {}

    // Template constructor to accept various callable types
    template <typename Callable>
    Delegate(Callable&& func) : callable(std::make_unique<CallableModel<Callable>>(std::forward<Callable>(func))) {}

    // Call operator to invoke the stored callable
    void operator()() const {
        if (callable) {
            callable->call();
        } else {
            throw std::runtime_error("Attempted to call an empty delegate");
        }
    }

private:
    // Concept for type erasure
    struct CallableConcept {
        virtual ~CallableConcept() = default;
        virtual void call() const = 0;
    };

    // Model to wrap callable types
    template <typename Callable>
    struct CallableModel : CallableConcept {
        CallableModel(Callable&& func) : func(std::forward<Callable>(func)) {}
        void call() const override { func(); }
        Callable func;
    };

    std::unique_ptr<CallableConcept> callable;
};

class ThreadPool {
public:
  ThreadPool();
  ~ThreadPool();
  void queueWork(Delegate task);
  void waitFinished();
  void restartPool();

private:
  unsigned int busy;
  // This condition variable is used for the threads to wait until there is work
  // to do
  std::condition_variable_any workQueueConditionVariable;

  // We store the threads in a vector, so we can later stop them gracefully
  std::vector<std::thread> threads;

  // Mutex to protect workQueue
  std::mutex workQueueMutex;

  // Queue of requests waiting to be processed
  std::queue<Delegate> workQueue;

  // This will be set to true when the thread pool is shutting down. This tells
  // the threads to stop looping and finish
  bool done;

  // Function used by the threads to grab work from the queue
  void doWork();
};

} // namespace opsqlite

#endif /* ThreadPool_h */
