#include <cctype>
#include <cstdlib>
#include <cmath>
#include <cstddef>

// Locale-free strtod replacement from OpenHRP3 commit
static inline double mystrtod(const char* nptr, char** endptr) {
    const char* org = nptr;
    bool valid = false;
    double value = 0.0;
    double sign = +1.0;

    if(*nptr == '+'){
        nptr++;
    } else if(*nptr == '-'){
        sign = -1.0;
        nptr++;
    }
    if(isdigit((unsigned char)*nptr)){
        valid = true;
        do {
            value = value * 10.0 + (*nptr - '0');
            nptr++;
        } while(isdigit((unsigned char)*nptr));
    }
    if(*nptr == '.'){
        valid = false;
        nptr++;
        if(isdigit((unsigned char)*nptr)){
            double small = 0.1;
            valid = true;
            do {
                value += small * (*nptr - '0');
                small *= 0.1;
                nptr++;
            } while(isdigit((unsigned char)*nptr));
        }
    }
    if(valid && (*nptr == 'e' || *nptr == 'E')){
        nptr++;
        valid = false;
        double psign = +1.0;
        if(*nptr == '+'){
            nptr++;
        } else if(*nptr == '-'){
            psign = -1.0;
            nptr++;
        }
        if(isdigit((unsigned char)*nptr)){
            valid = true;
            double p = 0.0;
            do {
                p = p * 10.0 + (*nptr - '0');
                nptr++;
            } while(isdigit((unsigned char)*nptr));
            value *= pow(10.0, psign * p);
        }
    }
    if(valid){
        *endptr = (char*)nptr;
    } else {
        *endptr = (char*)org;
    }
    return sign * value;
}

double parse_numbers(const char* text, size_t count) {
    const char* ptr = text;
    double sum = 0.0;
    for (size_t i = 0; i < count; ++i) {
        while (isspace((unsigned char)*ptr)) ++ptr;
        char* end;
        double v = mystrtod(ptr, &end);
        sum += v;
        ptr = end;
    }
    return sum;
}
