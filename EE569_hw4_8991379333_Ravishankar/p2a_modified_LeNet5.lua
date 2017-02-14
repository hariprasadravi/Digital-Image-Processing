
-- EE569 Homework Assignment #4
-- Date:     Dec 4, 2016
-- Name:     Hariprasad Ravishankar
-- ID:       8991379333
-- Email:    hravisha@usc.edu


-- Usage:
-- 		  For testing with MNIST using modified LeNet-5
--            th p2a_modified_LeNet5.lua [Pretrained Network]

--		  For testing with negative MNIST using modified LeNet-5
--            th p2a_modified_LeNet5.lua [Pretrained Network] 'negative'

--		  For testing with negative MNIST with Conv1 weights inverted
--            th p2a_modified_LeNet5.lua [Pretrained Network] 'negative' 'inverted'

-- This script adds 6 extra filters in the Conv1 using 
-- the pretrained LeNet-5 model that is passed. The weights of the 
-- first 6 filters of Conv1 are inverted and concatenated to form 12 layers of Conv1

-- The weights of dim 2 of conv 2 are replicated and concatenated to forwardpass model


require "nn"
require "optim"
require "image"
require "gnuplot"

-- Fixing seed to get consistent results
torch.manualSeed(0)

-- Declaring 10 class labels
classes = {'0','1','2','3','4','5','6','7','8','9'}

-- This function loads the train and test datasets from the disk
function loadDataset(flag)
	print('Loading MNIST Dataset...')
	trainset = torch.load('mnist-p1b-train.t7')

	testset = torch.load('mnist-p1b-test.t7')

	-- Casting to double type
	trainset.data = trainset.data:double()

	testset.data = testset.data:double()

	-- check flag
	if flag == 'negative' then
		print('Inverting test data...')
		testset.data = 255 - testset.data
	end


end

-- This function creates the modified LeNet-5 model
-- by loading a pretrained model that is 
-- passed as argument

function createModel(loadExisting, inverted)

	print('Creating Model...')
	if loadExisting ~= nil then
		net = torch.load(loadExisting)

		if inverted == 'inverted' then
			print('Inverting weights of Conv1...')
			net:get(1).weight:mul(-1)

		else
			print('Creating modified LeNet-5 with 12 Conv1 filters...')	
			-- Clone weights of Conv1
			C1_weights = net:get(1).weight:clone()

			-- Clone bias of Conv1
			C1_bias = net:get(1).bias:clone()

			-- Multiply Conv1 weights by -1
			C1_weights:mul(-1)

			-- Concatenate new weights to Conv1
			net:get(1).weight = torch.cat(net:get(1).weight,C1_weights,1)

			-- Concatenate biases for Conv1
			net:get(1).bias = torch.cat(net:get(1).bias,C1_bias,1)

			-- Clone weights of Conv2
			C2_weights = net:get(4).weight:clone()

			-- Concatenate Cloned weights to Conv2 along dimension 2
			net:get(4).weight = torch.cat(net:get(4).weight,C2_weights,2)

			torch.save('modified_LeNet-5.t7',net)
		end

	end  
end

function testing()

	-- Cost function is Negative Loss Likelihood
	criterion = nn.ClassNLLCriterion()

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
	
end

function main()

	-- Load pretrained model passed as argument
	PreloadedModel = arg[1]

	-- check for negative flag
	Negative = arg[2]

	-- inverted weights keywork
	inverted = arg[3]

	if Negative ~= 'negative' then
		inverted = Negative
	end


	-- Call function to load dataset
	loadDataset(Negative)

	-- Check if Pretrained model is passed
	if PreloadedModel == nil then
		print("Error! Please pass pretrained LeNet-5 model!")

	else
		createModel(PreloadedModel, inverted)
		testing()
	end
	


end
main()