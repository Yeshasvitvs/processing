function [act1 act2 act] = computeSTG( x1, y1, t )
%COMPUTESTG Summary of this function goes here
%   Detailed explanation goes here

theta = 2 * pi;

x = x1 * cos(theta) + y1 * sin(theta);
y = -x1 * sin(theta) + y1 * cos(theta);


fx0 = 0.057;
fy0 = 0.057;
var = 25;
%Monophasic Filter
u_mono = 0.28;
sig_mono = 0.07;
%Biphasic Filter
s1=0.705;
s2=0.8;

u_bi1 = 0.18;
u_bi2 = 0.42;
sig_bi1 = 0.082;
sig_bi2 = 0.18;


% im_spatial = 2 * pi * (1/var^2) * sin(2 * pi  * (fx0 * x + fy0 * y)) * exp(-2 * pi^2 * (x^2 + y^2) / var^2);
im_spatial = 2 * pi * (1/var^2) * sin(2 * pi  * (fx0 * x)) * exp(-2 * pi^2 * (x^2 + y^2) / var^2);
mono_temporal = exp(-0.5 * (t-u_mono)^2 / sig_mono^2);

% real_spatial = 2 * pi * (1/var^2) * cos(2 * pi  * (fx0 * x + fy0 * y)) * exp(-2 * pi^2 * (x^2 + y^2) / var^2);
real_spatial = 2 * pi * (1/var^2) * cos(2 * pi  * (fx0 * x)) * exp(-2 * pi^2 * (x^2 + y^2) / var^2);
bi_temporal = -s1 * exp(-0.5 * (t-u_bi1)^2 / sig_bi1^2) + s2 * exp(-0.5 * (t-u_bi2)^2 / sig_bi2^2) ;




act1 = real_spatial * bi_temporal;
act2 = im_spatial * mono_temporal;
act = act1 + act2;


%act = exp(-0.5 * x^2 / var^2);




end


