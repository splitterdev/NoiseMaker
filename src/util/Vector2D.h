#pragma once

#include <math.h>

class PolarVector2D;

class Vector2D {
    public:

    Vector2D() {
        x = 0.0;
        y = 0.0;
    }

    Vector2D( double X, double Y ) {
        x = X;
        y = Y;
    }

    Vector2D( PolarVector2D pvec );

    ~Vector2D() {
    }

    inline double dist2D() {
        return sqrt( x * x + y * y );
    }

    inline double dist2D( Vector2D v ) {
        double dx = v.x - x;
        double dy = v.y - y;
        return sqrt( dx * dx + dy * dy );
    }

    inline Vector2D add( Vector2D v ) {
        v.x += x;
        v.y += y;
        return v;
    }

    inline Vector2D sub( Vector2D v ) {
        v.x -= x;
        v.y -= y;
        return v;
    }

    inline Vector2D mul( double a ) {
        Vector2D v;
        v.x = x * a;
        v.y = y * a;
        return v;
    }

    inline Vector2D mul( Vector2D v ) {
        v.x *= x;
        v.y *= y;
        return v;
    }

    Vector2D normalize() {
        Vector2D vec;
        double d = dist2D();
        vec.x /= d;
        vec.y /= d;
        return vec;
    }

    double x;
    double y;
};

class PolarVector2D {
    public:
    PolarVector2D() {
        r = 1.0;
        a = 0.0;
    }

    PolarVector2D( double R, double A ) {
        r = R;
        a = A;
    }

    ~PolarVector2D() {
    }

    PolarVector2D( Vector2D vec );

    PolarVector2D normalize() {
        return PolarVector2D( 1.0, a );
    }

    double r;
    double a;
};

Vector2D::Vector2D( PolarVector2D pvec ) {
    x = sin( pvec.a ) * pvec.r;
    y = cos( pvec.a ) * pvec.r;
}

PolarVector2D::PolarVector2D( Vector2D vec ) {
    r = vec.dist2D();
    a = M_PI - atan2( vec.x, - vec.y );
}



#define DEGREE_RADIAN_RATIO 180.0 / M_PI
#define RADIAN_DEGREE_RATIO M_PI / 180.0

double degr( double valueRadians ) {
    return valueRadians * DEGREE_RADIAN_RATIO;
}

double radn( double valueDegrees ) {
    return valueDegrees * RADIAN_DEGREE_RATIO;
}
