#!/usr/bin/env python
import os
from optparse import OptionParser

## set up the option parser
parser = OptionParser(
    usage="usage: %prog [options] ARG",
    description="Script to setup a project to create datacards for a given ditau decay channel. This directory will be created and all necessary inputs for datacard creation will be copied there. The decay channel and category can be chosen by command line option. The category is defined a code of type XX-YY, where XX corresponds to the analysis type SM, MSSM, ZTT (Ztt cross section measurement) and YY corresponds to the event category (depending on the analysis type). For more details have a look into the README file in the setup directory of the package. Without channel specification a blank set of configuration files for the uncertainty configuration will be copied into the project directory, which should be edited by the user. ARG corresponds to the name of the project directory."
    )
parser.add_option("-c", "--decay-channel", dest="channel", default="new", type="choice", help="Ditau decay channel. [Default: new]", choices=["new", "em", "et","mt"])
parser.add_option("-e", "--event-category", dest="category", default="", type="string", help="Event category. [Default: \"\"]")


## check number of arguments; in case print usage
(options,args) = parser.parse_args()
if len(args) < 1 :
    parser.print_usage()
    exit(1)
    
CMSSW_BASE = os.environ["CMSSW_BASE"]

target_path = args[0]
source_path = "%s/src/HiggsAnalysis/HiggsToTauTau/setup/%s" % (CMSSW_BASE, options.channel)

## setup the new project directory 
d = os.path.dirname("%s/log.txt" % target_path)
if not os.path.exists(d):
    os.makedirs(d)

if options.channel == "new" :
    os.system("cp -r %s/cgs.conf %s/cgs.conf" % (source_path, target_path))
    os.system("cp -r %s/unc.conf %s/unc.conf" % (source_path, target_path))
    os.system("cp -r %s/unc.vals %s/unc.vals" % (source_path, target_path))
else :
    os.system("cp -r %s/cgs-%s.conf %s/cgs.conf" % (source_path, options.category, target_path))
    os.system("cp -r %s/unc-%s.conf %s/unc.conf" % (source_path, options.category, target_path))
    os.system("cp -r %s/unc-%s.vals %s/unc.vals" % (source_path, options.category, target_path))
if not options.channel == "new" :
    target_path = "{target}/{channel}.inputs-{type}.root".format(
        target=target_path, channel=options.channel, type=options.category[:options.category.find('-')].lower()
        )
    if not os.path.exists(target_path) :
        os.system(
            "cp {source}/htt_{channel}.inputs-{type}.root {target}".format(
            source=source_path, channel=options.channel, type=options.category[:options.category.find('-')].lower(), target=target_path)
            )
else:
    os.system("cp -r %s/src/HiggsAnalysis/HiggsToTauTau/setup/README %s" % (CMSSW_BASE, target_path))
