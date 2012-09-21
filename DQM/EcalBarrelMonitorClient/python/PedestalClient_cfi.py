from DQM.EcalBarrelMonitorTasks.PedestalTask_cfi import ecalPedestalTask

ecalPedestalClient = dict(
    expectedMeanG01 = 200.,
    expectedMeanG06 = 200.,
    expectedMeanG12 = 200.,
    toleranceMeanG01 = 25.,
    toleranceMeanG06 = 25.,
    toleranceMeanG12 = 25.,
    toleranceRMSG01 = 1.,
    toleranceRMSG06 = 1.2,
    toleranceRMSG12 = 2.,
    expectedPNMeanG01 = 750.,
    expectedPNMeanG16 = 750.,
    tolerancePNMeanG01 = 100.,
    tolerancePNMeanG16 = 100.,
    tolerancePNRMSG01 = 20.,
    tolerancePNRMSG16 = 20.,
    MEs = dict(
        Quality = dict(path = '%(subdet)s/%(prefix)sPedestalClient/%(prefix)sPT pedestal quality G%(gain)s %(sm)s', otype = 'SM', btype = 'Crystal', kind = 'TH2F', multi = 3),
        Mean = dict(path = '%(subdet)s/%(prefix)sPedestalClient/%(prefix)sPT pedestal mean G%(gain)s %(sm)s', otype = 'SM', btype = 'User', kind = 'TH1F', xaxis = {'nbins': 120, 'low': 170., 'high': 230.}, multi = 3),
        RMS = dict(path = "%(subdet)s/%(prefix)sPedestalClient/%(prefix)sPT pedestal rms G%(gain)s %(sm)s", otype = 'SM', btype = 'User', kind = 'TH1F', xaxis = {'nbins': 100, 'low': 0., 'high': 10.}, multi = 3),
        PNRMS = dict(path = '%(subdet)s/%(prefix)sPedestalClient/%(prefix)sPDT PNs pedestal rms %(sm)s G%(pngain)s', otype = 'SMMEM', btype = 'User', kind = 'TH1F', xaxis = {'nbins': 100, 'low': 0., 'high': 50.}, multi = 2),
        QualitySummary = dict(path = '%(subdet)s/%(prefix)sSummaryClient/%(prefix)sPT pedestal quality G%(gain)s summary', otype = 'Ecal3P', btype = 'SuperCrystal', kind = 'TH2F', multi = 3),
        PNQualitySummary = dict(path = '%(subdet)s/%(prefix)sSummaryClient/%(prefix)sPT PN pedestal quality G%(pngain)s summary', otype = 'MEM2P', btype = 'Crystal', kind = 'TH2F', multi = 2)
    ),
    sources = dict(
        Pedestal = ecalPedestalTask['MEs']['Pedestal'],
        PNPedestal = ecalPedestalTask['MEs']['PNPedestal']
    )
)
