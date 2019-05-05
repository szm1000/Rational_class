#include <iosfwd>
#include <numeric>
#include <stdexcept>
#include <cmath>
#include <variant>
#include <utility>
#include <string>
#include <sstream>

template<typename T>
T sq(T a){
    return a*a;
}

class Rational;
double sqrt(const Rational& r);
double sqrt(Rational& r);

class Rational{

    /***Numerator and denominator***/

    int num;
    int denom;

    /*** Simplify to prevent overflow***/

    constexpr Rational& simplify(){
        int gcd = std::gcd(num, denom);
        if(gcd != 0){
            num /= gcd;
            denom /= gcd;
        }
        if( num <= 0 && denom < 0 ){
            num *= -1;
            denom *= -1;
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
        num = cpy.num; 
        denom = cpy.denom;
    }

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
        return num;
    }

    constexpr int read_den() const& {
        return denom;
    }

    constexpr Rational rec(){
        return Rational(denom, num);
    }

    Rational& inv(){
        std::swap(num, denom);
        return *this;
    }

    constexpr double double_val() const& { 
        return num/static_cast<double>(denom);
    }
/*
    Rational& pow(const int& a){
        if(a >= 0){
            num = static_cast<int>(std::pow(num, a));
            denom = static_cast<int>(std::pow(denom, a));
        }
        else{
            (*this) = Rational(static_cast<int>(std::pow(denom, -a)), static_cast<int>(std::pow(num, -a)));
        }
        return simplify();
    }
*/

    // spec_sqrt can return Rational type if possible

    using sqrt_type = std::variant<double, Rational>;
    sqrt_type spec_sqrt(){
        if((*this).double_val() < 0){
            throw std::domain_error{"Error: result is complex"};
        }
        double sqrt_num = std::sqrt(num);
        double sqrt_den = std::sqrt(denom);
        int cast_num = static_cast<int>(sqrt_num + 0.1);
        int cast_den = static_cast<int>(sqrt_den + 0.1);
        if(sq(cast_num) == num && sq(cast_den) == denom){
            //(*this) = Rational(cast_num, cast_den);
           return Rational{cast_num, cast_den};// return *this;
        }
        else{
            return sqrt(*this);
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

// Comparsion operators


bool operator==(const Rational &r1, const Rational &r2){
    if(r1.read_num() == r2.read_num() && r1.read_den() == r2.read_den()){
        return true;
    }
    return false;
}
/*
bool operator!=(const Rational& r1, const Rational& r2){
    return !(r1 == r2);
}*/

bool operator<(const Rational &r1, const Rational &r2){
    if(r1.double_val() < r2.double_val()){
        return true;
    }
    return false;
}
/*
bool operator>(const Rational& r1, const Rational& r2){
    return !(r1 < r2)
}

bool operator<=(const Rational& r1, const Rational& r2){
    if (r1 < r2 || r1 == r2){
        return true;
    }
    return false;
}

bool operator>=(const Rational& r1, const Rational& r2){
    if (r1 > r2 || r1 == r2){
        return true;
    }
    return false;
}
*/

std::ostream& operator<<(std::ostream& o, Rational const &r){
        o<<r.read_num()<<"/"<<r.read_den()<<" ";
    return o;
}

std::istream& operator>>(std::istream& istr, Rational &r){
    auto rewind = [state = istr.rdstate(), pos = istr.tellg(), &istr](){istr.seekg(pos); istr.setstate(state);};
    int n = 0;
    int d = 1;
    std::string temp;
    if(std::getline(istr, temp, ',')){
        std::stringstream ss(temp);
        std::getline(ss, temp, '/');
        if(!ss){rewind(); std::cout<<"Read error!"<<std::endl; return istr;}
        n = std::stoi(temp);
        std::getline(ss, temp, ' '); 
        if(!ss){rewind(); std::cout<<"Read error!"<<std::endl; return istr;}
        d = std::stoi(temp);
    }
    else{
        std::cout<<"Read error!"<<std::endl;
        rewind;
        return istr;
    }
    r = Rational{n, d};
    return istr;
}

double sqrt(Rational& r){        
    if(r.double_val() < 0){
        throw std::domain_error{"Error: result is complex"};
    }
    return std::sqrt(r.double_val());
}

Rational pow(const Rational& r, const int& a){
        int n = 0;
        int d = 1;
        if(a >= 0){
            n = static_cast<int>(std::pow(r.read_num(), a));
            d = static_cast<int>(std::pow(r.read_den(), a));
        }
        else{
            n = static_cast<int>(std::pow(r.read_den(), -a));
            d = static_cast<int>(std::pow(r.read_num(), -a));
        }
    return Rational(n,d);
}

double pow(const Rational& r, const double& a){
    return std::pow(r.double_val(), a);
}

Rational abs(const Rational& r){
    return Rational(std::abs(r.read_num()), std::abs(r.read_den()));
}
