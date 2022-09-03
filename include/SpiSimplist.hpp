#ifndef SPI_SIMPLIST_HPP
#define SPI_SIMPLIST_HPP

// standard includes
#include <cstdint>
#include <map>

// esp32 includes

// project includes
#include "SpiSimplistTypes.hpp"

static const char *TAG_SPI_SIMPLIST = "SpiSimplist";

/** @brief What the class is for.
 */
template <typename TypeOfPreTransactionListener> class SpiSimplist {
    protected:
    std::map<SpiIdentifier, SpiHostSpecs *> hosts;

    public:
    SpiSimplist() {}
    virtual ~SpiSimplist();
    SpiSimplist *withHost(SpiHostSpecs *host) {
        hosts[host->getId()] = host;
        return this;
    }
    SpiHostSpecs *getHost(SpiIdentifier id) { return (hosts.count(id) > 0) ? hosts[id] : nullptr; }

    /**
     * @brief use all the provided specifications so far and instanciate the internal management of the SPI ports.
     *
     */
    virtual void start() = 0;
};

#endif