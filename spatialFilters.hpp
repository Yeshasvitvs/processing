/*
 * spatialFilter.hpp
 *
 *  Created on: Mar 23, 2015
 *      Author: yeshasvi tvs
 */


#include <iostream>
#include <stdio.h>
#include <math.h>
#include <cmath>
#include <vector>
#include <malloc.h>
#include <memory.h>

#if defined MAX_RES
#else
#define MAX_RES 128
#endif

#if defined MAX_SIZE
#else
#define MAX_SIZE 11
#endif

#define _USE_MATH_DEFINES

#define MAX_THETA 2*M_PI //Maximum theta value is 360 degrees
using namespace std;

#ifndef SPATIALFILTER_HPP_
#define SPATIALFILTER_HPP_

class spatialFilters{

	private:

		bool debug;

	public:

		//Spatial filters parameters
		std::vector<double> theta_spatial; //Orientation of the gabor filter
		std::vector<double>::iterator theta_spatial_it; //iterator for theta
		int theta_index;
		int centerX,centerY;
		int spatial_kernel_size;
		double theta_step_size;
		double gamma_spatial; //Aspect ratio, controls the ellipticity of the gabor filter
		double lambda_spatial; //Wavelength
		double sigma_spatial; //standard deviation
        int number_directions; //Variable to store the number of directions



		struct spatial_filter_array{

			//Arrays for Spatial filters
			long double spatial_even[MAX_SIZE][MAX_SIZE];
			long double spatial_odd[MAX_SIZE][MAX_SIZE];

		} ;

		spatial_filter_array* filter_array;

		//Struct to store normalized filters
		struct normalized_spatial_filter_array{

					//Arrays for Spatial filters
					long double spatial_even[MAX_SIZE][MAX_SIZE];
					long double spatial_odd[MAX_SIZE][MAX_SIZE];

        } ;

				normalized_spatial_filter_array* normalized_filter_array;

		//Constructor
        spatialFilters();

		//Destructor
        virtual ~spatialFilters();

		//Set the kernel size
		void setKernelSize(int&);

		//Set the theta vector index and theta values
		void setTheta(double&,double&);

		//Set the filter parameters gamma, lambda and sigma
		void setParameters(double&, double&, double&);

		//Get the center of the filter
		int getCenterX();
		int getCenterY();

		//spatial filter function with no args
		void computeSpatialFilters();

		//spatial filter function theta, kernel size, Index for theta
		void computeSpatialFilters(double& , int&, int&);

		//Normalizing function
		void normalizeFilters();

		//Show the filters
        void display(); //TODO finish this function in cpp


};


#endif /* SPATIALFILTER_HPP_ */
