#ifndef FIFOESCRITURA_H_
#define FIFOESCRITURA_H_

#include "Fifo.h"

class FifoEscritura : public Fifo {
public:
	FifoEscritura(const std::string nombre);
	~FifoEscritura();

	void abrir();
	ssize_t escribir(const void* buffer,const ssize_t buffsize) const;
};

#endif /* FIFOESCRITURA_H_ */
