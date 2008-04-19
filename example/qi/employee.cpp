/*=============================================================================
    Copyright (c) 2002-2007 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
///////////////////////////////////////////////////////////////////////////////
//
//  A parser for arbitrary tuples. This example presents a parser
//  for an employee structure.
//
//  [ JDG May 9, 2007 ]
//
///////////////////////////////////////////////////////////////////////////////
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>

#include <iostream>
#include <string>
#include <complex>

using namespace boost::phoenix;
using namespace boost::spirit;
using namespace boost::spirit::qi;
using namespace boost::spirit::ascii;
using namespace boost::spirit::arg_names;

///////////////////////////////////////////////////////////////////////////////
//  Our employee struct
///////////////////////////////////////////////////////////////////////////////
struct employee
{
    int age;
    std::string surname;
    std::string forename;
    double salary;
};

// We need to tell fusion about our employee struct
// to make it a first-class fusion citizen
BOOST_FUSION_ADAPT_STRUCT(
    employee,
    (int, age)
    (std::string, surname)
    (std::string, forename)
    (double, salary)
)

///////////////////////////////////////////////////////////////////////////////
//  Our employee parser
///////////////////////////////////////////////////////////////////////////////
template <typename Iterator>
struct employee_parser : grammar_def<Iterator, employee(), space_type>
{
    employee_parser()
    {
        quoted_string %= lexeme['"' >> +(char_ - '"') >> '"'];

        start %=
            lit("employee")
            >> '{'
            >>  int_ >> ','
            >>  quoted_string >> ','
            >>  quoted_string >> ','
            >>  double_
            >>  '}'
            ;
    }

    rule<Iterator, std::string(), space_type> quoted_string;
    rule<Iterator, employee(), space_type> start;
};

////////////////////////////////////////////////////////////////////////////
//  Main program
////////////////////////////////////////////////////////////////////////////
int
main()
{
    std::cout << "/////////////////////////////////////////////////////////\n\n";
    std::cout << "\t\tAn employee parser for Spirit...\n\n";
    std::cout << "/////////////////////////////////////////////////////////\n\n";

    std::cout
        << "Give me an employee of the form :"
        << "employee{age, \"surname\", \"forename\", salary } \n";
    std::cout << "Type [q or Q] to quit\n\n";

    typedef std::string::const_iterator iterator_type;
    typedef employee_parser<iterator_type> employee_parser;

    employee_parser def; //  Our grammar definition
    grammar<employee_parser> g(def); // Our grammar
    std::string str;
    while (getline(std::cin, str))
    {
        if (str.empty() || str[0] == 'q' || str[0] == 'Q')
            break;

        employee emp;
        std::string::const_iterator iter = str.begin();
        std::string::const_iterator end = str.end();
        bool r = phrase_parse(iter, end, g, emp, space);

        if (r && iter == end)
        {
            std::cout << boost::fusion::tuple_open('[');
            std::cout << boost::fusion::tuple_close(']');
            std::cout << boost::fusion::tuple_delimiter(", ");

            std::cout << "-------------------------\n";
            std::cout << "Parsing succeeded\n";
            std::cout << "got: " << boost::fusion::as_vector(emp) << std::endl;
            std::cout << "\n-------------------------\n";
        }
        else
        {
            std::cout << "-------------------------\n";
            std::cout << "Parsing failed\n";
            std::cout << "-------------------------\n";
        }
    }

    std::cout << "Bye... :-) \n\n";
    return 0;
}

