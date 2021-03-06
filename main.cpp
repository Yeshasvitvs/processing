//============================================================================
// Name        : eventGabor.cpp
// Author      : yeshasvi tvs && Himanshu Akolkar
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#if defined MAX_RES
#else
#define MAX_RES 128
#endif


#include <iostream>
#include <yarp/os/all.h>
#include <yarp/sig/all.h>
#include <yarp/conf/options.h>
#include <iCub/emorph/all.h>
#include <string>
#include <opencv/cxcore.h>
#include <opencv2/core/core.hpp>
#include <opencv/highgui.h>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cmath>
#include <iCub/emorph/activityMat.h>
#include "eventHistoryBuffer.hpp"
#include "spatialFilters.hpp"
#include "temporalFilters.hpp"


using namespace yarp::os;
using namespace emorph;
using namespace std;
using namespace cv;

//Function definition
double spatialProcessing(emorph::AddressEvent& , int& );
double temporalProcessing(emorph::AddressEvent&);
void motionHypothesis();
bool motion; //Bool for doing motion hypothesis

//Global variables

//Event History buffer instance
eventHistoryBuffer event_history;


//Creating an instances of filters
spatialFilters sfilters;
temporalFilters tfilters;

//double decayRate = 50;
//ActivityMat instances
//emorph::activityMat activity_mat(MAX_RES,MAX_RES, decayRate);
//ActivityMat array
emorph::activityMat activity_mat[8];



double conv_value_even_biphasic; //Even+biphasic convolution value storage variable
double conv_value_odd_monophasic; //Odd+monophaisc convolution value storage variable
double conv_value; //Convolution value storage variable

//Vector variable for storing different theta convolution values
std::vector<double> conv_value_vector;
std::vector<double>::iterator conv_value_vector_it;


emorph::vtsHelper unwrap;

