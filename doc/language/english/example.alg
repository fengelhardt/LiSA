# This is how to write a LiSA algorithm description file. 
#
# Make shure your algorithm accepts a call 
# name input_file output_file
# where the input file contains the objects <CONTROLPARAMETERS>,
# <PROBLEMTYPE>, <VALUES> (and <SCHEDULE>) in this order. Then follow
# the instructions in the lines starting with #
 

<GENERAL>
# Fill in a name of your algorithm which appears in the menues:
Name= 
# Change the following line to "Type= iterative" if your algorithm
# needs a start solution 
Type= constructive
# Fill in the file name of the executable (full path if not in LiSA/bin):
Call= 
Code= external
# Fill in the URL of a help file (filename if in LiSA/doc/language/english):
Help= 
</GENERAL>

# The following entries are necessary for LiSA to deceide wether your
# algorithm is applicable to a problem type. A wrong entry here may
# cause LiSA to compute wrong results.

<EXACT>
# insert <PROBLEMTYPE>-entries for each maximal problem which can be
# solved exactly by your algorithm. (Entering the problem into LiSA and
# writing it to a file might help you getting the correct syntax.)
# Example: <PROBLEMTYPE> Lisa_ProblemType= { J / r_i  / SumWiUi }
#          </PROBLEMTYPE>

</EXACT>

<HEURISTIC>
# Insert <PROBLEMTYPE>-entries for each maximal problem which can be
# solved heuristically by your algorithm.

</HEURISTIC>

<PARAMETERS>
# For each parameter to be given to your algorithm via the
# <CONTROLPARAMETERS>-object, insert a line containing its type, name,
# default value and a short decription which will occur in the window
# in place of the name of the parameter, for instance 
# long NB_SOLUTIONS 1 "Number of Solutions"


</PARAMETERS>

# Now copy this file to $LISA_HOME/data/extalg/language/english/ and
# start LiSA. Your algorithm will be registered during the start. 








