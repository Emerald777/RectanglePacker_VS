#include "levelh.h"


LevelH::LevelH(const Geometry::Rectangle &base, InputContainer *in, OutputContainer *out) :
    m_blocks(in), m_placedBlocks(out), m_count(0), m_available(base)
{
#ifdef DEBUG
    printf("*Level constructed* \n");
#endif
}

LevelH::~LevelH()
{
#ifdef DEBUG
    printf("*Level destroyed* \n\n");
#endif
}

Geometry::Rectangle LevelH::availableSpace() const
{
    return m_available;
}

int LevelH::blockAmount() const
{
    return m_count;
}

void LevelH::printAvailable() const
{
    std::cout << "LevelSpace: TopLeft= (" << m_available.topLeft() <<
                "), BotRight= (" << m_available.bottomRight() << ")\n";
}

void LevelH::printBlocksAmount() const
{
    printf("Level contains %d blocks\n", m_count);
}
