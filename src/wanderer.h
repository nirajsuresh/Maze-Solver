#ifndef __WANDERER_AGENT__H
#define __WANDERER_AGENT__H 

#include <string>
#include <math.h>
#include "enviro.h"

//! \file

namespace {

    using namespace enviro;

    //! Initial state; move forward at speed 6
    class MovingForward : public State, public AgentInterface {
        public:
        void entry(const Event& e) {}

        //! Maintain forward motion and check for turning conditions
        void during() { 
            //! Constant velocity forwards
            track_velocity(6,0);
            //! If within 10 of an object in the general forward direction, rotate away
            if ( sensor_value(0) < 10 || sensor_value(1) < 10 || sensor_value(2) < 10 ) {
                emit(Event(tick_name));
            }
            //! If large opening in the direction of the exit, rotate towards
            if (sensor_value(3) > 50 && angle() <= -1 && angle() >= -2.4) {
                emit(Event(tick2_name));
            } else if (sensor_value(4) > 50 && angle() >= 1 && angle() <= 2.4) {
                emit(Event(tick2_name));
            }
        }
        void exit(const Event& e) {}
        void set_tick_name(std::string s) { tick_name = s; }
        void set_tick2_name(std::string s) { tick2_name = s; }
        std::string tick_name;
        std::string tick2_name;
    };

    //! Rotating away from objects when too close
    class Rotating : public State, public AgentInterface {
        public:
        // Set rotation velocity initially to 0
        void entry(const Event& e) {
            direction = 0;
        }
        //! Logic for determining rotation
        void during() {
            //! If rotation direction not yet determined
            if (direction == 0) {
                //! If front sensor tripped, rotate accordingly
                if (sensor_value(2) < 10) {
                    if (sensor_value(3) > sensor_value(4)) {
                        direction = 2;
                    } else {
                        direction = -2;
                    }
                //! Else if front-left or front-right sensor tripped, rotate accordingly
                } else if (sensor_value(0) < 10) {
                    direction = -2;
                } else if (sensor_value(1) < 10) {
                    direction = 2;
                }
            }
            //! Rotate in the required direction
            track_velocity(0, direction);
            //! Transition to move forward if all three sensor values report enough room
            if ( sensor_value(0) > 10 && sensor_value(1) > 10 && sensor_value(2) > 10) {
                emit(Event(tick_name));
            }
        }
        void exit(const Event& e) {}
        double rate;
        double direction;
        void set_tick_name(std::string s) { tick_name = s; }
        std::string tick_name;        
    };

    //! Rotate toward opening if in general direction of the exit
    class RotateToward : public State, public AgentInterface {
        public:
        // Set rotation velocity initially to 0
        void entry(const Event& e) {
            direction = 0;
        }
        //! Logic for determining rotation
        void during() {
            if (direction == 0) {
                //! Get start angle (will rotate ~90 deg from this)
                start_angle = angle();
                //! Rotate in the direction of the sensor that was tripped
                if (sensor_value(3) > sensor_value(4)) {
                    direction = 2;
                } else {
                    direction = -2;
                }
            }
            track_velocity(0, direction);
            //! Rotation update until desired angle reached
            if ( abs(abs(start_angle) - abs(angle())) > 1.2) {
                emit(Event(tick2_name));
            }
        }
        void exit(const Event& e) {}
        double rate;
        double direction;
        double start_angle;
        void set_tick2_name(std::string s) { tick2_name = s; }
        std::string tick2_name;        
    };

    //! WandererController state machine
    class WandererController : public StateMachine, public AgentInterface {

        public:
        WandererController() : StateMachine(){

            //! Initial state to move forward
            set_initial(moving_forward);
            tick_name = "tick_" + std::to_string(rand()%1000); // use an agent specific generated 
                                                               // event name in case there are 
                                                               // multiple instances of this class
            tick2_name = "tick2_" + std::to_string(rand()%1000); 

            // Define transitions
            add_transition(tick_name, moving_forward, rotating);
            add_transition(tick2_name, moving_forward, rotate_toward);
            add_transition(tick_name, rotating, moving_forward);
            add_transition(tick2_name, rotate_toward, moving_forward);
            moving_forward.set_tick_name(tick_name);
            moving_forward.set_tick2_name(tick2_name);
            rotating.set_tick_name(tick_name);
            rotate_toward.set_tick2_name(tick2_name);

        }

        MovingForward moving_forward;
        Rotating rotating;
        RotateToward rotate_toward;
        std::string tick_name;
        std::string tick2_name;

    };

    //! Wanderer class
    class Wanderer : public Agent {

        public:
        Wanderer(json spec, World& world) : Agent(spec, world) {
            add_process(wc);
        }

        WandererController wc;

    };

    DECLARE_INTERFACE(Wanderer);

}

#endif