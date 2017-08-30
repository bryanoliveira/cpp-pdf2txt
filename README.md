# PDF 2 Text Converter
This is a PDF to text converter prototype.
This project converts scanned files (.pdf) into a set of images, reads the containing characters using a multilayer perceptron, runs an autocorrector and then writes a .txt file with the results.

## How it (supposedly) works

### MLP Training
- Initialize with the following parameters:
> Learning rate = 150
> Sigmoid Slope = 0.014
> Weight bias = 30
> Number of Epochs = 300-600
> Mean error threshold value = 0.0002
- Initialize random weights
- Load training sets
### Preparation
- Read PDF from command-line
- Convert it to a set of images using pdftoppm
### Preprocessing (for each image)
- Convert to grayscale
- Apply threshold
- Erode
- Dilate
- Apply pose transformation
### Detection
- Find lines (y axis limits)
- Find characters/digits (x axis limits)
- Extract char matrix
- Resize pixel matrix to MLP's input size
### Recognition (for each char)
- Send matrix to MLP
- Concatenate result to output text
- (Learning phase) Compare results and backpropagate errors
### Presentation
- (If language is supported) apply autocorrection
- Save results in a .txt file and show it on the screen

## Usage
You have to have pdftoppm and build-essential running on a GNU/Linux distribution.

## Notes

Please note that this project is no longer being updated and this version is incomplete (the remaining scripts don't talk to each other but its main idea is already implemented). Some of the code may be written in portuguese.

You can contact me at bryanufg@gmail.com if you have any doubts about the code.
