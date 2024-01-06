#ifndef ZPO_SIECI_SIMULATION_HPP
#define ZPO_SIECI_SIMULATION_HPP
#include "factory.hpp"
#include "types.hpp"

void simulate(Factory& f, TimeOffset d, std::function<void(Factory&, Time)> rf);
#endif //ZPO_SIECI_SIMULATION_HPP
