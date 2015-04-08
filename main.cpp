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

int main(int argc, char *argv[])
{
    bool normalize=false;
    long double activity_threshold = 0.2;
	double decayRate = 50;
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

   //ActivityMat instances
   emorph::activityMat activity_mat(MAX_RES,MAX_RES, decayRate);

   //Event History buffer instance
   eventHistoryBuffer event_history;



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

   //Creating an instance of spatial filters
   spatialFilters sfilters;
   sfilters.computeSpatialFilters();
   if(normalize==true){
	   sfilters.normalizeFilters();
   }

   temporalFilters tfilters;
   tfilters.computeTemporalFilters();
   //if(normalize==true){
   //    tfilters.normalizeFilters();
   //}
   //tfilters.display();




   emorph::vtsHelper unwrap;


   while(true){ //wait till the input port receives data and run


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
                //std::cout<<"Time stamp :"<<ev_t<<" X : "<<posX<<" Y : "<<posY<<" Polarity : "<<pol<<" Channel : "<<channel<<std::endl; //Debug Code

                //Processing each event with spatial filters and storing its activity level
                //TODO include the theta code later

                if(channel==0){

                    double conv_value=0; //Filter convolution value
                    int posX    = aep->getX();
                    int posY    = aep->getY();
                    int pol     = aep->getPolarity();

                    double ev_t = unwrap((*q_it)->getStamp()); //Get the current event time stamp
                    //cout<< "Time stamp : " << ev_t <<endl;
                   // std::cout<<"Pre "<<activity_mat.queryActivity(posX,posY);
                    activity_mat.addEvent(*aep); //Adding event to the activityMat, This is where activity is set to 1
                   // std::cout<<" New "<<activity_mat.queryActivity(posX,posY)<<std::endl;

                   //Updating event history buffer with time stamps
                   event_history.updateList(*aep);



                    //std::cout<< "Event at "<< "X : "<<posX<<" Y : "<<posY <<std::endl; //Debug code
                    //std::cout << "Spatial filter processing..." << std::endl; //Debug code
                	for(int j=0 ; j<11 ; j++){ //TODO check the rows and cols later

                		for(int i=0 ; i<11 ; i++){

                			//Pixel positions in activity filter
                            int pixelX = posX + i-5;
                	        int pixelY = posY + j-5;

                	        if(pixelX >= 0 && pixelY >= 0 && pixelX <= MAX_RES && pixelY <= MAX_RES){ //Checking for borders

                                long double activity_value = activity_mat.queryActivity(pixelX,pixelY);
                                long double even_conv = activity_value * sfilters.filter_array[3].spatial_even[j][i];
                                //long double odd_conv = activity_value * sfilters.filter_array[3].spatial_odd[j][i];
                                //convValue = convValue + even_conv + odd_conv;
                                conv_value = conv_value + even_conv;


                	        }
                	        else{
                                //std::cout<< "Pixels Out of Bordes...."<<std::endl; //Debug Code
                	        	continue;
                	        }

                		}
                	}
                   // std::cout<<"  "<<convValue<<std::endl; //Debug Code
                   // std::cout<<"Event processing done!!!"<<std::endl; //Debug Code

                    //std::cout<<" Pre Activity "<<activity_mat.queryActivity(posX,posY);
                    activity_mat.addEvent(*aep,conv_value); //Update the activity at the event location after processing each event

                    //Setting the temporal filter iterators to vector begin
                    tfilters.monophasic_temporal_it = tfilters.monophasic_temporal.begin();
                    tfilters.biphasic_temporal_it = tfilters.biphasic_temporal.begin();

                    //std::cout << "Temporal filter processing..." << std::endl; //Debug code

                    //NOTE  : problem with the temporal filter iterators
                    //std::cout << "Monophasic filter vaalue : "<< *tfilters.monophasic_temporal_it <<std::endl;//Debug code
                    //TODO temporal filter processing at the event location

                    //NOTE : Little trick to avoid accessing list with iterators if there is only one element
                    if(event_history.timeStampList[posX][posY].size()>1){

                        cout<<"Inside the IF trick"<<endl;//Debug Code


                        event_history.timeStampsList_it = event_history.timeStampList[posX][posY].begin();


                        //TODO figure out a way to do temporal processing
                        for( ; event_history.timeStampsList_it != event_history.timeStampList[posX][posY].end() ; event_history.timeStampsList_it++){

                            std::cout << "Buffer Size : " <<  event_history.timeStampList[posX][posY].size() << std::endl;//Debug Code

                            std::cout << "Buffer value : " <<  *event_history.timeStampsList_it<<std::endl;//Debug Code
                            long double temporal_difference = ev_t - *event_history.timeStampsList_it++;
                            //NOTE: The resolution is so small and leads to 0 value for temporal difference
                            std::cout << "Temporal difference : " << temporal_difference <<std::endl; //Debug code
                            conv_value = conv_value + temporal_difference * (*tfilters.monophasic_temporal_it) ;
                            tfilters.monophasic_temporal_it++;


                        }

                    }
                    else{

                        //cout<<"Inside the else trick"<<endl;//Debug Code
                        long double temporal_difference = ev_t - event_history.timeStampList[posX][posY].front();
                        //std::cout << "Temporal difference : " << temporal_difference <<std::endl; //Debug code
                        conv_value = conv_value + temporal_difference * (*tfilters.monophasic_temporal_it) ;
                        //event_history.timeStampList[posX][posY].pop_front(); //Removing the oldest event time stamp

                    }






                    //std::cout << "Mono conv value : "<< mono_conv << std::endl;//Debug Code

                    //After processing each event update the whole activity matrix
                    long double maxvalue=0;

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
                	/*for(int v=0; v< MAX_RES; v++){
                		for(int u=0; u< MAX_RES; u++){

                			eventImageMat.at<float>(v,u)= eventImageMat.at<float>(v,u) * (255/maxvalue);
                		if(eventImageMat.at<float>(v,u)!=0){

                			eventImageMat.at<float>(v,u) = 255;
                		}
                			//cout<<eventImageMat.at<float>(v,u);
                		}
                		//std::cout<<std::endl;
                	}*/
                	//cout << eventImageMat <<endl;
                	//std::cout<<"Image processing done"<<std::endl;//Debug Code
                	cv::namedWindow("EVENT IMAGE",WINDOW_NORMAL);
                    cv::imshow("EVENT IMAGE", eventImageMat);
                    cv::waitKey(10);
                } //End of channel if, one event processing done

	    }//End of event q loop





 	   //eventImagePort.write(eventImage); //write to the event image port
 	   //yarp.connect(eventImagePortName.c_str(),"/yarpview/img:i","tcp");

   } //end of while loop


   std::cout<<"End of while loop"<<std::endl; //Debug code

    return 0;
}
