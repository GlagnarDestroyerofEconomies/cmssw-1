#! /usr/bin/env cmsRun

import FWCore.ParameterSet.Config as cms

process = cms.Process("merge")
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'file:material_001.root', 
        'file:material_002.root', 
        'file:material_003.root', 
        'file:material_004.root', 
        'file:material_005.root', 
        'file:material_006.root', 
        'file:material_007.root', 
        'file:material_008.root', 
        'file:material_009.root', 
        'file:material_010.root', 
        'file:material_011.root', 
        'file:material_012.root', 
        'file:material_013.root', 
        'file:material_014.root', 
        'file:material_015.root', 
        'file:material_016.root', 
        'file:material_017.root', 
        'file:material_018.root', 
        'file:material_019.root', 
        'file:material_020.root', 
        'file:material_021.root', 
        'file:material_022.root', 
        'file:material_023.root', 
        'file:material_024.root', 
        'file:material_025.root', 
        'file:material_026.root', 
        'file:material_027.root', 
        'file:material_028.root', 
        'file:material_029.root', 
        'file:material_030.root', 
        'file:material_031.root', 
        'file:material_032.root', 
        'file:material_033.root', 
        'file:material_034.root', 
        'file:material_035.root', 
        'file:material_036.root', 
        'file:material_037.root', 
        'file:material_038.root', 
        'file:material_039.root', 
        'file:material_040.root', 
        'file:material_041.root', 
        'file:material_042.root', 
        'file:material_043.root', 
        'file:material_044.root', 
        'file:material_045.root', 
        'file:material_046.root', 
        'file:material_047.root', 
        'file:material_048.root', 
        'file:material_049.root', 
        'file:material_050.root', 
        'file:material_051.root', 
        'file:material_052.root', 
        'file:material_053.root', 
        'file:material_054.root', 
        'file:material_055.root', 
        'file:material_056.root', 
        'file:material_057.root', 
        'file:material_058.root', 
        'file:material_059.root', 
        'file:material_060.root', 
        'file:material_061.root', 
        'file:material_062.root', 
        'file:material_063.root', 
        'file:material_064.root', 
        'file:material_065.root', 
        'file:material_066.root', 
        'file:material_067.root', 
        'file:material_068.root', 
        'file:material_069.root', 
        'file:material_070.root', 
        'file:material_071.root', 
        'file:material_072.root', 
        'file:material_073.root', 
        'file:material_074.root', 
        'file:material_075.root', 
        'file:material_076.root', 
        'file:material_077.root', 
        'file:material_078.root', 
        'file:material_079.root', 
        'file:material_080.root', 
        'file:material_081.root', 
        'file:material_082.root', 
        'file:material_083.root', 
        'file:material_084.root', 
        'file:material_085.root', 
        'file:material_086.root', 
        'file:material_087.root', 
        'file:material_088.root', 
        'file:material_089.root', 
        'file:material_090.root', 
        'file:material_091.root', 
        'file:material_092.root', 
        'file:material_093.root', 
        'file:material_094.root', 
        'file:material_095.root', 
        'file:material_096.root', 
        'file:material_097.root', 
        'file:material_098.root', 
        'file:material_099.root', 
        'file:material_100.root', 
        'file:material_101.root', 
        'file:material_102.root', 
        'file:material_103.root', 
        'file:material_104.root', 
        'file:material_105.root', 
        'file:material_106.root', 
        'file:material_107.root', 
        'file:material_108.root', 
        'file:material_109.root', 
        'file:material_110.root', 
        'file:material_111.root', 
        'file:material_112.root', 
        'file:material_113.root', 
        'file:material_114.root', 
        'file:material_115.root', 
        'file:material_116.root', 
        'file:material_117.root', 
        'file:material_118.root', 
        'file:material_119.root', 
        'file:material_120.root', 
        'file:material_121.root', 
        'file:material_122.root', 
        'file:material_123.root', 
        'file:material_124.root', 
        'file:material_125.root', 
        'file:material_126.root', 
        'file:material_127.root', 
        'file:material_128.root', 
        'file:material_129.root', 
        'file:material_130.root', 
        'file:material_131.root', 
        'file:material_132.root', 
        'file:material_133.root', 
        'file:material_134.root', 
        'file:material_135.root', 
        'file:material_136.root', 
        'file:material_137.root', 
        'file:material_138.root', 
        'file:material_139.root', 
        'file:material_140.root', 
        'file:material_141.root', 
        'file:material_142.root', 
        'file:material_143.root', 
        'file:material_144.root', 
        'file:material_145.root', 
        'file:material_146.root', 
        'file:material_147.root', 
        'file:material_148.root', 
        'file:material_149.root', 
        'file:material_150.root', 
        'file:material_151.root', 
        'file:material_152.root', 
        'file:material_153.root', 
        'file:material_154.root', 
        'file:material_155.root', 
        'file:material_156.root', 
        'file:material_157.root', 
        'file:material_158.root', 
        'file:material_159.root', 
        'file:material_160.root', 
        'file:material_161.root', 
        'file:material_162.root', 
        'file:material_163.root', 
        'file:material_164.root', 
        'file:material_165.root', 
        'file:material_166.root', 
        'file:material_167.root', 
        'file:material_168.root', 
        'file:material_169.root', 
        'file:material_170.root', 
        'file:material_171.root', 
        'file:material_172.root', 
        'file:material_173.root', 
        'file:material_174.root', 
        'file:material_175.root', 
        'file:material_176.root', 
        'file:material_177.root', 
        'file:material_178.root', 
        'file:material_179.root', 
        'file:material_180.root', 
        'file:material_181.root', 
        'file:material_182.root', 
        'file:material_183.root', 
        'file:material_184.root', 
        'file:material_185.root', 
        'file:material_186.root', 
        'file:material_187.root', 
        'file:material_188.root', 
        'file:material_189.root', 
        'file:material_190.root', 
        'file:material_191.root', 
        'file:material_192.root', 
        'file:material_193.root', 
        'file:material_194.root', 
        'file:material_195.root', 
        'file:material_196.root', 
        'file:material_197.root', 
        'file:material_198.root', 
        'file:material_199.root', 
        'file:material_200.root')
)

process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('file:../../material.root')
)

process.end = cms.EndPath(process.out)
