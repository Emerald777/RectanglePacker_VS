#ifndef PACKER_H
#define PACKER_H

#include "userdefines.h"

class Packer
{
    InputContainer m_blocks;
    OutputContainer m_placedBlocks;
    Geometry::Rectangle m_basket;

    int m_basketCount;

private:
    void fillBasket(int& basketNum);

public:
    Packer();

    void setupContainer(Geometry::Rectangle const& newCont);
    Geometry::Rectangle container() const;

    void addBlock(float w, float h);

    void exec();

    void printTotalSquareToPlace() const;
    void printPlacedBlocksInfo() const;
    void printUnplacedBlocksInfo() const;

    void printBasket() const;
    void printInputBlocksInfo() const;
};

#endif // PACKER_H
