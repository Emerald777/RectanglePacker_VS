#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "point.h"

namespace Geometry
{

    enum class Orientation {
        Horizontal,
        Vertical,
        Custom
    };

    class Rectangle
    {
        float m_width;
        float m_height;

        float m_sqr;

        FPoint m_topLeft;
        FPoint m_topRight;
        FPoint m_btmLeft;
        FPoint m_btmRight;

    private:
        static void toggleOrientation(Orientation& o);
        void updateDimensions();

    public:
        Rectangle();
        Rectangle(float w, float h, bool normal = false); /// normal - width >= height
        Rectangle(const Rectangle& other) = default;

        float square() const;
        FPoint topLeft() const;
        FPoint topRight() const;
        FPoint bottomRight() const;
        FPoint bottomLeft() const;


        void move(const FPoint &newPos);
        void moveCenter(const FPoint &newPos);

        void cutTop(float dx);
        void cutBottom(float dx);
        void cutLeft(float dx);
        void cutRight(float dx);

        FPoint massCenter() const;

        Rectangle cloneHorizontal() const;
        Rectangle cloneVertical() const;
        Rectangle cloneRotated(float degrees) const;
        Rectangle cloneOriented(const Orientation &o) const;
        Rectangle cloneFitted(const Rectangle& other) const;

        bool tryToFit(const Rectangle& other, Orientation& orient) const;
        bool tryToFit(const Rectangle& other) const;
        bool tryToFitRotated(const Rectangle& other, Rectangle &fitted) const;

        Orientation orientation() const;

        void roundCoords();

        float width() const;
        float height() const;

        float normalizedWidth() const;   /// Maximum of dimensions
        float normalizedHeight() const;  /// Minimum of dimensions

        bool hasInside(const FPoint &p) const;

        bool operator<(const Rectangle& other) const {
            return this->m_sqr < other.m_sqr;
        }

        bool operator>(const Rectangle& other) const {
            return this->m_sqr > other.m_sqr;
        }

        Rectangle& operator=(const Rectangle& other) = default;

        void printBorders() const;
        void printDimensions() const;

    };

    std::ostream& operator<< (std::ostream &out, const Rectangle &r);
}

#endif // RECTANGLE_H
