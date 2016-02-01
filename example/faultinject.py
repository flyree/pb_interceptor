#!/usr/bin/python

import sys
import os
import getopt
import time
import random
import signal
import subprocess

#basedir = "/home/jshwei/Desktop/splash_time_automated"
#basedir = "."
currdir = "."
#progbin = currdir + "/libquantum"
progbin = "/home/pwu/proj/lulesh/LULESH"
#pinbin = "pin"
pinbin="/home/pwu/app/pin-2.11-49306-gcc.3.4.6-ia32_intel64-linux/pin"
#instcategorylib = "/ubc/ece/home/kp/grads/jwei/pin/source/tools/FaultInject/obj-intel64/instcategory.so"
#instcountlib = "/ubc/ece/home/kp/grads/jwei/pin/source/tools/FaultInject/obj-intel64/instcount.so"
#filib = "/ubc/ece/home/kp/grads/jwei/pin/source/tools/FaultInject/obj-intel64/faultinjection.so"
instcategorylib="/home/pwu/proj/pb_interceptor/obj-intel64/instcategory.so"
instcountlib="/home/pwu/proj/pb_interceptor/obj-intel64/instcount.so"
filib="/home/pwu/proj/pb_interceptor/obj-intel64/faultinjection.so"
#inputfile = currdir + "/inputs/input.2048"
outputdir = currdir + "/prog_output"
basedir = currdir + "/baseline"
errordir = currdir + "/error_output"
#memdir = currdir + "/memtrack"

if not os.path.isdir(outputdir):
  os.mkdir(outputdir)
if not os.path.isdir(basedir):
  os.mkdir(basedir)
if not os.path.isdir(errordir):
  os.mkdir(errordir)
#if not os.path.isdir(memdir):
#    os.mkdir(memdir)

timeout = 500

#optionlist = ["33", "5"]
optionlist = ["5"]

def execute( execlist):
	#print "Begin"
	#inputFile = open(inputfile, "r")
  global outputfile
  print ' '.join(execlist)
  #print outputfile
  outputFile = open(outputfile, "w")
  p = subprocess.Popen(execlist, stdout = outputFile)
  elapsetime = 0
  while (elapsetime < timeout):
    elapsetime += 1
    time.sleep(1)
    #print p.poll()
    if p.poll() is not None:
      print "\t program finish", p.returncode
      print "\t time taken", elapsetime
      #outputFile = open(outputfile, "w")
      #outputFile.write(p.communicate()[0])
      outputFile.close()
      #inputFile.close()
      return str(p.returncode)
  #inputFile.close()
  outputFile.close()
  print "\tParent : Child timed out. Cleaning up ... "
  p.kill()
  return "timed-out"
	#should never go here
  sys.exit(syscode)


def main():
  #clear previous output
  global run_number, optionlist, outputfile, errorfile
  outputfile = basedir + "/golden_output"
  execlist = [pinbin, '-t', instcategorylib, '--', progbin]
  execlist.extend(optionlist)
  execute(execlist)


  # baseline
  outputfile = basedir + "/golden_output"
  execlist = [pinbin, '-t', instcountlib, '--', progbin]
  execlist.extend(optionlist)
  execute(execlist)
  # fault injection
  for index in range(0, run_number):
    outputfile = outputdir + "/outputfile-" + str(index)
    errorfile = errordir + "/errorfile-" + str(index)
    execlist = [pinbin, '-t', filib, '-enablefi', '-memtrack', '--', progbin]
    execlist.extend(optionlist)
    ret = execute(execlist)
    if ret == "timed-out":
      error_File = open(errorfile, 'w')
      error_File.write("Program hang\n")
      error_File.close()
    elif int(ret) < 0:
      error_File = open(errorfile, 'w')
      error_File.write("Program crashed, terminated by the system, return code " + ret + '\n')
      error_File.close()
    elif int(ret) > 0:
      error_File = open(errorfile, 'w')
      error_File.write("Program crashed, terminated by itself, return code " + ret + '\n')
      error_File.close()

if __name__=="__main__":
  global run_number
  assert len(sys.argv) == 2 and "Format: prog fi_number"
  run_number = int(sys.argv[1])
  main()
