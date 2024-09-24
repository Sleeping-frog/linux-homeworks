#include "Complex.h"
#include <cmath>
#include <iostream>

Complex Complex::operator+(const Complex& other) const {
    return Complex(m_real + other.m_real, m_imag + other.m_imag);
}

Complex Complex::operator- (const Complex& other) const {
    return Complex(m_real - other.m_real, m_imag - other.m_imag);
}

Complex operator*(const double lhs, const Complex& rhs) {
    return Complex(lhs * rhs.m_real, lhs * rhs.m_imag);
}

bool Complex::operator< (const Complex& other) const {
    return this->absolute() < other.absolute();
}

Complex& Complex::operator= (const Complex& other) {
    m_real = other.m_real;
    m_imag = other.m_imag;
    return *this;
}

double Complex::absolute() const {
    return sqrt(m_real * m_real + m_imag * m_imag);
}

std::ostream& operator<< (std::ostream& os, const Complex& number) {
    os << number.m_real << " + " << number.m_imag << 'i';
    return os;
}
