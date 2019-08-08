#pragma once

#include "userdefines.h"
#include "rectangle.h"

/// Horizontal Level of blocks
/// Fills the empty space of base Rectangle with other rectangles

class LevelH
{
    InputContainer *m_blocks;
    OutputContainer *m_placedBlocks;

    int m_count;
    Geometry::Rectangle m_available;

public:
    LevelH(Geometry::Rectangle const &base, InputContainer *in, OutputContainer *out);
    ~LevelH();

    Geometry::Rectangle availableSpace() const;

    int blockAmount() const;

    template<class Iter>
    bool addBlock(int num, Iter const &it, Iter& next)
    {
#ifdef DEBUG
        printAvailable();
        m_available.printDimensions();
#endif
        Geometry::Orientation orient;
        if (!it->tryToFit(m_available, orient)) {
            next = it;
            return false;
        }

        Geometry::Rectangle rotated = it->cloneOriented(orient);
        rotated.move(m_available.topLeft());

        if (m_count == 0) {
           m_available.cutBottom(m_available.height() - rotated.height());
        }

        ++m_count;
        m_available.cutLeft(rotated.width());

        m_placedBlocks->insert(std::pair<int, Geometry::Rectangle>(num, rotated));

#ifdef DEBUG
        printAvailable();
#endif

        next = m_blocks->erase(it);
        return true;
    }

    void printAvailable() const;
    void printBlocksAmount() const;
};

