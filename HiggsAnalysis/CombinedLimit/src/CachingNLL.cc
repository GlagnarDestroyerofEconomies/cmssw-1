#include "../interface/CachingNLL.h"
#include "../interface/utils.h"
#include <stdexcept>
#include <RooCategory.h>
#include <RooDataSet.h>
#include <RooProduct.h>

cacheutils::ArgSetChecker::ArgSetChecker(const RooAbsCollection *set) 
{
    std::auto_ptr<TIterator> iter(set->createIterator());
    for (RooAbsArg *a  = dynamic_cast<RooAbsArg *>(iter->Next()); 
                    a != 0; 
                    a  = dynamic_cast<RooAbsArg *>(iter->Next())) {
        RooRealVar *rrv = dynamic_cast<RooRealVar *>(a);
        if (rrv && !rrv->isConstant()) { 
            vars_.push_back(rrv);
            vals_.push_back(rrv->getVal());
        }
    }
}


bool 
cacheutils::ArgSetChecker::changed(bool updateIfChanged) 
{
    std::vector<RooRealVar *>::const_iterator it = vars_.begin(), ed = vars_.end();
    std::vector<double>::iterator itv = vals_.begin();
    bool changed = false;
    for ( ; it != ed; ++it, ++itv) {
        double val = (*it)->getVal();
        if (val != *itv) { 
            //std::cerr << "var::CachingPdfable " << (*it)->GetName() << " changed: " << *itv << " -> " << val << std::endl;
            changed = true; 
            if (updateIfChanged) { *itv = val; }
            else break;
        }
    }
    return changed;
}

cacheutils::CachingPdf::CachingPdf(RooAbsReal *pdf, const RooArgSet *obs) :
    obs_(obs),
    pdfOriginal_(pdf),
    pdfPieces_(),
    pdf_(utils::fullCloneFunc(pdf, pdfPieces_)),
    lastData_(0)
{
    std::auto_ptr<RooArgSet> params(pdf_->getParameters(*obs_));
    checker_ = ArgSetChecker(&*params);
}

cacheutils::CachingPdf::CachingPdf(const CachingPdf &other) :
    obs_(other.obs_),
    pdfOriginal_(other.pdfOriginal_),
    pdfPieces_(),
    pdf_(utils::fullCloneFunc(pdfOriginal_, pdfPieces_)),
    lastData_(0)
{
    std::auto_ptr<RooArgSet> params(pdf_->getParameters(*obs_));
    checker_ = ArgSetChecker(&*params);
}

cacheutils::CachingPdf::~CachingPdf() 
{
}

const std::vector<Double_t> & 
cacheutils::CachingPdf::eval(const RooAbsData &data) 
{
    if (lastData_ != &data) {
        lastData_ = &data;
        pdf_->optimizeCacheMode(*data.get());
        pdf_->attachDataSet(data);
        const_cast<RooAbsData*>(lastData_)->setDirtyProp(false);
        vals_.resize(data.numEntries());
        realFill_(data);
        checker_.changed(true);
    } else if (checker_.changed(true)) {
        realFill_(data);
    } 
    return vals_;
}

void
cacheutils::CachingPdf::realFill_(const RooAbsData &data) 
{
    //std::auto_ptr<RooArgSet> params(pdf_->getObservables(*obs)); // for non-smart handling of pointers
    std::vector<Double_t>::iterator itv = vals_.begin();
    for (int i = 0, n = data.numEntries(); i < n; ++i, ++itv) {
        data.get(i);
        //*params = *data.get(i); // for non-smart handling of pointers
        *itv = pdf_->getVal(obs_);
        //std::cout << " at i = " << i << " pdf = " << *itv << std::endl;
    }
}

cacheutils::CachingAddNLL::CachingAddNLL(const char *name, const char *title, RooAbsPdf *pdf, RooAbsData *data) :
    RooAbsReal(name, title),
    pdf_(pdf),
    params_("params","parameters",this)
{
    if (pdf == 0) throw std::invalid_argument(std::string("Pdf passed to ")+name+" is null");
    setData(*data);
    setup_();
}

