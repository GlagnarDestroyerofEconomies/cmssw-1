import FWCore.ParameterSet.Config as cms

ptSeedParameterization = cms.PSet(
    SMB_21 = cms.vdouble(1.04575, -0.163287, 0.0, 0.183661, -0.049415, 
        0.0),
    SMB_20 = cms.vdouble(0.990194, -0.058027, 0.0, 0.157433, 0.049581, 
        0.0),
    SMB_22 = cms.vdouble(1.389607, -0.691458, 0.0, -0.204387, 0.471755, 
        0.0),
    OL_2213 = cms.vdouble(0.736644, -0.65969, 0.0, -0.429713, 0.813747, 
        0.0),
    # Sigle CSC Segments 
    # CSCDet_Id         p1        p2        p3       ep1      ep2      ep3
    #------------------------------------------------------------------------    #
    SME_11 = cms.vdouble(2.612846, -0.976852, 0.0, -5.986027, 4.000885, 
        0.0),
    SME_13 = cms.vdouble(0.576629, 0.010521, 0.0, 0.624575, 1.518433, 
        0.0),
    SME_12 = cms.vdouble(0.02713, 0.623194, 0.0, 1.623888, -0.362265, 
        0.0),
    SME_32 = cms.vdouble(-0.242825, 0.163158, 0.0, 183.95999, -133.799119, 
        0.0),
    SME_31 = cms.vdouble(-0.4583, 0.2501, 0.0, -202.733432, 92.182385, 
        0.0),
    # OL Parameters 
    # Det_Layers        p1        p2        p3       ep1      ep2      ep3
    #------------------------------------------------------------------------    #
    OL_1213 = cms.vdouble(1.089127, -0.865182, 0.0, 0.307029, -0.056826, 
        0.0),
    DT_13 = cms.vdouble(0.333344, 0.089449, -0.171128, 0.149048, -0.097795, 
        0.174794),
    # DT Parameters 
    # Det_Stations          p1        p2        p3       ep1      ep2      ep3
    #------------------------------------------------------------------------    #
    DT_12 = cms.vdouble(0.192702, 0.067231, -0.1026, 0.13832, -0.120017, 
        0.175494),
    DT_14 = cms.vdouble(0.380675, 0.091873, -0.172667, 0.192447, -0.04005, 
        0.100509),
    OL_1232 = cms.vdouble(0.19594, -0.012048, 0.0, 0.366937, -0.002214, 
        0.0),
    CSC_23 = cms.vdouble(-0.01233, -0.0407, 0.053044, -2.738639, 5.219037, 
        -1.185425),
    CSC_24 = cms.vdouble(0.578011, -0.66913, 0.219845, -15.85908, 19.289552, 
        -4.800992),
    CSC_03 = cms.vdouble(-0.444401, 1.200778, -0.274867, 8.319702, -9.177761, 
        2.737426),
    SMB_31 = cms.vdouble(0.5905, -0.172563, 0.0, 0.591524, -0.051003, 
        0.0),
    # CSC Parameters 
    # Det_Stations      p1        p2        p3       ep1      ep2      ep3
    #------------------------------------------------------------------------    #
    CSC_01 = cms.vdouble(0.394049, 0.001883, 0.0, 0.177829, 0.138271, 
        0.0),
    SMB_32 = cms.vdouble(0.632907, -0.255332, 0.0, 0.951006, -0.216346, 
        0.0),
    SMB_30 = cms.vdouble(0.54032, -0.064278, 0.0, 0.579023, -0.090837, 
        0.0),
    OL_2222 = cms.vdouble(0.067377, 0.0, 0.0, 0.76627, 0.0, 
        0.0),
    # Sigle DT Segments 
    # DTDet_Id          p1        p2        p3       ep1      ep2      ep3
    #------------------------------------------------------------------------    #
    SMB_10 = cms.vdouble(1.350718, -0.122906, 0.0, 0.118062, -0.079225, 
        0.0),
    SMB_11 = cms.vdouble(1.42858, -0.157879, 0.0, 0.075437, 0.031277, 
        0.0),
    SMB_12 = cms.vdouble(1.920222, -0.779074, 0.0, 0.065771, 0.066339, 
        0.0),
    DT_23 = cms.vdouble(0.136351, 0.04724, -0.089448, 0.220512, -0.056609, 
        0.167122),
    DT_24 = cms.vdouble(0.186817, 0.059153, -0.111626, 0.312139, -0.156575, 
        0.469607),
    SME_21 = cms.vdouble(1.155785, -0.364076, 0.0, -19.122395, 13.18083, 
        0.0),
    SME_22 = cms.vdouble(-0.739515, 0.769174, 0.0, 58.238261, -37.114818, 
        0.0),
    CSC_34 = cms.vdouble(0.464106, -0.507783, 0.141618, 159.877853, -112.931834, 
        21.376095),
    CSC_02 = cms.vdouble(-0.948241, 1.706327, -0.435503, 4.265801, -4.998774, 
        1.588346),
    SME_41 = cms.vdouble(-0.698854, 0.306105, 0.0, -44.919988, 17.730305, 
        0.0),
    DT_34 = cms.vdouble(0.040386, 0.007193, -0.016506, 1.514816, -1.460786, 
        1.555718),
    CSC_14 = cms.vdouble(-3.372616, 3.931644, -0.892588, 57.340432, -56.217921, 
        14.044798),
    OL_1222 = cms.vdouble(0.251087, 0.004875, 0.0, 0.282026, -0.001649, 
        0.0),
    CSC_13 = cms.vdouble(4.064405, -6.441406, 2.651774, 25.920833, -36.770089, 
        13.309607),
    CSC_12 = cms.vdouble(0.03475, -0.201493, 0.222517, 7.016939, -9.121204, 
        3.100699)
)


