#pragma once

template <typename T> void ember::saboteur::priv::flipflop<T>::resetCellList ()
{
    static std::random_device rd;
    static std::mt19937 gen (rd ());

    for (size_t it = 0; it < bw; it++)
    {
        cellList.push_back (it);
    }
    std::shuffle (cellList.begin (), cellList.end (), gen);
    freeSaLocations = bw;
}

template <typename T> void ember::saboteur::priv::flipflop<T>::genSeuMask ()
{
    size_t pos = ember::math::random::uniform<> ((size_t)0, (size_t)bw);
    *seuMask |= 0x1 << pos;

#if defined(DEBUG) || defined(VERBOSE)
    std::cout << "-I(" << __func__ << "): Generated SeuMask " << *seuMask << std::endl;
#endif
}

template <typename T> void ember::saboteur::priv::flipflop<T>::genSa1Mask ()
{
    if (!cellList.empty ())
    {
        assert (freeSaLocations > 0);
        size_t fbit = cellList[0];

        *sa1Mask |= 0x1 << fbit;

        freeSaLocations--;
        cellList.pop_front ();

#if defined(DEBUG) || defined(VERBOSE)
        assert (fbit < bw);

        std::cout << "-I(" << __func__ << "): Generated Sa1Mask " << *sa1Mask << std::endl;
#endif
    }
}

template <typename T> void ember::saboteur::priv::flipflop<T>::genSa0Mask ()
{
    if (!cellList.empty ())
    {
        assert (freeSaLocations > 0);
        size_t fbit = cellList[0];

        *sa0Mask &= ~(0x1 << fbit);

        freeSaLocations--;
        cellList.pop_front ();

#if defined(DEBUG) || defined(VERBOSE)
        assert (fbit < bw);

        std::cout << "-I(" << __func__ << "): Generated Sa0Mask " << *sa0Mask << std::endl;
#endif
    }
}

template <typename T> void ember::saboteur::priv::flipflop<T>::applySeuFault ()
{
    dout.write (dout.read () ^ *seuMask);

#if defined(DEBUG) || defined(VERBOSE)
    std::cout << "-I(" << __func__ << "): Injecting SeuMask " << *seuMask << std::endl;
#endif

    clearSeuMask ();
}

template <typename T> void ember::saboteur::priv::flipflop<T>::applySa1Fault ()
{
    dout.write (dout.read () | *sa1Mask);

#if defined(DEBUG) || defined(VERBOSE)
    std::cout << "-I(" << __func__ << "): Injecting Sa1Mask " << *sa1Mask << std::endl;
#endif
}

template <typename T> void ember::saboteur::priv::flipflop<T>::applySa0Fault ()
{
    dout.write (dout.read () & *sa0Mask);

#if defined(DEBUG) || defined(VERBOSE)
    std::cout << "-I(" << __func__ << "): Injecting Sa0Mask " << *sa0Mask << std::endl;
#endif
}

template <typename T> void ember::saboteur::priv::flipflop<T>::clearSeuMask ()
{
    *seuMask = 0;
}

template <typename T> void ember::saboteur::priv::flipflop<T>::clearSa1Mask ()
{
    *sa1Mask = 0;
}

template <typename T> void ember::saboteur::priv::flipflop<T>::clearSa0Mask ()
{
    *sa0Mask = ~0;
}

template <typename T> const char* ember::saboteur::priv::flipflop<T>::id ()
{
    return name.c_str ();
}

template <typename T> void ember::saboteur::priv::flipflop<T>::reset ()
{
    dout.write (0);
}

template <typename T> void ember::saboteur::priv::flipflop<T>::update ()
{
    dout.write (din.read ());
}

template <typename T>
std::vector<ember::ISaboteurBase*> ember::saboteur::priv::flipflop<T>::getSaboteurs ()
{
    std::vector<ember::ISaboteurBase*> aux;
    aux.push_back (this);
    return aux;
}

template <typename T>
const size_t ember::saboteur::priv::flipflop<T>::locations (const std::type_identity<ember::fault::seu_t>) const
{
   return bw;
}

template <typename T>
const size_t ember::saboteur::priv::flipflop<T>::locations (const std::type_identity<ember::fault::sa1_t>) const
{
   return freeSaLocations;
}

template <typename T>
const size_t ember::saboteur::priv::flipflop<T>::locations (const std::type_identity<ember::fault::sa0_t>) const
{
   return freeSaLocations;
}

template <typename T>
void ember::saboteur::priv::flipflop<T>::genFaultMask (const std::type_identity<ember::fault::seu_t>)
{
   genSeuMask();
}

template <typename T>
void ember::saboteur::priv::flipflop<T>::genFaultMask (const std::type_identity<ember::fault::sa1_t>)
{
   genSa1Mask();
}

template <typename T>
void ember::saboteur::priv::flipflop<T>::genFaultMask (const std::type_identity<ember::fault::sa0_t>)
{
   genSa0Mask();
}

template <typename T>
void ember::saboteur::priv::flipflop<T>::clearFaultMask (const std::type_identity<ember::fault::seu_t>)
{
    clearSeuMask();
}

template <typename T>
void ember::saboteur::priv::flipflop<T>::clearFaultMask (const std::type_identity<ember::fault::sa1_t>)
{
    clearSa1Mask();
}

template <typename T>
void ember::saboteur::priv::flipflop<T>::clearFaultMask (const std::type_identity<ember::fault::sa0_t>)
{
    clearSa0Mask();
}

template <typename T> void ember::saboteur::priv::flipflop<T>::clearAllMasks ()
{
    clearSeuMask ();
    clearSa0Mask ();
    clearSa1Mask ();
}

template <typename T>
void ember::saboteur::priv::flipflop<T>::applyFault (const std::type_identity<ember::fault::seu_t>)
{
    applySeuFault();
}

template <typename T>
void ember::saboteur::priv::flipflop<T>::applyFault (const std::type_identity<ember::fault::sa1_t>)
{
    applySa1Fault();
}

template <typename T>
void ember::saboteur::priv::flipflop<T>::applyFault (const std::type_identity<ember::fault::sa0_t>)
{
    applySa0Fault();
}


template <typename T>
void ember::saboteur::priv::flipflop<T>::applyAllFaults ()
{
    applySeuFault ();
    applySa0Fault ();
    applySa1Fault ();
}

template <typename T>
size_t ember::saboteur::priv::flipflop<T>::faultModelCount () const
{
        return 3;
}

template <typename T>
ember::saboteur::priv::flipflop<T>::flipflop (const size_t& bitwidth)
    : name ("reg"), bw (bitwidth), din ("din"), dout ("dout")
{
    // dout.bind();

    seuMask = new ember::fault::mask_t;
    sa0Mask = new ember::fault::mask_t;
    sa1Mask = new ember::fault::mask_t;

    clearSeuMask ();
    clearSa0Mask ();
    clearSa1Mask ();

    resetCellList ();
}

template <typename T>
ember::saboteur::priv::flipflop<T>::flipflop (const char* instanceId, const size_t& bitwidth)
    : name (instanceId), bw (bitwidth), din ("din"), dout ("dout")
{
    // dout.bind();

    seuMask = new ember::fault::mask_t;
    sa0Mask = new ember::fault::mask_t;
    sa1Mask = new ember::fault::mask_t;

    clearSeuMask ();
    clearSa0Mask ();
    clearSa1Mask ();

    resetCellList ();
}

template <typename T> ember::saboteur::priv::flipflop<T>::~flipflop ()
{
    delete seuMask;
    delete sa0Mask;
    delete sa1Mask;
}