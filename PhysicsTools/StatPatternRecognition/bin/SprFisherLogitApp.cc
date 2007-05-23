//$Id: SprFisherLogitApp.cc,v 1.5 2007/02/05 21:49:45 narsky Exp $

#include "PhysicsTools/StatPatternRecognition/interface/SprExperiment.hh"
#include "PhysicsTools/StatPatternRecognition/interface/SprFisher.hh"
#include "PhysicsTools/StatPatternRecognition/interface/SprLogitR.hh"
#include "PhysicsTools/StatPatternRecognition/interface/SprData.hh"
#include "PhysicsTools/StatPatternRecognition/interface/SprEmptyFilter.hh"
#include "PhysicsTools/StatPatternRecognition/interface/SprAbsReader.hh"
#include "PhysicsTools/StatPatternRecognition/interface/SprSimpleReader.hh"
#include "PhysicsTools/StatPatternRecognition/interface/SprDataFeeder.hh"
#include "PhysicsTools/StatPatternRecognition/interface/SprMyWriter.hh"
#include "PhysicsTools/StatPatternRecognition/interface/SprTrainedFisher.hh"
#include "PhysicsTools/StatPatternRecognition/interface/SprTrainedLogitR.hh"
#include "PhysicsTools/StatPatternRecognition/interface/SprStringParser.hh"
#include "PhysicsTools/StatPatternRecognition/interface/SprClass.hh"

#include "PhysicsTools/StatPatternRecognition/src/SprVector.hh"

#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <memory>
#include <cassert>

using namespace std;


void help(const char* prog) 
{
  cout << "Usage:  " << prog << " training_data_file" << endl;
  cout << "\t Options: " << endl;
  cout << "\t-h --- help                                        " << endl;
  cout << "\t-m order of Fisher                                 " << endl;
  cout << "\t\t 1 = linear                                      " << endl;
  cout << "\t\t 2 = quadratic                                   " << endl;
  cout << "\t\t 3 = both                                        " << endl;
  cout << "\t-l use logistic regression                         " << endl;
  cout << "\t-e accuracy for logistic regression (default=0.001)" << endl;
  cout << "\t-u update factor for logistic regression (default=1)"<< endl;
  cout << "\t-i initialize logistic regression coeffs to 0 (def=LDA output)"
       << endl;
  cout << "\t-y list of input classes (see SprAbsFilter.hh)     " << endl;
  cout << "\t-o output Tuple file                               " << endl;
  cout << "\t-s use standard output ranging from -infty to +infty"<< endl;
  cout << "\t-a input ascii file mode (see SprSimpleReader.hh)  " << endl;
  cout << "\t-v verbose level (0=silent default,1,2)            " << endl;
  cout << "\t-f store classifier configuration to file          " << endl;
  cout << "\t-t read validation/test data from a file           " << endl;
  cout << "\t\t (must be in same format as input data!!!        " << endl;
  cout << "\t-p output file to store validation/test data       " << endl;
  cout << "\t-w scale all signal weights by this factor         " << endl;
  cout << "\t-V include only these input variables              " << endl;
  cout << "\t-z exclude input variables from the list           " << endl;
  cout << "\t\t Variables must be listed in quotes and separated by commas." 
       << endl;
}


