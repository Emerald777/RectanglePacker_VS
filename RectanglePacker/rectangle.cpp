#include "rectangle.h"

#include <algorithm>


Geometry::Rectangle::Rectangle() : m_width(0), m_height(0), m_sqr(0)
{
}

Geometry::Rectangle::Rectangle(float w, float h, bool normal) : m_width(w), m_height(h), m_sqr(w * h),
    m_topLeft(0, 0), m_topRight(w, 0), m_btmLeft(0, h), m_btmRight(w, h)
{
    if (normal) {
        /// Ширину записываем не меньшую чем высоту
        auto mnmx = std::minmax(w, h);
        m_width = mnmx.second;
        m_height = mnmx.first;
    }
}

void Geometry::Rectangle::toggleOrientation(Geometry::Orientation &o)
{
     o = (o == Orientation::Horizontal) ? Orientation::Vertical
                                        : Orientation::Horizontal;
}

void Geometry::Rectangle::updateDimensions()
{
    auto dxdy = m_btmRight - m_topLeft;
    m_width = dxdy.x;
    m_height = dxdy.y;
    m_sqr = m_width * m_height;
}

float Geometry::Rectangle::square() const
{
    return m_sqr;
}

Geometry::FPoint Geometry::Rectangle::topLeft() const
{
    return m_topLeft;
}

Geometry::FPoint Geometry::Rectangle::topRight() const
{
    return m_topRight;
}

Geometry::FPoint Geometry::Rectangle::bottomRight() const
{
    return m_btmRight;
}

Geometry::FPoint Geometry::Rectangle::bottomLeft() const
{
    return m_btmLeft;
}

void Geometry::Rectangle::move(Geometry::FPoint const& newPos)
{
    m_topLeft += newPos;
    m_topRight += newPos;
    m_btmLeft += newPos;
    m_btmRight += newPos;
}

void Geometry::Rectangle::moveCenter(const Geometry::FPoint &newPos)
{
    move(newPos - massCenter());
}

void Geometry::Rectangle::cutTop(float dx)
{
    m_topLeft.y += dx;
    m_topRight.y += dx;
    updateDimensions();
}

void Geometry::Rectangle::cutBottom(float dx)
{
    m_btmLeft.y -= dx;
    m_btmRight.y -= dx;
    updateDimensions();
}

void Geometry::Rectangle::cutLeft(float dx)
{
    m_topLeft.x += dx;
    m_btmLeft.x += dx;
    updateDimensions();
}

void Geometry::Rectangle::cutRight(float dx)
{
    m_topRight.x -= dx;
    m_btmRight.x -= dx;
    updateDimensions();
}

Geometry::FPoint Geometry::Rectangle::massCenter() const
{
    return (m_topLeft + m_btmRight) / 2;
}

Geometry::Rectangle Geometry::Rectangle::cloneHorizontal() const
{
    auto mnmx = std::minmax(m_width, m_height);
    return Rectangle(mnmx.second, mnmx.first);
}

Geometry::Rectangle Geometry::Rectangle::cloneVertical() const
{
    auto mnmx = std::minmax(m_width, m_height);
    return Rectangle(mnmx.first, mnmx.second);
}

Geometry::Rectangle Geometry::Rectangle::cloneRotated(float degrees) const
{
    Rectangle r(*this);

    r.m_topLeft = r.m_topLeft.rotateAround(degrees, massCenter());
    r.m_topRight = r.m_topRight.rotateAround(degrees, massCenter());
    r.m_btmLeft = r.m_btmLeft.rotateAround(degrees, massCenter());
    r.m_btmRight = r.m_btmRight.rotateAround(degrees, massCenter());
    return r;
}

Geometry::Rectangle Geometry::Rectangle::cloneOriented(Orientation const& o) const
{
    return (o == Orientation::Horizontal) ? cloneHorizontal() : cloneVertical();
}

Geometry::Rectangle Geometry::Rectangle::cloneFitted(const Rectangle &other) const
{
    return (other.m_width > other.m_height) ? cloneHorizontal() : cloneVertical();
}


bool Geometry::Rectangle::tryToFit(const Rectangle &other, Orientation& orient) const
{
    orient = Orientation::Custom;
    if (other < *this) return false;

    bool ok = normalizedWidth() <= other.normalizedWidth() &&
               normalizedHeight() <= other.normalizedHeight();
    if (ok) {
        orient = orientation();
        if (!(m_width <= other.m_width && m_height <= other.m_height))
            toggleOrientation(orient);
    }

    return ok;
}

bool Geometry::Rectangle::tryToFit(const Geometry::Rectangle &other) const
{
    if (other < *this) return false;

    return normalizedWidth() <= other.normalizedWidth() &&
            normalizedHeight() <= other.normalizedHeight();
}

bool Geometry::Rectangle::tryToFitRotated(const Rectangle &other, Rectangle &fitted) const
{
    //auto r = cloneHorizontal();
    fitted = cloneHorizontal();

    fitted.moveCenter(other.massCenter());
  //  std::cout << r << std::endl;

    auto deg = static_cast<float>(atan(other.height() / other.width()) * 180 / M_PI);
   // std::cout << "Degrees diag = " << deg << std::endl;
    fitted = fitted.cloneRotated(deg);

  //  std::cout << r << std::endl;
    bool ok = (other.hasInside(fitted.topLeft()) && other.hasInside(fitted.topRight()) &&
               other.hasInside(fitted.bottomLeft()) && other.hasInside(fitted.bottomRight()));
    return ok;
}

Geometry::Orientation Geometry::Rectangle::orientation() const
{
    return (m_width > m_height) ? Orientation::Horizontal : Orientation::Vertical;
}

void Geometry::Rectangle::roundCoords()
{
    m_topLeft.makeRounded();
    m_topRight.makeRounded();
    m_btmLeft.makeRounded();
    m_btmRight.makeRounded();
}

float Geometry::Rectangle::width() const
{
    return m_width;
}

float Geometry::Rectangle::height() const
{
    return m_height;
}

float Geometry::Rectangle::normalizedWidth() const
{
    auto mnmx = std::minmax(m_width, m_height);
    return mnmx.second;
}

float Geometry::Rectangle::normalizedHeight() const
{
    auto mnmx = std::minmax(m_width, m_height);
    return mnmx.first;
}

bool Geometry::Rectangle::hasInside(const Geometry::FPoint &p) const
{
    return p.x >= this->topLeft().x && p.x <= this->bottomRight().x &&
            p.y >= this->topLeft().y && p.y <= this->bottomRight().y ;
}

void Geometry::Rectangle::printBorders() const
{
    std::cout << "TL: " << topLeft() << " ; BR: " << bottomRight() <<
                 " , Orient= " << static_cast<int>(orientation()) << std::endl;
}

void Geometry::Rectangle::printDimensions() const
{
    std::cout << "Width: " << width() << " ; Height: " << height() << std::endl;
}

std::ostream &Geometry::operator<<(std::ostream &out, const Geometry::Rectangle &r)
{
    out << r.topLeft() << "," << r.topRight() << "," << r.bottomRight() << "," << r.bottomLeft();
    return out;
}
