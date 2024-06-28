README
Introduction:- It is c++ code to provide multithreading matrix multiplication using setting affinity or normal scheduling
my file name is CE21BTECH11008_Assignment 2

//experiment-1

//bunch type without bounding
file name:- compile using:- g++ Assgn2_src_Bunch_CE21BTECH11008.cpp -o Assgn2_src_Bunch_CE21BTECH11008
execute:- ./Assgn2_src_Bunch_CE21BTECH11008.out
Input-first line N K
      Next line onwards matrix

//bunch type with bounding for given BT
file name:- compile using:- g++ Assgn2_src_CE21BTECH11008_Bunch _affinity.cpp -o Assgn2_src_CE21BTECH11008_Bunch _affinity
execute:- ./Assgn2_src_CE21BTECH11008_Bunch _affinity.out
Input-first line N K C BT
      Next line onwards matrix

//mixed type without bounding
file name:- compile using:- g++ Assgn2_src_Mixed_CE21BTECH11008.cpp -o Assgn2_src_Mixed_CE21BTECH11008
execute:- ./Assgn2_src_Mixed_CE21BTECH11008.out
Input-first line N K
      Next line onwards matrix

//mixed type with bounding for given BT
file name:- compile using:- g++ Assgn2_src_mixed_affinity_CE21BTECH11008.cpp -o Assgn2_src_mixed_affinity_CE21BTECH11008
execute:- ./Assgn2_src_mixed_affinity_CE21BTECH11008.out
Input-first line N K C BT
      Next line onwards matrix


//experiment-2

/bunch type without bounding
file name:- compile using:- g++ Assgn2_src_Bunch_CE21BTECH11008.cpp -o Assgn2_src_Bunch_CE21BTECH11008
execute:- ./Assgn2_src_Bunch_CE21BTECH11008.out
Input-first line N K
      Next line onwards matrix
      
/bunch type with half bounding
file name:- compile using:- g++ Assgn2_src_Bunch_CE21BTECH11008.cpp -o Assgn2_src_Bunch_CE21BTECH11008
execute:- ./Assgn2_src_Bunch_CE21BTECH11008.out
Input-first line N K C
      Next line onwards matrix
      
//mixed type without bounding
file name:- compile using:- g++ Assgn2_src_half_mixed_CE21BTECH11008.cpp -o Assgn2_src_half_mixed_CE21BTECH11008
execute:- ./Assgn2_src_half_mixed_CE21BTECH11008.out
Input-first line N K
      Next line onwards matrix
      
//mixed type with half bounding
file name:- compile using:- g++ Assgn2_src_half_mixed_CE21BTECH11008.cpp -o Assgn2_src_half_mixed_CE21BTECH11008
execute:- ./Assgn2_src_half_mixed_CE21BTECH11008.out
Input-first line N K C
      Next line onwards matrix
      
      
"g++ source.cpp -o output"  can also be used for getting executable file name as "output"
  
take input from : inp.txt
outputfile : outfile.txt
runnung will give the result outfile.txt

//to check the  ****performance**** used perf commnand
perf stat -e cache-misses,cache-references./<executable_file>

//keeping the executable_file and note this perf command is external so first install then use
