#include "DQMServices/Core/interface/QReport.h"
#include "DQMServices/Core/interface/QCriterion.h"
#include "DQMServices/Core/interface/QTestStatus.h"
#include <iostream>

using namespace std;

QReport::QReport(string qtname)
{
  init();
  setName(qtname);
}

QReport::QReport(QCriterion * qc)
{
  init();
  qcriterion_ = qc;
  if(qcriterion_)
    setName(qcriterion_->getName());
}

// initialization
void QReport::init(void)
{
  qcriterion_ = 0;
  setName("UNKNOWN_QTEST");
  resetStatusMessage();
}

// reset status & message
void QReport::resetStatusMessage(void)
{
  setStatus(dqm::qstatus::DID_NOT_RUN);
  setMessage(getNullMessage());
}

// run QCriterion algorithm
void QReport::runTest(void)
{
  // qcriterion may be null if results are received from another node
  if(!qcriterion_)return;

  int old_status = status_;
  string old_message = message_;

  badChannels_.clear();
  qcriterion_->runTest(myME_);
  setStatus(qcriterion_->getStatus());
  setMessage(qcriterion_->getMessage());
  badChannels_ = qcriterion_->getBadChannels();

  // if status or message has changed, need to notify derived class
  if(old_status != status_ || old_message != message_)
    // calling derived class method
    this->updateReport();
}

