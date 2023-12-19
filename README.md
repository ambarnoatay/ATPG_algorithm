# ATPG_algorithm
PODEM (Path-Oriented Decision Making) is an Automatic Test Pattern Generation (ATPG) algorithm.

# Deductive fault simulator

Given the input test vector, Deductive Fault Simulator will generate fault lists for all the nets using list operations (Union,Intersection,etc). 

# Implementation:
For input nets, invert their values as the fault to be deduced and insert in the fault set vector.

For each subsequent net, which will be an output to a gate, based on the gateid (OR,AND,XOR), figure out the function for the fault set operation between the two inputs and the output.

For Example, an AND gate with input nets 2 and 3 and output net 4. Say, net 2 has value 0, net3 has 1 and output is 0. The Fault set operation will be: (L2 - L3)Union(4 s-a-1). The result will be stored in the fault set vector of net 4.

Solve for all nets and the fault list at output is the final list of faults detected using Deductive Fault Simulation.

# PODEM

Initially, set all input nets to X (unknown state). Run the PODEM algorithm which should return "SUCCESS" or "FAILURE". 

If PODEM returns SUCCESS, we can find a test vector such that the given fault is excited. Otherwise, the fault is undetectable.

Run the test vector returned by PODEM through the DFS to verify the fault is detected.


# Steps to run the Deductive Fault Simulator:

1. Store the Deductive_FaultSim.cpp file in a directory (say D:/Project/).
   
2. Copy the three text files: inputckt.txt, faults.txt and test.txt.
   
3. inputckt.txt contains the circuit details. If you want to load a new circuit, you can copy the circuit from your text file to this text file.

4. faults.txt contains the list of faults you want to detect. Note: the deductive fault simulator will only display a fault if present in this text file.

5. test.txt contains the test vector you want to run. Again, you can only edit one line in which the test vector is present. If you add multiple rows of test vector, only the first one will be used.

6. Now that you have setup the files, open the terminal from your directory and run the command: g++ -o DFS Deductive_FaultSim.cpp

7. Then, run the command: ./DFS

8. It should display the results of the Deductive Fault simulation.


# Steps to run the PODEM Simulator:

1. Store the PODEM.cpp file in a directory (say D:/Project/).
   
2. Copy the three text files: inputckt.txt, faults.txt and test.txt.
   
3. inputckt.txt contains the circuit details. If you want to load a new circuit, you can copy the circuit from your text file to this text file.

4. faults.txt contains the list of faults you want to detect. Note: the deductive fault simulator will only display a fault if present in this text file.

5. test.txt contains the test vector you want to run. Again, you can only edit one line in which the test vector is present. If you add multiple rows of test vector, only the first one will be used.

6. Now that you have setup the files, open the terminal from your directory and run the command: g++ -o podem PODEM.cpp

7. Then, run the command: ./podem

8. It should display the results of PODEM and simulate the PODEM inputs through the deductive fault simulator.
