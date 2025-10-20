
#include <iostream>
#include "em_imodule.hpp"
#include "em_isaboteur.hpp"
#include "em_port.hpp"
#include "em_string.hpp"

#include "ems_stdcells.hpp"

const char* ember::saboteur::priv::gate::id()
{
    return name.c_str();
}

std::vector<ember::ISaboteur*> ember::saboteur::priv::gate::getSaboteurs()
{
    std::vector<ember::ISaboteur*> ret;
    ret.push_back(dynamic_cast<ember::ISaboteur*>(this));
    return ret;
}

const size_t ember::saboteur::priv::gate::locations(const ember::fault::model& fModel) const
{
    switch(fModel)
    {
        case ember::fault::model::seu:
        {
            return 1;
            break;
        }
        case ember::fault::model::sa0:
        {
            return (sa0Mask || sa1Mask)? 0 : 1;
            break;
        }
        case ember::fault::model::sa1:
        {
            return (sa0Mask || sa1Mask)? 0 : 1;
            break;
        }
        default:
        {
            throw std::invalid_argument("fModel is not supported");
        }
    }
}

void ember::saboteur::priv::gate::genFaultMask(const ember::fault::model& fModel)
{
    switch(fModel)
    {
        case ember::fault::model::seu:
        {
            seuMask = true;
            break;
        }
        case ember::fault::model::sa0:
        {
            sa0Mask = true;
            break;
        }
        case ember::fault::model::sa1:
        {
            sa1Mask = true;
            break;
        }
        default:
        {
            throw std::invalid_argument("fModel is not supported");
        }
    }
}

void ember::saboteur::priv::gate::clearFaultMask(const ember::fault::model& fModel)
{
    switch(fModel)
    {
        case ember::fault::model::seu:
        {
            seuMask = false;
            break;
        }
        case ember::fault::model::sa0:
        {
            sa0Mask = false;
            break;
        }
        case ember::fault::model::sa1:
        {
            sa1Mask = false;
            break;
        }
        default:
        {
            throw std::invalid_argument("fModel is not supported");
        }
    }
}

void ember::saboteur::priv::gate::clearAllMasks()
{
    seuMask = false;
    sa0Mask = false;
    sa1Mask = false;
}

////////////////////////////////////////////////////////////

// BUF
void ember::saboteur::buf::eval()
{
    if(sa1Mask) {
        Z.write(true);
    }
    else if(sa0Mask) {
        Z.write(false);
    }
    else if(seuMask) {
        Z.write(!A.read());
    }
    else {
        Z.write(A.read());
    }
}

ember::saboteur::buf::buf(const char* _id)
    :   ember::saboteur::priv::gate(_id),
        A(ember::hiercat(name, "A")),
        Z(ember::hiercat(name, "Z"))
{ }

ember::saboteur::buf::buf(const std::string _id)
    :   ember::saboteur::priv::gate(_id),
        A(ember::hiercat(name, "A")),
        Z(ember::hiercat(name, "Z"))
{ }

// INV
void ember::saboteur::inv::eval()
{
    if(sa1Mask) {
        Z.write(true);
    }
    else if(sa0Mask) {
        Z.write(false);
    }
    else if(seuMask) {
        Z.write(A.read());
    }
    else {
        Z.write(!A.read());
    }
}

ember::saboteur::inv::inv(const char* _id)
    :   ember::saboteur::priv::gate(_id),
        A(ember::hiercat(name, "A")),
        Z(ember::hiercat(name, "Z"))
{ }

ember::saboteur::inv::inv(const std::string _id)
    :   ember::saboteur::priv::gate(_id),
        A(ember::hiercat(name, "A")),
        Z(ember::hiercat(name, "Z"))
{ }

// AND2
void ember::saboteur::and2::eval()
{
    if(sa1Mask) {
        Z.write(true);
    }
    else if(sa0Mask) {
        Z.write(false);
    }
    else if(seuMask) {
        Z.write(!(A.read() && B.read()));
    }
    else {
        Z.write(A.read() && B.read());
    }
}

ember::saboteur::and2::and2(const char* _id)
    :   ember::saboteur::priv::gate(_id),
        A(ember::hiercat(name, "A")),
        B(ember::hiercat(name, "B")),
        Z(ember::hiercat(name, "Z"))
{ }