cacheutils::CachingAddNLL::CachingAddNLL(const CachingAddNLL &other, const char *name) :
    RooAbsReal(name ? name : (TString("nll_")+other.pdf_->GetName()).Data(), ""),
    pdf_(other.pdf_),
    params_("params","parameters",this)
{
    setData(*other.data_);
    setup_();
}

cacheutils::CachingAddNLL::~CachingAddNLL() 
{
    for (int i = 0, n = integrals_.size(); i < n; ++i) delete integrals_[i];
    integrals_.clear();
}

cacheutils::CachingAddNLL *
cacheutils::CachingAddNLL::clone(const char *name) const 
{
    return new cacheutils::CachingAddNLL(*this, name);
}

void
cacheutils::CachingAddNLL::setup_() 
{
    const RooArgSet *obs = data_->get();
    for (int i = 0, n = integrals_.size(); i < n; ++i) delete integrals_[i];
    integrals_.clear();
    RooAddPdf *addpdf = 0;
    RooRealSumPdf *sumpdf = 0;
    if ((addpdf = dynamic_cast<RooAddPdf *>(pdf_)) != 0) {
        isRooRealSum_ = false;
        int npdf = addpdf->coefList().getSize();
        coeffs_.reserve(npdf);
        pdfs_.reserve(npdf);
        for (int i = 0; i < npdf; ++i) {
            RooAbsReal * coeff = dynamic_cast<RooAbsReal*>(addpdf->coefList().at(i));
            RooAbsPdf  * pdfi  = dynamic_cast<RooAbsPdf *>(addpdf->pdfList().at(i));
            coeffs_.push_back(coeff);
            pdfs_.push_back(CachingPdf(pdfi, obs));
        }
    } else if ((sumpdf = dynamic_cast<RooRealSumPdf *>(pdf_)) != 0) {
        isRooRealSum_ = true;
        int npdf = sumpdf->coefList().getSize();
        coeffs_.reserve(npdf);
        pdfs_.reserve(npdf);
        integrals_.reserve(npdf);
        for (int i = 0; i < npdf; ++i) {
            RooAbsReal * coeff = dynamic_cast<RooAbsReal*>(sumpdf->coefList().at(i));
            RooAbsReal * funci = dynamic_cast<RooAbsReal*>(sumpdf->funcList().at(i));
            /// Temporarily switch this off, it doesn't work. Don't know why, however.
            if (0 && typeid(*funci) == typeid(RooProduct)) {
                RooArgList obsDep, obsInd;
                obsInd.add(*coeff);
                utils::factorizeFunc(*obs, *funci, obsDep, obsInd);
                std::cout << "Entry " << i << ": coef name " << (coeff ? coeff->GetName()   : "null") << 
                                              "  type " << (coeff ? coeff->ClassName() :  "n/a") << std::endl;
                std::cout << "       " <<     "; func name " << (funci ? funci->GetName()   : "null") << 
                                              "  type " << (funci ? funci->ClassName() :  "n/a") << std::endl;
                std::cout << "Terms depending on observables: " << std::endl; obsDep.Print("V");
                std::cout << "Terms not depending on observables: " << std::endl; obsInd.Print("V");
                if (obsInd.getSize() > 1) {
                    coeff = new RooProduct(TString::Format("%s_x_%s_obsIndep", coeff->GetName(), funci->GetName()), "", RooArgSet(obsInd));
                    addOwnedComponents(RooArgSet(*coeff));
                }
                if (obsDep.getSize() > 1) {
                    funci = new RooProduct(TString::Format("%s_obsDep", funci->GetName()), "", RooArgSet(obsInd));
                    addOwnedComponents(RooArgSet(*funci));
                } else if (obsDep.getSize() == 1) {
                    funci = (RooAbsReal *) obsDep.first();
                } else throw std::logic_error("No part of pdf depends on observables?");
            }
            coeffs_.push_back(coeff);
            pdfs_.push_back(CachingPdf(funci, obs));
            integrals_.push_back(funci->createIntegral(*obs));
        }
    } else {
        std::string errmsg = "ERROR: CachingAddNLL: Pdf ";
        errmsg += pdf_->GetName();
        errmsg += " is neither a RooAddPdf nor a RooRealSumPdf, but a ";
        errmsg += pdf_->ClassName();
        throw std::invalid_argument(errmsg);
    }

    std::auto_ptr<RooArgSet> params(pdf_->getParameters(*data_));
    std::auto_ptr<TIterator> iter(params->createIterator());
    for (RooAbsArg *a = (RooAbsArg *) iter->Next(); a != 0; a = (RooAbsArg *) iter->Next()) {
        RooRealVar *rrv = dynamic_cast<RooRealVar *>(a);
        if (rrv != 0 && !rrv->isConstant()) params_.add(*rrv);
    }
}

