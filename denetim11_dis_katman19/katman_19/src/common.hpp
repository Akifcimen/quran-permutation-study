#ifndef JOINT_COMMON_HPP
#define JOINT_COMMON_HPP
#include <array>
#include <cstdint>
namespace study {
inline constexpr int N=114,F=14,P=29,V=2,C=F*P,K=6;
inline constexpr std::array<int,F> AL={1,1,1,1,1,1,2,2,2,2,3,3,3,3};
inline constexpr std::array<int,F> BE={1,-1,2,-2,3,-3,1,-1,3,-3,1,-1,2,-2};
inline constexpr const char* POLICY[K]={"uniform","mixture","literal","discussion","anchored19","frequency"};
}
#endif
