
double             IntegratedLuminosity = 0.0084;

struct stSignal{
   string Type;
   string Name;
   string Legend;
   double Mass;
   double XSec;

   stSignal(); 
   stSignal(string Type_, string Name_, string Legend_, double Mass_, double XSec_){Type=Type_; Name=Name_; Legend=Legend_; Mass=Mass_; XSec=XSec_;}
};

void GetSignalDefinition(std::vector<stSignal>& signals){
//   signals.push_back(stSignal("Gluino", "Gluino200", "#tilde{g} 200"       , 200, 326.500000) ); //LO
//   signals.push_back(stSignal("Gluino", "Gluino300", "#tilde{g} 300"       , 300,  27.690000) ); //LO
//   signals.push_back(stSignal("Gluino", "Gluino400", "#tilde{g} 400"       , 400,   3.910000) ); //LO
//   signals.push_back(stSignal("Gluino", "Gluino500", "#tilde{g} 500"       , 500,   0.754600) ); //LO
//   signals.push_back(stSignal("Gluino", "Gluino600", "#tilde{g} 600"       , 600,   0.170700) ); //LO
//   signals.push_back(stSignal("Gluino", "Gluino900", "#tilde{g} 900"       , 900,   0.003942) ); //LO
   signals.push_back(stSignal("Gluino", "Gluino200", "#tilde{g} 200"       , 200, 606.000000) ); //NLO
   signals.push_back(stSignal("Gluino", "Gluino300", "#tilde{g} 300"       , 300,  57.200000) ); //NLO
   signals.push_back(stSignal("Gluino", "Gluino400", "#tilde{g} 400"       , 400,   8.980000) ); //NLO
   signals.push_back(stSignal("Gluino", "Gluino500", "#tilde{g} 500"       , 500,   1.870000) ); //NLO
   signals.push_back(stSignal("Gluino", "Gluino600", "#tilde{g} 600"       , 600,   0.465000) ); //NLO
   signals.push_back(stSignal("Gluino", "Gluino900", "#tilde{g} 900"       , 900,   0.012800) ); //NLO
//   signals.push_back(stSignal("Stop"  , "Stop130"  , "#tilde{t}_{1} 130"   , 130,  65.540000) ); //LO
//   signals.push_back(stSignal("Stop"  , "Stop200"  , "#tilde{t}_{1} 200"   , 200,   6.832000) ); //LO
//   signals.push_back(stSignal("Stop"  , "Stop300"  , "#tilde{t}_{1} 300"   , 300,   0.647800) ); //LO
//   signals.push_back(stSignal("Stop"  , "Stop500"  , "#tilde{t}_{1} 500"   , 500,   0.022920) ); //LO
//   signals.push_back(stSignal("Stop"  , "Stop800"  , "#tilde{t}_{1} 800"   , 800,   0.000542) ); //LO
//   signals.push_back(stSignal("MGStop", "MGStop130", "#tilde{t}_{1} 130"   , 130,  73.270000) ); //LO
//   signals.push_back(stSignal("MGStop", "MGStop200", "#tilde{t}_{1} 200"   , 200,   7.65100)  ); //LO
//   signals.push_back(stSignal("MGStop", "MGStop300", "#tilde{t}_{1} 300"   , 300,   0.754000) ); //LO
//   signals.push_back(stSignal("MGStop", "MGStop500", "#tilde{t}_{1} 500"   , 500,   0.026490) ); //LO
//   signals.push_back(stSignal("MGStop", "MGStop800", "#tilde{t}_{1} 800"   , 800,   0.000621) ); //LO
   signals.push_back(stSignal("Stop"  , "Stop130"  , "#tilde{t}_{1} 130"   , 130,  65.540000*1.4927) ); //NLO
   signals.push_back(stSignal("Stop"  , "Stop200"  , "#tilde{t}_{1} 200"   , 200,   6.832000*1.5623) ); //NLO
   signals.push_back(stSignal("Stop"  , "Stop300"  , "#tilde{t}_{1} 300"   , 300,   0.647800*1.6378) ); //NLO
   signals.push_back(stSignal("Stop"  , "Stop500"  , "#tilde{t}_{1} 500"   , 500,   0.022920*1.7654) ); //NLO
   signals.push_back(stSignal("Stop"  , "Stop800"  , "#tilde{t}_{1} 800"   , 800,   0.000542*1.9887) ); //NLO
   signals.push_back(stSignal("MGStop", "MGStop130", "#tilde{t}_{1} 130"   , 130,  73.270000*1.4927) ); //NLO
   signals.push_back(stSignal("MGStop", "MGStop200", "#tilde{t}_{1} 200"   , 200,   7.651000*1.5623) ); //NLO
   signals.push_back(stSignal("MGStop", "MGStop300", "#tilde{t}_{1} 300"   , 300,   0.754000*1.6378) ); //NLO
   signals.push_back(stSignal("MGStop", "MGStop500", "#tilde{t}_{1} 500"   , 500,   0.026490*1.7654) ); //NLO
   signals.push_back(stSignal("MGStop", "MGStop800", "#tilde{t}_{1} 800"   , 800,   0.000621*1.9887) ); //NLO
   signals.push_back(stSignal("Stau"  , "Stau100"  , "#tilde{#tau}_{1} 100", 100,   1.326000) ); //LO
   signals.push_back(stSignal("Stau"  , "Stau126"  , "#tilde{#tau}_{1} 126", 126,   0.330000) ); //LO
   signals.push_back(stSignal("Stau"  , "Stau156"  , "#tilde{#tau}_{1} 156", 156,   0.105000) ); //LO
   signals.push_back(stSignal("Stau"  , "Stau200"  , "#tilde{#tau}_{1} 200", 200,   0.025000) ); //LO
   signals.push_back(stSignal("Stau"  , "Stau247"  , "#tilde{#tau}_{1} 247", 247,   0.008000) ); //LO
   signals.push_back(stSignal("Stau"  , "Stau308"  , "#tilde{#tau}_{1} 308", 308,   0.002000) ); //LO
}

