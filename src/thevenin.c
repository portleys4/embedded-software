#include "thevenin.h"
#include "stdint.h"
#include "stdarg.h"

float ThevEq(uint8_t res_count, ...){

    va_list res_list;

    va_start(res_list, res_count);

    double res_total = 0;
    int i;
    for(i = 0; i < res_count; i++){
        res_total += 1 / va_arg(res_list, double);
    }

    va_end(res_list);

    return (float) 1 / res_total;

}