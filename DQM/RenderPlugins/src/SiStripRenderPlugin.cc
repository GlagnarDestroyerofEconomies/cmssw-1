/*!
  \file SiStripRenderPlugin
  \brief Display Plugin for SiStrip DQM Histograms
  \author S. Dutta
  \version $Revision: 1.11 $
  \date $Date: 2009/05/22 19:09:33 $
*/

#include "VisMonitoring/DQMServer/interface/DQMRenderPlugin.h"
#include "utils.h"

#include "TProfile2D.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TColor.h"
#include "TText.h"
#include <cassert>

class SiStripRenderPlugin : public DQMRenderPlugin
{
public:
  virtual bool applies( const DQMNet::CoreObject &o, const VisDQMImgInfo & )
    {
      if (o.name.find( "SiStrip/" ) == std::string::npos)
        return false;

      if( o.name.find( "/EventInfo/" ) != std::string::npos )
        return true;

      if( o.name.find( "/MechanicalView/" ) != std::string::npos )
        return true;

      if( o.name.find( "/ReadoutView/" ) != std::string::npos )
        return true;

      if( o.name.find( "/Tracks/" ) != std::string::npos )
        return true;

      return false;
    }

  virtual void preDraw( TCanvas *c, const DQMNet::CoreObject &o, const VisDQMImgInfo &, VisDQMRenderInfo & )
    {
      c->cd();

      if( dynamic_cast<TH2F*>( o.object ) )
      {
        preDrawTH2F( c, o );
      }
      else if( dynamic_cast<TH1F*>( o.object ) )
      {
        preDrawTH1F( c, o );
      }
      else if( dynamic_cast<TProfile2D*>( o.object ) )
      {
        preDrawTProfile2D( c, o );
      }
    }

  virtual void postDraw( TCanvas *c, const DQMNet::CoreObject &o, const VisDQMImgInfo & )
    {
      c->cd();

      if( dynamic_cast<TH1F*>( o.object ) )
      {
        postDrawTH1F( c, o );
      }
      if( dynamic_cast<TH2F*>( o.object ) )
      {
        postDrawTH2F( c, o );
      }
    }

private:
  void preDrawTH2F( TCanvas *, const DQMNet::CoreObject &o )
    {
      TH2F* obj = dynamic_cast<TH2F*>( o.object );
      assert( obj );

      // This applies to all
      gStyle->SetCanvasBorderMode( 0 );
      gStyle->SetPadBorderMode( 0 );
      gStyle->SetPadBorderSize( 0 );
      //    (data->pad)->SetLogy( 0 );;
      //  gStyle->SetOptStat( 0 );

      TAxis* xa = obj->GetXaxis();
      TAxis* ya = obj->GetYaxis();

      xa->SetTitleOffset(0.7);
      xa->SetTitleSize(0.05);
      xa->SetLabelSize(0.04);

      ya->SetTitleOffset(0.7);
      ya->SetTitleSize(0.05);
      ya->SetLabelSize(0.04);

      if( o.name.find( "PedsEvolution" ) != std::string::npos)
      {
        gStyle->SetOptStat( 1111 );
        obj->SetStats( kTRUE );
        obj->SetOption( "lego2" );
        return;
      }
      if( o.name.find( "CMDistribution " )  != std::string::npos)
      {
        obj->GetXaxis()->LabelsOption("d");
        obj->SetOption( "lego2" );
        return;
      }
      if( o.name.find( "CMSlopeDistribution " )  != std::string::npos)
      {
        obj->GetXaxis()->LabelsOption("d");
        obj->SetOption( "lego2" );
        return;
      }
      if( o.name.find( "PedestalDistribution " )  != std::string::npos)
      {
        obj->GetXaxis()->LabelsOption("d");
        obj->SetOption( "lego" );
        return;
      }
      if( o.name.find( "reportSummaryMap" )  != std::string::npos)
      {
        obj->SetStats( kFALSE );
        dqm::utils::reportSummaryMapPalette(obj);
        obj->SetOption("colztext");
        return;
      }
      if( o.name.find( "detFractionReportMap" )  != std::string::npos)
      {
        obj->SetStats( kFALSE );
        dqm::utils::reportSummaryMapPalette(obj);
        obj->SetOption("colztext");
        return;
      }
      if( o.name.find( "sToNReportMap" )  != std::string::npos)
      {
        obj->SetStats( kFALSE );
        dqm::utils::reportSummaryMapPalette(obj);
        obj->SetOption("colztext");
        return;
      }
      if( o.name.find( "SummaryOfCabling" )  != std::string::npos)
      {
        obj->SetStats( kFALSE );
        obj->SetOption("text");
        return;
      }
      if( o.name.find( "TkHMap" )  != std::string::npos)
      {
        obj->SetStats( kFALSE );
        obj->SetOption("colz");
        return;
      }
      return;
    }