void GetInputFiles(std::vector<string>& inputFiles, string SampleName){
   string BaseDirectory = "/storage/data/cms/users/quertenmont/HSCP/CMSSW_3_5_8_patch4/";
   switch(rand()%3){
      case  0: BaseDirectory += "Set1/"; break;
      case  1: BaseDirectory += "Set2/"; break;
      case  2: BaseDirectory += "Set3/"; break;
      default: BaseDirectory += "Set1/"; break;           
   }
   std::cout<<"BASE DATASET DIRECTORY FIXED TO: " << BaseDirectory << " For Sample " << SampleName << endl;

   if(SampleName=="Data"){
      inputFiles.push_back(BaseDirectory + "Data.root");
   }else if(SampleName=="MC"){
      inputFiles.push_back(BaseDirectory + "MC.root");
   }else if(SampleName=="Gluino200"){
      inputFiles.push_back(BaseDirectory + "Gluino200.root");
   }else if(SampleName=="Gluino300"){
      inputFiles.push_back(BaseDirectory + "Gluino300.root");
   }else if(SampleName=="Gluino400"){
      inputFiles.push_back(BaseDirectory + "Gluino400.root");
   }else if(SampleName=="Gluino500"){
      inputFiles.push_back(BaseDirectory + "Gluino500.root");
   }else if(SampleName=="Gluino600"){
      inputFiles.push_back(BaseDirectory + "Gluino600.root");
   }else if(SampleName=="Gluino900"){
      inputFiles.push_back(BaseDirectory + "Gluino900.root");
   }else if(SampleName=="MGStop130"){
      inputFiles.push_back(BaseDirectory + "MGStop130.root");
   }else if(SampleName=="MGStop200"){
      inputFiles.push_back(BaseDirectory + "MGStop200.root");
   }else if(SampleName=="MGStop300"){
      inputFiles.push_back(BaseDirectory + "MGStop300.root");
   }else if(SampleName=="MGStop500"){
      inputFiles.push_back(BaseDirectory + "MGStop500.root");
   }else if(SampleName=="MGStop800"){
      inputFiles.push_back(BaseDirectory + "MGStop800.root");
   }else if(SampleName=="Stau100"){
      inputFiles.push_back(BaseDirectory + "Stau100.root");
   }else if(SampleName=="Stau126"){
      inputFiles.push_back(BaseDirectory + "Stau126.root");
   }else if(SampleName=="Stau156"){
      inputFiles.push_back(BaseDirectory + "Stau156.root");
   }else if(SampleName=="Stau200"){
      inputFiles.push_back(BaseDirectory + "Stau200.root");
   }else if(SampleName=="Stau247"){
      inputFiles.push_back(BaseDirectory + "Stau247.root");
   }else if(SampleName=="Stau308"){
      inputFiles.push_back(BaseDirectory + "Stau308.root");
   }else if(SampleName=="Stop130"){
      inputFiles.push_back(BaseDirectory + "Stop130.root");
   }else if(SampleName=="Stop200"){
      inputFiles.push_back(BaseDirectory + "Stop200.root");
   }else if(SampleName=="Stop300"){
      inputFiles.push_back(BaseDirectory + "Stop300.root");
   }else if(SampleName=="Stop500"){
      inputFiles.push_back(BaseDirectory + "Stop500.root");
   }else if(SampleName=="Stop800"){
      inputFiles.push_back(BaseDirectory + "Stop800.root");
   }else{
      printf("\n\n\n!!!UNKOWN SAMPLE:%s!!!\n\n\n",SampleName.c_str());
   }

}
