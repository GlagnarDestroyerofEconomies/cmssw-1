/*! \file testJetFinder.cpp
 * \brief Procedural unit-test code for the L1GctJetFinder class.
 *
 *  This is code that tests each public method of the L1GctJetFinder
 *  class.  It takes data from a file to test the methods against known
 *  results.  Results are also output to file to allow debugging.
 *
 * \author Robert Frazier
 * \date March 2006
 */

#include "L1Trigger/GlobalCaloTrigger/interface/L1GctJetFinder.h"  //The class to be tested

//Custom headers needed for this test
#include "L1Trigger/GlobalCaloTrigger/interface/L1GctRegion.h"
#include "L1Trigger/GlobalCaloTrigger/interface/L1GctJet.h"

//Standard library headers
#include <fstream>   //for file IO
#include <string>
#include <vector>
#include <iostream>
#include <exception> //for exception handling
#include <stdexcept> //for std::runtime_error()
#include <map> //for pair
using namespace std;


//Typedefs for the vector templates and other types used
typedef vector<L1GctRegion> RegionsVector;
typedef vector<L1GctJet> JetsVector;
typedef unsigned long int ULong;


// Name of the files for test input data and results output.
const string testDataFile = "testJetFinderInput.txt";  
const string resultsFile = "testJetFinderOutput.txt";


// Global constants to tell the program how many things to read in from file
// THESE ARE TOTAL GUESSES!
const int numInputRegions = 48;  //Num. calorimeter regions given as input
const int numOutputJets = 6;     //Num. jets expected out
//There will be Jet Counts to be added here at some point


//  FUNCTION PROTOTYPES
/// Runs the test on the L1GctJetFinder instance passed into it.
void classTest(L1GctJetFinder *myJetFinder);
/// Loads test input regions and also the known results from a text file.
void loadTestData(RegionsVector &inputRegions, JetsVector &trueJets, ULong &trueHt); //Need to add Jet Counts eventually
/// Function to safely open input files of any name, using a referenced return ifstream
void safeOpenInputFile(ifstream &fin, const string name);
/// Function to safely open output files of any name, using a referenced return ofstream
void safeOpenOutputFile(ofstream &fout, const string name);
/// Reads regions from file and pushes the specified number into a vector of regions
void putRegionsInVector(ifstream &fin, RegionsVector &regions, const int numRegions);
/// Gets the data of a single region from the testDataFile (reasonably safely). 
L1GctRegion readSingleRegion(ifstream &fin);
/// Reads jets from file and pushes the specified number into a vector of jets
void putJetsInVector(ifstream &fin, JetsVector &jets, const int numJets);
/// Gets the data of a single jet from the testDataFile (reasonably safely).  
L1GctJet readSingleJet(ifstream &fin);
/// Compares RegionsVectors, prints a message about the comparison, returns true if identical, else false.
bool compareRegionsVectors(RegionsVector &vector1, RegionsVector &vector2, const string description);
/// Compares JetsVectors, prints a message about the comparison, returns true if identical, else false.
bool compareJetsVectors(JetsVector &vector1, JetsVector &vector2, const string description);
/// Writes out the entire contents of a RegionsVector to the given file output stream
void outputRegionsVector(ofstream &fout, RegionsVector &regions, string description = "Regions");
/// Writes out the entire contents of a JetsVector to the given file output stream
void outputJetsVector(ofstream &fout, JetsVector &jets, string description = "Jets");


/// Entrypoint of unit test code + error handling
int main(int argc, char **argv)
{
    cout << "\n*************************************" << endl;
    cout << "  L1GctJetFinder class unit tester." << endl;
    cout << "*************************************" << endl;

    try
    {
        L1GctJetFinder * myJetFinder = new L1GctJetFinder(9); //TEST OBJECT on heap;
        classTest(myJetFinder);
        delete myJetFinder;
    }
    catch(const exception &e)
    {
        cerr << "\nError! " << e.what() << endl;
    }
    catch(...)
    {
        cerr << "\nError! An unknown exception has occurred!" << endl;
    }
    
    return 0;   
}

