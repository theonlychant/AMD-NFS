#include "booster.hpp"
#include <string>
#include <sstream>
#include <iostream>
#include "../c/hip_kernels.h"

namespace booster {

void init(){
  // Placeholder for any initialization the booster may need
  std::cerr<<"[booster] init called"<<std::endl;
}

std::string generate_boosted(const std::string &prompt){
  // Try to use HIP-accelerated path if available, otherwise fallback
#if defined(HAVE_HIP) || defined(USE_HIP)
  // Call into the HIP kernel launcher as a placeholder for real acceleration
  int status = launch_gemm(nullptr, nullptr, nullptr, 1, 1, 1);
  if(status == 0){
    std::ostringstream ss;
    ss<<"[boosted-hip] "<<prompt;
    return ss.str();
  }
#endif
  // CPU fallback
  return std::string("[boosted-cpu] ") + prompt;
}

} // namespace booster
