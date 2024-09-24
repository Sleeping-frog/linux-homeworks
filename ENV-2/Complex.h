#pragma once

#include <iostream>


class Complex {
public:
    Complex (double real = 0, double imag = 0) : m_real(real), m_imag(imag) {}

    Complex (const Complex& other) : m_real(other.m_real), m_imag(other.m_imag) {}

    Complex operator+ (const Complex& other) const;

    Complex operator- (const Complex& other) const;

    friend Complex operator*(const double lhs, const Complex& rhs); 

    bool operator< (const Complex& other) const;

    Complex& operator= (const Complex& other);

    double absolute() const;

    friend std::ostream& operator<< (std::ostream& os, const Complex& number);

private:
    double m_real;
    double m_imag;
};
