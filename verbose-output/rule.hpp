#ifndef INC_RULE_HPP_
#define INC_RULE_HPP_

#include <boost/any.hpp>

#include "./../../Common/functor_info.hpp"

namespace utils { namespace verbose_output {
    /// @brief interface for rules tracking variable state changes
    class rule
        :    public utils::functor_info<bool(boost::any const&)>
    {
    public:
        rule(){}
        virtual ~rule(){}

        /// @brief tests if current state changing should be logged
        ///        into the output stream
        virtual result_type operator()(args<0>::type);
    };
}}

#endif
