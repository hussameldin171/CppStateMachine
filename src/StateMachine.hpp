/***************************************************************************************
 * 
 *                                State Machine Library
 * 
 *   File Name : StateMachine.hpp
 * 
 *   Author : Hussam Wael
 * 
 *   Description : This file contains the implementation of a generic state machine.
 * 
 * **************************************************************************************/

#ifndef __STATEMACHINE_H__
#define __STATEMACHINE_H__

#include <cstdint>
#include <array>
#include <initializer_list>
#include <cstddef>
#include "Hasher.hpp"
#include "HasherConcept.hpp"
#include <algorithm>
namespace sm{

    using action_t = void(*)(void);

    using predicate_t = bool(*)(void);


    template<typename StateId>
    struct State{

        /*1- The ID of the state*/
        StateId id;   

        /*2-Action to perform on entry: */
        action_t entryAction{};

        /*3-Action to perform during the execution of the state: */
        action_t duringAction{};

        /*4-Action to perform on exit: */
        action_t exitAction{};

    };

    template<typename StateId>
    struct Transition{

        /*1- State to transition from: */
        StateId fromState;

        /*2- State to transition to: */
        StateId toState;

        /*3- Action on transition:*/
        action_t action{};

        /*4- Condition to transit: */
        predicate_t condition{};

    };


    template<typename StateId , size_t NumberOfStates = static_cast<size_t>(StateId::NumberOfStates) , typename Hasher = DefaultHasher<NumberOfStates>>
    struct StateMachine{

        /**************************************************************************
         * 
         *                             Assertions
         * 
         * ***********************************************************************/
        
        static_assert(NumberOfStates > 0 , "Number of states must be greater than zero");

        static_assert(accepts_state_id_v<Hasher,StateId> , "Hasher must accept a StateId as an argument");

        static_assert(returns_size_t_v<Hasher,StateId> , "Hasher must return a value of type size_t");
        
        /**************************************************************************
         * 
         *                             Constants
         * 
         * ***********************************************************************/
        
        static constexpr size_t NumberOfTransitions = NumberOfStates * NumberOfStates;

        /**************************************************************************
         * 
         *                             Type Aliases
         * 
         * ***********************************************************************/
    public:
        
        /*1- Type alias for a state*/
        using state_t = State<StateId>;

        /*2- Type alias for a transition*/
        using transition_t = Transition<StateId>;
        
        /*3- Type alias for the list of states*/
        using states_list = std::array<state_t, NumberOfStates>;

        /*4- Type alias for the list of transitions*/
        using transitions_list = std::array<transition_t, NumberOfTransitions>;
        
        /*5- Type alias for the state transition map*/
        using state_transition_map = std::array< std::array< transition_t, NumberOfStates>, NumberOfStates>;
        
        private:
        /**************************************************************************
         * 
         *                             Private Members
         * 
         * ***********************************************************************/

        /*1- The list of states*/
        states_list m_states;

        /*2- The mapping of states to transitions*/
        state_transition_map m_transitions;

        /*3- The current state*/
        size_t current_state;

        /*4- The initial state (for resetting the state machine)*/
        size_t init_state;

        /*4- A hasher functor to convert StateId to indices in the range [0,NumberOfStates)*/
        Hasher hashFunc{};
        
        
        public:

        /**************************************************************************
         * 
         *                             Public Methods
         * 
         * ***********************************************************************/

        /* A constructor shall supply two parameters:
            1- The list of states
            2- The list of transitions
        */
        
        //1- Arrays Constructor: 
        StateMachine(const states_list& states,const transitions_list& transitions , StateId initialState = StateId{}) : m_states{states}{

            /*1- Sorting states ascendingly based on their hashing values: */
            std::sort(m_states.begin(), m_states.end() , [hasher = Hasher{}](state_t a , state_t b){return hasher(a.id) < hasher(b.id);});

            /*2- Initialize the state transition map*/
            for(auto && entry : transitions){
                m_transitions[hashFunc(entry.fromState)][hashFunc(entry.toState)] = entry;
            }

            /*3- Set the initial state*/

            init_state = hashFunc(initialState);

            current_state = init_state;

            /*4- Execute the entry action of the initial state*/
            if(m_states[current_state].entryAction){

                m_states[current_state].entryAction();
            
            }
        }

        //2- Initializer Lists Constructor:
        StateMachine(const std::initializer_list<state_t> & states , const std::initializer_list<transition_t> & transitions , StateId initialState = StateId{}){
            
            /*1- Moving ownership of the given states: */
            std::move(states.begin() , states.end() , m_states.begin());

            /*2- Sorting states ascendingly based on their hashing values: */
            std::sort(m_states.begin(), m_states.end() , [hasher = Hasher{}](state_t a , state_t b){return hasher(a.id) < hasher(b.id);});

            /*3- Initialize the state transition map*/
            for(auto && entry : transitions){
                m_transitions[hashFunc(entry.fromState)][hashFunc(entry.toState)] = entry;
            }

            /*4- Set the initial state*/
            
            init_state = hashFunc(initialState);

            current_state = init_state;

            /*3- Execute the entry action of the initial state*/
            if(m_states[current_state].entryAction){

                m_states[current_state].entryAction();
            
            }
        }

        
        void Step(void){
            
            /*1- Execute the during action of the current state: */
            if(m_states[current_state].duringAction){

                m_states[current_state].duringAction();
            
            }
            
            /*2- Searching for the next state to transition to: */
            for(size_t otherState = 0 ; otherState < NumberOfStates ; otherState++){

                if(otherState == current_state){ //Skip the current state
                    continue;
                }

                //Check if the condition is met
                if(m_transitions[current_state][otherState].condition &&
                m_transitions[current_state][otherState].condition()){

                    /*Now we're supposed to transition to 'otherState'*/
                    /*1- Execute the exiting action of the current state: */
                    if(m_states[current_state].exitAction){

                        m_states[current_state].exitAction();
                    
                    }

                    /*2- Execute the transition action: */
                    if(m_transitions[current_state][otherState].action){

                        m_transitions[current_state][otherState].action();
                    
                    }

                    /*3- Execute the entry action of the new state: */
                    if(m_states[otherState].entryAction){

                        m_states[otherState].entryAction();
                    
                    }

                    /*4- Update the current state: */
                    current_state = otherState;

                    /*5- Break the loop: */
                    break;
                }

            }
        }
    

    bool IsInState(StateId state) const{
        return current_state == hashFunc(state);
    
    }

    void reset(void){
        current_state = init_state;
    }

};
}
#endif // __STATEMACHINE_H__