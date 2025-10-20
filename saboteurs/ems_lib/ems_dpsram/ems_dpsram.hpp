#ifndef __EMBER_SABOTEURS_DPSRAM_HPP__
#define __EMBER_SABOTEURS_DPSRAM_HPP__

#include <deque>
#include <type_traits> // Required for std::is_array

#include "em_isaboteur.hpp"
#include "em_fault.hpp"
#include "em_types.hpp"

namespace ember
{
namespace saboteur
{

// General Declaration
template<typename T>
requires(!is_array<T>::value)
class dpsram : public ember::ISaboteur, ember::IModule
{
protected:
    const std::string name;
    const size_t lineStride;

    const size_t depth;
    const size_t totalBw;
    const size_t lineBw;
    const size_t elementBw;

    size_t freeSaLocations;
    std::deque<size_t> cellList;

    fault::mask_t *seuMask;
    fault::mask_t *sa0Mask;
    fault::mask_t *sa1Mask;

    array<T> aux;
    T *data;

    void randomize() {
        for (size_t it = 0; it < (depth * lineStride); it++) {
            data[it] = ember::math::random::random<T>(elementBw);
        }
    }

    void resetCellList() {
        static std::random_device rd;
        static std::mt19937 gen(rd());

        cellList.clear();
        for (size_t it = 0; it < totalBw; it++) {
            cellList.push_back(it);
        }
        std::shuffle(cellList.begin(), cellList.end(), gen);
        freeSaLocations = totalBw;
    }

    void genSeuMask() {
        size_t fpos =
            ember::math::random::uniform<>((size_t)0, (size_t)(totalBw));
        size_t fword = fpos / (elementBw);
        size_t fbit = fpos % (elementBw);
        seuMask[fword] |= 0x1 << fbit;

#if defined(DEBUG) || defined(VERBOSE)
        std::cout << "-I(" << __func__ << "): Generated SeuMask "
                  << seuMask[fword] << " at line " << fword << std::endl;
#endif
    }

    void genSa1Mask() {
        if (!cellList.empty()) {
            assert(freeSaLocations > 0);
            size_t fpos = cellList[0];

            size_t fword = fpos / (elementBw);
            size_t fbit = fpos % (elementBw);
            sa1Mask[fword] |= 0x1 << fbit;

            freeSaLocations--;
            cellList.pop_front();

#if defined(DEBUG) || defined(VERBOSE)
            assert(fbit < elementBw);
            assert(fword < depth * lineStride);

            std::cout << "-I(" << __func__ << "): Generated Sa0Mask "
                      << sa1Mask[fword] << " at line " << fword << std::endl;
#endif
        }
    }

    void genSa0Mask() {
        if (!cellList.empty()) {
            assert(freeSaLocations > 0);
            size_t fpos = cellList[0];

            size_t fword = fpos / (elementBw);
            size_t fbit = fpos % (elementBw);
            sa0Mask[fword] &= ~(0x1 << fbit);

            freeSaLocations--;
            cellList.pop_front();

#if defined(DEBUG) || defined(VERBOSE)
            assert(fbit < elementBw);
            assert(fword < depth * lineStride);

            std::cout << "-I(" << __func__ << "): Generated Sa0Mask "
                      << sa0Mask[fword] << " at line " << fword << std::endl;
#endif
        }
    }
    void applySeuFault() {
        for (size_t it = 0; it < (depth * lineStride); it++) {
            data[it] ^ seuMask[it];
        }

#if defined(DEBUG) || defined(VERBOSE)
        std::cout << "-I(" << __func__ << "): Injecting SeuMask" << std::flush;
        for (size_t it = 0; it < (depth * lineStride); it++) {
            std::cout << " " << seuMask[it] << std::flush;
        }
        std::cout << std::endl;
#endif

        clearSeuMask();
    }
    void applySa1Fault() {
        for (size_t it = 0; it < (depth * lineStride); it++) {
            data[it] | sa1Mask[it];
        }

#if defined(DEBUG) || defined(VERBOSE)
        std::cout << "-I(" << __func__ << "): Injecting Sa1Mask" << std::flush;
        for (size_t it = 0; it < (depth * lineStride); it++) {
            std::cout << " " << sa1Mask[it] << std::flush;
        }
        std::cout << std::endl;
#endif
    }

    void applySa0Fault() {
        for (size_t it = 0; it < (depth * lineStride); it++) {
            data[it] & sa0Mask[it];
        }

#if defined(DEBUG) || defined(VERBOSE)
        std::cout << "-I(" << __func__ << "): Injecting Sa0Mask" << std::flush;
        for (size_t it = 0; it < (depth * lineStride); it++) {
            std::cout << " " << sa0Mask[it] << std::flush;
        }
        std::cout << std::endl;
#endif
    }

    void clearSeuMask() {
        for (size_t it = 0; it < (depth * lineStride); it++) {
            seuMask[it] = 0;
        }
    }

    void clearSa1Mask() {
        for (size_t it = 0; it < (depth * lineStride); it++) {
            sa1Mask[it] = 0;
        }
    }

    void clearSa0Mask() {
        for (size_t it = 0; it < (depth * lineStride); it++) {
            sa0Mask[it] = ~0;
        }
    }

