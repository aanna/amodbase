/*
 * Booking.h
 *
 *  Created on: Mar 23, 2015
 *      Author: haroldsoh
 */

#ifndef Booking_H_
#define Booking_H_

#include "Types.hpp"
#include <istream>

namespace amod {
    
    struct Booking {
    public:
    	enum Mode {TELEPORT, AMODTRAVEL};

        Booking(int booking_id = 0, int vehicle_id = 0, int customer_id = 0,
        		Position dest_position = Position(),
        		double booking_time_s = 0.0,
        		amod::Booking::Mode trav_mode = amod::Booking::Mode::AMODTRAVEL):
        			id(booking_id),
        			veh_id(vehicle_id),
        			cust_id(customer_id),
        			destination(dest_position),
        			booking_time(booking_time_s),
        			travel_mode(trav_mode),
        			pickup_time(0), dispatch_time(0), dropoff_time(0) {};
        virtual ~Booking() {};
        
        int id;         // id of booking (valid bookings have > 0)
        int veh_id;     // veh_id (valid veh_ids > 0)
        int cust_id;    // cust_id (valid cust_ids > 0)

        Position destination;   // destination position
        double booking_time;    // booking time (in seconds)

        Mode travel_mode;	// travel mode for this booking

        // the following are mainly for logging purposes (optional)
        double dispatch_time;   // dispatch time (in seconds)
        double pickup_time;     // pickup time (in seconds)
        double dropoff_time;    // dropoff time (in seconds)

        bool operator<(const Booking &rhs) const {
        	return booking_time < rhs.booking_time;
        }
        
    };
    
    // to load in enums
    inline std::istream & operator>>(std::istream & str, Booking::Mode & v) {
    	unsigned int mode = 0;
    	if (str >> mode)
    		v = static_cast<Booking::Mode>(mode);
    	return str;
    }



} /* namespace AMOD */

#endif /* Booking_H_ */