ember::saboteur::and2::and2(const std::string _id)
    :   ember::saboteur::priv::gate(_id),
        A(ember::hiercat(name, "A")),
        B(ember::hiercat(name, "B")),
        Z(ember::hiercat(name, "Z"))
{ }

// AND3
void ember::saboteur::and3::eval()
{
    if(sa1Mask) {
        Z.write(true);
    }
    else if(sa0Mask) {
        Z.write(false);
    }
    else if(seuMask) {
        Z.write(!(A.read() && B.read() && C.read()));
    }
    else {
        Z.write(A.read() && B.read() && C.read());
    }
}

ember::saboteur::and3::and3(const char* _id)
    :   ember::saboteur::priv::gate(_id),
        A(ember::hiercat(name, "A")),
        B(ember::hiercat(name, "B")),
        C(ember::hiercat(name, "C")),
        Z(ember::hiercat(name, "Z"))
{ }

ember::saboteur::and3::and3(const std::string _id)
    :   ember::saboteur::priv::gate(_id),
        A(ember::hiercat(name, "A")),
        B(ember::hiercat(name, "B")),
        C(ember::hiercat(name, "C")),
        Z(ember::hiercat(name, "Z"))
{ }

// NAND2
void ember::saboteur::nand2::eval()
{
    if(sa1Mask) {
        Z.write(true);
    }
    else if(sa0Mask) {
        Z.write(false);
    }
    else if(seuMask) {
        Z.write(A.read() && B.read());
    }
    else {
        Z.write(!(A.read() && B.read()));
    }
}

ember::saboteur::nand2::nand2(const char* _id)
    :   ember::saboteur::priv::gate(_id),
        A(ember::hiercat(name, "A")),
        B(ember::hiercat(name, "B")),
        Z(ember::hiercat(name, "Z"))
{ }

ember::saboteur::nand2::nand2(const std::string _id)
    :   ember::saboteur::priv::gate(_id),
        A(ember::hiercat(name, "A")),
        B(ember::hiercat(name, "B")),
        Z(ember::hiercat(name, "Z"))
{ }

// NAND3
void ember::saboteur::nand3::eval()
{
    if(sa1Mask) {
        Z.write(true);
    }
    else if(sa0Mask) {
        Z.write(false);
    }
    else if(seuMask) {
        Z.write(A.read() && B.read() && C.read());
    }
    else {
        Z.write(!(A.read() && B.read() && C.read()));
    }
}

ember::saboteur::nand3::nand3(const char* _id)
    :   ember::saboteur::priv::gate(_id),
        A(ember::hiercat(name, "A")),
        B(ember::hiercat(name, "B")),
        C(ember::hiercat(name, "C")),
        Z(ember::hiercat(name, "Z"))
{ }

ember::saboteur::nand3::nand3(const std::string _id)
    :   ember::saboteur::priv::gate(_id),
        A(ember::hiercat(name, "A")),
        B(ember::hiercat(name, "B")),
        C(ember::hiercat(name, "C")),
        Z(ember::hiercat(name, "Z"))
{ }

// OR2
void ember::saboteur::or2::eval()
{
    if(sa1Mask) {
        Z.write(true);
    }
    else if(sa0Mask) {
        Z.write(false);
    }
    else if(seuMask) {
        Z.write(!(A.read() || B.read()));
    }
    else {
        Z.write(A.read() || B.read());
    }
}

ember::saboteur::or2::or2(const char* _id)
    :   ember::saboteur::priv::gate(_id),
        A(ember::hiercat(name, "A")),
        B(ember::hiercat(name, "B")),
        Z(ember::hiercat(name, "Z"))
{ }

ember::saboteur::or2::or2(const std::string _id)
    :   ember::saboteur::priv::gate(_id),
        A(ember::hiercat(name, "A")),
        B(ember::hiercat(name, "B")),
        Z(ember::hiercat(name, "Z"))
{ }

// OR3
void ember::saboteur::or3::eval()
{
    if(sa1Mask) {
        Z.write(true);
    }
    else if(sa0Mask) {
        Z.write(false);
    }
    else if(seuMask) {
        Z.write(!(A.read() || B.read() || C.read()));
    }
    else {
        Z.write(A.read() || B.read() || C.read());
    }
}

