/*
 * ******************** doc_nghb.hpp ******************************
 * 
 * description:      This is the www-documentation ...
 * 
 * owner:            Andreas Winkler
 *
 * date:             8.02.99
 *
 */


/** @name Neighbourhood Search

    Different metaheuristics based on neighbourhood search methods are defined 
    independently of the problem to be solved in a class #Lisa_Iterator#. To
    use these algorithms, the user has to define a problem dependent class
    inheriting from #Lisa_Neighborhood# which basicly describes how to generate 
    a neighbour of a given solution.
    
    An example of how to implement your own neighbourhood class can be found in 
    #LiSA/src/external/travel_neighbourhood/# .
*/
//@{
  //@Include: neighbor.hpp
  //@Include: iterate.hpp
  //@Include: tabu.hpp
  //@Include: nb_iter.hpp
  //@Include: ../travel_neighbour/travel.cpp
//@}


