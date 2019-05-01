#include <iosfwd>
#include <numeric>
#include <stdexcept>
#include <cmath>
#include <variant>

template<typename T>
T sq(T a){
    return a*a;
}

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

    constexpr int read_num() const& {
        return (*this).num;
    }

    constexpr int read_den() const& {
        return (*this).denom;
    }

    constexpr Rational rec(){
        return Rational((*this).denom, (*this).num);
    }

    Rational& inv(){
        std::swap((*this).num, (*this).denom);
        return *this;
    }

    constexpr double double_val() { 
        return (*this).num/static_cast<double>((*this).denom);
    }

    Rational& pow(const int& a){
        if(a >= 0){
            (*this).num = static_cast<int>(std::pow((*this).num, a));
            (*this).denom = static_cast<int>(std::pow((*this).denom, a));
        }
        else{
            (*this) = Rational(static_cast<int>(std::pow((*this).denom, -a)), static_cast<int>(std::pow((*this).num, -a)));
        }
        return simplify();
    }

    double pow(const double& a){
        return std::pow((*this).num, a) / std::pow((*this).denom, a);
    }

    double sqrt(){        
        if((*this).double_val() < 0){
            throw std::domain_error{"Error: result is complex"};
        }
        return std::sqrt((*this).double_val());
    }
    
    // spec_sqrt can return Rational type if possible

    using sqrt_type = std::variant<double, Rational>;
    sqrt_type spec_sqrt(){
        if((*this).double_val() < 0){
            throw std::domain_error{"Error: result is complex"};
        }
        double sqrt_num = std::sqrt((*this).num);
        double sqrt_den = std::sqrt((*this).denom);
        int cast_num = static_cast<int>(sqrt_num + 0.1);
        int cast_den = static_cast<int>(sqrt_den + 0.1);
        if(sq(cast_num) == num && sq(cast_den) == denom){
            (*this) = Rational(cast_num, cast_den);
            return *this;
        }
        else{
            return (*this).sqrt();
        }
    }
};

/*** Operators ***/

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
