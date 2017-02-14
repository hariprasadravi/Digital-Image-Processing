
-- EE569 Homework Assignment #4
-- Date:     Dec 4, 2016
-- Name:     Hariprasad Ravishankar
-- ID:       8991379333
-- Email:    hravisha@usc.edu

-- Usage: For training
--            th p2c_translatedMNIST.lua

-- 		  For testing
--            th p2c_translatedMNIST.lua [Pretrained Network]


-- This script tests a pretrained LeNet-5 model on
-- translated MNIST test data. Further, the script can
-- also be used to train the LeNet-5 model on translated MNIST training data by
-- not specifying any argument.


require "nn"
require "optim"
require "image"
require "gnuplot"

-- Fixing seed to get consistent results
torch.manualSeed(0)

-- Declaring 10 class labels
classes = {'0','1','2','3','4','5','6','7','8','9'}

-- Function that returns translated MNIST images
function TranslatedDataset(images)

	for i = 1,images:size(1) do
		-- Translate in x and y directions 
		images[i] = image.translate(images[i],torch.random(-5,5),torch.random(-5,5))

	end
	return images
end

-- This function loads the train and test datasets from the disk
function loadDataset()
	print('Loading MNIST Dataset...')
	trainset = torch.load('mnist-p1b-train.t7')

	testset = torch.load('mnist-p1b-test.t7')

	-- Casting to double type
	trainset.data = trainset.data:double()

	-- Call function to return translated images
	trainset.data = TranslatedDataset(trainset.data):clone()

	testset.data = testset.data:double()

	-- Call function to return translated images
	testset.data = TranslatedDataset(testset.data):clone()
	

end

-- This function creates the LeNet-5 model
-- or loads a pretrained model that is 
-- passed as argument

function createModel(loadExisting)

	print('Creating Model...')
	if loadExisting ~= nil then
		net = torch.load(loadExisting)


	else

		-- Define a sequential model
		net = nn.Sequential()

		-- Convolutional Layer with 1 input channel, 6 output channels, 5x5 kernel 
		net:add(nn.SpatialConvolution(1, 6, 5, 5))

              
		-- ReLU Non-Linearity
		net:add(nn.ReLU())

		-- Max-pooling layer with 2x2 window tih stride 2
		net:add(nn.SpatialMaxPooling(2,2,2,2))   

		-- Convolutional Layer with 6 input channels, 16 output channels, 5x5 kernel
		net:add(nn.SpatialConvolution(6, 16, 5, 5))
		
		-- ReLU Non-Linearity
		net:add(nn.ReLU())                       
		
		-- Max-pooling layer with 2x2 window tih stride 2
		net:add(nn.SpatialMaxPooling(2,2,2,2))

		-- reshapes from a 3D tensor of 16x5x5 into 1D tensor of 16*5*5
		net:add(nn.View(16*5*5))

		-- Dense layer with 16*5*5 inputs and 120 neurons
		net:add(nn.Linear(16*5*5, 120)) 

		-- ReLU Non-Linearity
		net:add(nn.ReLU())  
						       	
		-- Dense layer with 120 inputs and 84 neurons            
		net:add(nn.Linear(120, 84))

		-- ReLU Non-Linearity
		net:add(nn.ReLU())                       
		
		 -- 10 is the number of outputs of the network (in this case, 10 digits)
		net:add(nn.Linear(84, 10))    

		-- SoftMax layer to output Probability Scores
		net:add(nn.LogSoftMax())
	end  
end

