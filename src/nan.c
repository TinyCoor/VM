//
// Created by xm on 2021/8/14.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define INSPECT_VALUE(type,value,label)                           \
{                                                           \
      type name =(value);                                   \
      printf("%s = \n    ",label);                         \
      print_bits((uint8_t*)&name,sizeof(name));             \
      printf("    is_nan= %d\n",is_nan(name));              \
      printf("    isnan= %d\n",isnan(name));                \
}

#define EXP_MASK (((1LL << 11LL) -1LL) << 52LL)
#define FRACTION_MASK ((1LL << 52LL) -1LL)
#define SIGN_MASK (1LL<<63LL)
#define TYPE_MASK (((1LL << 4LL) -1LL) <<48LL)
#define VALUE_MASK ((1LL << 48LL) -1LL)
#define IS_NAN(x) ((*(uint64_t*)&x) & NAN_MASK) ==NAN_MASK
#define TYPE(n) ((1LL << 3) + n )

#define DOUBLE_TYPE 0
#define INTEGER_TYPE 1
#define POINTER_TYPE 2

double mk_inf(){
  uint64_t  y =EXP_MASK;
  return *(double*)&y;
}

int is_inf(double x){
  uint64_t y = (*(uint64_t*)&x);
  return ( (y & EXP_MASK) == EXP_MASK) && ( (y &FRACTION_MASK) == 0);
}

int is_nan(double x){
  uint64_t y = (*(uint64_t*)&x);
  return ((y & EXP_MASK) == EXP_MASK) && ((y & FRACTION_MASK) != 0);
}

double set_type(double x, uint64_t type){
  uint64_t y =*(uint64_t*)&x;
  y =(y &(~TYPE_MASK) ) | (((TYPE_MASK >> 48LL) & type) <<48LL);
  return *(double*)&y;
}

double set_value(double x,uint64_t value){
  uint64_t y =*(uint64_t*)&x;
  y = (y & (~VALUE_MASK) ) | ((value &VALUE_MASK));
  return *(double*)&y;
}

uint64_t get_type(double x){
  uint64_t y =(*(uint64_t*)&x);
  return (y &TYPE_MASK) >> 48LL;
}

uint64_t get_value(double x){
  uint64_t y =(*(uint64_t*)&x);
  return (y &VALUE_MASK);
}

void print_bits(uint8_t* bytes, size_t byte_size){
  for (int i =(int )byte_size - 1; i>= 0;--i){
    uint8_t byte = bytes[i];
    for (int j = 7;j > 0;j--){
      printf("%d", !!(byte & (1 << j)));
    }
    printf(" ");
  }
  printf("\n");
}

int is_double(double x){
  return !isnan(x);
}

int is_integer(double x){
  return isnan(x) &&  get_type(x) ==TYPE(INTEGER_TYPE);
}

int is_pointer(double x){
  return isnan(x) &&  get_type(x) == TYPE(POINTER_TYPE);
}

double as_double(double x){
  return x;
}

uint64_t as_integer(double x){
  return get_value(x);
}

void* as_pointer(double x){
  return (void*)get_value(x);
}

double box_double(double x){
  return x;
}

double box_integer(double x){
  return set_value(set_type(mk_inf(),TYPE(INTEGER_TYPE)),x);
}

double box_pointer(void* x){
  return set_value(set_type(mk_inf(),TYPE(POINTER_TYPE)),(uint64_t)x);
}

#define VALUES_CAPACITY 256
double values[VALUES_CAPACITY];
size_t values_size =0;

int main(int argc,char** argv){
  double x = 34894.34923;
  values[values_size++] = box_double(3.14159265359);
  values[values_size++] =box_integer(12345678LL);
  values[values_size++] = box_pointer(&x);
  for (size_t i =0; i<values_size;++i){
    printf("%d: is_double == %d\n",i,is_double(values[i]));
    printf("%d: is_integer == %d\n",i,is_integer(values[i]));
    printf("%d: is_pointer == %d\n",i,is_pointer(values[i]));
    printf("\n");
  }

  return 0;
}