int main(int argc, char ** argv)
{
  // check command line
  if( argc < 2 ) {
    help(argv[0]);
    return 1;
  }

  // init
  int fisherMode = 0;
  bool useLogit = false;
  double eps = 0.001;
  double updateFactor = 1;
  bool initToZero = false;
  string hbkFile;
  int readMode = 1;
  int verbose = 0;
  string outFile;
  string valFile;
  string valHbkFile;
  bool scaleWeights = false;
  double sW = 1.;
  string includeList, excludeList;
  string inputClassesString;
  bool useStandard = false;
  
  // decode command line
  int c;
  extern char* optarg;
  extern int optind;
  while( (c = getopt(argc,argv,"hm:le:u:iy:o:sa:v:f:t:p:w:V:z:")) != EOF ) {
    switch( c )
      {
      case 'h' :
	help(argv[0]);
	return 1;
      case 'm' :
	fisherMode = (optarg==0 ? 1 : atoi(optarg));
	break;
      case 'l' :
	useLogit = true;
	break;
      case 'e' :
	eps = (optarg==0 ? 0.001 : atof(optarg));
	break;
      case 'u' :
	updateFactor = (optarg==0 ? 1. : atof(optarg));
	break;
      case 'i' :
	initToZero = true;
	break;
      case 'y' :
	inputClassesString = optarg;
	break;
      case 'o' :
	hbkFile = optarg;
	break;
      case 's' :
	useStandard = true;
	break;
      case 'a' :
	readMode = (optarg==0 ? 1 : atoi(optarg));
	break;
      case 'v' :
	verbose = (optarg==0 ? 0 : atoi(optarg));
	break;
      case 'f' :
	outFile = optarg;
	break;
      case 't' :
	valFile = optarg;
	break;
      case 'p' :
	valHbkFile = optarg;
	break;
      case 'w' :
	if( optarg != 0 ) {
	  scaleWeights = true;
	  sW = atof(optarg);
	}
	break;
      case 'V' :
	includeList = optarg;
	break;
      case 'z' :
	excludeList = optarg;
	break;
      }
  }

  // training file name must be the only argument that appears
  // after all options on the command line
  string trFile;
  if( optind == argc-1 )
    trFile = argv[optind];
  if( trFile.empty() ) {
    cerr << "No training file is specified." << endl;
    return 1;
  }

  // sanity check
  if( fisherMode==0 && !useLogit ) {
    cerr << "Neither Fisher nor logistic regression is requested." << endl;
    return 1;
  }

  // make reader
  SprSimpleReader reader(readMode);

  // include variables
  set<string> includeSet;
  if( !includeList.empty() ) {
    vector<vector<string> > includeVars;
    SprStringParser::parseToStrings(includeList.c_str(),includeVars);
    assert( !includeVars.empty() );
    for( int i=0;i<includeVars[0].size();i++ ) 
      includeSet.insert(includeVars[0][i]);
    if( !reader.chooseVars(includeSet) ) {
      cerr << "Unable to include variables in training set." << endl;
      return 2;
    }
    else {
      cout << "Folowing variables have been included in optimization: ";
      for( set<string>::const_iterator 
	     i=includeSet.begin();i!=includeSet.end();i++ )
	cout << "\"" << *i << "\"" << " ";
      cout << endl;
    }
  }

  // exclude variables
  set<string> excludeSet;
  if( !excludeList.empty() ) {
    vector<vector<string> > excludeVars;
    SprStringParser::parseToStrings(excludeList.c_str(),excludeVars);
    assert( !excludeVars.empty() );
    for( int i=0;i<excludeVars[0].size();i++ ) 
      excludeSet.insert(excludeVars[0][i]);
    if( !reader.chooseAllBut(excludeSet) ) {
      cerr << "Unable to exclude variables from training set." << endl;
      return 2;
    }
    else {
      cout << "Folowing variables have been excluded from optimization: ";
      for( set<string>::const_iterator 
	     i=excludeSet.begin();i!=excludeSet.end();i++ )
	cout << "\"" << *i << "\"" << " ";
      cout << endl;
    }
  }

  // read training data from file
  auto_ptr<SprAbsFilter> filter(reader.read(trFile.c_str()));
  if( filter.get() == 0 ) {
    cerr << "Unable to read data from file " << trFile.c_str() << endl;
    return 2;
  }
  vector<string> vars;
  filter->vars(vars);
  cout << "Read data from file " << trFile.c_str() 
       << " for variables";
  for( int i=0;i<vars.size();i++ ) 
    cout << " \"" << vars[i].c_str() << "\"";
  cout << endl;
  cout << "Total number of points read: " << filter->size() << endl;

  // filter training data by class
  vector<SprClass> inputClasses;
  if( !filter->filterByClass(inputClassesString.c_str()) ) {
    cerr << "Cannot choose input classes for string " 
	 << inputClassesString << endl;
    return 2;
  }
  filter->classes(inputClasses);
  assert( inputClasses.size() > 1 );
  cout << "Training data filtered by class." << endl;
  for( int i=0;i<inputClasses.size();i++ ) {
    cout << "Points in class " << inputClasses[i] << ":   " 
	 << filter->ptsInClass(inputClasses[i]) << endl;
  }

  // scale weights
  if( scaleWeights ) {
    cout << "Signal weights are multiplied by " << sW << endl;
    filter->scaleWeights(inputClasses[1],sW);
  }

  // read validation data from file
  auto_ptr<SprAbsFilter> valFilter;
  if( !valFile.empty() ) {
    SprSimpleReader valReader(readMode);
    if( !includeSet.empty() ) {
      if( !valReader.chooseVars(includeSet) ) {
	cerr << "Unable to include variables in validation set." << endl;
	return 2;
      }
    }
    if( !excludeSet.empty() ) {
      if( !valReader.chooseAllBut(excludeSet) ) {
	cerr << "Unable to exclude variables from validation set." << endl;
	return 2;
      }
    }
    valFilter.reset(valReader.read(valFile.c_str()));
    if( valFilter.get() == 0 ) {
      cerr << "Unable to read data from file " << valFile.c_str() << endl;
      return 2;
    }
    vector<string> valVars;
    valFilter->vars(valVars);
    cout << "Read validation data from file " << valFile.c_str() 
	 << " for variables";
    for( int i=0;i<valVars.size();i++ ) 
      cout << " \"" << valVars[i].c_str() << "\"";
    cout << endl;
    cout << "Total number of points read: " << valFilter->size() << endl;
  }

  // filter validation data by class
  if( valFilter.get() != 0 ) {
    if( !valFilter->filterByClass(inputClassesString.c_str()) ) {
      cerr << "Cannot choose input classes for string " 
	   << inputClassesString << endl;
      return 2;
    }
    valFilter->classes(inputClasses);
    cout << "Validation data filtered by class." << endl;
    for( int i=0;i<inputClasses.size();i++ ) {
      cout << "Points in class " << inputClasses[i] << ":   " 
	   << valFilter->ptsInClass(inputClasses[i]) << endl;
    }
  }

  // scale weights
  if( scaleWeights && valFilter.get()!=0 )
    valFilter->scaleWeights(inputClasses[1],sW);

  // train Fisher
  auto_ptr<SprFisher> fisher;
  auto_ptr<SprTrainedFisher> trainedFisher1, trainedFisher2;
  bool both = false;
  if( fisherMode != 0 ) {
    if( fisherMode!=1 && fisherMode!=2 && fisherMode!=3 ) {
      cerr << "Unknown mode for Fisher " << fisherMode << endl;
      return 3;
    }
    if( fisherMode == 3 ) {
      both = true;
      fisherMode = 1;
    }
    cout << "Initializing Fisher in mode " << fisherMode << endl;
    fisher.reset(new SprFisher(filter.get(),fisherMode));
    if( !fisher->train(verbose) ) {
      cerr << "Unable to train Fisher." << endl;
      return 3;
    }
    else {
      cout << "Trained Fisher:" << endl;
      fisher->print(cout);
    }

    // make a trained Fisher
    trainedFisher1.reset(fisher->makeTrained());
    if( trainedFisher1.get() == 0 ) {
      cerr << "Unable to make a trained Fisher." << endl;
      return 4;
    }
    if( useStandard ) trainedFisher1->useStandard();

    // train another one if necessary
    if( both ) {
      fisher->setMode(2);
      if( !fisher->train(verbose) ) {
	cerr << "Unable to train 2nd Fisher." << endl;
	return 5;
      }
      else {
	cout << "Trained 2nd Fisher:" << endl;
	fisher->print(cout);
      }
      trainedFisher2.reset(fisher->makeTrained());
      if( trainedFisher2.get() == 0 ) {
	cerr << "Unable to make a trained 2nd Fisher." << endl;
	return 6;
      }
      if( useStandard ) trainedFisher2->useStandard();
    }
  }

  // train logistic regression
  auto_ptr<SprLogitR> logit;
  auto_ptr<SprTrainedLogitR> trainedLogit;
  if( useLogit ) {
    // init
    if( initToZero ) {
      SprVector beta(filter->dim());
      for( int i=0;i<filter->dim();i++ ) beta[i] = 0;
      logit.reset(new SprLogitR(filter.get(),0,beta,eps,updateFactor));
    }
    else {
      logit.reset(new SprLogitR(filter.get(),eps,updateFactor));
    }

    // train
    if( !logit->train(verbose) ) {
      cerr << "Unable to train logistic regression." << endl;
      return 7;
    }
    else {
      cout << "Trained Logistic Regression:" << endl;
      logit->print(cout);
    }

    // make trained logit
    trainedLogit.reset(logit->makeTrained());
    if( trainedLogit.get() == 0 ) {
      cerr << "Unable to make trained logistic regression." << endl;
      return 8;
    }
    if( useStandard ) trainedLogit->useStandard();
  }

  // save classifier configuration into file
  if( !outFile.empty() ) {
    if( both || (fisherMode>0 && useLogit) ) {
      cerr << "More than one classifier trained. " 
	   << "Cannot save classifier configurations to file." << endl;
      return 9;
    }
    SprAbsClassifier* trainable = 0;
    if( fisher.get() != 0 ) trainable = fisher.get();
    if( logit.get() != 0 ) trainable = logit.get();
    assert( trainable != 0 );
    if( !trainable->store(outFile.c_str()) ) {
      cerr << "Cannot store classifier in file " << outFile.c_str() << endl;
      return 9;
    }
  }

  // make histogram if requested
  if( hbkFile.empty() && valHbkFile.empty() ) 
    return 0;

  // feed training data
  if( !hbkFile.empty() ) {
    // make a writer
    SprMyWriter hbk("training");
    if( !hbk.init(hbkFile.c_str()) ) {
      cerr << "Unable to open output file " << hbkFile.c_str() << endl;
      return 10;
    }
    string firstClassifier;
    if( trainedFisher2.get()!=0 || fisherMode==1 )
      firstClassifier = "lin";
    else
      firstClassifier = "qua";
    // feed
    SprDataFeeder feeder(filter.get(),&hbk);
    feeder.addClassifier(trainedFisher1.get(),firstClassifier.c_str());
    feeder.addClassifier(trainedFisher2.get(),"qua");
    feeder.addClassifier(trainedLogit.get(),"logit");
    if( !feeder.feed(1000) ) {
      cerr << "Cannot feed data into file " << hbkFile.c_str() << endl;
      return 11;
    }
  }

  // feed validation data
  if( !valHbkFile.empty() ) {
    // make a writer
    SprMyWriter hbk("validation");
    if( !hbk.init(valHbkFile.c_str()) ) {
      cerr << "Unable to open output file " << valHbkFile.c_str() << endl;
      return 12;
    }
    string firstClassifier;
    if( trainedFisher2.get()!=0 || fisherMode==1 )
      firstClassifier = "lin";
    else
      firstClassifier = "qua";
    // feed
    SprDataFeeder feeder(valFilter.get(),&hbk);
    feeder.addClassifier(trainedFisher1.get(),firstClassifier.c_str());
    feeder.addClassifier(trainedFisher2.get(),"qua");
    feeder.addClassifier(trainedLogit.get(),"logit");
    if( !feeder.feed(1000) ) {
      cerr << "Cannot feed data into file " << valHbkFile.c_str() << endl;
      return 13;
    }
  }

  // exit
  return 0;
}
