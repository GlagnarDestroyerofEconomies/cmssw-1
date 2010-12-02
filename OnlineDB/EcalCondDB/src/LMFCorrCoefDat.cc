#include "OnlineDB/EcalCondDB/interface/LMFCorrCoefDat.h"

LMFCorrCoefDat::LMFCorrCoefDat() {
  init();
}

LMFCorrCoefDat::LMFCorrCoefDat(EcalDBConnection *c) {
  init();
  m_env = c->getEnv();
  m_conn = c->getConn();
}

LMFCorrCoefDat::LMFCorrCoefDat(oracle::occi::Environment* env,
			       oracle::occi::Connection* conn) {
  init();
  m_env = env;
  m_conn = conn;
}

LMFCorrCoefDat::~LMFCorrCoefDat() {
  std::map<int, LMFCorrCoefDatComponent*>::iterator i = m_data.begin(); 
  std::map<int, LMFCorrCoefDatComponent*>::iterator e = m_data.end(); 
  while (i != e) {
    delete i->second;
    i++;
  }
  m_data.clear();
}

void LMFCorrCoefDat::init() {
  m_data.clear();
  m_env = NULL;
  m_conn = NULL;
  nodebug();
}

LMFCorrCoefDat& LMFCorrCoefDat::setConnection(oracle::occi::Environment* env,
					      oracle::occi::Connection* conn) {
  m_env = env;
  m_conn = conn;
  std::map<int, LMFCorrCoefDatComponent*>::iterator i = m_data.begin(); 
  std::map<int, LMFCorrCoefDatComponent*>::iterator e = m_data.end(); 
  while (i != e) {
    i->second->setConnection(m_env, m_conn);
    i++;
  }
  return *this;
}

LMFCorrCoefDat& LMFCorrCoefDat::setP123(const LMFLmrSubIOV &iov, 
					const EcalLogicID &id, float p1, float p2, float p3) {
  find(iov)->setP123(id, p1, p2, p3);
  return *this;
}

LMFCorrCoefDat& LMFCorrCoefDat::setP123(const LMFLmrSubIOV &iov, const EcalLogicID &id, 
					float p1, float p2, float p3,
					float p1e, float p2e, float p3e) {
  find(iov)->setP123(id, p1, p2, p3, p1e, p2e, p3e);
  return *this;
}

LMFCorrCoefDat& LMFCorrCoefDat::setP123Errors(const LMFLmrSubIOV &iov,
					      const EcalLogicID &id, float p1e, float p2e,
					      float p3e) {
  find(iov)->setP123Errors(id, p1e, p2e, p3e);
  return *this;
}

LMFCorrCoefDat& LMFCorrCoefDat::setFlag(const LMFLmrSubIOV & iov, 
					const EcalLogicID &id, int flag) {
  find(iov)->setFlag(id, flag);
  return *this;
}

LMFCorrCoefDatComponent* LMFCorrCoefDat::find(const LMFLmrSubIOV &iov) {
  if (m_data.find(iov.getID()) != m_data.end()) {
    return m_data[iov.getID()];
  } else {
    LMFCorrCoefDatComponent *c = new LMFCorrCoefDatComponent();
    if (m_conn != NULL) {
      c->setConnection(m_env, m_conn);
    }
    c->setLMFLmrSubIOV(iov);
    m_data[iov.getID()] = c;
    return c;
  }
}

void LMFCorrCoefDat::dump() {
  std::cout << std::endl;
  std::cout << "##################### LMF_CORR_COEF_DAT ########################" << std::endl;
  std::cout << "This structure contains " << m_data.size() << " LMR_SUB_IOV_ID" << std::endl;
  std::map<int, LMFCorrCoefDatComponent*>::const_iterator i = m_data.begin();
  std::map<int, LMFCorrCoefDatComponent*>::const_iterator e = m_data.end();
  int count = 0;
  while (i != e) {
    std::cout << "### SUB IOV ID: " << i->second->getLMFLmrSubIOVID() << std::endl;
    std::list<int> logic_ids = i->second->getLogicIds();
    std::cout << "    Contains data for " << logic_ids.size() << " xtals" << std::endl; 
    count += logic_ids.size(); 
    i++;
  }
  std::cout << "Total no. of xtals for which data are stored: " << count << std::endl;
  std::cout << "##################### LMF_CORR_COEF_DAT ########################" << std::endl;
}

void LMFCorrCoefDat::writeDB() {
  std::map<int, LMFCorrCoefDatComponent*>::iterator i = m_data.begin();
  std::map<int, LMFCorrCoefDatComponent*>::iterator e = m_data.end();
  while (i != e) {
    if (m_debug) {
      std::cout << "Writing data for LMR_SUB_IOV_ID " << i->first << std::endl;
    }
    i->second->writeDB();
    i++;
  }
}

void LMFCorrCoefDat::debug() {
  m_debug = true;
}

void LMFCorrCoefDat::nodebug() {
  m_debug = false;
}

void LMFCorrCoefDat::fetch(const LMFLmrSubIOV &iov)
{
  // fetch data with given LMR_SUB_IOV_ID from the database
  if (m_debug) {
    std::cout << "Looking for SUB_IOV with ID " << iov.getID() << std::endl
	      << std::flush;
  }
  if (m_data.find(iov.getID()) == m_data.end()) {
    if (m_debug) {
      std::cout << "Not found. Getting it from DB " << std::endl
		<< std::flush;
    }
    LMFCorrCoefDatComponent *comp = new LMFCorrCoefDatComponent(m_env, m_conn);
    comp->setLMFLmrSubIOV(iov);
    comp->fetch();
    m_data[iov.getID()] = comp;
  } else if (m_debug) {
    std::cout << "Found in memory." << std::endl
	      << std::flush;
  }
}

std::vector<float> LMFCorrCoefDat::getParameters(const LMFLmrSubIOV &iov,
						 const EcalLogicID &id) {
  std::vector<float> x;
  int key = iov.getID();
  fetch(iov);
  if (m_data.find(key) != m_data.end()) {
    x = (m_data.find(key)->second)->getParameters(id);
  } 
  return x;
}

std::vector<float> LMFCorrCoefDat::getParameterErrors(const LMFLmrSubIOV &iov,
						      const EcalLogicID &id) 
{
  std::vector<float> x;
  int key = iov.getID();
  fetch(iov);
  if (m_data.find(key) != m_data.end()) {
    x = (m_data.find(key)->second)->getParameterErrors(id);
  }
  return x;
}

int LMFCorrCoefDat::getFlag(const LMFLmrSubIOV &iov, 
			    const EcalLogicID &id) {
  int x = -1;
  fetch(iov);
  if (m_data.find(iov.getID()) != m_data.end()) {
    x = (m_data.find(iov.getID())->second)->getFlag(id);
  }
  return x;
}

int LMFCorrCoefDat::size() const {
  int c = 0;
  std::map<int, LMFCorrCoefDatComponent *>::const_iterator i = m_data.begin();
  std::map<int, LMFCorrCoefDatComponent *>::const_iterator e = m_data.end();
  while (i != e) {
    c += i->second->size();
    i++;
  }
  return c;
}
