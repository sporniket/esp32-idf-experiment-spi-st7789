#ifndef MEMORY_MANAGEMENT_HPP
#define MEMORY_MANAGEMENT_HPP

// standard includes
#include <cstdint>

// esp32 includes

// project includes

// write code here

class MemoryArea {
    public:
    // Constructeur avec paramètres
    MemoryArea(uint8_t *start, uint32_t length) : start(start), length(length), end(start + length) {}

    // Destructeur
    ~MemoryArea() = default;

    // Accesseur pour start
    uint8_t *getStart() const { return start; }

    // Accesseur pour length
    uint32_t getLength() const { return length; }

    // Accesseur pour l'adresse de fin
    uint8_t *getEnd() const { return end; }

    private:
    uint8_t *const start;  // Pointeur immuable vers le début de la zone mémoire
    const uint32_t length; // Taille immuable de la zone mémoire en octets
    uint8_t *const end;    // Adresse immuable de fin calculée et stockée à la construction
};

#endif