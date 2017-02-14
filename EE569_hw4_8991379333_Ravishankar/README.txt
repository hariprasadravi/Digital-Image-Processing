
EE569 Homework Assignment #4
Date:     Dec 4, 2016
Name:     Hariprasad Ravishankar
ID:       8991379333
Email:    hravisha@usc.edu

Package Requirements:

LuaJIT
Image Package
nn Package
optim Package


Weight Initialization Script: Downloadable from https://github.com/e-lab/torch-toolbox/blob/master/Weight-init/weight-init.lua

'weight-init.lua'


Lua Scripts usage and description

Problem 1:

	p1_random_initialization.lua   - Trains and tests LeNet-5 with random initialzation on MNIST, and negative MNIST Data

		Usage: For training
	            th p1_random_initialization.lua

	 		  	For testing
	          	th p1_random_initialization.lua [Pretrained Network]

			  	For testing with negative MNIST
	           	th p1_random_initialization.lua [Pretrained Network] negative 

    p1_heuristic.lua 			- Trains and tests LeNet-5 with Heuristic initialzation on MNIST, and negative MNIST Data

	    Usage: For training
	           th p1_heuristic.lua

			  For testing
	           th p1_heuristic.lua [Pretrained Network]

			  For testing with negative MNIST
	           th p1_heuristic.lua [Pretrained Network] negative


Problem 2a:

	To test the two parameter settings on Negative MNIST


		th p1_random_initialization.lua [Pretrained Network] negative 

		th p1_heuristic.lua [Pretrained Network] negative


	Improvement Solutions


	p2a_modified_LeNet5.lua  - Adds 6 filters with inverted weights to Conv1 of a pretrained LeNet-5
								model on MNIST data as per " Understanding Convolutional Neural Networks with A
								Mathematical Model - C.C. Kuo "		

		Usage:
 		  	For testing with MNIST using modified LeNet-5
            	th p2a_modified_LeNet5.lua [Pretrained Network]

		  	For testing with negative MNIST using modified LeNet-5
            	th p2a_modified_LeNet5.lua [Pretrained Network] 'negative'

		  	For testing with negative MNIST with Conv1 weights inverted
            	th p2a_modified_LeNet5.lua [Pretrained Network] 'negative' 'inverted'


    Approach 2:
    p2a_mixedMNIST.lua        -  Trains LeNet-5 with 50% Negative MNIST training samples

     	Usage: For training with 50% negative MNIST
            	th p2a_mixedMNIST.lua

 		  	For testing with MNIST
            	th p2a_mixedMNIST.lua [Pretrained Network]

		  	For testing with negative MNIST
            	th p2a_mixedMNIST.lua [Pretrained Network] 'negative'


Problem 2b:

	p2b_MNIST-with-Bckgnd.lua  	- Trains MNIST with background images on LeNet-5

	 	Usage: For training
            	th p2b_MNIST-with-Bckgnd.lua

 		  	For testing
            	th p2b_MNIST-with-Bckgnd.lua [Pretrained Network]


Problem 2c:

	p2c_translatedMNIST.lua   - Generates translated test and train MNIST data to train on LeNet-5


	 Usage: For training with translated MNIST data
            	th p2c_translatedMNIST.lua

 		  	For testing a pretrained LeNET-5 model on translated MNIST test data
            	th p2c_translatedMNIST.lua [Pretrained Network]


     Approach 1:

     p2c_LeNet5_3x3MaxPool.lua  -  Modifies the LeNet5 to have max-pooling of 3x3 with 2 pixel stride

     Usage: For training
            	th p2c_LeNet5_3x3MaxPool.lua

 		  	For testing
            	th p2c_LeNet5_3x3MaxPool.lua [Pretrained Network]








