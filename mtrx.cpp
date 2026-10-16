
#include <iostream>
#include <cstdint>
#include <string>
#include <iomanip>
#include <cmath>      // std::abs, std::exp не конфликтует - мы переименовали свою функцию

//--------------------------------------------------------------------------------------------
double my_exp (double num , int64_t expon);

template <class T>
bool compare (const T& a ,const T& b);

template <class T , class returnT>
void bubsort (T* arr , uint16_t size , returnT (*compare)(const T& a ,const T& b));

template <class T>
void swapp(T& a , T& b);

template<class T>
void showarr (const T * arr , uint16_t size);

double mtrx_det (double ** mtrx , int64_t rows);   // cols убран: для det нужна только квадратная часть

//--------------------------------------------------------------------------------------------

int main(){
    // --- тест сортировки ---
    int * arr = new int[20]{20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1};
    bubsort(arr , 20 , compare<int>);
    showarr(arr , 20);
    delete[] arr;                 // утечка памяти исправлена
    std::cout << "\n\n";

    // --- тест определителя (сингулярная матрица, det должен быть 0) ---
    double r1[3] = {1,2,3}, r2[3] = {4,5,6}, r3[3] = {7,8,9};
    double* m1[3] = {r1,r2,r3};
    std::cout << "det1 (singular, ждем 0)      = " << mtrx_det(m1,3) << "\n";

    // det = 1*(2*2-3*1) - 1*(1*2-3*0) + 0 = 1 - 2 = -1? считаем: 2*2-3*1=1; 1*2-3*0=2; 1-2=-1... проверим программой
    double a1[3] = {1,1,0}, a2[3] = {1,2,3}, a3[3] = {0,1,2};
    double* m2[3] = {a1,a2,a3};
    std::cout << "det2                         = " << mtrx_det(m2,3) << "\n";

    // --- тест my_exp ---
    std::cout << "my_exp(2, 10)   = " << my_exp(2,10)    << "  (ждем 1024)\n";
    std::cout << "my_exp(2, -3)   = " << my_exp(2,-3)    << "  (ждем 0.125)\n";
    std::cout << "my_exp(5, 0)    = " << my_exp(5,0)     << "  (ждем 1)\n";
}

//============================================================================================
template <class T>
bool compare (const T& a ,const T& b){
    return a > b;
}

template <class T , class returnT>
void bubsort (T* arr , uint16_t size , returnT (*compare)(const T& a ,const T& b)){
    for (uint16_t i{} ; i < size/2 ; ++i){
        for (uint16_t j{0} ; j < size - 1 ; ++j){
            if(compare(arr[j] , arr[j + 1])){
                swapp(arr[j], arr[ j + 1]);
            }
            if(compare(arr[size - 2 - j] ,arr[size - j - 1])){
                swapp(arr[size - 2 - j] ,arr[size - 1 - j ]);
            }
        }
    }
}

template<class T>
void showarr (const T * arr , uint16_t size){
    for(uint16_t i{} ; i < size ; ++i){
        std::cout << arr[i] <<'\t';
    }
}

template <class T>
void swapp(T& a , T& b){
    T temp = a;
    a = b;
    b = temp;
}

double mtrx_det (double ** mtrx , int64_t rows){
    double det{1.0};

    for(int64_t i{} ; i < rows ; ++i){
        int64_t pivot = i;                                   // было int32_t - исправлено
        for(int64_t j{i + 1} ; j < rows ; ++j){
            if(std::abs(mtrx[j][i]) > std::abs(mtrx[pivot][i])){   // std::abs + <cmath>
                pivot = j;
            }
        }

        if(std::abs(mtrx[pivot][i]) < 1e-9){
            return 0.0;
        }
        if(i != pivot){
            std::swap(mtrx[i] , mtrx[pivot]);
            det *= (-1.0);
        }
        det *= mtrx[i][i];
        for (int64_t j{i + 1} ; j < rows ; ++j){
            double factor = mtrx[j][i] / mtrx[i][i];         // теперь double / double
            for (int64_t k{i + 1} ; k < rows ; ++k){         // k = i+1: диагональ и так станет 0
                mtrx[j][k] -= factor * mtrx[i][k];
            }
        }
    }
    return det;
}

double my_exp (double num , int64_t expon){
    if(num == 0.0 && expon < 0){   // защита от 1/0
        return HUGE_VAL;
    }
    if(expon < 0){
        num = 1.0 / num;           // 1.0, а не 1
        expon = -expon;
    }

    double result{1.0};
    while(expon > 0){
        if(expon % 2 != 0){
            result *= num;
        }
        num *= num;
        expon /= 2;
    }
    return result;
}
