#include <string.h>
#include "TChain.h"
#include "TFile.h"
#include "TH1.h"
#include "TTree.h"
#include "TKey.h"
#include "Riostream.h"

TList *FileList;
TFile *Target;
vector<string> ExcludedHistosNames;

void MergeRootfile( TDirectory *target, TList *sourcelist );


void hadd() {
  // in an interactive ROOT session, edit the file names
  // Target and FileList, then
  // root > .L hadd.C
  // root > hadd()
   
  Target = TFile::Open( "result.root", "RECREATE" );
  
  FileList = new TList();
  FileList->Add( TFile::Open("hsimple1.root") );
  FileList->Add( TFile::Open("hsimple2.root") );
  
  MergeRootfile( Target, FileList );

}   

void MergeRootfile( TDirectory *target, TList *sourcelist ) {

  //  cout << "Target path: " << target->GetPath() << endl;
  TString path( (char*)strstr( target->GetPath(), ":" ) );
  path.Remove( 0, 2 );

  TFile *first_source = (TFile*)sourcelist->First();
  first_source->cd( path );
  TDirectory *current_sourcedir = gDirectory;

  // loop over all keys in this directory
  TChain *globChain = 0;
  TIter nextkey( current_sourcedir->GetListOfKeys() );
  TKey *key;
  while ( (key = (TKey*)nextkey())) {

    // read object from first source file
    first_source->cd( path );
    TObject *obj = key->ReadObj();

    if ( obj->IsA()->InheritsFrom( "TH1" ) ) {
      // descendant of TH1 -> merge it

      //cout << "Merging histogram " << obj->GetName() << endl;
      TH1 *h1 = (TH1*)obj;
      
      bool DoMerge=true;
      for (size_t i=0;i<ExcludedHistosNames.size();++i){
	//cout << h1->GetTitle() << " " << ExcludedHistosNames[i].c_str() << endl;
	if (strstr(h1->GetTitle(),ExcludedHistosNames[i].c_str())!=NULL){
	  std::cout << "skipping merge for histo " << h1->GetTitle() << endl;
	  DoMerge=false;
	  break;
	}
      }
      
      // loop over all source files and add the content of the
      // correspondant histogram to the one pointed to by "h1"
      if (DoMerge){
	
	//std::cout << "merge for histo " << h1->GetTitle() << endl;

	TFile *nextsource = (TFile*)sourcelist->After( first_source );
	while ( nextsource ) {
	  
	  // make sure we are at the correct directory level by cd'ing to path
	  nextsource->cd( path );
	  TH1 *h2 = (TH1*)gDirectory->Get( h1->GetName() );
	  if ( h2 ) {
	    h1->Add( h2 );
	    delete h2; // don't know if this is necessary, i.e. if 
	    // h2 is created by the call to gDirectory above.
	  }
	  
	  nextsource = (TFile*)sourcelist->After( nextsource );
	}
      }
    }
    else if ( obj->IsA()->InheritsFrom( "TTree" ) ) {
      
      // loop over all source files create a chain of Trees "globChain"
      const char* obj_name= obj->GetName();

      globChain = new TChain(obj_name);
      globChain->Add(first_source->GetName());
      TFile *nextsource = (TFile*)sourcelist->After( first_source );
      //      const char* file_name = nextsource->GetName();
      // cout << "file name  " << file_name << endl;
      while ( nextsource ) {
          
	globChain->Add(nextsource->GetName());
	nextsource = (TFile*)sourcelist->After( nextsource );
      }

    } else if ( obj->IsA()->InheritsFrom( "TDirectory" ) ) {
      // it's a subdirectory

      cout << "Found subdirectory " << obj->GetName() << endl;

      // create a new subdir of same name and title in the target file
      target->cd();
      TDirectory *newdir = target->mkdir( obj->GetName(), obj->GetTitle() );

      // newdir is now the starting point of another round of merging
      // newdir still knows its depth within the target file via
      // GetPath(), so we can still figure out where we are in the recursion
      MergeRootfile( newdir, sourcelist );

    } else {

      // object is of no type that we know or can handle
      cout << "Unknown object type, name: " 
           << obj->GetName() << " title: " << obj->GetTitle() << endl;
    }

    // now write the merged histogram (which is "in" obj) to the target file
    // note that this will just store obj in the current directory level,
    // which is not persistent until the complete directory itself is stored
    // by "target->Write()" below
    if ( obj ) {
      target->cd();

      //!!if the object is a tree, it is stored in globChain...
      if(obj->IsA()->InheritsFrom( "TTree" ))
	globChain->Write( key->GetName() );
      else{
        obj->Write( key->GetName() );
	//cout << "writing object " << obj->GetTitle() << endl;
      }    
    }

  } // while ( ( TKey *key = (TKey*)nextkey() ) )

  // save modifications to target file
  target->Write();

}


// Class to create the TList and pass it to the merger
void AddHisto( const char *pcFILE_IN,
	       const char *jobList,
	       char* ExcludedHistosList="") {

  FileList = new TList();
  TString FileName;

  cout << "jobList " << jobList << endl;
  
  char * jobNum = std::strtok(jobList,"-");
  std::cout << "jobNum = " << jobNum << std::endl;
  while ( jobNum != NULL ) {
    std::cout << "Chaining file: " << pcFILE_IN << jobNum << std::endl;
//    FileName = *pcFILE_IN + "_" + *jobNum + ".root";
    FileName=pcFILE_IN;
    FileName+="_";
    FileName+=jobNum;
    FileName+=".root";
    std::cout << "FileName = " << FileName << std::endl;
    FileList->Add( TFile::Open( FileName , "READ" ) );
    jobNum = std::strtok(NULL,"-");
  }

  TString NameOut=pcFILE_IN;
  NameOut+=".root";

  Target = TFile::Open( NameOut, "RECREATE" );


  char *ptr=strtok(ExcludedHistosList," ");
  while (ptr!=NULL){
    ExcludedHistosNames.push_back(string(ptr));
    ptr=strtok(NULL," ");
  }

  
  for (size_t i=0;i<ExcludedHistosNames.size();++i)
    cout << "Exclude from merge Histo" << ExcludedHistosNames[i] << endl;
  
  MergeRootfile( Target, FileList );
};
