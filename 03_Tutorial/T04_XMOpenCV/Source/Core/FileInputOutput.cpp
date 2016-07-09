/* --------------------------------------------------------------------------
 *
 *      File            FileInputOutput.cpp
 *      Description     Shows the usage of the OpenCV serialization functionality.
 *                      through pixels.      
 *      Author          OpenCV team, Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (C) 2010-2012 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com 
 * 
 * --------------------------------------------------------------------------
 * 
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or (at your option) any later version.
 * 
 *      This library is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *      Lesser General Public License for more details.
 * 
 *      You should have received a copy of the GNU Lesser General Public
 *      License along with this library in the file COPYING.LIB;
 *      if not, write to the Free Software Foundation, Inc.,
 *      59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 *
 * -------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "../Controller.h"

class MyData
{
	public :

		MyData ( ) : A ( 0 ), X ( 0 ), id ( )
		{

		}

		// explicit to avoid implicit conversion
		explicit MyData ( int ) : A ( 97 ), X ( CV_PI ), id ( "mydata1234" ) 
		{

		}

		// Write serialization for this class
		void write ( FileStorage& fs ) const                        
		{
			fs << "{" << "A" << A << "X" << X << "id" << id << "}";
		}

		// Read serialization for this class
		void read ( const FileNode& node )                          
		{
			A  = (int) node["A"];
			X  = (double) node["X"];
			id = (string) node["id"];
		}

	public :   

		// Data Members
		int			A;
		double		X;
		string		id;
};

// These write and read functions must be defined for the serialization in FileStorage to work
void write ( FileStorage& fs, const std::string&, const MyData& x )
{
    x.write(fs);
}

void read ( const FileNode& node, MyData& x, const MyData& default_value = MyData ( ) )
{
    if ( node.empty ( ) )
	{
        x = default_value;
	}
    else
	{
        x.read ( node );
	}
}

KDvoid FileInputOutput ( KDint nIdx )
{
    string  filename = "/data/outputfile.xml.gz";
	string  sOuput;

	// write
    { 
        Mat R = Mat_<uchar>::eye(3, 3),
            T = Mat_<double>::zeros(3, 1);
        MyData m(1);

        FileStorage fs ( filename, FileStorage::WRITE );

        fs << "iterationNr" << 100;
        fs << "strings" << "[";                              // text - string sequence
        fs << "image1.jpg" << "Awesomeness" << "baboon.jpg";
        fs << "]";                                           // close sequence
        
        fs << "Mapping";                              // text - mapping
        fs << "{" << "One" << 1;
        fs <<        "Two" << 2 << "}";               

        fs << "R" << R;                                      // cv::Mat
        fs << "T" << T;

        fs << "MyData" << m;                                // your own data structures

        fs.release();                                       // explicit close
    }

	// read
    {
		sOuput += "Reading: \n";
        
        FileStorage		fs; 
        fs.open ( filename, FileStorage::READ );

        int  itNr;
        itNr = (int) fs [ "iterationNr" ];
		sOuput += itNr;

        if (!fs.isOpened())
        {
            return;
        }

        FileNode n = fs["strings"];                         // Read string sequence - Get node
        if (n.type() != FileNode::SEQ)
        {
            return;
        }

        n = fs["Mapping"];                                // Read mappings from a sequence

        MyData m;
        Mat R, T;

        fs["R"] >> R;                                      // Read cv::Mat
        fs["T"] >> T;
        fs["MyData"] >> m;                                 // Read your own structure_

        //cout << endl 
        //    << "R = " << R << endl;
        //cout << "T = " << T << endl << endl;
        //cout << "MyData = " << endl << m << endl << endl;

        //Show default behavior for non existing nodes
        //cout << "Attempt to read NonExisting (should initialize the data structure with its default).";  
        fs["NonExisting"] >> m;
    }

	g_pController->setMessage ( "See Data Folder" );
}

/*


// This function will print our custom class to the console
ostream& operator<<(ostream& out, const MyData& m) 
{ 
    out << "{ id = " << m.id << ", ";
    out << "X = " << m.X << ", ";
    out << "A = " << m.A << "}";
    return out;
}

int main(int ac, char** av)
{
    if (ac != 2)
    {
        help(av);
        return 1;
    }


    {//read
        cout << endl << "Reading: " << endl;
        FileStorage fs; 
        fs.open(filename, FileStorage::READ);

        int itNr; 
        //fs["iterationNr"] >> itNr;
        itNr = (int) fs["iterationNr"];
        cout << itNr;
        if (!fs.isOpened())
        {
            cerr << "Failed to open " << filename << endl;
            help(av);
            return 1;
        }

        FileNode n = fs["strings"];                         // Read string sequence - Get node
        if (n.type() != FileNode::SEQ)
        {
            cerr << "strings is not a sequence! FAIL" << endl;
            return 1;
        }

        FileNodeIterator it = n.begin(), it_end = n.end(); // Go through the node
        for (; it != it_end; ++it)
            cout << (string)*it << endl;
        
        
        n = fs["Mapping"];                                // Read mappings from a sequence
        cout << "Two  " << (int)(n["Two"]) << "; "; 
        cout << "One  " << (int)(n["One"]) << endl << endl; 
        

        MyData m;
        Mat R, T;

        fs["R"] >> R;                                      // Read cv::Mat
        fs["T"] >> T;
        fs["MyData"] >> m;                                 // Read your own structure_

        cout << endl 
            << "R = " << R << endl;
        cout << "T = " << T << endl << endl;
        cout << "MyData = " << endl << m << endl << endl;

        //Show default behavior for non existing nodes
        cout << "Attempt to read NonExisting (should initialize the data structure with its default).";  
        fs["NonExisting"] >> m;
        cout << endl << "NonExisting = " << endl << m << endl;
    }

    cout << endl 
        << "Tip: Open up " << filename << " with a text editor to see the serialized data." << endl;

    return 0;
}
*/
