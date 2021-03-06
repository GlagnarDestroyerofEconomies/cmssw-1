class Comparator3 {

public:

  enum Mode {
    NORMAL,
    SCALE,
    EFF
  };


  Comparator3() : rebin_(-1), xMin_(0), xMax_(0), resetAxis_(false), 
		 s0_(0), s1_(0),s2_(0), legend_(0,0,1,1) {}

  Comparator3( const char* file0,
	      const char* dir0,
	      const char* file1,
	      const char* dir1, 
              const char* file2,
              const char* dir2 ) :

    rebin_(-1), xMin_(0), xMax_(0), resetAxis_(false), 
    s0_(0), s1_(0), s2_(0), legend_(0,0,1,1) {
    
    SetDirs( file0, dir0, file1, dir1, file2, dir2);
  }
  
  void SetDirs( const char* file0,
		const char* dir0,
		const char* file1,
		const char* dir1,
		const char* file2,   
                const char* dir2 ) {

    file0_ = new TFile( file0 );
    if( file0_->IsZombie() ) exit(1);
    dir0_ = file0_->GetDirectory( dir0 );
    if(! dir0_ ) exit(1);
    
    file1_ = new TFile( file1 );
    if( file1_->IsZombie() ) exit(1);
    dir1_ = file1_->GetDirectory( dir1 );
    if(! dir1_ ) exit(1);

    file2_ = new TFile( file2 );  
    if( file2_->IsZombie() ) exit(1);
    dir2_ = file2_->GetDirectory( dir2 );
    if(! dir2_ ) exit(1);


  }

  // set the rebinning factor and the range
  void SetAxis( int rebin,
		float xmin, 
		float xmax) {
    rebin_ = rebin;
    xMin_ = xmin;
    xMax_ = xmax;
    resetAxis_ = true;
  }
  
  // set the rebinning factor, unset the range
  void SetAxis( int rebin ) {
    rebin_ = rebin;
    resetAxis_ = false;
  }
  
  // draws a Y projection of a slice along X
  void DrawSlice( const char* key, 
		  int binxmin, int binxmax, 
		  Mode mode ) {
    
    static int num = 0;
    
    ostrstream out0;
    out0<<"h0_2d_"<<num;
    ostrstream out1;
    out1<<"Lx4322d_"<<num;
    ostrstream out2;
    out2<<"Lx4322d_"<<num;
    num++;
    
    

    string name0 = out0.str();
    string name1 = out1.str();
    string name1 = out2.str();  

    TH1* h0 = Histo( key, 0);
    TH1* h1 = Histo( key, 1);
    TH1* h2 = Histo( key, 2);


    TH2* Lx3452d = dynamic_cast< TH2* >(h0);
    TH2* Lx4322d = dynamic_cast< TH2* >(h1);
    TH2* Lx64_4322d = dynamic_cast< TH2* >(h2);
    
    if(Lx3452d->GetNbinsY() == 1 || 
       Lx4322d->GetNbinsY() == 1 )||
       Lx64_4322d->GetNbinsY() == 1 ) {
      cerr<<key<<" is not 2D"<<endl;
      return;
    }
    
    TH1::AddDirectory( false );

    TH1D* Lx345slice = Lx3452d->ProjectionY(name0.c_str(),
					binxmin, binxmax, "");
    TH1D* Lx432slice = Lx4322d->ProjectionY(name1.c_str(),
					binxmin, binxmax, "");
    TH1D* Lx64_432slice = Lx64_4322d->ProjectionY(name2.c_str(),
                                        binxmin, binxmax, "");   

    TH1::AddDirectory( true );
    Draw( Lx345slice, Lx432slice, Lx64_432slice, mode);        
  }


  void Draw( const char* key, Mode mode) {

    TH1::AddDirectory( false );
    TH1* h0 = Histo( key, 0);
    TH1* h1 = Histo( key, 1)->Clone("h1");
    TH1* h2 = Histo( key, 2)->Clone("h2");     

    int Lx345entries = h0->GetEntries();
    int Lx432entries = h1->GetEntries();
    int Lx64_432entries = h2->GetEntries();

    cout << " Lx345entries =" << Lx345entries << " Lx432entries =" <<  Lx432entries << " Lx64_432entries =" <<  Lx64_432entries << endl;
//    gStyle->SetStatColor( (Lx345entries == Lx432entries == Lx64_432entries) ? 3 : 2); 
 
 //   double pv = h1->KolmogorovTest(h0,"OU");
 //   gStyle->SetStatColor(pv > 0.1 ? 3 : 2);
    TH1::AddDirectory( true );
    Draw( h0, h1, h2, mode);    
  }

  
  void Draw( const char* key0, const char* key1, const char* key2, Mode mode) {
    TH1* h0 = Histo( key0, 0);
    TH1* h1 = Histo( key1, 1);
    TH1* h2 = Histo( key2, 2); 
    
    Draw( h0, h1, h2, mode);
  }

  // cd to a give path
  void cd(const char* path ) {
    path_ = path;
  }
  
  // return the three temporary 1d histograms, that have just
  // been plotted
  TH1* h0() {return Lx345;}
  TH1* h1() {return Lx432;}
  TH1* h2() {return Lx64_432;} 

  const TLegend& Legend() {return legend_;}
 
 
  // set the styles for further plots
  void SetStyles( Style* s1, Style* s2, Style* s3,
		  const char* leg0,
		  const char* leg1, 
		  const char* leg2){ 
    s0_ = s1; 
    s1_ = s2;
    s2_ = s3;
    
    legend_.Clear();
    legend_.AddEntry( s0_, leg0, "mlf");
    legend_.AddEntry( s1_, leg1, "mlf");
    legend_.AddEntry( s2_, leg2, "mlf");
  }
  