// Runs the test, and returns a string with the test result message in.
void classTest(L1GctJetFinder *myJetFinder)
{
    bool testPass = true; //flag to mark test failure
    
    // Vectors for reading in test data from the text file.
    RegionsVector inputRegions;  //Size?
    JetsVector trueJets;         //Size?
    ULong trueHt;
    //Jet Counts to be added at some point
    
    // Vectors for receiving the output from the object under test.
    RegionsVector outputRegions; //Size?
    JetsVector outputJets;       //Size?
    ULong outputHt;
    //Jet Counts to be added at some point
    
    // Load our test input data and known results
    loadTestData(inputRegions, trueJets, trueHt);  //will need to add Jet Counts eventually
    
    //Fill the L1GctJetFinder with regions.
    for(int i = 0; i < numInputRegions; ++i)
    {
        myJetFinder->setInputRegion(i, inputRegions[i]);
    }

    // Test the getInputRegion method
    outputRegions = myJetFinder->getInputRegions();
    if(!compareRegionsVectors(outputRegions, inputRegions, "initial data input/output")) { testPass = false; }

    myJetFinder->process();  //Run algorithm
    
    //Get the outputted data and store locally
    outputJets = myJetFinder->getJets();
    outputHt = myJetFinder->getHt();
    //Will need to add a line here for jet counts eventually

    //Test the outputted jets against the known results
    if(!compareJetsVectors(outputJets, trueJets, "outputted jets")) { testPass = false; }
    
    //Test the outputted Ht against known result
    if(outputHt != trueHt)
    {
        cout << "\nTest class has FAILED Ht comparison!" << endl;
        testPass = false;
    }
    else
    {
        cout << "\nTest class has passed Ht comparison." << endl;
    }
        
    //Will have to add a jet counts comparison section here eventually

    //Write out all outputtable information to file
    cout << "\nWriting results of processing to file " << resultsFile << "..." << endl;;
    ofstream fout;
    safeOpenOutputFile(fout, resultsFile);
    outputRegionsVector(fout, outputRegions, "Inputted Regions");
    outputJetsVector(fout, outputJets, "Outputted Jets");
    fout << "Outputted Ht" << endl;
    fout << outputHt << endl << endl;
    //will need to output jet counts here eventually
    fout.close();
    cout << "Done!" << endl;
    
    //Run the reset method.
    myJetFinder->reset();
    
    //get all the data again - should all be empty
    outputRegions = myJetFinder->getInputRegions();
    outputJets = myJetFinder->getJets();
    outputHt = myJetFinder->getHt();
    //will need to and jets counts line here eventually
    
    //an empty regions vector for reset comparison
    vector<L1GctRegion> blankRegionsVec(numInputRegions);
    vector<L1GctJet> blankJetsVec(numOutputJets);
    
    //Test that all the vectors/values are empty/zero
    if(compareRegionsVectors(outputRegions, blankRegionsVec, "input regions reset") &&
       compareJetsVectors(outputJets, blankJetsVec, "output jets reset") &&
       outputHt == 0)
    { 
        cout << "\nTest class has passed reset method testing." << endl;
    }
    else
    {
        cout << "\nTest class has FAILED reset method testing!" << endl;
        testPass = false;
    }

    //Print overall results summary to screen
    if(testPass)
    {
        cout << "\n*************************************" << endl;
        cout << "      Class has passed testing." << endl;
        cout << "*************************************" << endl;
    }
    else
    {
        cout << "\n*************************************" << endl;
        cout << "      Class has FAILED testing!" << endl;
        cout << "*************************************" << endl;
    }
    
    return;                
}


