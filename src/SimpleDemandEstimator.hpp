//
//  SimpleDemandEstimator.h
//  AMODBase
//
//  Created by Harold Soh on 18/4/15.
//  Copyright (c) 2015 Harold Soh. All rights reserved.
//

#ifndef __AMODBase__SimpleDemandEstimator__
#define __AMODBase__SimpleDemandEstimator__

#include <utility>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>

#include "Types.hpp"
#include "Booking.hpp"
#include "DemandEstimator.hpp"
#include "Location.hpp"
#include "World.hpp"
#include "KDTree.hpp"


namespace amod {
    class SimpleDemandEstimator : DemandEstimator {
    public:
        SimpleDemandEstimator(double bin_width = 3600): bin_width_(bin_width),
            daily_mean_(0.0),
            daily_var_(100.0) { }; //1 hour bins by default
        
        virtual ~SimpleDemandEstimator() { };
        
        // predict
        // predicts the demand (number of customer bookings) at a given position pos or location id
        // at a given time t
        // returns a std::pair with the mean prediction and the uncertainty
        virtual std::pair<double, double> predict(const amod::Position &pos, const amod::World &world_state, double t);
        virtual std::pair<double, double> predict(int loc_id, const amod::World &world_state, double t);
        
        
        // loadDemandFromFile
        // loads demand from a file specified by filename that the manager should respond to.
        // if the call is successful, it returns amod::SUCESSS. Otherwise, it returns
        // one of the amod::ReturnCode error codes.
        virtual amod::ReturnCode loadDemandFromFile(const std::string filename);
        
        // loadStations
        // loads the locations which the predictions will be based on
        virtual void loadLocations(std::vector<amod::Location> &locations);

        
    private:
        
        struct Demand {
            int id;
            double t;
            int loc_id;
            Position pos;
        };
        
        const double kSecondsInDay = 86400;
        double bin_width_;
        std::unordered_map<int, std::unordered_map<int, double>> demands_hist_;
        std::unordered_map<int, std::unordered_map<int, double>> day_counts_;
        
        kdt::KDTree<amod::Location> locs_tree_; //for fast NN lookup

        // defaults for zero prediction
        double daily_mean_;
        double daily_var_;
        
        // makeBookingsHist
        // creates the booking histogram (overwrites any existing histogram)
        virtual void makeDemandHist(const std::vector<Demand> &demands);
    };
}



#endif /* defined(__AMODBase__SimpleDemandEstimator__) */
