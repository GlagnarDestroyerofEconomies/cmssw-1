#ifndef CSCTrackFinder_CSCTrackFinderDataTypes_h
#define CSCTrackFinder_CSCTrackFinderDataTypes_h

// Address Types                                                                                      
typedef class local_phi_address
{
 public:
  unsigned int strip        : 8;
  unsigned int clct_pattern : 3;
  unsigned int pattern_type : 1; // 1 is half strip 0 is di strip
  unsigned int quality      : 4;
  unsigned int lr           : 1;
  unsigned int spare        : 2;
  unsigned int zero         : 13;

  local_phi_address() {};
  local_phi_address(const unsigned& u) { this->operator=(u); }

  local_phi_address& operator=(const unsigned& u);
  unsigned toint() const;

} lclphiadd;

typedef class global_phi_address
{
 public:
  unsigned int phi_local    : 10;
  unsigned int wire_group   : 5;  // bits 2-6 of wg                                                   
  unsigned int cscid        : 4;
  unsigned int zero         : 13;
  
  global_phi_address() {};
  global_phi_address(const unsigned& u) { this->operator=(u); }

  global_phi_address& operator=(const unsigned& u);
  unsigned toint() const;

} gblphiadd;

typedef class global_eta_address
{
 public:
  unsigned int phi_bend     : 6;
  unsigned int phi_local    : 2;
  unsigned int wire_group   : 7;
  unsigned int cscid        : 4;
  unsigned int zero         : 13;

  global_eta_address() {};
  global_eta_address(const unsigned& u) { this->operator=(u); }

  global_eta_address& operator=(const unsigned& u);
  unsigned toint() const;

} gbletaadd;

/// Data Types                                                                                        
typedef class local_phi_data
{
 public:
  unsigned short phi_local      : 10;
  unsigned short phi_bend_local : 6;

  local_phi_data() {};
  explicit local_phi_data(const unsigned short& us) { this->operator=(us); }

  local_phi_data& operator=(const unsigned short& us);
  unsigned short toint() const;

} lclphidat;

typedef class global_phi_data
{
 public:
  unsigned short global_phi : 12;
  unsigned short spare      : 4;
  
  global_phi_data() {};
  global_phi_data(const unsigned short& us) { this->operator=(us); }

  global_phi_data& operator=(const unsigned short& us);
  unsigned short toint() const;

} gblphidat;

typedef class global_eta_data
{
 public:
  unsigned short global_eta  : 7;
  unsigned short global_bend : 5;
  unsigned short spare       : 4;
  
  global_eta_data() {};
  global_eta_data(const unsigned short& us) { this->operator=(us); }

  global_eta_data& operator=(const unsigned short& us);
  unsigned short toint() const;

} gbletadat;

#endif
