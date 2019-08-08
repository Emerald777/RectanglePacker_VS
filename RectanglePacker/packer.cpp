#include "packer.h"

#include <iostream>
#include <algorithm>
#include <cassert>
#include <typeinfo>

#include "levelh.h"

Packer::Packer() : m_basketCount(0)
{

}

void Packer::setupContainer(const Geometry::Rectangle &newCont)
{
    m_basket = newCont;
    m_basketCount = 1;
}

Geometry::Rectangle Packer::container() const
{
    return m_basket;
}

void Packer::addBlock(float w, float h)
{
    m_blocks.push_back(Geometry::Rectangle(w, h, true));
}

bool cmpHeight(const Geometry::Rectangle &a, const Geometry::Rectangle &b)
{
    return a.height() > b.height();
}

bool cmpWidth(const Geometry::Rectangle &a, const Geometry::Rectangle &b)
{
    return a.width() > b.width();
}

bool cmpSquare(const Geometry::Rectangle &a, const Geometry::Rectangle &b)
{
    return a.square() < b.square();
}


void Packer::exec()
{
#ifndef RAW_OUT_DATA

#ifdef DEBUG
    printBasket();
#endif

    printInputBlocksInfo();
#endif

    int ignoredBlocks = 0;
    m_basketCount = 0;

    /// Ignore blocks which are too big for our Basket
    for (auto it = m_blocks.begin(); it != m_blocks.end();)
    {
        if (!it->tryToFit(m_basket))
        {
            Geometry::Rectangle r;
            if (!it->tryToFitRotated(m_basket, r))
                ++ignoredBlocks;
            else {
                r.roundCoords();
                m_placedBlocks.insert(
                     std::pair<int, Geometry::Rectangle>(m_basketCount++, r));
            }

            it = m_blocks.erase(it);
        }
        else
            ++it;
    }

#ifndef RAW_OUT_DATA
    printTotalSquareToPlace();
#endif

    m_blocks.sort(cmpHeight);

    while (!m_blocks.empty())
    {
        fillBasket(m_basketCount);
        ++m_basketCount;
    }

#ifndef RAW_OUT_DATA
    printf("%d blocks was ignored\n", ignoredBlocks);
#endif

    printPlacedBlocksInfo();

#ifndef RAW_OUT_DATA
    printUnplacedBlocksInfo();
#endif
}

void Packer::printTotalSquareToPlace() const
{
    float totalSqr = 0;
    for (auto it = m_blocks.cbegin(); it != m_blocks.cend(); ++it) {
        totalSqr += it->square();
    }
    std::cout << "total block square is " << totalSqr << std::endl;
}

void Packer::printPlacedBlocksInfo() const
{
    const char* COMMA = ",";

#ifndef RAW_OUT_DATA
    printf("\nAmount of placed blocks: %d\n", m_placedBlocks.size());
    printf("\nPlaced blocks:\n");
#endif

    std::cout << m_basketCount << COMMA << m_basket.width() << COMMA << m_basket.height() << std::endl;
    for (auto el : m_placedBlocks)
    {
        std::cout << el.first << COMMA << el.second.topLeft() << COMMA << el.second.topRight() << COMMA
                  << el.second.bottomRight() << COMMA << el.second.bottomLeft() << std::endl;
    }
}

void Packer::printUnplacedBlocksInfo() const
{
    printf("\nAmount of unplaced blocks: %d\n", m_blocks.size());

    if (!m_blocks.empty()) {
        printf("\nUnplaced blocks are:\n");
        for (auto el : m_blocks) {
            std::cout << el.width() << "," << el.height() << std::endl;
        }
    }
}

void Packer::printBasket() const
{
    std::cout << "Basket: TopLeft= (" << m_basket.topLeft() <<
                      "), BotRight= (" << m_basket.bottomRight() << ")\n";
}

void Packer::printInputBlocksInfo() const
{
    printf("We have %d blocks to place\n", m_blocks.size());
}

void Packer::fillBasket(int &basketNum)
{
    int amount = 0;
    Geometry::Rectangle space(m_basket);

    do
    {
        LevelH level = LevelH(space, &m_blocks, &m_placedBlocks);

#ifdef DEBUG
        std::cout << level.availableSpace().width() << std::endl;
#endif

        for (auto it = m_blocks.begin(); it != m_blocks.end(); )
        {
            auto itnext = m_blocks.end();

            if (level.addBlock(basketNum, it, itnext))
                it = itnext;
            else
                ++it;
        }

#ifdef DEBUG
        space.printBorders();
        std::cout << level.availableSpace().width() << std::endl;
#endif

        space.cutTop(level.availableSpace().height());

        amount = level.blockAmount();
    }
    while (amount > 0);
}