// Loads test input regions from a text file.
void loadTestData(RegionsVector &inputRegions, JetsVector &trueJets, ULong &trueHt) //Need to add Jet Counts eventually
{
    // File input stream
    ifstream fin;
    
    safeOpenInputFile(fin, testDataFile);  //open the file
    
    putRegionsInVector(fin, inputRegions, numInputRegions);  //How many input regions? See global constants.
    putJetsInVector(fin, trueJets, numOutputJets);           //How many?? See global constants.
    
    if(fin.eof() || fin.bad())
    {
       throw std::runtime_error("Error reading Ht data from " + testDataFile + "!");
    }
    else
    {
        fin >> trueHt;
    }
    
    //Will need to add a Jet Counts bit to this
    
    fin.close();
    
    return;
}

// Function to safely open input files of any name, using a referenced return ifstream
void safeOpenInputFile(ifstream &fin, const string name)
{
    //Opens the file
    fin.open(name.c_str(), ios::in);

    //Throw an exception if something is wrong
    if(!fin.good())
    {
        throw std::runtime_error("Couldn't open the file " + name + " for reading!");
    }
    return;
}

// Function to safely open output files of any name, using a referenced return ofstream
void safeOpenOutputFile(ofstream &fout, const string name)
{
    //Opens the file
    fout.open(name.c_str(), ios::trunc);
    
    //Throw an exception if something is wrong
    if(!fout.good())
    {
        throw std::runtime_error("Couldn't open the file " + name + " for writing!");
    }
    return;
}

//Reads regions from file and pushes the specified number into a vector of regions
void putRegionsInVector(ifstream &fin, RegionsVector &regions, const int numRegions)
{
    for(int i=0; i < numRegions; ++i)
    {
        regions.push_back(readSingleRegion(fin));
    }
}

//Gets the data of a single region from the testDataFile (reasonably safely). 
L1GctRegion readSingleRegion(ifstream &fin)
{   
    //Represents how many numbers there are per line for a region in the input file
    const int numRegionComponents = 7; //the phi/eta co-ords, Et, Mip, Quiet, tauVeto & overFlow.
    
    ULong regionComponents[numRegionComponents];
    
    for(int i=0; i < numRegionComponents; ++i)
    {
        //check to see if the input stream is still ok first
        if(fin.eof() || fin.bad())
        {
           throw std::runtime_error("Error reading region data from " + testDataFile + "!");
        }
        else
        {
            fin >> regionComponents[i];  //read in the components.
        }
    }
    
    //return object
    L1GctRegion tempRegion(regionComponents[0],
                           regionComponents[1],
                           regionComponents[2],
                           static_cast<bool>(regionComponents[3]),
                           static_cast<bool>(regionComponents[4]),
                           static_cast<bool>(regionComponents[5]),
                           static_cast<bool>(regionComponents[6]));
    
    return tempRegion;
}

//Reads jets from file and pushes the specified number into a vector of jets
void putJetsInVector(ifstream &fin, JetsVector &jets, const int numJets)
{
    for(int i=0; i < numJets; ++i)
    {
        jets.push_back(readSingleJet(fin));
    }
}

//Gets the data of a single jet from the testDataFile (reasonably safely). 
L1GctJet readSingleJet(ifstream &fin)
{
    //This reperesents how many numbers there are per line for a jet in the input file
    const int numJetComponents = 4; //4 since we have rank, eta, phi & tauVeto.
    
    ULong jetComponents[numJetComponents];

    //read in the data from the file
    for(int i=0; i < numJetComponents; ++i)
    {
        //check to see if the input stream is still ok first
        if(fin.eof() || fin.bad())
        {
           throw std::runtime_error("Error reading jet data from " + testDataFile + "!");
        }
        else
        {
            fin >> jetComponents[i];  //read in the components.
        }
    }
   
    //return object
    L1GctJet tempJet(jetComponents[0], jetComponents[1],
                     jetComponents[2], static_cast<bool>(jetComponents[3]));

    return tempJet;
}

