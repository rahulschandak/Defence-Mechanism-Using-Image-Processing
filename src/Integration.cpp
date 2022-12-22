#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>
#include <wiringPi.h>
#include <iostream>
#include <wiringSerial.h>

using namespace cv;
using namespace std;

//int servox = 10;
//int servoy = 6;

int main()
{
	int fd ;
//	char fd1;  
	
    if (((fd = serialOpen ("/dev/ttyUSB0",9600)) < 0))								//Defining USB port where Arduino is connected							
	{
		fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
		return 1 ;
    }
    
    //if (  ((fd1 = serialOpen ("/dev/ttyUSB0",9600)) < 0)   )								//Defining USB port where Arduino is connected							
	//{
		//fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
		//return 1 ;
    //}

	wiringPiSetup();
	int a1, a2, a3, a4, a5, a6, a7, a8;
	int centrefacex, centrefacey, centrecols, centrerows , valx , valy ;
	int xdist; 									// xdist is the horizontal distance of detected object from the centre
	int ydist ;									// ydist is the vertical distance of detected object from the centre
	int deg = 12, num = 1;
	int countx = 0, county = 0;
	int motionx = 0 , motiony =0;
	int shoot;
				
	VideoCapture cap(0);												
	cap.open(0);														//Camera Open
	Mat img;
	CascadeClassifier face_cascade;
	face_cascade.load("black.xml");				//xml file containing trained images of black balloon
	for (;;)
	{
		cap >> img;		
		cap.retrieve(img, CAP_OPENNI_BGR_IMAGE);
		resize(img, img, Size(900,630));
		vector<Rect> faces;
		face_cascade.detectMultiScale(img, faces, 1.1, 1 ,30| CASCADE_SCALE_IMAGE, Size(150, 150));

		a5 = (((img.cols) / 2) - 5);  			//top right x-coordinate of center aim
		a6 = (((img.cols) / 2) + 5);  			//bottom left x-coordinate of center aim
		a7 = (((img.rows) / 2) - 5);  			// top right y-coordinate of centre aim
		a8 = (((img.rows) / 2) + 5);  			// bottom left y-coordinate of centre aim

		valx = img.cols / deg;
		valy = img.rows / deg;
		
		rectangle(img, Point(((img.cols) / 2) - 5, ((img.rows) / 2) - 5), Point(((img.cols) / 2) + 5, ((img.rows) / 2) + 5), Scalar(255, 0, 0), 2, 8);
		
		for (unsigned i = 0; i < faces.size(); i++)
		{
			 a1 = (faces[i].x);
			 a2 = (faces[i].x + faces[i].width);
			 a3 = (faces[i].y);
			 a4 = (faces[i].y + faces[i].height);  

			rectangle(img, faces[i], Scalar(255, 0, 0), 2, 1);
			
			if ((a7>a3) &&(a4>a8) && (a5>a1) && (a2>a6))
			{
				{
					cout << "shoot" << endl;
					shoot=20;
					serialPutchar(fd,shoot);
				}
			}
			
			else
			{
				cout << "go on" << endl;
			}

			centrecols = ((img.cols) / 2);      	                  //getting screen centre x-coordinate
			centrerows = ((img.rows) / 2);          	              //getting screen centre y-coordinate
			centrefacex = (faces[i].x + (faces[i].width) / 2); 	   	  //getting centre of object x-coordinate
			centrefacey = (faces[i].y + (faces[i].height) / 2);  	  //getting centre of object y-coordinate

			if ((centrefacex > centrecols) && (centrefacey < centrerows))			//object is in I quadrant
			{
				xdist = centrefacex - centrecols;
				ydist = centrerows - centrefacey;
				//cout <<"xdist = "<< xdist << "ydist = " << ydist << endl;
				if(motionx <= xdist)
				{
					while (motionx <= xdist)
					{
						motionx = motionx + valx;
						countx -= 1;
						if (motionx==xdist)
							break;
					}
				}
				
				else 
					if (motionx > xdist && motionx > 0)
					{
						while (motionx >= xdist)
						{	
							motionx = motionx-valx; 
							countx += 1;
							if (motionx==xdist)		
								break;
						}
					}
				if(motiony <= ydist)
				{
					while (motiony <= ydist)
					{ 
						motiony = motiony + valy;
						county += 1;
						if (motiony==ydist)
							break;
					}
				}
				else 
				if (motiony >= ydist && motiony > 0)
				{
				    while (motiony >= ydist)
				    {	
						motiony = motiony -valy;
						county -= 1;
						if (motiony==ydist)
							break;
					}
				}
				
				//cout<<"countx = "<<countx<<"----"<<"county = "<<county<<"---- motionx = "<<motionx <<"----"<<"motiony = "<<motiony<< endl;
				serialPutchar (fd, countx/num) ;									//Sending 'x' axis' value to Arduino
				serialPutchar (fd, county/num) ;									//Sending 'y' axis' value to Arduino
				cout<<" countx = "<< countx <<"county = "<< county <<endl;
			}
				
			//////////////////////////////////////////
				
			else if ((centrefacex < centrecols) && (centrefacey < centrerows))			//II quadrant
			{	
				xdist = centrefacex - centrecols;
				ydist = centrerows - centrefacey;

				//cout <<"xdist = "<< xdist << "ydist = " << ydist << endl;
				if(motionx >= xdist)
				{
					while (motionx >= xdist)
					{
						motionx = motionx - valx;
						countx +=1;
						if (motionx==xdist)
							break;
					}
			    } 
			    else if(motionx <= xdist && motionx < 0)
			    {
					while (motionx <= xdist)
					{
						motionx = motionx + valx;
						countx -= 1;
						if (motionx==xdist)
							break;
					}
				}
				if (motiony <= ydist)
				{	
					while (motiony <= ydist)
					{
						motiony = motiony + valy;
						county += 1;
						if (motiony==ydist)
							break;

					}
				}
				else if (motiony >= ydist && motiony > 0)
				{
					while (motiony >= ydist)
					{
						motiony = motiony - valy;
						county -= 1;
						if (motiony==ydist)
							break;
					}
				}
				//cout<<"countx = "<<countx<<"----"<<"county = "<<county<<"---- motionx = "<<motionx <<"----"<<"motiony = "<<motiony<< endl;
				  serialPutchar (fd, countx/num) ;												//Sending 'x' axis value to Arduino
				  serialPutchar (fd, county/num) ;												//Sending 'y' axis value to Arduino
				  cout<<" countx = "<< countx <<"county = "<< county <<endl;
			}
			
			/////////////////////////
			
			else if ((centrefacex < centrecols) && (centrefacey > centrerows))                  //III quadrant
				{
				xdist = centrefacex - centrecols;
				ydist = centrerows - centrefacey;
				//cout <<"xdist = "<< xdist << "ydist = " << ydist << endl;
				if(motionx >= xdist)
				{
					while (motionx >= xdist)
					{
						motionx = motionx - valx;	
						countx +=1;
						if (motionx==xdist)
							break;
					}
				}
				else if (motionx <= xdist && motionx <0)
				{
					while (motionx <= xdist )
					{
						motionx = motionx + valx;
						countx -=1;
						if (motionx==xdist)
							break;
					}
				}
				if (motiony >= ydist)
				{
					while (motiony >= ydist)
					{
						motiony = motiony - valy;
						county -= 1;
						if (motiony==ydist)
								break;
					}
				}
				else if (motiony <= ydist && motiony < 0)
				{
					while(motiony <= ydist)
					{
						motiony = motiony +valy;
						county += 1;
						if (motiony==ydist)
							break;
					}
					
				}
				//cout<<"countx = "<<countx<<"----"<<"county = "<<county<<"---- motionx = "<<motionx <<"----"<<"motiony = "<<motiony<< endl;
				serialPutchar (fd, countx/num) ;												//Sending 'x' axis value to Arduino
				serialPutchar (fd, county/num) ;												//Sending 'y' axis value to Arduino			
				cout<<" countx = "<< countx <<"county = "<< county <<endl;
			}
			
			/////////////////////////////////
			
			else if ((centrefacex > centrecols) && (centrefacey > centrerows))			// IV quadrant
			{				
				xdist = centrefacex - centrecols;
				ydist = centrerows - centrefacey;
				//cout <<"xdist = "<< xdist << "ydist = " << ydist << endl;
				if (motionx <= xdist)
				{					
					while (motionx <= xdist)
					{
						motionx = motionx + valx;
						countx -=1;
						if (motionx==xdist)
							break;
					}
				}
				else if (motionx >= xdist && motionx >0)
				{
					while (motionx >= xdist)
					{
						motionx =motionx - valx;
						countx += 1;
						if (motionx==xdist)
							break;
					}
				}
				if (motiony >= ydist)
				{
					while (motiony >= ydist)
					{
						motiony = motiony - valy;
						county -= 1;			
						if (motiony==ydist)
								break;
					}
				}
				else if (motiony <= ydist && motiony <0)
				{
					while (motiony <= ydist)
					{
						motiony = motiony + valy;
						county += 1;
						if (motiony==ydist)
							break;
					}
				}
				//cout<<"countx = "<<countx<<"----"<<"county = "<<county<<"---- motionx = "<<motionx <<"----"<<"motiony = "<<motiony<< endl;
				serialPutchar (fd, countx/num) ;										//Sending 'x' axis' value to Arduino
				serialPutchar (fd, county/num) ;										//Sending 'y' axis' value to Arduino			
				cout<<" countx = "<< countx <<"county = "<< county <<endl;

			}
           }
			
		imshow("Display", img);
		if (waitKey(20) == 27)
		{
			break;
		}
	}
	return 0;
}
