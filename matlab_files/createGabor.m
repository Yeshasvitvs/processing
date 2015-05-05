function [fr,fc]=createGabor

% function createGabor 

center(1)=16;
center(2)=16;
p=1;

[ft_mono, ft_bi]=temporal_filters;

for theta=0:0.25*pi:0.5*pi;
for i=1:32;
for j=1:32;
    pixel = [i,j];
    %Generating gabor filters of kernel 11X11, about a center value
    %using only single Theta value
    [fr(i,j),fc(i,j)]= gaborFilter(theta, center, pixel,1 ,100, 10);
%     F(i,j)=fr(i,j)+fc(i,j);
end
end



%function used to display the gabor filters
figure(p)
subplot(2,2,1)
title('Real')
surf(fr)
subplot(2,2,3)
imagesc(fr)

subplot(2,2,2)
title('Img')
surf(fc)
subplot(2,2,4)
imagesc(fc)

% subplot(2,3,3)
% title('Complex')
% surf(F)
% subplot(2,3,6)
% imagesc(F)
p=p+1;

end
end