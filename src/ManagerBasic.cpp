//
//  ManagerBasic.cpp
//  AMODBase
//
//  Created by Harold Soh on 29/3/15.
//  Copyright (c) 2015 Harold Soh. All rights reserved.
//

#include "ManagerBasic.hpp"

namespace amod {
    ManagerBasic::ManagerBasic() {
        return;
    }
    
    ManagerBasic::~ManagerBasic() {
        return;
    }
    
    amod::ReturnCode ManagerBasic::init(World *world_state) {
        return amod::SUCCESS;
    }
    
    amod::ReturnCode ManagerBasic::update(World *world_state) {
        Simulator *sim = Manager::getSimulator();
        if (!sim) {
            return amod::SIMULATOR_IS_NULLPTR;
        }
        
        // get simulator time
        double current_time = world_state->getCurrentTime();
        
        // get events
        std::vector<Event> events;
        world_state->getEvents(&events);
        
        // respond to events
        for (auto e:events) {
            std::cout << e.t << ": Event #" << e.id << " " << e.name << " Entities: ( ";
            for (auto ent: e.entity_ids) {
                std::cout << ent << " ";
            }
            std::cout << ")" << std::endl;
        }
        world_state->clearEvents();
        
        // dispatch bookings
        auto itr = bookings_.begin();
        while (itr != bookings_.end()) {
            // check if the time is less
            if (itr->first <= current_time) {
                
                // service this booking
                Customer cust = world_state->getCustomer(itr->second.cust_id);
                
                // assign a vehicle to this customer booking
                
                // find closest free vehicle
                // simple iterative method first
                std::vector<Vehicle> vehs;
                world_state->getVehicles(&vehs);
                double min_dist = -1;
                int best_veh_id = 0;
                for (auto vitr=vehs.begin(); vitr != vehs.end(); ++vitr) {
                    double dist = sim->getDistance(vitr->getPosition(), cust.getPosition());
                    if (min_dist < 0 || dist < min_dist) {
                        amod::VehicleStatus status;
                        if (!best_veh_id || status == amod::PARKED) {
                            min_dist = dist;
                            best_veh_id = vitr->getId();
                        }
                    }
                }
                
                if (best_veh_id) {
                    Booking bk = itr->second;
                    bk.veh_id = best_veh_id;
                    
                    // tell the simulator to service this booking
                    sim->serviceBooking(bk);
                    
                    // erase the booking
                    bookings_.erase(itr);
                    
                    // back to the beginning
                    itr = bookings_.begin();
                } else {
                    ++itr;
                }
                
            } else {
                break;
            }
            
        }
        
        return amod::SUCCESS;
    }
    
    amod::ReturnCode ManagerBasic::loadBookings(const std::vector<Booking> &bookings) {
        for (auto b : bookings) {
            bookings_.emplace( b.booking_time, b);
        }
        return amod::SUCCESS;
    }
    
    amod::ReturnCode ManagerBasic::loadBookingsFromFile(const std::string filename) {
        std::ifstream in(filename.c_str());
        if (!in.good()) {
            std::cout << "Cannot read: " << filename << std::endl;
            return amod::ERROR_READING_BOOKINGS_FILE;
        }
        
        while (in.good()) {
            Booking b;
            in >> b.id >> b.booking_time >> b.cust_id >> b.destination.x >> b.destination.y;
            bookings_.emplace(b.booking_time, b);
        }
        
        return amod::SUCCESS;
    }
}