  void preDrawTH1F( TCanvas *, const DQMNet::CoreObject &o )
    {
      TH1F* obj = dynamic_cast<TH1F*>( o.object );
      assert( obj );

      // This applies to all
      gStyle->SetOptStat(1110);
      //  if ( obj->GetMaximum(1.e5) > 0. ) {
      //    gPad->SetLogy(1);
      //  } else {
      //   gPad->SetLogy(0);
      //  }
      if( o.name.find( "Summary_MeanNumberOfDigis" )  != std::string::npos)
      {
        obj->SetStats( kFALSE );
        obj->SetMaximum(2.0);
        obj->SetMinimum(-0.1);
        return;
      }
      if( o.name.find( "Summary_MeanNumberOfClusters" )  != std::string::npos)
      {
        obj->SetStats( kFALSE );
        obj->SetMaximum(0.05);
        obj->SetMinimum(-0.001);
        return;
      }
      if( o.name.find( "Summary_MeanClusterWidth" )  != std::string::npos)
      {
        obj->SetStats( kFALSE );
        obj->SetMaximum(20.0);
        obj->SetMinimum(-1.0);
        return;
      }
    }

  void preDrawTProfile2D( TCanvas *, const DQMNet::CoreObject &o )
    {
      TProfile2D* obj = dynamic_cast<TProfile2D*>( o.object );
      assert( obj );

      // This applies to all
      gStyle->SetCanvasBorderMode( 0 );
      gStyle->SetPadBorderMode( 0 );
      gStyle->SetPadBorderSize( 0 );
      //    (data->pad)->SetLogy( 0 );;
      //  gStyle->SetOptStat( 0 );

      TAxis* xa = obj->GetXaxis();
      TAxis* ya = obj->GetYaxis();

      xa->SetTitleOffset(0.7);
      xa->SetTitleSize(0.05);
      xa->SetLabelSize(0.04);

      ya->SetTitleOffset(0.7);
      ya->SetTitleSize(0.05);
      ya->SetLabelSize(0.04);

      if( o.name.find( "TkHMap" )  != std::string::npos)
      {
        obj->SetStats( kFALSE );
        obj->SetOption("colz");
        return;
      }
      return;
    }
  void postDrawTH1F( TCanvas *, const DQMNet::CoreObject &o )
    {
      TText tt;
      tt.SetTextSize(0.12);
      if (o.flags == 0) return;
      else
      {
        if (o.flags & DQMNet::DQM_FLAG_REPORT_ERROR)
        {
          tt.SetTextColor(2);
          tt.DrawTextNDC(0.5, 0.5, "Error");
        }
        else if (o.flags & DQMNet::DQM_FLAG_REPORT_WARNING)
        {
          tt.SetTextColor(5);
          tt.DrawTextNDC(0.5, 0.5, "Warning");
        }
        else if (o.flags & DQMNet::DQM_FLAG_REPORT_OTHER)
        {
          tt.SetTextColor(1);
          tt.DrawTextNDC(0.5, 0.5, "Other ");
        }
      }
    }

  void postDrawTH2F( TCanvas *c, const DQMNet::CoreObject &o )
    {
      TH2F* obj = dynamic_cast<TH2F*>( o.object );
      assert( obj );

      std::string name = o.name.substr(o.name.rfind("/")+1);

      if( name.find( "reportSummaryMap" ) != std::string::npos )
      {
        c->SetGridx();
        c->SetGridy();
        return;
      }
      if( name.find( "detFractionReportMap" ) != std::string::npos )
      {
        c->SetGridx();
        c->SetGridy();
        return;
      }
      if( name.find( "sToNReportMap" ) != std::string::npos )
      {
        c->SetGridx();
        c->SetGridy();
        return;
      }
      if( name.find( "SummaryOfCabling" ) != std::string::npos )
      {
        c->SetGridx();
        c->SetGridy();
        return;
      }
    }
};

static SiStripRenderPlugin instance;