int main(int argc, char *argv[])
{
    bool normalize=false;
    long double activity_threshold = 0.2;

   //Resource finder 
   yarp::os::ResourceFinder rf;
   rf.configure(argc,argv);
	
   //Set up yarp network
   yarp::os::Network yarp;

   //Variables
   yarp::os::BufferedPort<emorph::vBottle> inputPort;
   yarp::os::BufferedPort<emorph::vBottle> outputPort;

   yarp::sig::ImageOf<yarp::sig::PixelMono> eventImage; //Putting each event as a snapshot image for processing
   yarp::os::Port eventImagePort; //Port to output event image
   //int MAX_RES = 128;

   emorph::vBottle *tempEvents; //Temporary events
   
   cv::Mat eventImageMat = Mat(128,128,CV_32FC1,Scalar(0)); //OpenCV MAT variable

   //Using Resource finder for port names
   string inputPortName = rf.find("inputPort").asString();
   string outputPortName = rf.find("outputPort").asString();    
   string sourcePortName = rf.find("sourcePort").asString();
   string eventImagePortName = rf.find("eventImagePort").asString();

   //Setting default port names
   if(inputPortName == ""){ inputPortName = "/inputEvents"; }
   if(outputPortName == ""){ outputPortName = "/outputEvents"; }
   if(sourcePortName == ""){ sourcePortName = "/aexGrabber/vBottle:o"; }
   if(eventImagePortName == ""){eventImagePortName = "/eventImage";}
   //Port connections
   bool ok = inputPort.open(inputPortName.c_str()); //Opening input port
   ok = ok && outputPort.open(outputPortName.c_str()); //Opening output port
   ok = ok && eventImagePort.open(eventImagePortName.c_str());
   
   //checking ports
   if (!ok) {
    fprintf(stderr, "Failed to create ports.\n");
    fprintf(stderr, "Maybe you need to start a nameserver (run 'yarpserver')\n");
    return 1;
   }

   if(yarp.connect(sourcePortName.c_str(), inputPortName.c_str(), "udp")){
 	  
	std::cout << "source port to input port connection successful.... " <<std::endl;  

   }
   else{std::cout << "source port to input port connection failed! " <<std::endl;}

   //Computing the filter values
   sfilters.computeSpatialFilters();
   //if(normalize==true){
   //    sfilters.normalizeFilters();
   //}

   tfilters.computeTemporalFilters();
   //if(normalize==true){
   //    tfilters.normalizeFilters();
   //}
   //tfilters.display();



   while(true){ //TODO wait till the input port receives data and run


	tempEvents = inputPort.read(true); //Read the incoming events into a temporary vBottle
    emorph::vQueue q; //Event queue
   	emorph::vQueue::iterator q_it;//Event queue iterator
 	
	tempEvents->getAll(q); // 1 ms timeframe
    //std::cout << "Processing " << q.size()<< " events" <<std::endl;


 	for (q_it = q.begin(); q_it != q.end(); q_it++) //Processing each event
            {

                emorph::AddressEvent *aep = (*q_it)->getAs<emorph::AddressEvent>();
                if(!aep) continue; //If AER is not received continue
                int channel = aep->getChannel();

                //Processing each event of channel 0
                if(channel==0){


                    //NOTE : Sensor X and Y are reversed
                    std::cout<< "Event at "<< "X : "<< int ( aep->getY() ) <<" Y : "<< int ( aep->getX() ) <<std::endl; //Debug code
                    sfilters.theta_spatial_it = sfilters.theta_spatial.begin(); //Iterating over all directions, Theta
                    sfilters.theta_index = 0;

                    for( ; sfilters.theta_spatial_it != sfilters.theta_spatial.end() ; ++sfilters.theta_spatial_it ){

                        // convolution value reset to zero for each event processing
                        conv_value_even_biphasic=0;
                        conv_value_odd_monophasic=0;
                        conv_value=0;

                        //std::cout << "Theta index value : " << sfilters.theta_index << std::endl; //Debug Code

                        //NOTE : Update the activity when an event is about to be processed       
                         activity_mat[sfilters.theta_index].addEvent(*aep); //Adding event to the activityMat, This is where activity is set to 1


                        spatialProcessing(*aep , sfilters.theta_index); //Calling spatial processing function
                        // std::cout<<"Event processing done!!!"<<std::endl; //Debug Code

                        conv_value = conv_value_odd_monophasic + conv_value_odd_monophasic ;
                        //NOTE : Update the activity after event processing
                        //activity_mat[sfilters.theta_index].addEvent(*aep,conv_value); //Update the activity at the event location after processing each event
                        activity_mat[sfilters.theta_index].addEvent(*aep,conv_value);


                        conv_value_vector.push_back( conv_value );

                        ++sfilters.theta_index;
                        motion = true;
                    } //End of theta for loop


                    //Generating single motion hypothesis
                    if(motion == true){
                        motionHypothesis();
                    }



/*


                    //std::cout << "Mono conv value : "<< mono_conv << std::endl;//Debug Code

                    //After processing each event update the whole activity matrix
                    //long double maxvalue=0;

                    for(int v=0; v< MAX_RES; v++){

                		for(int u=0; u< MAX_RES; u++){

                            //cout<<activity_mat.queryActivity(v,u);
                			long double temp=activity_mat.queryActivity(v,u);
                            //cout<<temp;
                            if(abs(temp) > activity_threshold){
                                //cout<<" Temp Value : "<<abs(temp)<<std::endl;
                                eventImageMat.at<float>(v,u)=255;
                            }
                            else{

                                temp=0;
                				eventImageMat.at<float>(v,u)=temp;
                            }

                			//maxvalue=max(temp,maxvalue);
                		}
                        //cout<<endl;
                    }
                	//cout<<"Max: " <<maxvalue<<endl;
                    for(int v=0; v< MAX_RES; v++){
                		for(int u=0; u< MAX_RES; u++){

                			eventImageMat.at<float>(v,u)= eventImageMat.at<float>(v,u) * (255/maxvalue);
                		if(eventImageMat.at<float>(v,u)!=0){

                			eventImageMat.at<float>(v,u) = 255;
                		}
                			//cout<<eventImageMat.at<float>(v,u);
                		}
                		//std::cout<<std::endl;
                    }
                	//cout << eventImageMat <<endl;
                	//std::cout<<"Image processing done"<<std::endl;//Debug Code
                    //cv::namedWindow("EVENT IMAGE",WINDOW_NORMAL);
                    //cv::imshow("EVENT IMAGE", eventImageMat);
                    //cv::waitKey(10);

                    */
                } //End of channel if, one event processing done

	    }//End of event q loop


 	   //eventImagePort.write(eventImage); //write to the event image port
       //yarp.connect(eventImagePortName.c_str(),"/yarpview/img:i","tcp");

   } //end of while loop


   std::cout<<"End of while loop"<<std::endl; //Debug code

    return 0;
}



