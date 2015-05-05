function [Fr,Fc] = gaborFilter(theta, center, pixel, gamma, lambda, sigma)

x = pixel(1);
y = pixel(2);

u = center(1);
v = center(2);

xdash = (x )*cos(theta) + (y )*sin(theta);
ydash = - (x )*sin(theta) + (y )*cos(theta);


temp = - 0.5 * (xdash*xdash + gamma*gamma*ydash*ydash)/(2*sigma*sigma);
% temp = 1;

Fr = exp(temp)*cos(2*pi*xdash/lambda);
Fc = exp(temp)*sin(2*pi*xdash/lambda);

% imagesc(Fs)
% [x y xdash ydash Fs]