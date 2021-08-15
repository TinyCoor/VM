//
// Created by xm on 2021/8/14.
//
#pragma once
#include <iostream>

class Double{
 public:
  explicit Double(double val);

 private:
  bool is_nan();

 private:
  double value_;

};

inline std::ostream& operator<<(std::ostream& os,Double d){
  size_t byte_size = sizeof(Double);
  bytes = &d;
  for (int i =(int )byte_size - 1; i>= 0;--i){
    uint8_t byte = bytes[i];
    for (int j = 7;j > 0;j--){
      os << !!(byte & (1 << j));
    }
    os <<" ";
  }
  os << "\n";
  return os;
}

