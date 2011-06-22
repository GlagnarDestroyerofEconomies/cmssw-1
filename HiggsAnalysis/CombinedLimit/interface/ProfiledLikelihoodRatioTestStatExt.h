#ifndef HiggsAnalysis_CombinedLimit_ProfiledLikelihoodRatioTestStatExt_h
#define HiggsAnalysis_CombinedLimit_ProfiledLikelihoodRatioTestStatExt_h

#include <memory>
#include <RooAbsPdf.h>
#include <RooAbsData.h>
#include <RooArgSet.h>
#include <RooStats/TestStatistic.h>
#include "../interface/RooSimultaneousOpt.h"
#include "../interface/CachingNLL.h"


class ProfiledLikelihoodRatioTestStatOpt : public RooStats::TestStatistic {
    public:
        ProfiledLikelihoodRatioTestStatOpt(const RooArgSet &obs, RooAbsPdf &pdfNull, RooAbsPdf &pdfAlt, 
                const RooArgSet *nuisances, const RooArgSet & paramsNull = RooArgSet(), const RooArgSet & paramsAlt = RooArgSet(),
                int verbosity=0) ;
 
        virtual Double_t Evaluate(RooAbsData& data, RooArgSet& nullPOI) ;

        virtual const TString GetVarName() const {
            return TString::Format("-log(%s/%s)", pdfNull_->GetName(), pdfAlt_->GetName()); 
        }

        // Verbosity (default: 0)
        void setPrintLevel(Int_t level) { verbosity_ = level; }

    private:
        RooAbsPdf *pdfNull_, *pdfAlt_;
        RooArgSet snapNull_, snapAlt_; 
        RooArgSet nuisances_; 
        std::auto_ptr<RooArgSet> paramsNull_, paramsAlt_;
        Int_t verbosity_;

        double minNLL(RooAbsPdf &pdf, RooAbsData &data) ;
}; // TestSimpleStatistics


class ProfiledLikelihoodTestStatOpt : public RooStats::TestStatistic {
    public:
        ProfiledLikelihoodTestStatOpt(const RooArgSet & observables,
                RooAbsPdf &pdf, 
                const RooArgSet *nuisances, 
                const RooArgSet & params, const RooArgList &gobsParams, const RooArgList &gobs, int verbosity=0) ; 

        virtual Double_t Evaluate(RooAbsData& data, RooArgSet& nullPOI) ;

        virtual const TString GetVarName() const { return "- log (#lambda)"; }

        // Verbosity (default: 0)
        void setPrintLevel(Int_t level) { verbosity_ = level; }

    private:
        RooAbsPdf *pdf_;
        RooArgSet snap_, poi_, nuisances_; 
        std::auto_ptr<RooArgSet> params_;
        std::auto_ptr<RooAbsReal> nll_;
        RooArgList gobsParams_, gobs_;
        Int_t verbosity_;

        // create NLL. if returns true, it can be kept, if false it should be deleted at the end of Evaluate
        bool createNLL(RooAbsPdf &pdf, RooAbsData &data) ;
        double minNLL() ;
}; // TestSimpleStatistics


#endif
