/*
 * filters.hpp
 *
 *  Created on: Mar 10, 2015
 *      Author: yeshi
 */
#include <iostream>
#include <math.h>
#include <cmath>
#include <vector>
#include <malloc.h>
#include <memory.h>

#define _USE_MATH_DEFINES
#if defined MAX_RES
#else
#define MAX_RES 128
#endif
using namespace std;


#ifndef FILTERS_HPP_
#define FILTERS_HPP_

class temporalFilters{

private:


    //Monophasic filter parameters
    double omega_m1_mono_temporal;
    double mu_m1_mono_temporal;
    double sigma_m1_mono_temporal;


    double omega_m2_mono_temporal;
    double mu_m2_mono_temporal;
    double sigma_m2_mono_temporal;

    //Biphasic filter parameters
    double omega_m1_bi_temporal;
    double mu_m1_bi_temporal;
    double sigma_m1_bi_temporal;

    double omega_m2_bi_temporal;
    double mu_m2_bi_temporal;
    double sigma_m2_bi_temporal;

public:

    double temporal_step; //TODO has to be adjusted to event history buffer size
    int temporal_samples;//Debug Code
    bool normalize;

    //Vectors for storing temporal filters
    std::vector <long double> monophasic_temporal;
    std::vector <long double> biphasic_temporal;


    //Vectors for storing normalized temporal filters
    //std::vector <long double> normalized_monophasic_temporal;
    //std::vector <long double> normalized_biphasic_temporal;

    //Temporal filter vector iterators
    std::vector<long double>::iterator monophasic_temporal_it;
    std::vector<long double>::iterator biphasic_temporal_it;



    //Constructor
    temporalFilters();

    //temporal filter functions
    long double monophasic_filter(double);
    long double biphasic_filter(double);

    //Temporal Filter computation
    double computeTemporalFilters();

    //Normalizing function
    //void normalizeFilters();

    //Display the filters
    void display();

    //Destructor
    virtual ~temporalFilters();


};





#endif /* FILTERS_HPP_ */
