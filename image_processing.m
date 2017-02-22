% read image
% [img, imgColorMap] = imread('capture.png');
[img, imgColorMap] = imread('gridtest.jpg');
[rows, columns, numColorBands] = size(img); 

% gaussian the image
gauss_img = imgaussfilt(img,2);

% sharpen image
sharpened = imsharpen(gauss_img);
imwrite(sharpened,'sharp.png');

% convert to grayscale
grayscale = rgb2gray(sharpened);

% calculate normalized image and get an intensity mask
normalized = grayscale;
threshold = 188;
numOver = length(find(normalized > threshold))
normalized(normalized < threshold) = 0;
normalized(normalized >= threshold) = 1;
intensityMask = grayscale .* normalized;

% convert to HSV
hsvImage = rgb2hsv(img);
% convert sharpened to HSV
hsvSharp = rgb2hsv(sharpened);

% create sobel
% sobel = fspecial('sobel');
% filter sobel
% sobeled = imfilter(grayscale, sobel);

% get hue mask
hueThresholdLow = 0.3;
hueThresholdHigh = 1.0;
hueMask = (hsvSharp(:,:,1) >= hueThresholdLow) & (hsvSharp(:,:,1) <= hueThresholdHigh);

% sum hue mask with intensity mask
index = (intensityMask ~= 0);
hueMask(index) = intensityMask(index);

% get rid of small areas
smallestAcceptableArea = 50;
filteredHueMask = uint8(bwareaopen(hueMask, smallestAcceptableArea));

% border smooth
structuringElement = strel('disk', 4);
smoothedHueMask = imclose(filteredHueMask, structuringElement);

% region filling
filledHueMask = imfill(logical(smoothedHueMask), 'holes');

% mask image
filledHueMask = cast(filledHueMask, 'like', img);
maskedImageR = filledHueMask .* img;

%% 

% show sharpened
figure;
imshow(sharpened);
drawnow;
title('sharpened')

% show sobel
% figure;
% imshow(sobeled);
% drawnow;
% title('sobel')

% show hsv
subplot(1,2,1)
h1 = imshow(hsvImage(:,:,1));
hHuePI = impixelinfo(h1);
set(hHuePI, 'Units', 'Normalized', 'Position',[.1 .2 .4 .1]);
title('clean hue')
subplot(1,2,2)
imshow(hsvSharp(:,:,1));
title('sharp hue')

% show hue mask
figure;
subplot(1,3,1)
imshow(hueMask);
title('hue mask')

subplot(1,3,2)
imshow(intensityMask)
title('intensity mask')

subplot(1,3,3)
imshow(filledHueMask);
title('filled mask')

% show masked image
figure;
imshow(maskedImageR);
drawnow;
title('masked image red')

%% 

% Feature Detection

grayscale_masked = rgb2gray(maskedImageR);
corners = detectMinEigenFeatures(grayscale_masked);

figure;
imshow(img); hold on;
plot(corners.selectStrongest(100));