Double_t 
cacheutils::CachingAddNLL::evaluate() const 
{
    std::fill( partialSum_.begin(), partialSum_.end(), 0.0 );

    std::vector<RooAbsReal*>::iterator  itc = coeffs_.begin(), edc = coeffs_.end();
    std::vector<CachingPdf>::iterator   itp = pdfs_.begin(),   edp = pdfs_.end();
    std::vector<double>::const_iterator itw, bgw = weights_.begin(),    edw = weights_.end();
    std::vector<double>::iterator       its, bgs = partialSum_.begin(), eds = partialSum_.end();
    double sumCoeff = 0;
    //std::cout << "Performing evaluation of " << GetName() << std::endl;
    for ( ; itc != edc; ++itp, ++itc ) {
        // get coefficient
        double coeff = (*itc)->getVal();
        if (isRooRealSum_) {
            sumCoeff += coeff * integrals_[itc - coeffs_.begin()]->getVal();
            //std::cout << "  coefficient = " << coeff << ", integral = " << integrals_[itc - coeffs_.begin()]->getVal() << std::endl;
        } else {
            sumCoeff += coeff;
        }
        // get vals
        const std::vector<Double_t> &pdfvals = itp->eval(*data_);
        // update running sum
        std::vector<Double_t>::const_iterator itv = pdfvals.begin();
        for (its = bgs; its != eds; ++its, ++itv) {
             *its += coeff * (*itv); // sum (n_i * pdf_i)
        }
    }
    // then get the final nll
    double ret = 0;
    for ( its = bgs, itw = bgw ; its != eds ; ++its, ++itw ) {
        if (*itw != 0 && *its == 0) std::cerr << "WARNING: underflow to zero" << std::endl;
        if (*itw) ret += (*itw) * (*its == 0 ? -9e9 : log( ((*its) / sumCoeff) ));
    }
    // then flip sign
    ret = -ret;
    // std::cout << "AddNLL for " << pdf_->GetName() << ": " << ret << std::endl;
    // and add extended term: expected - observed*log(expected);
    double expectedEvents = (isRooRealSum_ ? pdf_->getNorm(data_->get()) : sumCoeff);
    if (expectedEvents < 0) {
        logEvalError("Expected number of events is negative");
        expectedEvents = 1e-6;
    }
    ret += expectedEvents - UInt_t(sumWeights_) * log(expectedEvents);
    // std::cout << "     plus extended term: " << ret << std::endl;
    return ret;
}

void 
cacheutils::CachingAddNLL::setData(const RooAbsData &data) 
{
    //std::cout << "Setting data for pdf " << pdf_->GetName() << std::endl;
    //utils::printRAD(&data);
    data_ = &data;
    setValueDirty();
    sumWeights_ = 0.0;
    weights_.resize(data.numEntries());
    partialSum_.resize(data.numEntries());
    std::vector<double>::iterator itw = weights_.begin();
    for (int i = 0, n = data.numEntries(); i < n; ++i, ++itw) {
        data.get(i);
        *itw = data.weight();
        sumWeights_ += *itw;
    }
    for (std::vector<CachingPdf>::iterator itp = pdfs_.begin(), edp = pdfs_.end(); itp != edp; ++itp) {
        itp->setDataDirty();
    }
}

RooArgSet* 
cacheutils::CachingAddNLL::getObservables(const RooArgSet* depList, Bool_t valueOnly) const 
{
    return new RooArgSet();
}

RooArgSet* 
cacheutils::CachingAddNLL::getParameters(const RooArgSet* depList, Bool_t stripDisconnected) const 
{
    return new RooArgSet(params_); 
}