  public:
    // Ensure T is not an array type
    static_assert(!std::is_array_v<T>, "T cannot be an array type");

    // IO Ports
    inPort<bool> we;
    inPort<size_t> wAddr;
    inPort<array<T>> din;

    inPort<bool> re;
    inPort<size_t> rAddr;
    outPort<array<T>> dout;

    // Methods - IModule
    const char *id() { return name.c_str(); }

    void update() {
        // Read First
        if (re.read()) {
            if (rAddr.read() < depth) {
                aux.memcpy(&data[rAddr.read() * lineStride]);
                dout.write(aux);
            }
#ifdef VERBOSE
            else {
                std::cout << "-W(" << __FILE__ << ", " << __func__
                          << "): Out of scope read address (given="
                          << rAddr.read() << " / depth=" << depth << ")"
                          << std::endl;
            }
#endif
        }

        // Write
        if (we.read()) {
            if (wAddr.read() < depth) {
                aux = din.read();
                aux.cpArray(&data[wAddr.read() * lineStride]);
            }
#ifdef VERBOSE
            else {
                std::cout << "-W(" << __FILE__ << ", " << __func__
                          << "): Out of scope write address (given="
                          << rAddr.read() << " / depth=" << depth << ")"
                          << std::endl;
            }
#endif
        }
    }
    void eval() {}
    std::vector<ember::ISaboteur *> getSaboteurs() {
        std::vector<ember::ISaboteur *> aux;
        aux.push_back(this);
        return aux;
    }
    void reset() { std::fill(&data[0], &data[(depth * lineStride)], 0); }

    // Methods - ISaboteur
    const size_t locations(const ember::fault::model &fModel) const {
        switch (fModel) {
        case ember::fault::model::seu: {
            return totalBw;
            break;
        }
        case ember::fault::model::sa0: {
            return freeSaLocations;
            break;
        }
        case ember::fault::model::sa1: {
            return freeSaLocations;
            break;
        }
        default: {
            throw std::invalid_argument("fModel is not supported");
        }
        }
    }

    void genFaultMask(const ember::fault::model &fModel) {
        switch (fModel) {
        case ember::fault::model::seu: {
            genSeuMask();
            break;
        }
        case ember::fault::model::sa0: {
            genSa0Mask();
            break;
        }
        case ember::fault::model::sa1: {
            genSa1Mask();
            break;
        }
        default: {
            throw std::invalid_argument("fModel is not supported");
        }
        }
    }

    void clearFaultMask(const ember::fault::model &fModel) {
        switch (fModel) {
        case ember::fault::model::seu: {
            clearSeuMask();
            break;
        }
        case ember::fault::model::sa0: {
            clearSa0Mask();
            break;
        }
        case ember::fault::model::sa1: {
            clearSa1Mask();
            break;
        }
        default: {
            throw std::invalid_argument("fModel is not supported");
        }
        }
    }
    void clearAllMasks() {
        clearSeuMask();
        clearSa0Mask();
        clearSa1Mask();
    }

    void applyFault(const ember::fault::model &fModel) {
        switch (fModel) {
        case ember::fault::model::seu: {
            applySeuFault();
            break;
        }
        case ember::fault::model::sa0: {
            applySa0Fault();
            break;
        }
        case ember::fault::model::sa1: {
            applySa1Fault();
            break;
        }
        default: {
            throw std::invalid_argument("fModel is not supported");
        }
        }
    }

    void applyAllFaults() {
        applySeuFault();
        applySa0Fault();
        applySa1Fault();
    }

    // CTORS
    dpsram(const size_t depth, const size_t linesize, const size_t bitwidth)
        : lineStride(linesize), depth(depth),
          totalBw(depth * linesize * bitwidth), lineBw(linesize * bitwidth),
          elementBw(bitwidth), we("we"), re("re"), wAddr("wAddr"),
          rAddr("rAddr"), din("din", linesize, bitwidth),
          dout("dout", linesize, bitwidth), name("dpsram"),
          aux(linesize, bitwidth) {
        data = new T[depth * linesize];
        seuMask = new ember::fault::mask_t[depth * linesize];
        sa0Mask = new ember::fault::mask_t[depth * linesize];
        sa1Mask = new ember::fault::mask_t[depth * linesize];

        resetCellList();
        randomize();
    }

    dpsram(const char *instanceId, const size_t depth, const size_t linesize,
           const size_t bitwidth)
        : lineStride(linesize), depth(depth),
          totalBw(depth * linesize * bitwidth), lineBw(linesize * bitwidth),
          elementBw(bitwidth), name(instanceId), we("we"), re("re"),
          wAddr("wAddr"), rAddr("rAddr"), din("din", linesize, bitwidth),
          dout("dout", linesize, bitwidth), aux(linesize, bitwidth) {
        data = new T[depth * linesize];
        seuMask = new ember::fault::mask_t[depth * linesize];
        sa0Mask = new ember::fault::mask_t[depth * linesize];
        sa1Mask = new ember::fault::mask_t[depth * linesize];

        resetCellList();
        randomize();
    }
    ~dpsram() {
        delete[] data;
        delete[] seuMask;
        delete[] sa0Mask;
        delete[] sa1Mask;
    }
};

} // namespace saboteur
    
} // namespace ember

#endif
