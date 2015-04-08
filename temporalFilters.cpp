//============================================================================
// Name        : filters.cpp
// Author      : yeshasvi tvs
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <math.h>
#include <vector>
#include "temporalFilters.hpp"


using namespace std;

temporalFilters::temporalFilters() {

   std::cout << "Temporal Filter parameters initialization..." <<std::endl;//Debug code
   temporal_step=0.05;
   temporal_samples=0;
   normalize = true;
   //Monophasic filter parameters
   omega_m1_mono_temporal=1.2;
   mu_m1_mono_temporal=0.5;
   sigma_m1_mono_temporal=0.10;


   omega_m2_mono_temporal=0.23;
   mu_m2_mono_temporal=-0.5;
   sigma_m2_mono_temporal=0.16;

   //Biphasic filter parameters
   omega_m1_bi_temporal=54;
   mu_m1_bi_temporal=0.5;
   sigma_m1_bi_temporal=0.16;

   omega_m2_bi_temporal=54;
   mu_m2_bi_temporal=0.505;
   sigma_m2_bi_temporal=0.16;

}





double temporalFilters::computeTemporalFilters(){

    std::cout << "Computing the temporal filters with default parameters..." << std::endl;
    for(double i=-0;i<=1;i=i+temporal_step){


        try{

                monophasic_temporal.push_back(monophasic_filter(i));
        }
        catch (const std::bad_alloc&){

            std::cout<<"Bad Alloc in Mono"<<std::endl;
            return -1;

        }

        try{

                        biphasic_temporal.push_back(biphasic_filter(i));
                }
                catch (const std::bad_alloc&){

                    std::cout<<"Bad Alloc in Bi"<<std::endl;
                    return -1;

                }

        temporal_samples++;
    }

    return 0;

}

void temporalFilters::display(){

    std::cout<<"Temporal Samples : "<<temporal_samples<<std::endl;

    std::cout << "Monophasic Temporal Filter" <<std::endl;

    for(monophasic_temporal_it=monophasic_temporal.begin();monophasic_temporal_it!=monophasic_temporal.end();++monophasic_temporal_it){
            std::cout<<*monophasic_temporal_it<<std::endl;
        }

    std::cout << "Biphasic Temporal Filter" <<std::endl;
    for(biphasic_temporal_it=biphasic_temporal.begin();biphasic_temporal_it!=biphasic_temporal.end();++biphasic_temporal_it){
            std::cout<<*biphasic_temporal_it<<std::endl;
        }

}

//Normalizing function
/*void temporalFilters::normalizeFilters(){

    std::cout<< "Normalizing temporal filters..."<<std::endl;
    long double mono_sum=0;
    for(mono_it=mono.begin();mono_it!=mono.end();++mono_it){
            mono_sum=mono_sum + *mono_it;
        }
    for(mono_it=mono.begin();mono_it!=mono.end();++mono_it){
            *mono_it = *mono_it/mono_sum;
        }

    long double bi_sum=0;
    for(bi_it=bi.begin();bi_it!=bi.end();++bi_it){
            bi_sum = bi_sum + *bi_it;
        }
    for(bi_it=bi.begin();bi_it!=bi.end();++bi_it){
            *bi_it = *bi_it/bi_sum;
        }

}*/



//Monophasic filter funtion
long double temporalFilters::monophasic_filter(double time){

	//std::cout<<"Time in Mono : "<<time<<std::endl;//Debug Code


    long double D1=-(	pow( (time-mu_m1_mono_temporal),2.0)	/	(2*sigma_m1_mono_temporal*sigma_m1_mono_temporal)	);
    long double D2=-(	pow((time-mu_m2_mono_temporal),2.0)	/	(2*sigma_m2_mono_temporal*sigma_m2_mono_temporal)	);
    long double ft_mono=(omega_m1_mono_temporal*exp(D1))-(omega_m2_mono_temporal*exp(D2));

	//std::cout<<"Found Value in Mono"<<std::endl; //Debug Code
	return ft_mono;

}

//Biphasic filter funtion
long double temporalFilters::biphasic_filter(double time){

	//std::cout<<"Time in Bi : "<<time<<std::endl;//Debug Code

    long double D1=-(	pow( (time-mu_m1_bi_temporal),2.0 )	/	(2*sigma_m1_bi_temporal*sigma_m1_bi_temporal) );
    long double D2=-( pow( (time-mu_m2_bi_temporal),2.0 )  /	(2*sigma_m2_bi_temporal*sigma_m2_bi_temporal) );
    long double ft_bi=(omega_m1_bi_temporal*exp(D1))-(omega_m2_bi_temporal*exp(D2));

	//std::cout<<"Found Value in Bi"<<std::endl;//Debug Code
	return ft_bi;

}


//Destructor
temporalFilters::~temporalFilters(){

}

