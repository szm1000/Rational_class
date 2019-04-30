#include <iostream>
#include <numeric>

class Rational{

    /***Numerator and denominator***/

    int num;
    int denom;

    /*** Simplify to prevent overflow***/

    void simplify(){
        int gcd = std::gcd(num, denom);
        if(gcd != 0){
            (*this).num /= gcd;
            (*this).denom /= gcd;
        }
    };
    public:

    /*** Constructors ***/

    Rational(): num{0}, denom{1}{};
    Rational(int a, int b){
        (*this).num = a;
        if(b != 0){
            (*this).denom = b;
        }
        else{
            std::cout<<"exception"<<std::endl;
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

    Rational& operator= (const Rational&);
    Rational& operator+= (const Rational&);
    Rational& operator-= (const Rational&);
    Rational& operator*= (const Rational&);
    Rational& operator/= (const Rational&);

    /*** Other functions ***/

    int read_num() const& {
        return (*this).num;
    }

    int read_den() const& {
        return (*this).denom;
    }
};

std::ostream& operator<<(std::ostream& o, Rational const& r){
    int d = r.read_den();
    if (d == 1){
        o<<r.read_num()<<" ";
    }
    else{
        o<<r.read_num()<<"/"<<d<<" ";
    }
    return o;
}