private:

  // retrieve an histogram in one of the two directories
  TH1* Histo( const char* key, unsigned dirIndex) {
    if(dirIndex<0 || dirIndex>2) { 
      cerr<<"bad dir index: "<<dirIndex<<endl;
      return 0;
    }
    TDirectory* dir;
    if(dirIndex == 0) dir = dir0_;
    if(dirIndex == 1) dir = dir1_;
    if(dirIndex == 2) dir = dir2_;
    
    dir->cd();

    TH1* h = (TH1*) dir->Get(key);
    if(!h)  
      cerr<<"no key "<<key<<" in directory "<<dir->GetName()<<endl;
    return h;
  }

  // draw 2 1D histograms.
  // the histograms can be normalized to the same number of entries, 
  // or plotted as a ratio.
  void Draw( TH1* h0, TH1* h1, TH1* h2, Mode mode ) {
    if( !(h0 && h1 && h2) ) { 
      cerr<<"invalid histo"<<endl;
      return;
    }
    
    TH1::AddDirectory( false );
    Lx345 = (TH1*) h0->Clone( "Lx345");
    Lx432 = (TH1*) h1->Clone( "Lx432");
    Lx64_432 = (TH1*) h2->Clone( "Lx64_432");

    TH1::AddDirectory( true );
    
    // unsetting the title, since the title of projections
    // is still the title of the 2d histo
    // and this is better anyway
    Lx345->SetTitle("");
    Lx432->SetTitle("");    
    Lx64_432->SetTitle("");

    Lx345->SetStats(1);
    Lx432->SetStats(1);
    Lx64_432->SetStats(1);

    if(rebin_>1) {
      Lx345->Rebin( rebin_);
      Lx432->Rebin( rebin_);
      Lx64_432->Rebin( rebin_);
    }
    if(resetAxis_) {
      Lx345->GetXaxis()->SetRangeUser( xMin_, xMax_);
      Lx432->GetXaxis()->SetRangeUser( xMin_, xMax_);
      Lx64_432->GetXaxis()->SetRangeUser( xMin_, xMax_);
    }

    switch(mode) {
    case SCALE:
      Lx432->Scale( Lx345->GetEntries()/Lx432->GetEntries() );
    case NORMAL:
      if(s0_)
	FormatHisto( Lx345 , s0_);
      if(s1_)
 	FormatHisto( Lx432 , s1_);
      if(s2_)
        FormatHisto( Lx64_432 , s2_);

      gStyle->SetStatColor(2);
      Lx345->Draw();
      gPad->Update();
      TPaveStats *st = (TPaveStats*)Lx345 ->FindObject("stats");
      st->SetY1NDC(0.52);
      st->SetY2NDC(0.68);
      gStyle->SetStatColor(3);
      Lx432->Draw("sames");
      gPad->Update();
      TPaveStats *st = (TPaveStats*)Lx432 ->FindObject("stats");
      st->SetY1NDC(0.68);
      st->SetY2NDC(0.84);
      gStyle->SetStatColor(4);
      Lx64_432->Draw("sames");
      break;
    case EFF:
      Lx432->Divide( Lx345 );
      if(s1_)
 	FormatHisto( Lx432 , s0_);
      Lx432->Draw();
    default:
      break;
    }
  }

  int rebin_;
  float xMin_;
  float xMax_;
  bool resetAxis_;

  TFile*      file0_;
  TDirectory* dir0_;
  TFile*      file1_;
  TDirectory* dir1_;
  TFile*      file2_;   
  TDirectory* dir2_; 
  
  TH1* Lx345;
  TH1* Lx432;
  TH1* Lx64_432; 
  
  Style* s0_;
  Style* s1_;
  Style* s2_;
  
  TLegend legend_;

  string path_;
};

