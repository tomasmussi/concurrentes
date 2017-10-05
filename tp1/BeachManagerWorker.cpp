#include "BeachManagerWorker.h"
#include "Person.h"

#define MIN_PEOPLE 10

BeachManagerWorker::BeachManagerWorker(int m, const std::string& fifo_read, const std::string& fifo_write)
   : _pipe_writer(fifo_write), _pipe_reader(fifo_read), _i(0), _m(m)  {
}

BeachManagerWorker::~BeachManagerWorker() {
}

void BeachManagerWorker::sendPerson(int i) {
    Person p(i);
    std::string timestamp = Logger::get_date();
    _pipe_writer.escribir(static_cast<void*>(&p), sizeof(Person));
    Logger::log(prettyName(), Logger::DBG, "Enviada persona " + p.id() + " para jugar", timestamp);
}

int BeachManagerWorker::do_work() {
    int j;
    Logger::log(prettyName(), Logger::DBG, "Esperando para leer nuevos jugadores", Logger::get_date());
    _pipe_reader.leer(&j, sizeof(int));
    std::stringstream ss;
    ss << "Lei un nuevo jugador: " << j;
    std::string s = ss.str();
    Logger::log(prettyName(), Logger::DBG, s, Logger::get_date());

    // Esto funciona porque el NewPlayerHandler escribe numeros secuenciales. Se podria hacer que solo mande señales y
    // que el contador esté solo del lado del BeachManagerWorker
    _i = j;
    // _i representa la cantidad de usuarios que quisieron ingresar al torneo, no la cantidad actual de usuarios
    // Es decir, el número del semáforo debería empezar en _m e ir decrementandolo a medida que ingresan players
    // semaphore--
    // if semaphore != blocked ...

    // TODO: Esto es sólo si la cantidad de usuarios actuales es menor que _m!
    // Si se cumplio que ya quisieron ingresar MIN_PEOPLE personas, mando a todas haciendo que arranque el torneo
    if (j == MIN_PEOPLE - 1) {
        for(int i = 0; i < MIN_PEOPLE; i++) {
            sendPerson(i);
        }
    } else {
        // Una vez que ya arranco el torneo (Ya quisieron ingresar mas de MIN_PEOPLE), mando de a una persona
        if (j >= MIN_PEOPLE) {
            sendPerson(j);
        }
    }
    return 0;
}

void BeachManagerWorker::initialize() {
    Logger::log(prettyName(), Logger::DBG, "Inicializando pipes", Logger::get_date());
    _pipe_reader.abrir();
    _pipe_writer.abrir();
    Logger::log(prettyName(), Logger::DBG, "Fin inicializacion pipes", Logger::get_date());
}

void BeachManagerWorker::finalize() {
    Logger::log(prettyName(), Logger::DBG, "Cerrando pipes", Logger::get_date());
    _pipe_reader.cerrar();
    _pipe_writer.cerrar();
    _pipe_writer.eliminar();
    Logger::log(prettyName(), Logger::DBG, "Fin clausura de pipes", Logger::get_date());
}

std::string BeachManagerWorker::prettyName() {
    return "Beach Manager Worker";
}
