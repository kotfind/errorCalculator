// From http://mathhelpplanet.com/viewtopic.php?f=44&t=76364

#include <math.h>

double StatCom(double q, int i, int j)
{   
    double zz = 1, z = zz;
    int k = i;
    while (k <= j){ zz *= q*k/(k+1); z += zz; k += 2; }
    return z;
}

double StudentT(double t, int n)
{   
    double th = atan(abs(t)/sqrt((double)n)), pi2 = acos((double)-1)/2;

    if (n == 1) return ( 1 - th / pi2 );

    double sth = sin(th), cth = cos(th);

    if (n%2 == 1) return ( 1 - (th + sth * cth * StatCom(cth * cth, 2, n-3)) / pi2 );
    else          return ( 1 - sth * StatCom(cth * cth, 1, n-3) );
}

double AStudentT(int n, double alpha)  // Возвращает табличное значение t-критерия Стьюдента
{                                      // по числу степеней свободы n и уровню значимости alpha
                                       // (alpha = 1-p, где p - доверительная вероятность)
    double v = 0.5, dv = 0.5, t = 0;
    while (dv > 1e-10)
     { t = 1/v-1; dv /= 2;
       if (StudentT(t,n) > alpha) v -= dv;
       else v += dv; 
     }
    return t;
}
