/***************************************************************************************
 * 
 *                                State Machine Library
 * 
 *   File Name : StateFactory.hpp
 * 
 *   Author : Hussam Wael
 * 
 *   Description : This file contains the implementation of factory functions for creating states, 
 *                 transitions and state machines.
 * 
 * **************************************************************************************/

#ifndef __STATEFACTORY_H__
#define __STATEFACTORY_H__

#include "StateMachine.hpp"



namespace sm{
/**************************************************************************
 * 
 *                             Helper Functions
 * 
 * ***********************************************************************/

constexpr void DoNothing(void){}

constexpr bool Always(void){ return true; }

constexpr bool Never(void) {return false; }

/**************************************************************************
 * 
 *                             Factory Functions
 * 
 * ***********************************************************************/

//1- State Factory Function:
template<typename StateId>
auto MakeState(StateId id , action_t  during = DoNothing,  action_t entry = DoNothing , action_t exit = DoNothing){
    
    return State<StateId>{id , entry , during , exit};

}

//2- Transition Factory Function:
template<typename StateId>
auto MakeTransition(StateId fromState , StateId toState , action_t  action = DoNothing, predicate_t  condition = Always ){

    return Transition<StateId>{fromState , toState , action , condition};

}

//3- State Machine Factory Function: Array Constructor
template<typename StateId , size_t NumberOfStates = static_cast<size_t>(StateId::NumberOfStates)>
auto MakeStateMachine(const typename StateMachine<StateId>::states_list& states , const typename StateMachine<StateId>::transitions_list& transitions , StateId initialState = StateId{}){

    return StateMachine<StateId , NumberOfStates>{states , transitions , initialState};

}

//4- State Machine Factory Function: Array Constructor
template<size_t NumberOfStates , typename StateId>
auto MakeStateMachine(const typename StateMachine<StateId>::states_list& states , const typename StateMachine<StateId>::transitions_list& transitions , StateId initialState = StateId{}){

    return StateMachine<StateId , NumberOfStates>{states , transitions,initialState};

}

//5- State Machine Factory Function: Initializer Lists Constructor
template<typename StateId , size_t NumberOfStates = static_cast<size_t>(StateId::NumberOfStates)>
auto MakeStateMachine(const std::initializer_list<State<StateId>> & states , const std::initializer_list<Transition<StateId>> & transitions , StateId initialState = StateId{}){

    return StateMachine<StateId , NumberOfStates>{states , transitions, initialState};

}

//6- State Machine Factory Function: Initializer Lists Constructor
template<size_t NumberOfStates , typename StateId>
auto MakeStateMachine(const std::initializer_list<State<StateId>> & states , const std::initializer_list<Transition<StateId>> & transitions , StateId initialState = StateId{}){

    return StateMachine<StateId , NumberOfStates>{states , transitions , initialState};

}

}




#endif // __STATEFACTORY_H__