#include "RecoEgamma/MaterialConversionModules/interface/Pi0MaterialConversion.h"
 
 
Pi0MaterialConversion::Pi0MaterialConversion() {
  mass_=0;
  coneIso_=0;
  pi0energy_=0;
  pi0phi_=0;
  pi0eta_=0;
  e1energy_=0;
  e1phi_=0;
  e1eta_=0;
  e2energy_=0;
  e2phi_=0;
  e2eta_=0;
  phoenergy_=0;
  phoeta_=0;
  phophi_=0;
  ConversionREst_=0;
  ConversionPhiEst_=0;
  stubchar1_=0;
  stubchar2_=0;
  track1ptIS_=0;
  track1phiIS_=0;
  track1etaIS_=0;
  track1ptFS_=0;
  track1phiFS_=0;
  track1etaFS_=0;
  track1Charge_=0;
  nHits1_=-1;
  track1chi2_=-999;
  track1SmallRhit_=0;
  track2ptIS_=0;
  track2phiIS_=0;
  track2etaIS_=0;
  track2ptFS_=0;
  track2phiFS_=0;
  track2etaFS_=0;
  nHits2_=-1;
  track2chi2_=-999;
  track2SmallRhit_=0;
  track2Charge_=0;
  VtxXpos_=0;
  VtxYpos_=0;
  VtxZpos_=0;
  VtxRpos_=0;
  VtxPhipos_=0;
  VtxChi2_=-999;
  stub1Hits.clear();
  stub2Hits.clear();
  trk1Hits.clear();
  trk2Hits.clear();

  e1Emax_=0;
  e1E2nd_=0;
  e1E2x2_=0;
  e1E3x3_=0;
  e1etaeta_=0;
  e1phiphi_=0;
  e1etaphi_=0;

  e2Emax_=0;
  e2E2nd_=0;
  e2E2x2_=0;
  e2E3x3_=0;
  e2etaeta_=0;
  e2phiphi_=0;
  e2etaphi_=0;


  phoEmax_=0;
  phoE2nd_=0;
  phoE2x2_=0;
  phoE3x3_=0;
  phoetaeta_=0;
  phophiphi_=0;
  phoetaphi_=0;
}
 
Pi0MaterialConversion::~Pi0MaterialConversion() {
  
}

Pi0MaterialConversion::Pi0MaterialConversion(
					     double photonE, double photoneta, double photonphi,
					     double ele1E, double ele1eta, double ele1phi,
				double ele2E, double ele2eta, double ele2phi,
				double RConv, double PhiConv, double coneIso, 
                                double mass//, double pi0E, double pi0eta, double pi0phi
				)
{
  mass_=mass;
  coneIso_=coneIso;
//   pi0energy_=pi0E;
//   pi0phi_=pi0phi;
//   pi0eta_=pi0eta;
  e1energy_=ele1E;
  e1phi_=ele1phi;
  e1eta_=ele1eta;
  e2energy_=ele2E;
  e2phi_=ele2phi;
  e2eta_=ele2eta;
  phoenergy_=photonE;
  phoeta_=photoneta;
  phophi_=photonphi;
  ConversionREst_=RConv;
  ConversionPhiEst_=PhiConv;
  stubchar1_=0;
  stubchar2_=0;
  track1ptIS_=0;
  track1phiIS_=0;
  track1etaIS_=0;
  track1ptFS_=0;
  track1phiFS_=0;
  track1etaFS_=0;
  nHits1_=-1;
  track1Charge_=0;
  track1chi2_=-999;
  track1SmallRhit_=0;
  track2ptIS_=0;
  track2phiIS_=0;
  track2etaIS_=0;
  track2ptFS_=0;
  track2phiFS_=0;
  track2etaFS_=0;
  nHits2_=-1;
  track2chi2_=-999;
  track2SmallRhit_=0;
  track2Charge_=0;
  VtxXpos_=0;
  VtxYpos_=0;
  VtxZpos_=0;
  VtxRpos_=0;
  VtxPhipos_=0;
  VtxChi2_=-999;

  e1Emax_=0;
  e1E2nd_=0;
  e1E2x2_=0;
  e1E3x3_=0;
  e1etaeta_=0;
  e1phiphi_=0;
  e1etaphi_=0;

  e2Emax_=0;
  e2E2nd_=0;
  e2E2x2_=0;
  e2E3x3_=0;
  e2etaeta_=0;
  e2phiphi_=0;
  e2etaphi_=0;


  phoEmax_=0;
  phoE2nd_=0;
  phoE2x2_=0;
  phoE3x3_=0;
  phoetaeta_=0;
  phophiphi_=0;
  phoetaphi_=0;

}