cacheutils::CachingSimNLL::CachingSimNLL(RooSimultaneous *pdf, RooAbsData *data, const RooArgSet *nuis) :
    pdfOriginal_(pdf),
    dataOriginal_(data),
    nuis_(nuis),
    params_("params","parameters",this)
{
    setup_();
}

cacheutils::CachingSimNLL::CachingSimNLL(const CachingSimNLL &other, const char *name) :
    pdfOriginal_(other.pdfOriginal_),
    dataOriginal_(other.dataOriginal_),
    nuis_(other.nuis_),
    params_("params","parameters",this)
{
    setup_();
}

cacheutils::CachingSimNLL *
cacheutils::CachingSimNLL::clone(const char *name) const 
{
    return new cacheutils::CachingSimNLL(*this, name);
}

void
cacheutils::CachingSimNLL::setup_() 
{
    RooAbsPdf *pdfclone = utils::fullClonePdf(pdfOriginal_, piecesForCloning_);
    std::auto_ptr<RooArgSet> params(pdfclone->getParameters(*dataOriginal_));
    params_.add(*params);

    RooArgList constraints;
    factorizedPdf_.reset(dynamic_cast<RooSimultaneous *>(utils::factorizePdf(*dataOriginal_->get(), *pdfclone, constraints)));
    
    RooSimultaneous *simpdf = factorizedPdf_.get();
    constrainPdfs_.clear(); 
    if (constraints.getSize()) {
        //constrainPdfs_.push_back(new RooProdPdf("constraints","constraints", constraints));
        for (int i = 0, n = constraints.getSize(); i < n; ++i) {
            constrainPdfs_.push_back(dynamic_cast<RooAbsPdf*>(constraints.at(i)));
            //std::cout << "Constraint pdf: " << constraints.at(i)->GetName() << std::endl;
        }
    } else {
        std::cerr << "PDF didn't factorize!" << std::endl;
        std::cout << "Parameters: " << std::endl;
        params->Print("V");
        std::cout << "Obs: " << std::endl;
        dataOriginal_->get()->Print("V");
        factorizedPdf_.release();
        simpdf = dynamic_cast<RooSimultaneous *>(pdfclone);
    }

    
    std::auto_ptr<RooAbsCategoryLValue> catClone((RooAbsCategoryLValue*) simpdf->indexCat().Clone());
    pdfs_.resize(catClone->numBins(NULL), 0);
    //dataSets_.reset(dataOriginal_->split(pdfOriginal_->indexCat(), true));
    datasets_.resize(pdfs_.size(), 0);
    splitWithWeights(*dataOriginal_, simpdf->indexCat(), true);
    //std::cout << "Pdf " << simpdf->GetName() <<" is a SimPdf over category " << catClone->GetName() << ", with " << pdfs_.size() << " bins" << std::endl;
    for (int ib = 0, nb = pdfs_.size(); ib < nb; ++ib) {
        catClone->setBin(ib);
        RooAbsPdf *pdf = simpdf->getPdf(catClone->getLabel());
        if (pdf != 0) {
            RooAbsData *data = (RooAbsData *) datasets_[ib]; //dataSets_->FindObject(catClone->getLabel());
            //RooAbsData *data = (RooAbsData *) dataSets_->FindObject(catClone->getLabel());
            //std::cout << "   bin " << ib << " (label " << catClone->getLabel() << ") has pdf " << pdf->GetName() << " of type " << pdf->ClassName() << " and " << (data ? data->numEntries() : -1) << " dataset entries" << std::endl;
            if (data == 0) { throw std::logic_error("Error: no data"); }
            pdfs_[ib] = new CachingAddNLL(catClone->getLabel(), "", pdf, data);
        } else { 
            pdfs_[ib] = 0; 
            //std::cout << "   bin " << ib << " (label " << catClone->getLabel() << ") has no pdf" << std::endl;
        }
    }   

    setValueDirty();
}

