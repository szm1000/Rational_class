#include <iosfwd>
#include <numeric>
#include <stdexcept>

class Rational{

    /***Numerator and denominator***/

    int num;
    int denom;

    /*** Simplify to prevent overflow***/

    constexpr Rational& simplify(){
        int gcd = std::gcd(num, denom);
        if(gcd != 0){
            (*this).num /= gcd;
            (*this).denom /= gcd;
        }
        return *this;
    };
    
    public:

    /*** Constructors ***/

    constexpr Rational(int a): num{a}, denom{1}{}

    //Defalut constructor
    constexpr Rational(): Rational{0}{}

    constexpr Rational(int a, int b): num{a}, denom{b}{
        if(denom == 0){
            throw std::domain_error{"Error: zero denominator"};
        }
        simplify();
    }

    //Copy constructor
    
    Rational(const Rational& cpy){
        (*this).num = cpy.num; 
        (*this).denom = cpy.denom;
    }

    //Move constructor

    Rational(Rational &&) = default; 	

    /***Assignment operators***/

    constexpr Rational& operator=(const Rational& r){
        num = r.num; 
        denom = r.denom; 
        return *this;
    }

    constexpr Rational& operator+=(const Rational& r){
        num = num * r.denom + denom * r.num;
        denom *= r.denom;
        return simplify();
    }

    constexpr Rational& operator-=(const Rational& r){
        num = num * r.denom - denom * r.num;
        denom *= r.denom;
        return simplify();
    }

    constexpr Rational& operator*=(const Rational& r){
        num *= r.num;
        denom *= r.denom;
        return simplify();
    }
    constexpr Rational& operator/=(const Rational& r){
        if(r.num == 0){
            throw std::domain_error{"Error: division by zero"};
        }
        num *= r.denom;
        denom *= r.num;
        return simplify();
    }

    /*** Other functions ***/

    int read_num() const& {
        return (*this).num;
    }

    int read_den() const& {
        return (*this).denom;
    }

    double double_val() { 
        return (*this).num/static_cast<double>((*this).denom);
    }
};

Rational operator+(const Rational& r1, const Rational& r2){
    Rational result = r1;
    result += r2;
    return result; 
}

Rational operator+(const Rational& r, const int & c){
    Rational result(c);
    result += r;
    return result; 
}

Rational operator+(const int& c, const Rational& r){
    Rational result(c);
    result += r;
    return result; 
}

Rational operator-(const Rational& r1, const Rational& r2){
    Rational result = r1;
    result -= r2;
    return result; 
}

Rational operator-(const Rational& r, const int & c){
    Rational result(-1*c);
    result += r;
    return result; 
}

Rational operator-(const int& c, const Rational& r){
    Rational result(c);
    result -= r;
    return result; 
}

Rational operator*(const Rational& r1, const Rational& r2){
    Rational result = r1;
    result*= r2;
    return result; 
}

Rational operator*(const Rational& r, const int& c){
    Rational result(c);
    result *= r;
    return result; 
}

Rational operator*(const int& c, const Rational& r){
    return r*c; 
}

Rational operator/(const Rational& r1, const Rational& r2){
    Rational result = r1;
    result/= r2;
    return result; 
}

Rational operator/(const Rational& r, const int& c){
    Rational result(r);
    result /= Rational(c);
    return result; 
}

Rational operator/(const int& c, const Rational& r){
    Rational result{c};
    result/= r;
    return result; 
}

std::ostream& operator<<(std::ostream& o, Rational const& r){
    int d = r.read_den();
    int n = r.read_num();
    if (d == 1 || n == 0){
        o<<n<<" ";
    }
    else{
        o<<n<<"/"<<d<<" ";
    }
    return o;
}

std::istream& operator>>(std::istream& istr, Rational& r){
    int a, b;
    istr>>a>>b;
    r = Rational{a, b};
    return istr;
}
