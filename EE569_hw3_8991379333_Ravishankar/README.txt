# EE569 Homework Assignment #3
# Date:     Nov 6, 2016
# Name:     Hariprasad Ravishankar
# ID:       8991379333
# Email:    hravisha@usc.edu

# Compiled in Ubuntu 16.01 terminal using g++ 11
# on Sublime Text


##  Package requirements
# OpenCV 3.0
# OpenCV_contrib 
# g++ 11 compiler
# cmake
# MATLAB Image Processing Toolbox
# Piotr's Matlab Toolbox (version 3.26 or later) https://pdollar.github.io/toolbox/
# Structured Edge Detection Toolbox V3.0 https://github.com/pdollar/edges

## For package installing instruction please follow link above

# Instructions to execute programs 

# Problem 1a Texture Classification

	#1. Navigate to Texture_Classification folder inside P1 folder	
	#2. Populate this folder with images Texture1.raw to Texture12.raw
	#3. Enter the following command in the terminal. NOTE : if Build error after make, enter cmake . again in terminal, then make
		cmake .
		make
		./p1a_TextureClassification

# Problem 1b Texture Segmentation

	#1. Navigate to Texture_Segmentation folder inside P1 folder	
	#2. Populate this folder with images Comb_1.raw and Comb_2.raw
	#3. Enter the following command in the terminal. NOTE : if Build error after make, enter cmake . again in terminal, then make
		cmake .
		make
		./p1b_TextureSegmentation Comb_1.raw 1 400 525
		./p1b_TextureSegmentation Comb_2.raw 1 512 512

# Problem 2 SIFT Features and Matches

	#1. Navigate to SIFT folder inside P2 folder	
	#2. Populate this folder with images bus.jpg,  jeep.jpg, rav4_1.jpg, rav4_2.jpg
	#3. Enter the following command in the terminal. NOTE : if Build error after make, enter cmake . again in terminal, then make
		cmake .
		make
		./p2_SIFT bus.jpg jeep.jpg

# Problem 2 SURF Features and Matches

	#1. Navigate to SURF folder inside P2 folder	
	#2. Populate this folder with images bus.jpg,  jeep.jpg, rav4_1.jpg, rav4_2.jpg
	#3. Enter the following command in the terminal. NOTE : if Build error after make, enter cmake . again in terminal, then make
		cmake .
		make
		./p2_SURF bus.jpg jeep.jpg

# Problem 2c Bag of Words Matching

	#1. Navigate to Bag_of_words folder inside P2 folder	
	#2. Populate this folder with images bus.jpg,  jeep.jpg, rav4_1.jpg, rav4_2.jpg
	#3. Enter the following command in the terminal. NOTE : if Build error after make, enter cmake . again in terminal, then make
		cmake .
		make
		./p2c_BoW jeep.jpg bus.jpg rav4_1.jpg rav4_2.jpg