function training(loadExisting)

	-- Cost function is Negative Loss Likelihood
	criterion = nn.ClassNLLCriterion()

	-- Declare Confution Matrix
	confusion = optim.ConfusionMatrix(classes)

	-- Variables to store epoch count, train accuracy and test accuracy
	epochCount ={}
	accuracyCount ={}
	accuracyCountTest ={}

	-- Check if preloaded model is present
	if loadExisting == nil then
		
		-- zero all gradients initially
		net:zeroGradParameters()

		print('Training...')
		
		-- Get Weights and Gradients of Cost w.r.t weights
		parameters,gradParameters = net:getParameters()

		-- Set Number of Epochs
		local nEpoch = 5

		-- Initiate training
		for e = 1,nEpoch do
			
		  	-- Number of training samples.
			local size  = trainset.data:size()[1]

			-- 	Batch size
			local bsize = 128

			 --	Total loss.
			local tloss = 0

			for t  = 1,size,bsize do
				
				-- Check for last batch
				local bsize = math.min(bsize,size-t+1)
				
				-- load input batch
			  	local input  =trainset.data:narrow(1,t,bsize)
			  
			  	-- load batch targets
			  	local target = trainset.label:narrow(1,t,bsize)

			  	-- Reset the gradients to zero.
			  	gradParameters:zero()
			  				  	
			  	-- forward propagate input for evert batch
			  	local output = net:forward(input)

			  	-- Compute loss for output
			  	local loss   = criterion:forward(output,target)

			  	-- Collect Statistics
			  	tloss = tloss + loss * bsize
			  	
			  	-- Add output and target to confusion matix
			  	confusion:batchAdd(output,target)
			  	
			  	-- Compute gradients w.r.t parameters across all layers
			  	local gradOutput = criterion:backward(output,target)

			  	-- update weights
			  	net:backward(input,gradOutput)

			  	-- Dummy function to return loss and gradParam for optim
				local function feval()
			    	return loss,gradParameters
			  	end

			  	-- SGD Parameters
				local config = {
				   learningRate = 0.015,
				   learningRateDecay = 1e-1,
				   weightDecay = 1e-2,
				   momentum = 0.99 
				}
								  	
			  	-- We use the SGD method.
			  	optim.sgd(feval, parameters,config)
			  	
			  	-- Show the progress.
			  	io.write(string.format("progress: %4d/%4d\r",t,size))
			  	io.flush()
			end
			-- Compute the average loss.
			tloss = tloss / size

			-- Update the confusion matrix.
			confusion:updateValids()

			-- Print Loss and Accuracy for every Epoch
			print(string.format('epoch = %2d/%2d  loss = %.2f accuracy = %.2f',e,nEpoch,tloss,100*confusion.totalValid))

			-- Print Confusion Matrix
			--	print(confusion)
			
			-- Stack epoch count to table
			table.insert(epochCount,e)

			-- Stack train accuracy to table
			table.insert(accuracyCount,100*confusion.totalValid)
			
			-- Stack test accuracy to table
			table.insert(accuracyCountTest,testing())
			print('')
			
		end

		-- Convert to tensor type
		epochCount = torch.Tensor(epochCount)
		accuracyCount = torch.Tensor(accuracyCount)
		accuracyCountTest = torch.Tensor(accuracyCountTest)

		-- Save Plot as PNG
		gnuplot.pngfigure('LeNet-5_Translated_training.png')
		gnuplot.title('Epoch Vs Accuracy Curve')
		gnuplot.xlabel(' Number of Epochs')
		gnuplot.ylabel(' Accuracy ')
		gnuplot.plot(
			{'Training',epochCount,accuracyCount,'-'},
			{'Testing', epochCount,accuracyCountTest,'-'}
			)
		gnuplot.plotflush()  

		-- Clean temporary data to reduce the size of the network file.
		net:clearState()
		
		-- Save the network.
		torch.save('LeNet-5_Translated_training.t7',net)
	end
end

function testing()

	-- Define local confusion matrix for testing
	local confusion = optim.ConfusionMatrix(classes)
	
	-- Preclear entries
	confusion:zero()

	-- Size of testdata
	local size  = testset.data:size()[1]

	-- 	Batch size.
	local bsize = 128

	--	Total loss.
	local tloss = 0

	-- mAP 
	local mAP = 0

	-- Total batches
	local totalbatches=0
	
	-- Test batchwise
	for t  = 1,size,bsize do

		-- Check for last batch
		local bsize = math.min(bsize,size-t+1)

		-- Load input batch
	  	local input  = testset.data:narrow(1,t,bsize)
	  
	  	-- Load target batch
	  	local target = testset.label:narrow(1,t,bsize)

	  	-- Forward the data and compute the loss.
	  	local output = net:forward(input)
	  	
	  	-- Compute loss 
	  	local loss   = criterion:forward(output,target)

	  	-- Collect Statistics
	  	tloss = tloss + loss * bsize

	  	-- Add prediction and target to confusion matrix
	  	confusion:batchAdd(output,target)

	  	-- mAP

	  	-- Sort Predictions in descending order
	  	local confidences, indices = torch.sort(output, true)

	  	-- Clear AP
	  	local AP =0

	  	-- Clear Correct count
	  	local correct = 0

	  	-- Typecast to long
	  	target = target:long()
		
		-- Returns a binary vector corresponding to correct prediction
	  	local prediction = torch.eq(indices[{ {},{1}}], target)
	  
	  	-- Iterate over each element in binary vector
 		for i = 1,bsize do
  			
  			-- Check if prediction is correct
  			if(torch.eq(prediction[i],1)[1] == 1) then

  				-- Increment Correct count
  				correct = correct + 1
  				
  				-- Sum
  				AP = AP + (correct/i)
  			end

  		end
  		
  		-- Count number of batches
  		totalbatches = totalbatches + 1

  		-- Accumalate mAP for every batch
  		mAP = mAP + AP/correct

	  
	  	-- Show the progress.
	  	io.write(string.format("progress: %4d/%4d\r",t,size))
	  	io.flush()
	end
	
	-- Average mAP over all batches
	mAP = mAP/totalbatches

	-- Compute Average Loss
	tloss = tloss / size


	-- Update the confusion matrix.
	confusion:updateValids()

	-- Print Loss and Prediction
	print(string.format('Test loss = %.2f Test accuracy = %.2f',tloss,100*confusion.totalValid))
	
	-- Print Confusion Matrix
	--	print(confusion)
	
	-- Print mAP score
	print(string.format('mAP = %.3f',mAP))
	
	-- Returns accuracy
	return 100*confusion.totalValid
end

function main()

	-- Load pretrained model passed as argument
	PreloadedModel = arg[1]

	-- Call function to load dataset
	loadDataset()

	-- Check if Pretrained model is passed
	if PreloadedModel == nil then
		createModel()
		training()		

	else
		createModel(PreloadedModel)
		training(1)	
		testing()
	end
	


end
main()