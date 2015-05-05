#include "eventHistoryBuffer.hpp"

//Constructor Definition
eventHistoryBuffer::eventHistoryBuffer(){ //one time initialization

    std::cout << "Event History Buffer Constructor..." << std::endl;
    height = MAX_RES;
    width = MAX_RES;
    bufferSize = 20;
    disp = false;

    //Setting the memory for array of Lists

    /*for(int j=0 ; j < height; j++){
        for(int i=0 ; i < width; i++){
           timeStampList[j][i].resize(bufferSize); //Setting the buffer size for each list and initializing the values to zero
        }
    }*/


    //Clearing the memory in destructor
    // delete[] timeStampList;



}


double eventHistoryBuffer::updateList(emorph::AddressEvent &event){


    //std::cout<< "Updating the List..." <<std::endl;//Debug Code
    //NOTE : The Sensor X and Y are reversed
    x = event.getY();
    y = event.getX();

    if(x < 0 || x > width-1 || y < 0 || y > height-1) return -1;

    //Get the event timestamp
    event_time = unwrap(event.getStamp());

    //Pushing the event time stamp into the list
    timeStampList[x][y].push_back(event_time);

    //Check if the list size is more than buffer size
    if(timeStampList[x][y].size() > bufferSize){

        timeStampList[x][y].pop_front(); //if list is more than buffer size reduce it to buffer size
    }


    //Call the display
    if(disp==true){ display(); }


    return 0;

}

void eventHistoryBuffer::display(){


    //Checking the event history buffer size
    std::cout << "Stored Buffer Size : " <<  timeStampList[x][y].size() << std::endl;//Debug Code

    if(!timeStampList[x][y].empty()){

        std::cout << "Stored Buffer value : " << timeStampList[x][y].front() << std::endl; //Debug Code

    }



}

//Destructor
eventHistoryBuffer::~eventHistoryBuffer(){

}