Double_t 
cacheutils::CachingSimNLL::evaluate() const 
{
    double ret = 0;
    for (std::vector<CachingAddNLL*>::const_iterator it = pdfs_.begin(), ed = pdfs_.end(); it != ed; ++it) {
        if (*it != 0) {
            double nllval = (*it)->getVal();
            // what sanity check could I put here?
            ret += nllval;
        }
    }
    if (!constrainPdfs_.empty()) {
        for (std::vector<RooAbsPdf *>::const_iterator it = constrainPdfs_.begin(), ed = constrainPdfs_.end(); it != ed; ++it) { 
            double pdfval = (*it)->getVal(nuis_);
            if (pdfval == 0) logEvalError((std::string("Constraint pdf ")+(*it)->GetName()+" evaluated to zero").c_str());
            ret -= (pdfval > 1e-7 ? log(pdfval) : log(1e-7)-pdfval);
        }
    }
    static unsigned long _trace_ = 0; _trace_++;
    if (_trace_ % 10 == 0) { putchar('.'); fflush(stdout); }
    //if (_trace_ % 250 == 0) { printf("               NLL % 10.4f after %10lu evals.\n", ret, _trace_); fflush(stdout); }
    return ret;
}

void 
cacheutils::CachingSimNLL::setData(const RooAbsData &data) 
{
    dataOriginal_ = &data;
    //std::cout << "combined data has " << data.numEntries() << " dataset entries (sumw " << data.sumEntries() << ", weighted " << data.isWeighted() << ")" << std::endl;
    //utils::printRAD(&data);
    //dataSets_.reset(dataOriginal_->split(pdfOriginal_->indexCat(), true));
    splitWithWeights(*dataOriginal_, pdfOriginal_->indexCat(), true);
    for (int ib = 0, nb = pdfs_.size(); ib < nb; ++ib) {
        CachingAddNLL *canll = pdfs_[ib];
        if (canll == 0) continue;
        RooAbsData *data = datasets_[ib];
        //RooAbsData *data = (RooAbsData *) dataSets_->FindObject(canll->GetName());
        if (data == 0) { throw std::logic_error("Error: no data"); }
        //std::cout << "   bin " << ib << " (label " << canll->GetName() << ") has pdf " << canll->pdf()->GetName() << " of type " << canll->pdf()->ClassName() <<
        //             " and " << (data ? data->numEntries() : -1) << " dataset entries (sumw " << data->sumEntries() << ", weighted " << data->isWeighted() << ")" << std::endl;
        canll->setData(*data);
    }
}

void cacheutils::CachingSimNLL::splitWithWeights(const RooAbsData &data, const RooAbsCategory& splitCat, Bool_t createEmptyDataSets) {
    RooCategory *cat = dynamic_cast<RooCategory *>(data.get()->find(splitCat.GetName()));
    if (cat == 0) throw std::logic_error("Error: no category");
    int nb = cat->numBins((const char *)0), ne = data.numEntries();
    RooArgSet obs(*data.get()); obs.remove(*cat, true, true);
    RooArgSet obsplus(obs);
    RooRealVar weight("_weight_","",1);
    if (data.isWeighted()) obsplus.add(weight);
    if (nb != int(datasets_.size())) throw std::logic_error("Number of categories changed"); // this can happen due to bugs in RooDataSet
    for (int ib = 0; ib < nb; ++ib) {
        if (datasets_[ib] == 0) datasets_[ib] = new RooDataSet("", "", obsplus, data.isWeighted() ? "_weight_" : (const char*)0);
        else datasets_[ib]->reset();
    }
    //utils::printRDH((RooAbsData*)&data);
    for (int i = 0; i < ne; ++i) {
        data.get(i); if (data.weight() == 0) continue;
        int ib = cat->getBin();
        //std::cout << "Event " << i << " of weight " << data.weight() << " is in bin " << ib << " label " << cat->getLabel() << std::endl;
        if (data.weight() > 0) datasets_[ib]->add(obs, data.weight());
    }
}

RooArgSet* 
cacheutils::CachingSimNLL::getObservables(const RooArgSet* depList, Bool_t valueOnly) const 
{
    return new RooArgSet();
}

RooArgSet* 
cacheutils::CachingSimNLL::getParameters(const RooArgSet* depList, Bool_t stripDisconnected) const 
{
    return new RooArgSet(params_); 
}
