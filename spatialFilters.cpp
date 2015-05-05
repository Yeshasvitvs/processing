/*
 * spatialFilters.cpp
 *
 *  Created on: Mar 23, 2015
 *      Author: yeshasvi tvs
 */

#include "spatialFilters.hpp"


//Constructor definition
spatialFilters::spatialFilters(){

    std::cout<<"Spatial Filter parameters initialization..."<<std::endl;
	debug=true; //Debug Boolean
	//Spatial filters default parameters
    number_directions = 1;
	theta_step_size=0.25*M_PI;
	 for(double i=0;i<MAX_THETA;i=i+theta_step_size){
		 //std::cout<<" theta Value : "<<i<<std::endl;//Debug code
		 theta_spatial.push_back(i); //Orientation of the gabor filter
         ++number_directions;
	 }
	 theta_index=0;
	 spatial_kernel_size=11; //size of the spatial kernel
	 centerX= int (spatial_kernel_size/2)+1; //center pixel
	 centerY= int (spatial_kernel_size/2)+1; //center pixel
	 gamma_spatial=0.3; //Aspect ratio, controls the ellipticity of the gabor filter
	 lambda_spatial=10; //Wavelength
	 sigma_spatial=2; //standard deviation

	 //Initializing an array of structurs to store spatial filters
	 //spatial_filter_array *filter_array=new spatial_filter_array[theta_spatial.size()];
	 filter_array=new spatial_filter_array[theta_spatial.size()];
	 normalized_filter_array=new normalized_spatial_filter_array[theta_spatial.size()];




}

//Destructor
spatialFilters::~spatialFilters(){

}

//Filter parameters setting function
void spatialFilters::setParameters(double& gamma,double& lambda,double& sigma){

	gamma_spatial=gamma; //Aspect ratio, controls the ellipticity of the gabor filter
	lambda_spatial=lambda; //Wavelength
	sigma_spatial=sigma; //standard deviation

}

//Filter Kernel size setting function
void spatialFilters::setKernelSize(int& kernel_size){

	spatial_kernel_size=kernel_size;

}

//Theta vector with given step size and max theta
void spatialFilters::setTheta(double& theta_step, double& max_theta){

	theta_step_size=theta_step;
	for(double i=0;i<max_theta;i=i+theta_step_size){
		 //std::cout<<" theta Value : "<<i<<std::endl;//Debug code
		 theta_spatial.push_back(i); //Orientation of the gabor filter
	}
	//Todo put the theta index to the first element in the vector
}


//Spatial filters computation with given args
void spatialFilters::computeSpatialFilters(double& theta,int& kernel_size, int& index){


    //std::cout<<"Center Value : "<<center<<std::endl;//Debug Code
    //std::cout<<"Theta Value : "<<theta<<std::endl;//Debug Code
    //std::cout<<"Kernel size Value : "<<kernel_size<<std::endl;//Debug Code

	for(int y=0;y<kernel_size;y++){
		//std::cout<<"Computing Spatial Filter Y loop : "<<y<<std::endl;//Debug Code
		for(int x=0;x< kernel_size;x++){

			//std::cout<<"Computing Spatial Filter X loop : "<<x<<std::endl;//Debug Code
			int xdash = -(x - centerX)*sin(theta) + (y - centerY)*cos(theta);
			int ydash =	(x - centerX)*cos(theta) + (y - centerY)*sin(theta);
			long double temp = -(xdash*xdash + gamma_spatial*gamma_spatial*ydash*ydash)/(2*sigma_spatial*sigma_spatial);

			long double Gabor_real = exp(temp) * cos(2*M_PI*xdash/lambda_spatial);
			long double Gabor_img = exp(temp) * sin(2*M_PI*xdash/lambda_spatial);
			//std::cout<<"Gabor value : "<<Gabor_real<<" , "<<Gabor_img<<std::endl;//Debug Code
			//std::cout<<"Theta Index Value : "<<index<<std::endl;

			filter_array[index].spatial_even[y][x]=Gabor_real;
			filter_array[index].spatial_odd[y][x]=Gabor_img;

		}
	}


}

//Spatial filters computation with defaults parameters
void spatialFilters::computeSpatialFilters(){

	std::cout<<"Computing the spatial filters with default parameters...."<<std::endl;
    //std::cout<<"Center Value : "<<center<<std::endl;//Debug Code
    //std::cout<<"Theta Value : "<<theta<<std::endl;//Debug Code
    //std::cout<<"Kernel size Value : "<<kernel_size<<std::endl;//Debug Code
	theta_index=0;
	for(theta_spatial_it=theta_spatial.begin(); theta_spatial_it!=theta_spatial.end(); theta_spatial_it++){
		for(int y=0;y<spatial_kernel_size;y++){
			//std::cout<<"Computing Spatial Filter Y loop : "<<y<<std::endl;//Debug Code
			for(int x=0;x<spatial_kernel_size;x++){

				//std::cout<<"Computing Spatial Filter X loop : "<<x<<std::endl;//Debug Code
				int xdash = -(x - centerX)*sin(*theta_spatial_it) + (y - centerY)*cos(*theta_spatial_it);
				int ydash =	(x - centerX)*cos(*theta_spatial_it) + (y - centerY)*sin(*theta_spatial_it);
				long double temp = -(xdash*xdash + gamma_spatial*gamma_spatial*ydash*ydash)/(2*sigma_spatial*sigma_spatial);

				long double Gabor_real = exp(temp) * cos(2*M_PI*xdash/lambda_spatial);
				long double Gabor_img = exp(temp) * sin(2*M_PI*xdash/lambda_spatial);
				//std::cout<<"Gabor value : "<<Gabor_real<<" , "<<Gabor_img<<std::endl;//Debug Code
				//std::cout<<"Theta Index Value : "<<index<<std::endl; //Debug Code

				filter_array[theta_index].spatial_even[y][x]=Gabor_real;
				filter_array[theta_index].spatial_odd[y][x]=Gabor_img;

				//std::cout<< "Even Filter : "<<filter_array[theta_index].spatial_even[y][x] << " Odd filter"<<filter_array[theta_index].spatial_odd[y][x]<<std::endl;

			}
		}

		theta_index++;
	}
}

void spatialFilters::normalizeFilters(){

	std::cout << "Normalizing Filters...." << std::endl;
	theta_index=0;
	long double filter_sum_even=0;
	long double filter_sum_odd=0;
	for(theta_spatial_it=theta_spatial.begin(); theta_spatial_it!=theta_spatial.end(); theta_spatial_it++){

		for(int y=0;y<spatial_kernel_size;y++){
			for(int x=0;x<spatial_kernel_size;x++){

				filter_sum_even = filter_sum_even + filter_array[theta_index].spatial_even[y][x];
				filter_sum_odd = filter_sum_odd + filter_array[theta_index].spatial_odd[y][x];

			}

		}

		for(int y=0;y<spatial_kernel_size;y++){
			for(int x=0;x<spatial_kernel_size;x++){

				normalized_filter_array[theta_index].spatial_even[y][x] = filter_array[theta_index].spatial_even[y][x] / filter_sum_even;
				normalized_filter_array[theta_index].spatial_odd[y][x] = filter_array[theta_index].spatial_odd[y][x] / filter_sum_odd;


			}

		}
		theta_index++;
	}
}