double  spatialProcessing (emorph::AddressEvent &event , int &theta_index){

    //NOTE : Sensor X and Y are reversed
    int posX    = event.getY();
    int posY    = event.getX();

    //std::cout << "Theta index value : "<<theta_index <<std::endl; //Debug Code

    //std::cout << "Spatial filter processing..." << std::endl; //Debug code
    for(int j=0 ; j<11 ; j++){ //TODO check the rows and cols later

        for(int i=0 ; i<11 ; i++){

            //Pixel positions in activity filter
            int pixelX = posX + i-5;
            int pixelY = posY + j-5;

            if(pixelX >= 0 && pixelY >= 0 && pixelX < MAX_RES && pixelY < MAX_RES){ //Checking for borders

                long double activity_value = activity_mat[sfilters.theta_index].queryActivity(pixelY,pixelX);
                //std::cout << " " << activity_value ; //Debug Code

                //Even filter convolution
                long double even_conv = activity_value * sfilters.filter_array[theta_index].spatial_even[j][i];
                conv_value_even_biphasic = conv_value_even_biphasic + even_conv;

                //Odd filter convolution
                long double odd_conv = activity_value * sfilters.filter_array[theta_index].spatial_odd[j][i];
                conv_value_odd_monophasic= conv_value_odd_monophasic + odd_conv;

            }
            else{

                //std::cout<< "Pixels Out of Bordes...."<<std::endl; //Debug Code
                continue;
            }


        }

        //std::cout <<std::endl; //Debug Code
    }
    //std::cout<< "Convolution Value oddMonophasic : " << conv_value_odd_monophasic <<std::endl;//Debug Code
    //std::cout<< "Convolution Value evenBiphasic : " << conv_value_even_biphasic <<std::endl;//Debug Code

    temporalProcessing(event); //Calling temporal processing function

}



double temporalProcessing(emorph::AddressEvent &event){


    //std::cout << "Temporal filter processing..." << std::endl; //Debug code

    //NOTE : Sensor X and Y are reversed
    int posX    = event.getY();
    int posY    = event.getX();

    double event_time = unwrap(event.getStamp()); //Get the current event time stamp
    //std::cout<< "Time stamp : " << event_time <<std::endl;//Debug Code

    //Updating event history buffer with time stamps
    event_history.updateList(event);

    //Setting the temporal filter iterators to vector begin
    tfilters.monophasic_temporal_it = tfilters.monophasic_temporal.begin();
    tfilters.biphasic_temporal_it = tfilters.biphasic_temporal.begin();


    //std::cout << "Buffer Size : " <<  event_history.timeStampList[posX][posY].size() << std::endl;//Debug Code
    event_history.timeStampsList_it = event_history.timeStampList[posX][posY].rbegin(); //Going from latest event pushed in the back
    //std::cout << "Buffer value back : " <<  *event_history.timeStampsList_it<<std::endl;//Debug Code
    //std::cout << "Buffer value front : " <<  event_history.timeStampList[posX][posY].front() << std::endl;//Debug Code

    //NOTE : List size is always limited to the buffer size, took care of this in the eventbuffer code
    for( int list_length = 1 ; list_length <= event_history.timeStampList[posX][posY].size() ; ){

         //std::cout << "Buffer value : " <<  *event_history.timeStampsList_it<<std::endl;//Debug Code
         long double temporal_difference = event_time - *event_history.timeStampsList_it; //The first value is always zero
         temporal_difference = temporal_difference / 1000000 ; //Converting from Micro secs TODO : check if it is right
         //std::cout << "Temporal difference : " << temporal_difference <<std::endl; //Debug code

         conv_value_odd_monophasic = conv_value_odd_monophasic + temporal_difference * (*tfilters.monophasic_temporal_it) ;
         conv_value_even_biphasic = conv_value_even_biphasic + temporal_difference * (*tfilters.biphasic_temporal_it) ;


         ++event_history.timeStampsList_it;
         ++list_length;

         //Increment temporal filter iterators
         ++tfilters.monophasic_temporal_it;
         ++tfilters.biphasic_temporal_it;
    }

    //std::cout<< "Convolution Value oddMonophasic : " << conv_value_odd_monophasic <<std::endl;//Debug Code
    //std::cout<< "Convolution Value evenBiphasic : " << conv_value_even_biphasic <<std::endl;//Debug Code


}

void motionHypothesis(){


    //Velocity components set to zero
    double Ux = 0;
    double Uy = 0 ;

    //Code for single motion hypothesis generation
    conv_value_vector_it = conv_value_vector.begin();
    sfilters.theta_spatial_it = sfilters.theta_spatial.begin();
    for(; conv_value_vector_it != conv_value_vector.end() ; ++conv_value_vector_it){

        //std::cout << "The convolotion value : " << *conv_value_vector_it ; //Debug Code
        //std::cout << " for theta Value  " << *sfilters.theta_spatial_it << std::endl; //Debug Code

         Ux = Ux + ( *conv_value_vector_it ) * ( cos(*sfilters.theta_spatial_it) ) ;
         Uy = Uy - ( *conv_value_vector_it ) * ( sin(*sfilters.theta_spatial_it) );


         ++sfilters.theta_spatial_it;
    }

    std::cout << "Velocity Components Ux : " << Ux << " Uy : " << Uy << std::endl; //Debug Code

    //Clear the convolution vector after processing for single motion hypothesis
    conv_value_vector.erase(conv_value_vector.begin(), conv_value_vector.end());
    motion = false;
}



