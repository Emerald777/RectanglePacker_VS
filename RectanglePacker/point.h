#ifndef POINT_H
#define POINT_H

#define _USE_MATH_DEFINES

#include <iostream>
#include <cmath>
#include <limits>

namespace Geometry
{
    template<class T>
    struct Point
    {
        T x;
        T y;

        Point(T x0 = 0, T y0 = 0) : x(x0), y(y0) {}
        Point(const Point& other) = default;

        Point operator+(const Point& other) const {
            return Point(x + other.x, y + other.y);
        }

        Point& operator+=(const Point& other) {
            x += other.x;
            y += other.y;
            return *this;
        }       

        Point operator-(const Point& other) const {
            return Point(this->x - other.x, this->y - other.y);
        }

        Point& operator-=(const Point& other) {
            x -= other.x;
            y -= other.y;
            return *this;
        }

        Point operator/(float n) {
            return Point(x / n, y / n);
        }

        bool operator==(const Point& other) const
        {
            return std::is_floating_point<T>::value
                   ? is_equal_floating(x, other.x) && is_equal_floating(y, other.y)
                   : (x == other.x && y == other.y);
        }

        Point rotateAround(float degrees, const Point &center) const
        {
            Point pt(*this - center);

            float radians = static_cast<float> (degrees * M_PI / 180.0f);
            Point res(
                 pt.x * std::cos(radians) + pt.y * std::sin(radians),
                -pt.x * std::sin(radians) + pt.y * std::cos(radians)
            );
            return res + center;
        }

        void makeRounded() {
            x = std::round(x);
            y = std::round(y);
        }

        void printString() const
        {
            std::cout << "Point = (" << x << ", " << y << ")" << std::endl;
        }

    private:
        template<typename F>
        bool is_equal_floating(const F& a, const F& b) const
        {
            return std::fabs(a - b) < std::numeric_limits<F>::epsilon();
        }

    };

    template<class C>
    std::ostream& operator<< (std::ostream &out, const Point<C> &point)
    {
        out << point.x << "," << point.y;
        return out;
    }

    using FPoint = Point<float>;
    //using IPoint = Point<int>;
}

#endif // POINT_H
