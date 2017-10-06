#ifndef TP1_SEMAPHORE_H_
#define TP1_SEMAPHORE_H_

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <string>

class Semaphore {

private:
    int id;
    int initial_value;

    int initialize() const;

public:
    Semaphore(const std::string &name, const int initial_value);

    ~Semaphore();

    int p() const;  // decrementa
    int v() const;  // incrementa

    int p(short n) const; // decrementa n
    int v(short n) const; // incrementa n

    int w() const;  // espera a que sea 0
    void remove() const;
};

#endif /* TP1_SEMAPHORE_H_ */