// Compares RegionsVectors, prints a message about the comparison, returns true if identical, else false.
bool compareRegionsVectors(RegionsVector &vector1, RegionsVector &vector2, const string description)
{
    bool testPass = true;
    
    if(vector1.size() != vector2.size())  //First check overall size is the same 
    {
        testPass = false;
    }
    else
    {
        if(!vector1.empty())  //make sure it isn't empty
        {
            //compare the vectors
            for(ULong i = 0; i < vector1.size(); ++i)
            {
                if(vector1[i].getEta() != vector2[i].getEta()) { testPass = false; break; }
                if(vector1[i].getPhi() != vector2[i].getPhi()) { testPass = false; break; }
                if(vector1[i].getEt() != vector2[i].getEt()) { testPass = false; break; }
                if(vector1[i].getMip() != vector2[i].getMip()) { testPass = false; break; }
                if(vector1[i].getQuiet() != vector2[i].getQuiet()) {testPass = false; break; }
                if(vector1[i].getTauVeto() != vector2[i].getTauVeto()) {testPass = false; break; }
                if(vector1[i].getOverFlow() != vector2[i].getOverFlow()) {testPass = false; break; }
            }
        }
    }
        
    //Print results to screen
    if(testPass == false)
    {
        cout << "\nTest class has FAILED " << description << "comparison!" << endl;
        return false;
    }
    else
    {
        cout << "\nTest class has passed " << description << " comparison." << endl;
    }
    return true;
}

// Compares JetsVectors, prints a message about the comparison, returns true if identical, else false.
bool compareJetsVectors(JetsVector &vector1, JetsVector &vector2, const string description)
{
    bool testPass = true;
    
    if(vector1.size() != vector2.size())  //First check overall size is the same
    {
        testPass = false;
    }
    else
    {
        if (!vector1.empty())  //Make sure it isn't empty
        {
            //compare the vectors
            for(unsigned int i = 0; i < vector1.size(); ++i)
            {
                if(vector1[i].getRank() != vector2[i].getRank()) { testPass = false; break; }
                if(vector1[i].getEta() != vector2[i].getEta()) { testPass = false; break; }
                if(vector1[i].getPhi() != vector2[i].getPhi()) { testPass = false; break; }
                if(vector1[i].getTauVeto() != vector2[i].getTauVeto()) { testPass = false; break; }
            }
        }
    }
    
    //Print results to screen
    if(testPass == false)
    {
        cout << "\nTest class has FAILED " << description << " comparison!" << endl;
        return false;
    }
    else
    {
        cout << "\nTest class has passed " << description << " comparison." << endl;
    }
    return true;
}

// Writes out the entire contents of a RegionsVector to the given file output stream
void outputRegionsVector(ofstream &fout, RegionsVector &regions, string description)
{
    fout << description << endl; //brief description of the RegionsVector content
    
    if(!regions.empty())  //check it isn't an empty vector
    {
        for (unsigned int i=0; i < regions.size(); ++i)
        {
            fout << regions[i].getEta() << "\t"
                 << regions[i].getPhi() << "\t"
                 << regions[i].getEt() << "\t"
                 << regions[i].getMip() << "\t"
                 << regions[i].getQuiet() << "\t"
                 << regions[i].getTauVeto() << "\t"
                 << regions[i].getOverFlow() << endl;
        }
    }
    fout << endl;  //write a blank line to separate data
}

// Writes out the entire contents of a JetsVector to the given file output stream
void outputJetsVector(ofstream &fout, JetsVector &jets, string description)
{
    fout << description << endl; //brief description for each JetsVector content
    
    if(!jets.empty())  //check it isn't an empty vector
    {
        for(unsigned int i=0; i < jets.size(); ++i)
        {
            fout << jets[i].getRank() << "\t" 
                 << jets[i].getEta()  << "\t"
                 << jets[i].getPhi()  << "\t"
                 << jets[i].getTauVeto() << endl;
        }
    }
    fout << endl;  //write a blank line to separate data
}
