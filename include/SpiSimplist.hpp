#ifndef SPI_SIMPLIST_HPP
#define SPI_SIMPLIST_HPP

// standard includes
#include <cstdint>
#include <map>

// esp32 includes

// project includes
#include "SpiSimplistTypes.hpp"

/** @brief What the class is for.
 */
template<typename TypeOfPreTransactionListener>
class SpiSimplist {
    protected:
        std::map<SpiIdentifier, SpiHostSpecs<TypeOfPreTransactionListener> *> hosts;
    public:
        SpiSimplist() {}
        virtual ~SpiSimplist()
        SpiSimplist *withHost(SpiHostSpecs<TypeOfPreTransactionListener> host) {
            hosts[host->getId()] = host ;
            return this ;
        }
        SpiHostSpecs<TypeOfPreTransactionListener> * getHost(SpiIdentifier id) {
            return (hosts.count(id) > 0) ? return hosts[id] : (nullptr) 0 ;
        }

        /**
         * @brief use all the provided specifications so far and instanciate the internal management of the SPI ports.
         * 
         */
        virtual void start() = 0 ;
} ;

#endif