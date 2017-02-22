% [img1, imgColorMap1] = imread('2.png');
% [img5, imgColorMap5] = imread('4.png');

% C = imfuse(img1,img5);
C = imread('capture-25-Jan-2017-26.png');
figure;
F = imshow(C);
pinfo = impixelinfo(F);
set(pinfo, 'Units', 'Normalized', 'Position',[.1 .05 .3 .05]);
set(gcf, 'WindowButtonDownFcn', @getMousePositionOnImage);

function getMousePositionOnImage(src, event)
handles = guidata(src);
disp(handles)
cursorPoint = get(handles.axesImage, 'CurrentPoint');
curX = cursorPoint(1,1);
curY = cursorPoint(1,2);
disp(curX)
disp(curY)
end