ember::saboteur::or3::or3(const char* _id)
    :   ember::saboteur::priv::gate(_id),
        A(ember::hiercat(name, "A")),
        B(ember::hiercat(name, "B")),
        C(ember::hiercat(name, "C")),
        Z(ember::hiercat(name, "Z"))
{ }

ember::saboteur::or3::or3(const std::string _id)
    :   ember::saboteur::priv::gate(_id),
        A(ember::hiercat(name, "A")),
        B(ember::hiercat(name, "B")),
        C(ember::hiercat(name, "C")),
        Z(ember::hiercat(name, "Z"))
{ }

// NOR2
void ember::saboteur::nor2::eval()
{
    if(sa1Mask) {
        Z.write(true);
    }
    else if(sa0Mask) {
        Z.write(false);
    }
    else if(seuMask) {
        Z.write(A.read() || B.read());
    }
    else {
        Z.write(!(A.read() || B.read()));
    }
}

ember::saboteur::nor2::nor2(const char* _id)
    :   ember::saboteur::priv::gate(_id),
        A(ember::hiercat(name, "A")),
        B(ember::hiercat(name, "B")),
        Z(ember::hiercat(name, "Z"))
{ }

ember::saboteur::nor2::nor2(const std::string _id)
    :   ember::saboteur::priv::gate(_id),
        A(ember::hiercat(name, "A")),
        B(ember::hiercat(name, "B")),
        Z(ember::hiercat(name, "Z"))
{ }

// NOR3
void ember::saboteur::nor3::eval()
{
    if(sa1Mask) {
        Z.write(true);
    }
    else if(sa0Mask) {
        Z.write(false);
    }
    else if(seuMask) {
        Z.write(A.read() || B.read() || C.read());
    }
    else {
        Z.write(!(A.read() || B.read() || C.read()));
    }
}

ember::saboteur::nor3::nor3(const char* _id)
    :   ember::saboteur::priv::gate(_id),
        A(ember::hiercat(name, "A")),
        B(ember::hiercat(name, "B")),
        C(ember::hiercat(name, "C")),
        Z(ember::hiercat(name, "Z"))
{ }

ember::saboteur::nor3::nor3(const std::string _id)
    :   ember::saboteur::priv::gate(_id),
        A(ember::hiercat(name, "A")),
        B(ember::hiercat(name, "B")),
        C(ember::hiercat(name, "C")),
        Z(ember::hiercat(name, "Z"))
{ }

// XOR2
void ember::saboteur::xor2::eval()
{
    if(sa1Mask) {
        Z.write(true);
    }
    else if(sa0Mask) {
        Z.write(false);
    }
    else if(seuMask) {
        Z.write(!(A.read() != B.read()));
    }
    else {
        Z.write(A.read() != B.read());
    }
}

ember::saboteur::xor2::xor2(const char* _id)
    :   ember::saboteur::priv::gate(_id),
        A(ember::hiercat(name, "A")),
        B(ember::hiercat(name, "B")),
        Z(ember::hiercat(name, "Z"))
{ }

ember::saboteur::xor2::xor2(const std::string _id)
    :   ember::saboteur::priv::gate(_id),
        A(ember::hiercat(name, "A")),
        B(ember::hiercat(name, "B")),
        Z(ember::hiercat(name, "Z"))
{ }

// XNOR2
void ember::saboteur::xnor2::eval()
{
    if(sa1Mask) {
        Z.write(true);
    }
    else if(sa0Mask) {
        Z.write(false);
    }
    else if(seuMask) {
        Z.write(!(A.read() == B.read()));
    }
    else {
        Z.write(A.read() == B.read());
    }
}

ember::saboteur::xnor2::xnor2(const char* _id)
    :   ember::saboteur::priv::gate(_id),
        A(ember::hiercat(name, "A")),
        B(ember::hiercat(name, "B")),
        Z(ember::hiercat(name, "Z"))
{ }

ember::saboteur::xnor2::xnor2(const std::string _id)
    :   ember::saboteur::priv::gate(_id),
        A(ember::hiercat(name, "A")),
        B(ember::hiercat(name, "B")),
        Z(ember::hiercat(name, "Z"))
{ }