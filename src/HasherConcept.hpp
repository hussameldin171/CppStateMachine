/***************************************************************************************
 * 
 *                                State Machine Library
 * 
 *   File Name :HasherConcept.hpp
 * 
 *   Author : Hussam Wael
 * 
 *   Description : This file contains a simple metafunction to check if a type is a valid hasher.
 * 
 * **************************************************************************************/

#ifndef __HASHERCONCEPT_H__
#define __HASHERCONCEPT_H__

#include <type_traits>
#include <cstdint>

namespace sm{

    /**************************************************************************
     * 
     *                             Utilities
     * 
     * ***********************************************************************/
    
    /*Defining void_t so this library can be used in C++11 and C++14 code: */
    template<typename ...>
    using void_t = void;

    /*Defining And Metafunction: */
    template<bool cond1 , bool cond2>
    using And = std::integral_constant<bool , cond1 && cond2>;
    
    template<bool cond1 , bool cond2>
    static constexpr bool And_v = And<cond1,cond2>::value;

    /*Defining Or Metafunction: */
    template<bool cond1 , bool cond2>
    using Or = std::integral_constant<bool , cond1 || cond2>;

    template<bool cond1 , bool cond2>
    static constexpr bool Or_v = Or<cond1,cond2>::value;

    /**************************************************************************
     * 
     *                             Hashing Metafunctions
     * 
     * ***********************************************************************/

    /*
        1- A hasher functor shall have its functor operator overloaded to accept a StateId.
    */

    template<typename Hasher, typename StateId , typename = void>
    struct accepts_state_id : std::false_type{};

    template<typename Hasher, typename StateId>
    using function_operator_return_t = decltype( std::declval<Hasher>()(std::declval<StateId&>() ) );

    template<typename Hasher,typename StateId>
    struct accepts_state_id< Hasher, StateId , void_t< function_operator_return_t<Hasher,StateId> >> : std::true_type{};

    template<typename Hasher,typename StateId>
    static constexpr bool accepts_state_id_v = accepts_state_id<Hasher,StateId,void>::value;

    /**************************************************************************/

    /*2- A hasher functor shall return a value of type size_t */
    template<typename Hasher , typename StateId>
    struct returns_size_t : std::is_same< function_operator_return_t<Hasher,StateId> , std::size_t>{};

    template<typename Hasher,typename StateId>
    static constexpr bool returns_size_t_v = returns_size_t<Hasher,StateId>::value;

    /*3- A hasher functor shall be a valid hasher if it accepts a StateId and returns a size_t */
    template<typename Hasher, typename StateId>
    struct is_valid_hasher : And< accepts_state_id_v<Hasher,StateId> ,  returns_size_t_v<Hasher,StateId> >{};

    template<typename Hasher, typename StateId>
    static constexpr bool is_valid_hasher_v = is_valid_hasher<Hasher,StateId>::value;

};






#endif // __HASHERCONCEPT_H__