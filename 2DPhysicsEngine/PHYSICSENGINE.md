# Phyiscs Engine Documentation

## Overview
This is a 2D physics engine written in C++ currently used for educational purposes, but will be improved upon in the future to improve readability and performance.

## Vector Class

### Vec2

**Member Variables**  
float x  
float y  

**Methods**  
void Add(const Vec2& v)  
void Sub(const Vec2& v)  
void Scale(const float n)  
Vec2 Rotate(const float angle) const  
float Magnitude() const  
float MagnitudeSquared() const  
Vec2& Normalize()  
Vec2 UnitVector() const  
Vec2 Normal() const    
float Dot(const Vec2& v) const  
float Cross(const Vec2& v) const  

**Operator Overloads**  
Vec2& operator = (const Vec2& v);  
bool operator == (const Vec2& v) const;  
bool operator != (const Vec2& v) const;  
    
Vec2 operator + (const Vec2& v) const;  
Vec2 operator - (const Vec2& v) const;  
Vec2 operator * (const float n) const;  
Vec2 operator / (const float n) const;  
Vec2 operator - ();       

Vec2& operator += (const Vec2& v);  
Vec2& operator -= (const Vec2& v); 
Vec2& operator *= (const float n);  
Vec2& operator /= (const float n); 